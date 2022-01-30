#include "App.hpp"

App::App() 
{
	m_paused = false;
	m_mute = false;

	m_window.setFramerateLimit(60);
	m_window.setVerticalSyncEnabled(false);
	m_window.setKeyRepeatEnabled(false);
	m_window.create(sf::VideoMode(1024, 512), "Platformer");

	m_score = 0;

	m_game.setup(getWinSize());

	loadTextures();

	setupGUI();
	if (m_game_music.openFromFile("data/assets/audio/Terraria_Underground.wav"));

	m_game_music.play();
	m_musicVolume = m_game_music.getVolume();
	m_slider_vol->shiftOffset(m_musicVolume);

	m_prevState = MAIN_MENU;
	loadAppState(MAIN_MENU);
}

void App::loadTextures()
{
	m_font.loadFromFile("data/assets/fonts/consola.ttf");
	m_font_level.loadFromFile("data/assets/fonts/Pixeboy.ttf");
}

void App::setupGUI()
{
	// frames
	m_frame_mainMenu.setWindow(m_window);
	m_frame_pauseMenu.setWindow(m_window);
	m_frame_gameOverMenu.setWindow(m_window);
	m_frame_levelMenu.setWindow(m_window);
	m_frame_optionsMenu.setWindow(m_window);

	// button stylings
	m_button_start = zui::create<zui::TextButton>();
	m_button_start->setSize({ 160,60 });
	m_button_start->setOrigin(m_button_start->getSize() / 2.f);
	m_button_start->setSelectionScale({ 1.2f, 1.2f });

	m_button_level = zui::copy(m_button_start);
	m_button_level->setString("level");

	m_button_options = zui::copy(m_button_start);
	m_button_options->setString("Options");

	m_button_exit = zui::copy(m_button_start);
	m_button_exit->setString("Exit");

	m_button_resume = zui::copy(m_button_start);
	m_button_resume->setString("Resume");

	m_button_restart = zui::copy(m_button_start);
	m_button_restart->setString("Restart");

	m_textbox_scoreBoard = zui::create<zui::Textbox>();
	m_textbox_scoreBoard->setSize({ 300, 200 });
	m_textbox_scoreBoard->setOrigin(m_textbox_scoreBoard->getSize() / 2.f);

	m_textbox_score = zui::create<zui::Textbox>();
	m_textbox_score->setSize({ 150, 40 });
	m_textbox_score->setOrigin(m_textbox_score->getSize() / 2.f);
	m_textbox_score->setFillColor(sf::Color::Transparent);
	m_textbox_score->setCharacterSize(38);
	m_textbox_score->setTextFillColor(sf::Color(255, 221, 124));
	m_textbox_score->setFont(m_font);
	m_textbox_score->setTextOutlineThickness(3);

	m_button_menu = zui::copy(m_button_start);
	m_button_menu->setString("Menu");

	m_button_vol = zui::create<zui::TextButton>();
	m_button_vol->setSize({ 60,60 });
	m_button_vol->setOrigin(m_button_vol->getSize() / 2.f);
	m_button_vol->setSelectionScale({ 1.2f, 1.2f });
	m_button_vol->setString("Vol");

	m_button_back = zui::copy(m_button_start);
	m_button_back->setString("Back to Menu");

	m_slider_vol = zui::create<zui::Slider>();
	m_slider_vol->setSize({ 105, 40 });
	m_slider_vol->setOrigin(m_slider_vol->getSize() / 2.f);
	m_slider_vol->setBarFillColor(sf::Color::Red);
	m_slider_vol->setBarSelectionColor(sf::Color::Yellow);
	m_slider_vol->setFillColor(sf::Color::Transparent);
	m_slider_vol->setVariable(m_musicVolume, 0, 100);
	
	m_textbox_control_left = zui::create<zui::Textbox>();
	m_textbox_control_left->setSize({ 150, 40 });
	m_textbox_control_left->setOrigin(m_textbox_control_left->getSize() / 2.f);

	m_textbox_control_right = zui::copy(m_textbox_control_left);

	m_inputbox_control_left = zui::create<zui::Inputbox>();
	m_inputbox_control_left->setSize({ 150, 40 });
	m_inputbox_control_left->setOrigin(m_inputbox_control_left->getSize() / 2.f);
	m_inputbox_control_left->setFillColor(sf::Color(228, 169, 79));
	m_inputbox_control_left->setOutlineColor(sf::Color(178, 75, 48));
	m_inputbox_control_left->setOutlineThickness(3);
	m_inputbox_control_left->setSelectionFillColor(sf::Color(228, 169, 79, 126));
	m_inputbox_control_left->setSelectionOutlineColor(sf::Color(178, 75, 48));
	m_inputbox_control_left->setSelectionOutlineThickness(5);
	m_inputbox_control_left->setCharacterSize(22);
	m_inputbox_control_left->setTextFillColor(sf::Color(95, 38, 9));
	m_inputbox_control_left->setFont(m_font);

	m_inputbox_control_right = zui::create<zui::Inputbox>();
	m_inputbox_control_right->setSize({ 150, 40 });
	m_inputbox_control_right->setOrigin(m_inputbox_control_right->getSize() / 2.f);
	m_inputbox_control_right->setFillColor(sf::Color(228, 169, 79));
	m_inputbox_control_right->setOutlineColor(sf::Color(178, 75, 48));
	m_inputbox_control_right->setOutlineThickness(3);
	m_inputbox_control_right->setSelectionFillColor(sf::Color(228, 169, 79, 126));
	m_inputbox_control_right->setSelectionOutlineColor(sf::Color(178, 75, 48));
	m_inputbox_control_right->setSelectionOutlineThickness(5);
	m_inputbox_control_right->setCharacterSize(22);
	m_inputbox_control_right->setTextFillColor(sf::Color(95, 38, 9));
	m_inputbox_control_right->setFont(m_font);
	
	m_button_save = zui::copy(m_button_start);
;	m_button_save->setString("Save Changes");


	for (int i = 0; i <  m_game.getTotalLevels(); i++) {
		m_levels.emplace_back();
		m_levels[i] = zui::create<zui::TextButton>();
		m_levels[i]->setSize(sf::Vector2f(140, 40));

		m_frame_levelMenu.addEntity(m_levels[i].get());
		m_frame_levelMenu.push_in_navigationOrder(*m_levels[i]);

		m_levels[i]->setOrigin(m_levels[i]->getSize() / 2.f);
		m_levels[i]->setSelectionScale({ 1.2f, 1.2f });
		m_levels[i]->setFont(m_font_level);
		m_levels[i]->setString("Level " + std::to_string(i + 1));
		
	}

	{
		m_frame_mainMenu.addEntity(m_button_start.get());
		m_frame_mainMenu.addEntity(m_button_level.get());
		m_frame_mainMenu.addEntity(m_button_options.get());
		m_frame_mainMenu.addEntity(m_button_exit.get());

		m_frame_mainMenu.push_in_navigationOrder(*m_button_start);
		m_frame_mainMenu.push_in_navigationOrder(*m_button_level);
		m_frame_mainMenu.push_in_navigationOrder(*m_button_options);
		m_frame_mainMenu.push_in_navigationOrder(*m_button_exit);

		m_texture_mainMenu.loadFromFile("data/assets/images/BG2.png");
		m_bgImg_mainMenu.setTexture(m_texture_mainMenu);
		m_bgImg_mainMenu.setScale(getWinSize().x / m_texture_mainMenu.getSize().x, getWinSize().y / m_texture_mainMenu.getSize().y);
	}
	{
		m_frame_pauseMenu.addEntity(m_button_resume.get());
		m_frame_pauseMenu.addEntity(m_button_restart.get());
		m_frame_pauseMenu.addEntity(m_button_options.get());
		m_frame_pauseMenu.addEntity(m_button_menu.get());
		m_frame_pauseMenu.addEntity(m_button_exit.get());


		m_frame_pauseMenu.push_in_navigationOrder(*m_button_resume);
		m_frame_pauseMenu.push_in_navigationOrder(*m_button_restart);
		m_frame_pauseMenu.push_in_navigationOrder(*m_button_options);
		m_frame_pauseMenu.push_in_navigationOrder(*m_button_menu);
		m_frame_pauseMenu.push_in_navigationOrder(*m_button_exit);
	}
	{
		m_frame_gameOverMenu.addEntity(m_textbox_scoreBoard.get());
		m_frame_gameOverMenu.addEntity(m_textbox_score.get());
		m_frame_gameOverMenu.addEntity(m_button_restart.get());
		m_frame_gameOverMenu.addEntity(m_button_menu.get());
		m_frame_gameOverMenu.addEntity(m_button_exit.get());
		
		m_frame_gameOverMenu.push_in_navigationOrder(*m_button_restart);
		m_frame_gameOverMenu.push_in_navigationOrder(*m_button_menu);
		m_frame_gameOverMenu.push_in_navigationOrder(*m_button_exit);
	}
	{
		m_frame_optionsMenu.addEntity(m_button_back.get());
		m_frame_optionsMenu.addEntity(m_button_vol.get());
		m_frame_optionsMenu.addEntity(m_slider_vol.get());
		m_frame_optionsMenu.addEntity(m_textbox_control_left.get());
		m_frame_optionsMenu.addEntity(m_textbox_control_right.get());
		m_frame_optionsMenu.addEntity(m_inputbox_control_left.get());
		m_frame_optionsMenu.addEntity(m_inputbox_control_right.get());
		m_frame_optionsMenu.addEntity(m_button_save.get());


		m_frame_optionsMenu.push_in_navigationOrder(*m_button_back);
		m_frame_optionsMenu.push_in_navigationOrder(*m_button_vol);
		m_frame_optionsMenu.push_in_navigationOrder(*m_slider_vol);
		m_frame_optionsMenu.push_in_navigationOrder(*m_inputbox_control_left);
		m_frame_optionsMenu.push_in_navigationOrder(*m_inputbox_control_right);
		m_frame_optionsMenu.push_in_navigationOrder(*m_button_save);
	}

	// button actions
	m_button_start->setAction([this]()
		{
			m_scoreClock.reset();
			loadAppState(GAME);
			m_game.start();
		}
	);

	m_button_exit->setAction([this]()
		{
			loadAppState(EXIT);
		}
	);

	m_button_menu->setAction([this]()
		{
			loadAppState(MAIN_MENU);
			m_game.loadSameLevel();
		}
	);

	m_button_back->setAction([this]()
		{
			loadAppState(m_prevState);
		}
	);

	m_button_options->setAction([this]()
		{
			loadAppState(OPTIONS_MENU);
		}
	);

	m_button_resume->setAction([this]()
		{
			loadAppState(GAME);
		}
	);

	m_button_restart->setAction([this]()
		{
			m_scoreClock.reset();
			loadAppState(GAME);
			m_game.loadSameLevel();
		}
	);

	m_button_save->setAction([this]()
		{
			std::ofstream settingsOutput;
			settingsOutput.open("data/assets/Settings/settings.txt");

			if (settingsOutput.is_open()) {
				settingsOutput << "Left " << m_inputbox_control_left->getString() << '\n';
				settingsOutput << "Right " << m_inputbox_control_right->getString() << '\n';
				settingsOutput << "TotalLevels " << m_game.getTotalLevels() << '\n';
			}
			settingsOutput.close();
			m_game.setup(getWinSize());
		}
	);

	m_button_level->setAction([this]()
		{
			loadAppState(LEVEL_MENU);
		}
	);

	m_button_vol->setAction([this]()
		{
			m_mute = !m_mute;
			if (!m_mute) {
				m_button_vol->setTexture(&m_button_textures["vol"]);
				m_game_music.play();
			}
			else {
				m_button_vol->setTexture(&m_button_textures["volMute"]);
				m_game_music.pause();
			}
		}
	);
	for (int i = 0; i < m_levels.size(); i++) {
		m_levels[i]->setAction([this,i]()
			{
				m_scoreClock.reset();
				loadAppState(GAME);
				m_game.loadLevel(i+1);
			}
		);
	}
	
	{
		m_button_textures["start"].loadFromFile("data/assets/images/buttons/play.png");
		m_button_textures["level"].loadFromFile("data/assets/images/buttons/level.png");
		m_button_textures["options"].loadFromFile("data/assets/images/buttons/options.png");
		m_button_textures["exit"].loadFromFile("data/assets/images/buttons/exit.png");

		m_button_textures["resume"].loadFromFile("data/assets/images/buttons/resume.png");
		m_button_textures["restart"].loadFromFile("data/assets/images/buttons/restart.png");
		m_button_textures["menu"].loadFromFile("data/assets/images/buttons/menu.png");

		m_button_textures["vol"].loadFromFile("data/assets/images/buttons/vol.png");
		m_button_textures["volMute"].loadFromFile("data/assets/images/buttons/vol_mute.png");
		m_vols[0] = m_button_textures["vol"]; 
		m_vols[1] = m_button_textures["volMute"];
		m_button_textures["moveright"].loadFromFile("data/assets/images/buttons/moveright.png");
		m_button_textures["moveleft"].loadFromFile("data/assets/images/buttons/moveleft.png");
		m_button_textures["back"].loadFromFile("data/assets/images/buttons/back.png");
		m_button_textures["save"].loadFromFile("data/assets/images/buttons/save.png");

		m_button_textures["levels"].loadFromFile("data/assets/images/buttons/level-num.png");

		m_button_textures["score"].loadFromFile("data/assets/images/buttons/scoreboard.png");
	}
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
	m_prevState = m_state;
	m_state = GAME;

	m_game.setPaused(false);

	m_scoreClock.resume();

	m_game.resetKillCount();
}

