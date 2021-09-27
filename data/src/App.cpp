#include "App.hpp"

App::App() 
{
	m_paused = false;

	m_window.setFramerateLimit(60);
	m_window.setVerticalSyncEnabled(false);
	m_window.setKeyRepeatEnabled(false);
	m_window.create(sf::VideoMode(1024, 512), "Shishimanu");

	m_game.setup(getWinSize());

	loadTextures();

	setupGUI();

	loadAppState(MAIN_MENU);
}

void App::loadTextures()
{
	m_font.loadFromFile("data/assets/fonts/consola.ttf");
}

void App::setupGUI()
{
	// frames
	m_frame_mainMenu.setWindow(m_window);
	m_frame_pauseMenu.setWindow(m_window);
	m_frame_gameOverMenu.setWindow(m_window);
	m_frame_levelMenu.setWindow(m_window);
	m_frame_optionsMenu.setWindow(m_window);

	{
		m_frame_mainMenu.addEntity(m_button_start);
		m_frame_mainMenu.addEntity(m_button_level);
		m_frame_mainMenu.addEntity(m_button_options);
		m_frame_mainMenu.addEntity(m_button_exit);

		m_frame_mainMenu.push_in_navigationOrder(m_button_start);
		m_frame_mainMenu.push_in_navigationOrder(m_button_level);
		m_frame_mainMenu.push_in_navigationOrder(m_button_options);
		m_frame_mainMenu.push_in_navigationOrder(m_button_exit);

		m_texture_mainMenu.loadFromFile("data/assets/images/BG2.png");
		m_bgImg_mainMenu.setTexture(m_texture_mainMenu);
		m_bgImg_mainMenu.setScale(getWinSize().x / m_texture_mainMenu.getSize().x, getWinSize().y / m_texture_mainMenu.getSize().y);
	}
	{
		m_frame_pauseMenu.addEntity(m_button_resume);
		m_frame_pauseMenu.addEntity(m_button_restart);
		m_frame_pauseMenu.addEntity(m_button_options);
		m_frame_pauseMenu.addEntity(m_button_menu);
		m_frame_pauseMenu.addEntity(m_button_exit);


		m_frame_pauseMenu.push_in_navigationOrder(m_button_resume);
		m_frame_pauseMenu.push_in_navigationOrder(m_button_restart);
		m_frame_pauseMenu.push_in_navigationOrder(m_button_options);
		m_frame_pauseMenu.push_in_navigationOrder(m_button_menu);
		m_frame_pauseMenu.push_in_navigationOrder(m_button_exit);
	}
	{
		m_frame_gameOverMenu.addEntity(m_button_restart);
		m_frame_gameOverMenu.addEntity(m_button_menu);
		m_frame_gameOverMenu.addEntity(m_button_exit);


		m_frame_gameOverMenu.push_in_navigationOrder(m_button_restart);
		m_frame_gameOverMenu.push_in_navigationOrder(m_button_menu);
		m_frame_gameOverMenu.push_in_navigationOrder(m_button_exit);
	}
	{
		m_frame_optionsMenu.addEntity(m_button_back);
		m_frame_optionsMenu.addEntity(m_button_vol);
		m_frame_optionsMenu.addEntity(m_slider_vol);
		m_frame_optionsMenu.addEntity(m_textbox_control_left);
		m_frame_optionsMenu.addEntity(m_textbox_control_right);
		m_frame_optionsMenu.addEntity(m_inputbox_control_left);
		m_frame_optionsMenu.addEntity(m_inputbox_control_right);
		m_frame_optionsMenu.addEntity(m_textbox_control_jump);
		m_frame_optionsMenu.addEntity(m_button_save);


		m_frame_optionsMenu.push_in_navigationOrder(m_button_back);
		m_frame_optionsMenu.push_in_navigationOrder(m_button_vol);
		m_frame_optionsMenu.push_in_navigationOrder(m_slider_vol);
		m_frame_optionsMenu.push_in_navigationOrder(m_inputbox_control_left);
		m_frame_optionsMenu.push_in_navigationOrder(m_inputbox_control_right);
		m_frame_optionsMenu.push_in_navigationOrder(m_button_save);
	}

	// button stylings
	m_button_start.setSize({ 160,40 });
	m_button_start.setOrigin(m_button_start.getSize() / 2.f);
	m_button_start.setSelectionScale({ 1.2f, 1.2f });
	/*m_button_start.setFillColor(sf::Color::Transparent);
	m_button_start.setOutlineColor(sf::Color(160, 32, 240));
	m_button_start.setOutlineThickness(3);
	m_button_start.setSelectionFillColor(sf::Color(160, 32, 240, 126));
	m_button_start.setSelectionOutlineColor(sf::Color(124, 163, 251));
	m_button_start.setSelectionOutlineThickness(5);
	m_button_start.setCharacterSize(22);
	m_button_start.setTextFillColor(sf::Color::White);
	m_button_start.setFont(m_font);
	m_button_start.setString("Start");*/

	m_button_level = m_button_start;
	m_button_level.setString("level");

	m_button_options = m_button_start;
	m_button_options.setString("Options");

	m_button_exit = m_button_start;
	m_button_exit.setString("Exit");

	m_button_resume = m_button_start;
	m_button_resume.setString("Resume");

	m_button_restart = m_button_start;
	m_button_restart.setString("Restart");

	m_button_menu = m_button_start;
	m_button_menu.setString("Menu");

	m_button_vol = m_button_start;
	m_button_vol.setSize({ 40,40 });
	m_button_vol.setOrigin(m_button_vol.getSize() / 2.f);
	m_button_vol.setString("Vol");

	m_button_back = m_button_menu;
	m_button_back.setSize({ 160,40 });
	m_button_back.setOrigin(m_button_back.getSize() / 2.f);
	m_button_back.setString("Back to Menu");

	m_slider_vol.setSize({ 105, 40 });
	m_slider_vol.setOrigin(m_slider_vol.getSize() / 2.f);
	m_slider_vol.setBarFillColor(sf::Color::Red);
	m_slider_vol.setBarSelectionColor(sf::Color::Yellow);
	m_slider_vol.setFillColor(sf::Color::Transparent);
	m_slider_vol.setOutlineColor(sf::Color(160, 32, 240));
	m_slider_vol.setOutlineThickness(3);
	m_slider_vol.setTextFillColor(sf::Color::White);
	m_slider_vol.setFont(m_font);
	m_slider_vol.setCharacterSize(22);
	//m_slider_vol.setString("Volume");
	m_slider_vol.setVariable(m_musicVolume, 0, 100);
	
	m_textbox_control_left.setSize({ 150, 40 });
	m_textbox_control_left.setOrigin(m_textbox_control_left.getSize() / 2.f);
	m_textbox_control_left.setFillColor(sf::Color::Transparent);
	m_textbox_control_left.setOutlineColor(sf::Color(160, 32, 240));
	m_textbox_control_left.setOutlineThickness(3);
	m_textbox_control_left.setCharacterSize(22);
	m_textbox_control_left.setTextFillColor(sf::Color::White);
	m_textbox_control_left.setFont(m_font);
	m_textbox_control_left.setString("Move Left");

	m_textbox_control_right = m_textbox_control_left;
	m_textbox_control_right.setString("Move Right");

	m_inputbox_control_left.setSize({ 150, 40 });
	m_inputbox_control_left.setOrigin(m_inputbox_control_left.getSize() / 2.f);
	m_inputbox_control_left.setFillColor(sf::Color::Transparent);
	m_inputbox_control_left.setOutlineColor(sf::Color(160, 32, 240));
	m_inputbox_control_left.setOutlineThickness(3);
	m_inputbox_control_left.setSelectionFillColor(sf::Color(160, 32, 240, 126));
	m_inputbox_control_left.setSelectionOutlineColor(sf::Color(124, 163, 251));
	m_inputbox_control_left.setSelectionOutlineThickness(5);
	m_inputbox_control_left.setCharacterSize(22);
	m_inputbox_control_left.setTextFillColor(sf::Color::White);
	m_inputbox_control_left.setFont(m_font);
	m_inputbox_control_left.setString("Move Left");

	m_inputbox_control_right = m_inputbox_control_left;

	m_button_save = m_button_start;
	m_button_save.setSize({ 160, 40 });
	m_button_save.setOrigin(m_button_save.getSize() / 2.f);
;	m_button_save.setString("Save Changes");

	// button actions
	m_button_start.setAction([this]()
		{
			loadAppState(GAME);

			m_game.start();
		}
	);

	m_button_exit.setAction([this]()
		{
			loadAppState(EXIT);
		}
	);

	m_button_menu.setAction([this]()
		{
			loadAppState(MAIN_MENU);
		}
	);

	m_button_back.setAction([this]()
		{
			loadAppState(MAIN_MENU);
		}
	);

	m_button_options.setAction([this]()
		{
			loadAppState(OPTIONS_MENU);
		}
	);

	m_button_resume.setAction([this]()
		{
			loadAppState(GAME);
		}
	);

	m_button_restart.setAction([this]()
		{
			loadAppState(GAME);
			m_game.loadSameLevel();
		}
	);

	m_button_save.setAction([this]()
		{
			//std::ofstream settings;
			//settings.open("data/assets/Settings/settings.txt");

			//settings << "Helo noobll /t/t" << "A/n";
			//settings << "Right/t/t" << "D";

			//settings.close();
		}
	);


	//std::ofstream settings;
	//settings.open("data/assets/Settings/settings.txt");

	//if (settings.good()) 
	//	std::cout << 0;

	////settings << "Helo noobll /t/t" << "A/n";
	////settings << "Right/t/t" << "D";

	//settings.close();

	test.loadFromFile("data/assets/images/gamebutton1.png");
	test.setRepeated(true);
}

