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

void addObstacles(const sf::Vector2i& tilePos, const sf::Vector2i& tileCount, const sf::Vector2f& tileSize, sf::Texture& texture) {
	Obstacle::m_obstacles.push_back(std::make_unique<Obstacle>());
	Obstacle::m_obstacles.back()->setSize(tileCount * tileSize);
	Obstacle::m_obstacles.back()->setPosition(tilePos * tileSize);
	Obstacle::m_obstacles.back()->setID(Collidable::OBSTACLE);
	Obstacle::m_obstacles.back()->m_img.setTexture(texture);
	texture.setRepeated(true);
	Obstacle::m_obstacles.back()->m_img.setTextureRect({ 0, 0, (int)texture.getSize().x * tileCount.x, (int)texture.getSize().y * tileCount.y });
	Obstacle::m_obstacles.back()->m_img.setScale(tileSize/ texture.getSize());
	Obstacle::m_obstacles.back()->m_img.setPosition(tileSize * tilePos);
}

void drawAllObstacles(sf::RenderTarget& target, sf::RenderStates states)
{
	for (int i = 0; i < Obstacle::m_obstacles.size(); i++) {
		target.draw(Obstacle::m_obstacles[i]->m_img, states);
	}
}

void removeAllObstacles()
{
	Obstacle::m_obstacles.clear();
}
