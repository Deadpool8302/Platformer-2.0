#include "Obstacle.hpp"
#include "Utility.hpp"
#include "Enemy.hpp"

std::vector<std::unique_ptr<Obstacle>> Obstacle::m_obstacles;

Obstacle::Obstacle()
{
}

Obstacle::~Obstacle()
{
}

void addObstacles(const sf::Vector2i& tilePos, const sf::Vector2i& tileCount, const sf::Vector2f& tileSize) {
	Obstacle::m_obstacles.push_back(std::make_unique<Obstacle>());
	Obstacle::m_obstacles.back()->setSize(tileCount * tileSize);
	Obstacle::m_obstacles.back()->setPosition(tilePos * tileSize);
	Obstacle::m_obstacles.back()->setID(Collidable::OBSTACLE);
	//Platform::m_obstacles.back()->loadTileTextures(tileCount, tileSize, tileTextures);
}

void drawAllObstacles(sf::RenderTarget& target, sf::RenderStates states)
{
	for (int i = 0; i < Obstacle::m_obstacles.size(); i++) {
		Obstacle::m_obstacles[i]->draw(target, states);
	}
}

void removeAllObstacles()
{
	Obstacle::m_obstacles.clear();
}
