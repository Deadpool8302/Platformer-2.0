#pragma once
#include <SFML/Graphics.hpp>
#include "GUI.hpp"

class Renderer {
private:
	sf::RenderWindow window;
	bool running;
	bool paused;


	sf::Font font;
	gui::Frame frame;
	float val;
	gui::Button rectB;
	gui::TextButton button, button2, button3, dbutton1, dbutton2, dbutton3, dbutton4, dbutton5, dbutton6;
	gui::Page page, page2, page3;
	gui::Slider slider;
	gui::Textbox text;
	gui::Dropdown drop;

	void initWindow();
public:
	Renderer();
	~Renderer();

	bool isRunning();

	void update();
	void pollEvents();
	void render();

	sf::Vector2f getMousePosition();
	sf::Vector2f getWinSize();
};