void App::loadMainMenu()
{
	m_prevState = m_state;
	m_state = MAIN_MENU;

	m_button_start->setPosition(m_window.getSize().x / 2.f, 150);
	m_button_start->setTexture(&m_button_textures["start"]);

	m_button_level->setPosition(m_window.getSize().x / 2.f, 230);
	m_button_level->setTexture(&m_button_textures["level"]);

	m_button_options->setPosition(m_window.getSize().x / 2.f, 310);
	m_button_options->setTexture(&m_button_textures["options"]);

	m_button_exit->setPosition(m_window.getSize().x / 2.f, 390);
	m_button_exit->setTexture(&m_button_textures["exit"]);

	m_game.resetKillCount();

	m_scoreClock.pause();
}

void App::loadPauseMenu()
{
	m_prevState = m_state;
	m_state = PAUSE_MENU;

	m_game.setPaused(true);

	m_button_resume->setPosition(m_window.getSize().x / 2.f, 100);
	m_button_resume->setTexture(&m_button_textures["resume"]);

	m_button_restart->setPosition(m_window.getSize().x / 2.f, 180);
	m_button_restart->setTexture(&m_button_textures["restart"]);

	m_button_options->setPosition(m_window.getSize().x / 2.f, 260);
	m_button_options->setTexture(&m_button_textures["options"]);

	m_button_menu->setPosition(m_window.getSize().x / 2.f, 340);
	m_button_menu->setTexture(&m_button_textures["menu"]);

	m_button_exit->setPosition(m_window.getSize().x / 2.f, 420);
	m_button_exit->setTexture(&m_button_textures["exit"]);

	m_scoreClock.pause();
}
void App::loadGameOverMenu()
{
	m_prevState = m_state;
	m_state = GAME_OVER_MENU;

	m_textbox_scoreBoard->setPosition(m_window.getSize().x / 2.f, 120);
	m_textbox_scoreBoard->setTexture(&m_button_textures["score"]);
	m_textbox_score->setPosition(m_window.getSize().x / 2.f, 140);
	m_score += m_game.getTotalKillCount() * 100;
	std::string score = std::to_string(m_score);
	m_textbox_score->setString(score.substr(0, (int)score.size() - 4));

	m_button_restart->setPosition(m_window.getSize().x / 2.f, 280);
	m_button_restart->setTexture(&m_button_textures["restart"]);

	m_button_menu->setPosition(m_window.getSize().x / 2.f, 360);
	m_button_menu->setTexture(&m_button_textures["menu"]);

	m_button_exit->setPosition(m_window.getSize().x / 2.f, 440);
	m_button_exit->setTexture(&m_button_textures["exit"]);

	m_game.resetKillCount();
	m_scoreClock.pause();
}

