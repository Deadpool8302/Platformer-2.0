#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <array>
#include <memory>
#include "Platform.hpp"

class Obstacle : public Platform
{
public:
	/////////////////////////////////////////////////
	/// \brief Default Constructor
	///
	/////////////////////////////////////////////////
	Obstacle();

	/////////////////////////////////////////////////
	/// \brief Default Destructor
	///
	/////////////////////////////////////////////////
	~Obstacle();

private:
	friend void addObstacles(const sf::Vector2i& tilePos, const sf::Vector2i& tileCount, const sf::Vector2f& tileSize); // to do => textures of obstacles 
	friend void drawAllObstacles(sf::RenderTarget& target, sf::RenderStates states);
	friend void removeAllObstacles();

	static std::vector<std::unique_ptr<Obstacle>> m_obstacles;
};

void addObstacles(const sf::Vector2i& tilePos, const sf::Vector2i& tileCount, const sf::Vector2f& tileSize);
void removeAllObstacles();
void drawAllObstacles(sf::RenderTarget& target, sf::RenderStates states);