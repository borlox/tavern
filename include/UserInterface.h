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

	void Update(float elapsed);

	void Render(sf::RenderWindow& target);

	bool ShouldQuit() const
	{
		return shouldQuit;
	}

	bool PopupVisible() const
	{
		return popupOpen;
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

	static void exp_PrepareDialog(std::string text)
	{
		UserInterface::Get().PrepareDialog(text);
	}

	static void exp_AddDialogButton(std::string id, std::string text)
	{
		UserInterface::Get().AddDialogButton(id, text);
	}

	static void exp_ShowDialog()
	{
		UserInterface::Get().ShowDialog();
	}

	static luabind::scope ExportClass()
	{
		return
			luabind::def("DisplayText", UserInterface::exp_ShowTextWindow),
			luabind::def("LoadUiTheme", UserInterface::exp_LoadUiTheme),
			luabind::def("PrepareDialog", UserInterface::exp_PrepareDialog),
			luabind::def("AddDialogButton", UserInterface::exp_AddDialogButton),
			luabind::def("ShowDialog", UserInterface::exp_ShowDialog)
		;
	}

private:
	UserInterface()
	{}

	void SetCommonHandler(sfg::Widget::Ptr widget);

	void MarkEventHandled();

	void PrepareDialog(const std::string& text);
	void AddDialogButton(std::string id, std::string text);
	void ShowDialog();
	void ClearDialog();
	void HideDialog();

	sf::Event currentEvent;

	bool shouldQuit;
	bool popupOpen;

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

	struct {
		sfg::Window::Ptr win;
		sfg::Box::Ptr layout;
		sfg::Label::Ptr label;
		sfg::Separator::Ptr sep;
		std::vector<sfg::Button::Ptr> buttons;
		bool clearButtons;
	} dialogWindow;
	void InitDialogWindow();
};

#endif //USER_INTERFACE_H
