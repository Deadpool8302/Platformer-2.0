#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
#include<iostream>
#include<unordered_map>
#include<map>
#include"Game.hpp"
#include<GUI.hpp>
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

	gui::Frame m_frame_mainMenu;
	sf::Sprite m_bgImg_mainMenu;
	sf::Texture m_texture_mainMenu;

	gui::Frame m_frame_pauseMenu;
	gui::Frame m_frame_gameOverMenu;
	gui::Frame m_frame_levelMenu;
	gui::Frame m_frame_optionsMenu;

	// Main Menu Buttons
	gui::TextButton m_button_start;
	gui::TextButton m_button_level;
	gui::TextButton m_button_options;
	gui::TextButton m_button_exit;

	sf::Texture test;

	// Pause Menu Buttons
	gui::TextButton m_button_resume;
	gui::TextButton m_button_restart;
	gui::TextButton m_button_menu;

	// Options Menu Buttons
	gui::TextButton m_button_vol;
	gui::Slider m_slider_vol;

	gui::TextButton m_button_back;
	gui::Textbox m_textbox_control_left;
	gui::Textbox m_textbox_control_right;
	gui::Inputbox m_inputbox_control_left;
	gui::Inputbox m_inputbox_control_right;

	gui::Textbox m_textbox_control_jump; // temp

	gui::TextButton m_button_save;

};