#include "pch.h"
#include "UserInterface.h"
#include "Scripting.h"
#include "TextureManager.h"

using namespace sf;
using namespace sfg;

namespace fs = boost::filesystem;

void UserInterface::Initialize()
{
	popupOpen = false;

	topBar.setTexture(*TextureManager::Get().Load("assets/image/interface/TopBar.png"));
	topBar.setPosition(0.f, 0.f);

	bottomBar.setTexture(*TextureManager::Get().Load("assets/image/interface/BottomBar.png"));
	bottomBar.setPosition(0.f, 500.f);
	
	fixedWin = sfg::Window::Create(0);
	fixedWin->SetPosition(Vector2f(5.f, 505.f));

	fixedLayout = Fixed::Create();
	fixedWin->Add(fixedLayout);

	quitBtn = Button::Create("Quit");
	quitBtn->GetSignal(Widget::OnLeftClick).Connect(Delegate([&]() {
		shouldQuit = true;
	}));
	fixedLayout->Put(quitBtn, Vector2f(0.f, 33.f));

	InitScript();	
	fixedLayout->Put(script.layout, Vector2f(0.f, 0.f));

	InitTextWindow();
	InitDialogWindow();

	desktop.Add(textWindow.win);
	desktop.Add(dialogWindow.win);
	desktop.Add(fixedWin);

	desktop.LoadThemeFromFile("assets/misc/ui.theme");
}

void UserInterface::Render(sf::RenderWindow& target)
{
	target.draw(topBar);
	target.draw(bottomBar);
	sfgui.Display(target);
}

void UserInterface::InitScript()
{
	script.layout = Box::Create(Box::HORIZONTAL, 5.0f);
	script.entry = Entry::Create();
	script.entry->SetRequisition(Vector2f(300.f, 20.f));
	script.execBtn = Button::Create("Run");

	auto execString = Delegate([&]() {
		std::string str = script.entry->GetText();
		if (!str.empty())
			Scripting::Get().ExecuteString(str);
		script.entry->SetText("");
	});

	script.execBtn->GetSignal(Widget::OnLeftClick).Connect(execString);
	script.entry->GetSignal(Widget::OnKeyPress).Connect(Delegate([execString,this]() { // capture execString so it doesnt get lost
		if (currentEvent.key.code == Keyboard::Return)
			execString();
	}));

	script.reloadBtn = Button::Create("Reload");
	script.reloadBtn->GetSignal(Widget::OnLeftClick).Connect(Delegate(std::function<void(void)>([&]() {
		Scripting::Get().Reset();
		Scripting::Get().Initialize();
	})));

	script.sep = Separator::Create(Separator::VERTICAL);

	script.layout->Pack(script.entry);
	script.layout->Pack(script.execBtn);
	script.layout->Pack(script.sep);
	script.layout->Pack(script.reloadBtn);
}

void UserInterface::InitTextWindow()
{
	auto& tw = textWindow;
	tw.win = sfg::Window::Create();
	tw.win->SetPosition(Vector2f(350.f, 250.f));

	tw.layout = Box::Create(Box::VERTICAL, 5.f);

	tw.label = Label::Create();
	tw.layout->Pack(tw.label);

	tw.sep = Separator::Create(Separator::HORIZONTAL);
	tw.sep->SetClass("textSeparator");
	tw.layout->Pack(tw.sep);

	tw.okBtn = Button::Create("Ok");
	tw.okBtn->GetSignal(Widget::OnLeftClick).Connect(Delegate([&]() {
		textWindow.win->Show(false);
	}));
	tw.layout->Pack(tw.okBtn);

	tw.win->Add(tw.layout);
	tw.win->Show(false);
}

void UserInterface::InitDialogWindow()
{
	auto& dw = dialogWindow;

	dw.win = sfg::Window::Create();
	dw.win->SetPosition(Vector2f(250, 250));

	dw.layout = Box::Create(Box::VERTICAL, 5.f);

	dw.label = Label::Create();
	dw.layout->Pack(dw.label);

	dw.sep = Separator::Create(Separator::HORIZONTAL);
	dw.sep->SetClass("textSeparator");
	dw.layout->Pack(dw.sep);

	dw.win->Add(dw.layout);
	dw.win->Show(false);
}

void UserInterface::HandleEvent(const Event& event)
{
	currentEvent = event;
	desktop.HandleEvent(event);
}

void UserInterface::Update(float elapsed)
{
	desktop.Update(elapsed);
	if (!textWindow.win->IsLocallyVisible() && !dialogWindow.win->IsLocallyVisible())
		popupOpen = false;
}


void UserInterface::ShowTextWindow(std::string file)
{
	//fs::path path = "assets/texts";
	//path /= file;
	fs::path path = file;

	std::string text;

	std::ifstream inf(path.string());
	std::string line;

	while (std::getline(inf, line))
		text += line + '\n';

	textWindow.label->SetText(text);
	textWindow.label->RequestResize();
	textWindow.win->Show();
	popupOpen = true;
}

void UserInterface::PrepareDialog(const std::string& text)
{
	ClearDialog();
	auto& dw = dialogWindow;

	dw.label->SetText(text);
}

void UserInterface::AddDialogButton(std::string id, std::string text)
{
	Button::Ptr btn = Button::Create(text);
	btn->GetSignal(Widget::OnLeftClick).Connect(Delegate([=]() {
		Scripting::Get().PostEvent("DialogButtonClicked", id);
		HideDialog();
	}));

	auto& dw = dialogWindow;
	dw.buttons.push_back(btn);
	dw.layout->Pack(btn);
}

void UserInterface::ShowDialog()
{
	dialogWindow.win->Refresh();
	dialogWindow.win->Show();
	popupOpen = true;
}

void UserInterface::HideDialog()
{
	auto& dw = dialogWindow;

	dw.win->Show(false);
}

void UserInterface::ClearDialog()
{
	auto& dw = dialogWindow;

	for (auto& btn: dw.buttons)
		dw.layout->Remove(btn);

	dw.buttons.clear();
}
