#include "Enemy.hpp"
#include <array>
#include <chrono>

std::vector<std::unique_ptr<Enemy>> Enemy::m_enemies;
sf::FloatRect Enemy::m_viewBox;

Enemy::Enemy(sf::Texture& m_enemy, sf::Texture& deadEnemy) :
	m_isDying(false),
	m_vel(0.1, 0),
	moveDir(-1),
	m_inView(false)
{
	m_hitbox.width = m_hitbox.height = 64;

	m_currAnim.setTexture(m_enemy);
	m_currAnim.setFrameLimit(7);
	m_currAnim.setFrameSize(sf::Vector2i(m_enemy.getSize().x / 8, m_enemy.getSize().y));
	m_currAnim.setTextureRect_allFrames(sf::IntRect(0, 0, m_enemy.getSize().x, m_enemy.getSize().y));
	m_currAnim.setScale({ m_hitbox.width / 400, m_hitbox.height / 400 });
	m_currAnim.setOrigin(428 / 2.f, 454 / 2.f);
	m_currAnim.setPosition(m_hitbox.width * 0.5f, m_hitbox.height * 0.5f);
	m_currAnim.setSpeed(15);
	m_currAnim.reset();

	m_deadAnim.setTexture(deadEnemy);
	m_deadAnim.setFrameLimit(11);
	m_deadAnim.setFrameSize(sf::Vector2i(deadEnemy.getSize().x / 12, deadEnemy.getSize().y));
	m_deadAnim.setTextureRect_allFrames(sf::IntRect(0, 0, deadEnemy.getSize().x, deadEnemy.getSize().y));
	m_deadAnim.setScale({ m_hitbox.width / 400, m_hitbox.height / 460 });
	m_deadAnim.setOrigin(629 / 2.f, 526 / 2.f);
	m_deadAnim.setPosition(m_hitbox.width * 0.5f, m_hitbox.height * 0.5f);
	m_deadAnim.setSpeed(20);
	m_deadAnim.reset();

	m_shape.setFillColor(sf::Color::Transparent);
	m_shape.setSize({ 70, 70 });
	m_shape.setOutlineColor(sf::Color::Green);
	m_shape.setOutlineThickness(-2);
}

Enemy::~Enemy()
{
}

bool Enemy::get_dead() const
{
	return m_isDying;
}

void addEnemy(sf::Vector2f position, sf::Texture& texture, sf::Texture& deadTexture)
{
	Enemy::m_enemies.push_back(std::make_unique<Enemy>(texture, deadTexture));
	Enemy::m_enemies.back()->setPosition(position);
}

void drawAllEnemies(sf::RenderTarget& target, sf::RenderStates states)
{
	for (int i = 0; i < Enemy::m_enemies.size(); i++) {
		if (Enemy::m_enemies[i]->m_inView == false) 
			Enemy::m_enemies[i]->m_inView = Enemy::m_viewBox.contains(Enemy::m_enemies[i]->getPosition());
		if(Enemy::m_enemies[i]->m_inView) Enemy::m_enemies[i]->draw(target, states);
	}
}

void removeAllEnemies()
{
	Enemy::m_enemies.clear();
}

void updateAllEnemies(float dt)
{ 
	for (int i = 0; i < Enemy::m_enemies.size(); i++) {
		if(Enemy::m_enemies[i]->m_inView) 
			Enemy::m_enemies[i]->update(dt);
	}
}

bool areEnemiesColliding(const sf::FloatRect& player)
{
	for (int i = 0; i < Enemy::m_enemies.size(); i++) {
		if (Enemy::m_enemies[i]->getGlobalBounds().intersects(player)) return true;
	}
	return false;
}

void resetAllEnemies()
{
	for (int i = 0; i < Enemy::m_enemies.size(); i++) {
		Enemy::Enemy::m_enemies[i]->m_inView = false;
		Enemy::Enemy::m_enemies[i]->m_isDying = false;
	}
}

void Enemy::die()
{
	m_isDying = true; 
	m_deadAnim.setInverted(m_currAnim.getInverted().x , false);
}

