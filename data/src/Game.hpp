#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Utility.hpp"
#include <array>
#include <iostream>
#include <algorithm>

class Game
{
public:
	Game();  
	~Game();
	
	void setup(const sf::Vector2f& winSize);

	void setControlKeys(char left, char right, char jump) {}

	void setPaused(bool pause);
	bool isPaused() const;

	bool isOver() const;

	void start();
	void loadLevel(int level);
	void loadSameLevel();
	void loadNextLevel();

	int getTotalLevels();

	bool pollEvents(const sf::Event& event);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

	sf::View m_camView;

	bool isGameOver(); // temp

private:
	void loadTextures();
	void reset();
	
private:

	bool m_paused, m_over;
	int m_curLevel; 
	int m_totalLevels;
	sf::Vector2f m_lvlSize;
	sf::Vector2f m_winSize;
	const sf::Vector2f m_tileSize = { 32, 32 };

	sf::Sprite m_bgImg;
	sf::Texture m_bgTexture;

	Player m_player;

	std::vector<sf::Texture> m_allTextures;
	std::array<sf::Texture, 12> m_tile_textures;
};