#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
#include<iostream>
#include<unordered_map>
#include<map>
#include"Game.hpp"
#include<ZUI.hpp>
#include<fstream>
#include<sstream>

class App {
public:

	enum class AppState {
		EXIT, MAIN_MENU, GAME, PAUSE_MENU, GAME_OVER_MENU, LEVEL_MENU, OPTIONS_MENU
	};
	using enum AppState;

	/////////////////////////////////////////////////
	/// \brief Default Constructor
	///
	/////////////////////////////////////////////////
	App();

	void loadTextures();

	void setupGUI();

	void loadGame();
	void loadMainMenu();
	void loadPauseMenu();
	void loadGameOverMenu();
	void loadLevelMenu();
	void loadOptionsMenu();

	void loadAppState(AppState state);

	/////////////////////////////////////////////////
	/// \brief update time and App Variables
	///
	/////////////////////////////////////////////////
	void update();

	/////////////////////////////////////////////////
	/// \brief Render to the target
	///
	/////////////////////////////////////////////////
	void render();

	/////////////////////////////////////////////////
	/// \brief check for any events that may have occured since last call
	///
	/////////////////////////////////////////////////
	void pollEvents();

	/////////////////////////////////////////////////
	/// \brief get if the app is running or not / open or closed
	///	
	/// \return true if running, false otherwise
	///
	/////////////////////////////////////////////////
	bool isRunning() const;

	/////////////////////////////////////////////////
	/// \brief get size of the target
	///
	/// \return size of the target
	///
	/////////////////////////////////////////////////
	sf::Vector2f getWinSize() const;

private:
	/////////////////////////////////////////////////
	// Member Data
	/////////////////////////////////////////////////

	sf::RenderWindow m_window;			/// < main target used for event polling and renderer
	bool m_paused;						/// < pause state of the game, true if paused, false otherwise

	Game m_game;						/// < game object
	sf::Clock m_clock;					/// < main clock used for updating relative to time

	AppState m_state;

	sf::Font m_font;

	float m_musicVolume;

	zui::Frame m_frame_mainMenu;
	sf::Sprite m_bgImg_mainMenu;
	sf::Texture m_texture_mainMenu;

	zui::Frame m_frame_pauseMenu;
	zui::Frame m_frame_gameOverMenu;
	zui::Frame m_frame_levelMenu;
	zui::Frame m_frame_optionsMenu;

	// Main Menu Buttons
	zui::TextButton m_button_start;
	zui::TextButton m_button_level;
	zui::TextButton m_button_options;
	zui::TextButton m_button_exit;

	sf::Texture test;

	// Pause Menu Buttons
	zui::TextButton m_button_resume;
	zui::TextButton m_button_restart;
	zui::TextButton m_button_menu;

	// Options Menu Buttons
	zui::TextButton m_button_vol;
	zui::Slider m_slider_vol;

	zui::TextButton m_button_back;
	zui::Textbox m_textbox_control_left;
	zui::Textbox m_textbox_control_right;
	zui::Inputbox m_inputbox_control_left;
	zui::Inputbox m_inputbox_control_right;

	zui::Textbox m_textbox_control_jump; // temp

	zui::TextButton m_button_save;

};