void App::loadAppState(AppState state)
{
	switch (state) {
	case GAME:				loadGame(); break;
	case MAIN_MENU:			loadMainMenu(); break;
	case GAME_OVER_MENU:	loadGameOverMenu(); break;
	case PAUSE_MENU:		loadPauseMenu(); break;
	case LEVEL_MENU:		loadLevelMenu(); break;
	case OPTIONS_MENU:		loadOptionsMenu(); break;
	case EXIT:				m_state = EXIT;  break;
	}
}

void App::loadGame()
{
	m_state = GAME;

	m_game.setPaused(false);
}

void App::loadMainMenu()
{
	m_state = MAIN_MENU;

	m_button_start.setPosition(m_window.getSize().x / 2.f, 70);
	m_button_start.setTexture(&test);

	m_button_level.setPosition(m_window.getSize().x / 2.f, 180);
	m_button_options.setPosition(m_window.getSize().x / 2.f, 290);
	m_button_exit.setPosition(m_window.getSize().x / 2.f, 400);
}

void App::loadPauseMenu()
{
	m_state = PAUSE_MENU;

	m_game.setPaused(true);

	m_button_resume.setPosition(m_window.getSize().x / 2.f, 150);
	m_button_restart.setPosition(m_window.getSize().x / 2.f, 210);
	m_button_options.setPosition(m_window.getSize().x / 2.f, 270);
	m_button_menu.setPosition(m_window.getSize().x / 2.f, 330);
	m_button_exit.setPosition(m_window.getSize().x / 2.f, 390);
}
void App::loadGameOverMenu()
{
	m_state = GAME_OVER_MENU;

	m_button_restart.setPosition(m_window.getSize().x / 2.f, 150);
	m_button_menu.setPosition(m_window.getSize().x / 2.f, 210);
	m_button_exit.setPosition(m_window.getSize().x / 2.f, 270);
}

