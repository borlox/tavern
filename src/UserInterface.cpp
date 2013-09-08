#include "pch.h"
#include "UserInterface.h"
#include "Scripting.h"

using namespace sf;
using namespace sfg;

namespace fs = boost::filesystem;

void UserInterface::Initialize()
{
	fixedWin = sfg::Window::Create(0);
	fixedWin->SetPosition(Vector2f(5.f, 390.f));

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

	desktop.Add(textWindow.win);
	desktop.Add(fixedWin);
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
	tw.win->SetPosition(Vector2f(100.f, 100.f));

	tw.layout = Box::Create(Box::VERTICAL, 5.f);

	tw.label = Label::Create();
	tw.layout->Pack(tw.label);

	tw.sep = Separator::Create(Separator::HORIZONTAL);
	tw.layout->Pack(tw.sep);

	tw.okBtn = Button::Create("Ok");
	tw.okBtn->GetSignal(Widget::OnLeftClick).Connect(Delegate([&]() {
		textWindow.win->Show(false);
	}));
	tw.layout->Pack(tw.okBtn);

	tw.win->Add(tw.layout);
	tw.win->Show(false);
}

void UserInterface::HandleEvent(const Event& event)
{
	currentEvent = event;
	desktop.HandleEvent(event);
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
}
