#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// Including SFGUI/SFGUI.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>

class SpinButtonExample {
	public:
		// Our value change handler.
		void OnValueChange();

		void Run();

	private:
		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		// Create the SpinButton pointer here to reach it from OnValueChange().
		sfg::SpinButton::Ptr m_spinbutton;

		// Create the ProgressBar pointer here to reach it from OnValueChange().
		sfg::ProgressBar::Ptr m_progress_bar;
};

void SpinButtonExample::OnValueChange() {
	const sfg::Adjustment::Ptr& adjustment( m_spinbutton->GetAdjustment() );

	float range = adjustment->GetUpper() - adjustment->GetLower();
	float inverse_fraction = 1.f - ( m_spinbutton->GetValue() - adjustment->GetLower() ) / range;

	m_progress_bar->SetFraction( inverse_fraction );
}

void SpinButtonExample::Run() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI SpinButton Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	sfg::Window::Ptr window;
	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create a box to contain our SpinButton and progress bar.
	sfg::Box::Ptr box = sfg::Box::Create( sfg::Box::VERTICAL, 10.f );

	// Create the SpinButton itself.
	m_spinbutton = sfg::SpinButton::Create( -2.f, 18.f, .4f );

	// Just like an Entry, you need to specify a minimum width for the SpinButton.
	m_spinbutton->SetRequisition( sf::Vector2f( 80.f, 0.f ) );

	// Set the number of digits to display after the decimal point.
	m_spinbutton->SetDigits( 2 );

	// Connect the OnValueChanged signal so we get notified when the SpinButton's value changes.
	m_spinbutton->GetSignal( sfg::SpinButton::OnValueChanged ).Connect( &SpinButtonExample::OnValueChange, this );

	// Add the SpinButton to the box.
	box->Pack( m_spinbutton );

	// Create our progress bar and set its size.
	m_progress_bar = sfg::ProgressBar::Create();
	m_progress_bar->SetRequisition( sf::Vector2f( 80.f, 20.f ) );

	// Add the progress bar to the box.
	box->Pack( m_progress_bar );

	// Set the initial value of the SpinButton.
	// We can only do this after we create our progress bar because the way we
	// set it, it will try to update the progress bar when we change its value.
	m_spinbutton->SetValue( 4.f );

	// Add the box to the window.
	window->Add( box );

	// Our clock.
	sf::Clock clock;

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Handle events
			window->HandleEvent( event );

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
			}
		}

		// Update the GUI every 1ms
		if( clock.getElapsedTime().asMicroseconds() >= 1000 ) {
			float delta = static_cast<float>( clock.getElapsedTime().asMicroseconds() ) / 1000000.f;

			// Update() takes the elapsed time in seconds.
			window->Update( delta );

			clock.restart();
		}

		// Clear screen
		app_window.clear();

		// Draw the GUI
		sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}
}

int main() {
	SpinButtonExample example;
	example.Run();

	return EXIT_SUCCESS;
}