void App::loadLevelMenu()
{
	m_state = LEVEL_MENU;
}

void App::loadOptionsMenu()
{
	m_state = OPTIONS_MENU;

	m_button_back.setPosition(m_window.getSize().x / 2.f, 150);
	m_button_vol.setPosition(m_window.getSize().x / 2.f - 60, 210);
	m_slider_vol.setPosition(m_window.getSize().x / 2.f + 28, 210);
	m_textbox_control_left.setPosition(m_window.getSize().x / 2.f - 85, 270);
	m_textbox_control_right.setPosition(m_window.getSize().x / 2.f - 85, 330);

	m_inputbox_control_left.setPosition(m_window.getSize().x / 2.f + 85, 270);
	m_inputbox_control_right.setPosition(m_window.getSize().x / 2.f + 85, 330);

	m_button_save.setPosition(m_window.getSize().x / 2.f, 390);

	std::ifstream settings;
	settings.open("data/assets/Settings/settings.txt");
	std::string s;
	while (std::getline(settings, s)) {
		std::stringstream line(s);

		std::string control;
		char key;

		line >> control >> key;

		if (control == "Left")
			m_inputbox_control_left.setString(std::string() + key);

		else if (control == "Right")
			m_inputbox_control_right.setString(std::string() + key);

		m_game.setControlKeys(m_inputbox_control_left.getString()[0], m_inputbox_control_right.getString()[0], 0);
	}

	settings.close();
}

