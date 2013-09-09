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

	void Render(sf::RenderWindow& target);

	bool ShouldQuit() const
	{
		return shouldQuit;
	}

	bool PopupVisible() const
	{
		return textWindow.win->IsLocallyVisible();
	}

	void ShowTextWindow(std::string file);

	static void exp_ShowTextWindow(std::string file)
	{
		UserInterface::Get().ShowTextWindow(file);
	}

	static bool exp_LoadUiTheme(std::string theme)
	{
		return UserInterface::Get().desktop.LoadThemeFromFile("assets/misc/" + theme + ".theme");
	}

	static luabind::scope ExportClass()
	{
		return
			luabind::def("DisplayText", UserInterface::exp_ShowTextWindow),
			luabind::def("LoadUiTheme", UserInterface::exp_LoadUiTheme)
		;
	}

private:
	UserInterface()
	{}

	void SetCommonHandler(sfg::Widget::Ptr widget);

	void MarkEventHandled();

	sf::Event currentEvent;

	bool shouldQuit;

	sfg::SFGUI sfgui;
	sfg::Desktop desktop;

	sfg::Window::Ptr fixedWin;
	sfg::Fixed::Ptr fixedLayout;

	sfg::Button::Ptr quitBtn;

	sf::Sprite topBar;
	sf::Sprite bottomBar;

	struct {
		sfg::Box::Ptr layout;
		sfg::Entry::Ptr entry;
		sfg::Button::Ptr execBtn;
		sfg::Button::Ptr reloadBtn;
		sfg::Separator::Ptr sep;
	} script;
	void InitScript();

	struct {
		sfg::Window::Ptr win;
		sfg::Box::Ptr layout;
		sfg::Label::Ptr label;
		sfg::Separator::Ptr sep;
		sfg::Button::Ptr okBtn;
	} textWindow;
	void InitTextWindow();
};

#endif //USER_INTERFACE_H