void App::loadLevelMenu()
{
	m_prevState = m_state;
	m_state = LEVEL_MENU;

	for (int i = 0; i < m_levels.size(); i++) {
		m_levels[i]->setPosition(128 * (2 * (i % 4 + 1) - 1), 64*(2*((i + 4)/4) - 1));
		m_levels[i]->setTexture(&m_button_textures["levels"]);
	}

	m_game.resetKillCount();

	m_scoreClock.pause();
}

void App::loadOptionsMenu()
{
	m_prevState = m_state;
	m_state = OPTIONS_MENU;

	m_button_back->setPosition(m_window.getSize().x / 2.f, 100);
	m_button_back->setTexture(&m_button_textures["back"]);

	m_button_vol->setPosition(m_window.getSize().x / 2.f - 70, 180);
	if (!m_mute) m_button_vol->setTexture(&m_button_textures["vol"]);
	else m_button_vol->setTexture(&m_button_textures["volMute"]);

	m_slider_vol->setPosition(m_window.getSize().x / 2.f + 28, 180);
	m_textbox_control_left->setPosition(m_window.getSize().x / 2.f - 85, 260);
	m_textbox_control_left->setTexture(&m_button_textures["moveleft"]);
	m_textbox_control_left->setScale(1, 1.4);
	m_textbox_control_right->setPosition(m_window.getSize().x / 2.f - 85, 340);
	m_textbox_control_right->setTexture(&m_button_textures["moveright"]);
	m_textbox_control_right->setScale(1, 1.4);

	m_inputbox_control_left->setPosition(m_window.getSize().x / 2.f + 85, 260);
	m_inputbox_control_right->setPosition(m_window.getSize().x / 2.f + 85, 340);

	m_button_save->setPosition(m_window.getSize().x / 2.f, 420);
	m_button_save->setTexture(&m_button_textures["save"]);

	std::ifstream settingsInput;
	settingsInput.open("data/assets/Settings/settings.txt");
	std::string s;
	while (std::getline(settingsInput, s)) {
		std::stringstream line(s);

		std::string key;
		char val;

		line >> key >> val;

		if (key == "Left")
			m_inputbox_control_left->setString(std::string() + val);

		else if (key == "Right")
			m_inputbox_control_right->setString(std::string() + val);

		m_game.setControlKeys(m_inputbox_control_left->getString()[0], m_inputbox_control_right->getString()[0], 0);
	}

	settingsInput.close();

	m_scoreClock.pause();
}