void App::update()
{
	float dt = m_clock.restart().asSeconds();

	switch(m_state){
	case GAME:				m_game.update(dt); break;
	case MAIN_MENU:			m_frame_mainMenu.update(); break;
	case PAUSE_MENU:		m_frame_pauseMenu.update(); break;
	case GAME_OVER_MENU:	m_frame_gameOverMenu.update(); break;
	case LEVEL_MENU:		m_frame_levelMenu.update(); break;
	case OPTIONS_MENU:		m_frame_optionsMenu.update(); break;
	}

	pollEvents();
}

void App::render()
{
	m_window.clear();

	if (m_state == MAIN_MENU) {
		m_window.draw(m_bgImg_mainMenu);
	}

	switch (m_state) {
	case GAME:				m_game.draw(m_window); break;
	case MAIN_MENU:			m_frame_mainMenu.draw(); break;
	case PAUSE_MENU:		m_frame_pauseMenu.draw(); break;
	case GAME_OVER_MENU:	m_frame_gameOverMenu.draw(); break;
	case LEVEL_MENU:		m_frame_levelMenu.draw(); break;
	case OPTIONS_MENU:		m_frame_optionsMenu.draw(); break;
	}
	
	//*********************
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(0, m_window.getSize().y));
	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOrigin(rect.getSize() / 2.f);
	rect.setPosition(m_window.getSize().x/2.f, m_window.getSize().y / 2.f);
	m_window.draw(rect);
	//*********************

	m_window.display();
}

void App::pollEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event)) {

		if (event.type == sf::Event::Closed)
			m_window.close();

		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
			m_paused = !m_paused;

		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
			switch (m_state) {
			case GAME:			loadAppState(PAUSE_MENU); break;
			case PAUSE_MENU:	loadAppState(GAME); break;
			case OPTIONS_MENU:	loadAppState(MAIN_MENU); break;
			}
		}	

		switch (m_state) {
		case GAME:				m_game.pollEvents(event); break;
		case MAIN_MENU:			m_frame_mainMenu.pollEvents(event); break;
		case PAUSE_MENU:		m_frame_pauseMenu.pollEvents(event); break;
		case GAME_OVER_MENU:	m_frame_gameOverMenu.pollEvents(event); break;
		case LEVEL_MENU:		m_frame_levelMenu.pollEvents(event); break;
		case OPTIONS_MENU:		m_frame_optionsMenu.pollEvents(event); break;
		}

		if (m_inputbox_control_left.getString().size() > 1)
			m_inputbox_control_left.setString(std::string() + (char)toupper(m_inputbox_control_left.getString().erase(0, 1)[0]));

		if (m_inputbox_control_right.getString().size() > 1)
			m_inputbox_control_right.setString(std::string() + (char)toupper(m_inputbox_control_right.getString().erase(0, 1)[0]));
		
	}
}

bool App::isRunning() const
{
	return m_window.isOpen() && m_state != EXIT;
}

sf::Vector2f App::getWinSize() const
{
	return sf::Vector2f(m_window.getSize());
}
