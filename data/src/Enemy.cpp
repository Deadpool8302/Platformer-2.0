#include "Enemy.hpp"
#include <array>
#include <chrono>

std::vector<std::unique_ptr<Enemy>> Enemy::m_enemies;
sf::FloatRect Enemy::m_viewBox;

Enemy::Enemy(sf::Texture& m_enemy, sf::Texture& deadEnemy) :
	m_isDying(false),
	m_vel(0.2, 0),
	moveDir(-1),
	m_inView(false)
{
	m_hitbox.width = m_hitbox.height = 64;

	ver.width = m_hitbox.width;
	ver.height = 5;

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
		 if(Enemy::m_enemies[i]->m_inView) 
			Enemy::m_enemies[i]->draw(target, states);
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
	
	
	// CHECK COLLISION
	boundRectX();
	boundRectY();
	bool m_grounded = false;
	{
		for (auto it = Collidable::allCollidables.begin(); it != Collidable::allCollidables.end(); it++) {
			if (ver.intersects((*it)->getGlobalBounds())) {
				m_grounded = true;
			}

			if (areColliding(horizontal, **it)) {

				if ((*it)->getID() == Collidable::OBSTACLE)
					die();

				if ((*it)->getCollidableSpeed().x < 0) {    // moving left 
					
					if (getPosition().x < (*it)->getGlobalBounds().left + (*it)->getGlobalBounds().width / 2.f) {
						setPosition((*it)->getGlobalBounds().left - m_hitbox.width, getPosition().y);
					}
				}
				else if ((*it)->getCollidableSpeed().x > 0) {    // moving right 
					
					if (getPosition().x > (*it)->getGlobalBounds().left + (*it)->getGlobalBounds().width / 2.f) {
						setPosition((*it)->getGlobalBounds().left + (*it)->getGlobalBounds().width, getPosition().y);
					}
				}

				if (moveDir > 0) {   // moving right
					setPosition((*it)->getGlobalBounds().left - m_hitbox.width - 5, getPosition().y);
					moveDir = -1;
					m_currAnim.setInverted(false, false);
				}
				else if (moveDir < 0) {   // moving left
					setPosition((*it)->getGlobalBounds().left + (*it)->getGlobalBounds().width + 5, getPosition().y);
					moveDir = 1;
					m_currAnim.setInverted(true, false);
				}
			}
			if (areColliding(vertical, **it)) {

				if ((*it)->getID() == Collidable::OBSTACLE)
					die();

				if ((*it)->getCollidableSpeed().y < 0) {    // moving up
					m_vel.y = 0;
					if (getPosition().y < (*it)->getGlobalBounds().top + (*it)->getGlobalBounds().height / 2.f) {
						setPosition(getPosition().x, (*it)->getGlobalBounds().top - m_hitbox.height);
					}
				}
				else if ((*it)->getCollidableSpeed().y > 0) {    // moving down
					m_vel.y = 0;
					if (getPosition().y > (*it)->getGlobalBounds().top + (*it)->getGlobalBounds().height / 2.f) {
						setPosition(getPosition().x, (*it)->getGlobalBounds().top + (*it)->getGlobalBounds().height);
					}
				}

				if (m_vel.y < 0) {    // moving up 
					m_vel.y = 0;
					setPosition(getPosition().x, (*it)->getGlobalBounds().top + (*it)->getGlobalBounds().height);
				}
				else if (m_vel.y > 0) {   // moving down
					m_vel.y = 0;
					setPosition(getPosition().x, (*it)->getGlobalBounds().top - m_hitbox.height);
				}
			}
		}

	}

	if(m_grounded) move(this->m_vel.x * moveDir, this->m_vel.y);
	else move(0, this->m_vel.y);

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
	}

	/*sf::RectangleShape hor, ver, shape;
	shape.setSize({m_hitbox.width, m_hitbox.height});
	shape.setPosition(getPosition());
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(-3);
	hor.setSize({ horizontal.width, horizontal.height });
	ver.setSize({ vertical.width, vertical.height });
	hor.setPosition({horizontal.left, horizontal.top});
	ver.setPosition({ vertical.left, vertical.top });
	target.draw(hor);
	target.draw(ver);
	target.draw(shape);*/
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

void Enemy::boundRectX()
{
	horizontal.width = m_hitbox.width + m_vel.x * moveDir;
	horizontal.height = m_hitbox.height - 12;
	horizontal.left = getPosition().x + m_vel.x * 20 * moveDir;
	horizontal.top = getPosition().y + 6;
}

void Enemy::boundRectY()
{
	vertical.width = m_hitbox.width - 12;
	vertical.height = m_hitbox.height + m_vel.y;
	vertical.left = getPosition().x + 6;
	vertical.top = getPosition().y + (m_vel.y >= 0 ? m_vel.y * 13 : m_vel.y * 8);

	ver.left = getPosition().x;
	ver.top = getPosition().y + getLocalBounds().height;
}