void App::update()
{
	float dt = m_clock.restart().asSeconds();
	
	m_score = 5*m_scoreClock.getElapsedTime().asSeconds();
	std::string score = std::to_string(m_score);
	m_window.setTitle("Platformer    Score : " + score.substr(0, (int)score.size() - 4));

	switch(m_state){
	case GAME:				m_game.update(dt); break;
	case MAIN_MENU:			m_frame_mainMenu.update(); break;
	case PAUSE_MENU:		m_frame_pauseMenu.update(); break;
	case GAME_OVER_MENU:	m_frame_gameOverMenu.update(); break;
	case LEVEL_MENU:		m_frame_levelMenu.update(); break;
	case OPTIONS_MENU:		m_frame_optionsMenu.update(); break;
	}

	if (m_game.isGameOver() && m_state != GAME_OVER_MENU) {
		m_game.update(dt);
		loadAppState(GAME_OVER_MENU);
	}

	m_game_music.setVolume(m_musicVolume);
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
			case OPTIONS_MENU:	loadAppState(m_prevState); break;
			case LEVEL_MENU:	loadAppState(MAIN_MENU); break;
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

		if (m_inputbox_control_left->getString().size() > 1)
			m_inputbox_control_left->setString(std::string() + (char)toupper(m_inputbox_control_left->getString().erase(0, 1)[0]));
		else if(m_inputbox_control_left->getString().size() == 1) 
			m_inputbox_control_left->setString(std::string() + (char)toupper(m_inputbox_control_left->getString()[0]));

		if (m_inputbox_control_right->getString().size() > 1)
			m_inputbox_control_right->setString(std::string() + (char)toupper(m_inputbox_control_right->getString().erase(0, 1)[0]));
		else if (m_inputbox_control_right->getString().size() == 1)
			m_inputbox_control_right->setString(std::string() + (char)toupper(m_inputbox_control_right->getString()[0]));
		
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
