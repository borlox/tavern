#include "pch.h"
#include "UserInterface.h"
#include "Scripting.h"

using namespace sf;
using namespace sfg;

void UserInterface::Initialize()
{
	fixedWin = sfg::Window::Create(0);
	fixedWin->SetPosition(Vector2f(0.f, 0.f));

	fixedLayout = Fixed::Create();
	fixedWin->Add(fixedLayout);

	quitBtn = Button::Create("Quit");
	quitBtn->GetSignal(Widget::OnLeftClick).Connect(Delegate([&]() {
		shouldQuit = true;
	}));
	fixedLayout->Put(quitBtn, Vector2f(350.f, 5.f));

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
	script.entry->GetSignal(Widget::OnKeyPress).Connect(Delegate([execString,this]() {
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

	fixedLayout->Put(script.layout, Vector2f(5.f, 390.f));

	desktop.Add(fixedWin);
}

void UserInterface::HandleEvent(const Event& event)
{
	currentEvent = event;
	desktop.HandleEvent(event);
}
