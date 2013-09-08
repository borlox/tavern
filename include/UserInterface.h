#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

class UserInterface
{
public:
	static UserInterface& Get()
	{
		static UserInterface instance;
		return instance;
	}

	void Initialize();

	void HandleEvent(const sf::Event& event);

	void Update(float elapsed)
	{
		desktop.Update(elapsed);
	}

	void Render(sf::Window& target)
	{
		sfgui.Display(target);
	}

	bool ShouldQuit() const
	{
		return shouldQuit;
	}

private:
	UserInterface()
	{}

	void SetCommonHandler(sfg::Widget::Ptr widget);

	void MarkEventHandled();

	sf::Event currentEvent;
	//volatile bool eventHandled;

	bool shouldQuit;

	sfg::SFGUI sfgui;
	sfg::Desktop desktop;

	sfg::Window::Ptr fixedWin;
	sfg::Fixed::Ptr fixedLayout;

	sfg::Button::Ptr quitBtn;

	struct {
		sfg::Box::Ptr layout;
		sfg::Entry::Ptr entry;
		sfg::Button::Ptr execBtn;
		sfg::Button::Ptr reloadBtn;
		sfg::Separator::Ptr sep;
	} script;
};

#endif //USER_INTERFACE_H