void Enemy::update(float dt)
{
	// Gravity 
	this->m_vel.y += this->m_gravity;
	this->m_vel.y *= this->m_friction;
	
	if (this->m_isDying) {
		m_vel.x = 0;
	}
	move(this->m_vel.x * moveDir, this->m_vel.y);

	// check collison 
	move(-this->m_vel.x * moveDir, -this->m_vel.y);
	{
		for (auto it = Collidable::allCollidables.begin(); it != Collidable::allCollidables.end(); it++) {
			move(this->m_vel.x * moveDir, this->m_vel.y);
			bool haveCollided = areColliding(getGlobalBounds(), **it);
			move(-this->m_vel.x * moveDir, -this->m_vel.y);
			if (!haveCollided) {
				continue;
			}
			else {
				if ((*it)->getID() == Collidable::OBSTACLE) die();
			}

			sf::FloatRect plat = (*it)->getGlobalBounds();
			sf::FloatRect playerBox(getGlobalBounds());

			// 0-top, 1-right, 2-bottom, 3-left
			enum Region { TOP, RIGHT, BOTTOM, LEFT } region;
			region = BOTTOM;

			if (playerBox.top >= plat.top + plat.height)
			{
				if (playerBox.left > plat.left + plat.width)
				{
					sf::Vector2f ratio = this->m_vel / sf::Vector2f(playerBox.left - plat.left - plat.width, playerBox.top - plat.top - plat.height);
					if (ratio.x < ratio.y) region = BOTTOM;
					else region = RIGHT;
				}
				else if (playerBox.left + playerBox.width < plat.left)
				{
					sf::Vector2f ratio = this->m_vel / sf::Vector2f(plat.left - playerBox.left - playerBox.width, playerBox.top - plat.top - plat.height);
					if (ratio.x < ratio.y) region = BOTTOM;
					else region = LEFT;
				}
				else region = BOTTOM;
			}
			else if (playerBox.top + playerBox.height <= plat.top)
			{
				if (playerBox.left > plat.left + plat.width)
				{
					sf::Vector2f ratio = this->m_vel / sf::Vector2f(playerBox.left - plat.left - plat.width, plat.top - playerBox.top - playerBox.height);
					if (ratio.x < ratio.y) region = TOP;
					else region = RIGHT;
				}
				else if (playerBox.left + playerBox.width < plat.left)
				{
					sf::Vector2f ratio = this->m_vel / sf::Vector2f(plat.left - playerBox.left - playerBox.width, plat.top - playerBox.top - playerBox.height);
					if (ratio.x < ratio.y) region = TOP;
					else region = LEFT;
				}
				else region = TOP;
			}

			else if (playerBox.left >= plat.left + plat.width)
			{
				if (playerBox.top + playerBox.height >= plat.top && playerBox.top <= plat.top + plat.height)
					region = RIGHT;
			}
			else if (playerBox.left + playerBox.width <= plat.left)
			{
				if (playerBox.top + playerBox.height >= plat.top && playerBox.top <= plat.top + plat.height)
					region = LEFT;
			}


			if (region == TOP || region == BOTTOM)
			{
				this->m_vel.y = 0;
				if (region == TOP) setPosition(getPosition().x, plat.top - playerBox.height);
				else if (region == BOTTOM) setPosition(getPosition().x, plat.top + plat.height);
			}
			else if (region == LEFT || region == RIGHT)
			{
				/*this->m_vel.x = 0;*/
				if (region == LEFT) {
					setPosition(plat.left - playerBox.width, getPosition().y);
					moveDir = -1;
					m_currAnim.setInverted(false, false);
				}
				else if (region == RIGHT) {
					setPosition(plat.left + plat.width, getPosition().y);
					moveDir = 1;
					m_currAnim.setInverted(true, false);
				}
			}
		}
	}

	move(this->m_vel.x * moveDir, this->m_vel.y);

	if (this->m_isDying) m_deadAnim.animate(dt);
	else m_currAnim.animate(dt);
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) 
{
	states.transform *= getTransform();
	if (m_isDying && m_deadAnim.isAnimationComp()) {
		for (int i = 0; i < m_enemies.size(); i++) {
			if (m_enemies[i].get() == this) m_enemies.erase(m_enemies.begin() + i);
		}
	}
	else {
		if (m_isDying) m_deadAnim.draw(target, states);
		else m_currAnim.draw(target, states);
		//target.draw(m_shape, states);
	}
}

sf::FloatRect Enemy::getLocalBounds() const
{
	return { 0, 0, m_hitbox.width, m_hitbox.height };
}

sf::FloatRect Enemy::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

sf::FloatRect& Enemy::getHitbox()
{
	return m_hitbox;
}