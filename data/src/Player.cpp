#include "Player.hpp"

Player::Player()
	: m_grounded(false), m_playerDead(false), m_playerDying(false)
{
	m_currState = IDLE;
	m_hitbox.width = m_hitbox.height = 64;

	m_shape.setFillColor(sf::Color::Transparent);
	m_shape.setSize({70, 70 });
	m_shape.setOutlineColor(sf::Color::Green);
	m_shape.setOutlineThickness(-2);

	m_ninja.loadFromFile("data/assets/images/ninja.png");
	m_deadNinja.loadFromFile("data/assets/images/dead_ninja.png");

	m_currAnim.setTexture(m_ninja);
	m_currAnim.setFrameLimit(9);
	m_currAnim.setFrameSize(sf::Vector2i(m_ninja.getSize().x / 10, m_ninja.getSize().y / 2));
	m_currAnim.setTextureRect_allFrames(sf::IntRect(0, 0, m_ninja.getSize().x , m_ninja.getSize().y / 2));
	m_currAnim.setScale({ m_hitbox.width / 300, m_hitbox.height / 400 });
	m_currAnim.setOrigin(363/2.f, 458/2.f);
	m_currAnim.setPosition(m_hitbox.width * 0.5f, m_hitbox.height * 0.5f);
	m_currAnim.setSpeed(20);
	m_currAnim.reset();

	m_deadAnim.setTexture(m_deadNinja);
	m_deadAnim.setFrameLimit(9);
	m_deadAnim.setFrameSize(sf::Vector2i(m_deadNinja.getSize().x / 10, m_deadNinja.getSize().y));
	m_deadAnim.setTextureRect_allFrames(sf::IntRect(0, 0, m_deadNinja.getSize().x, m_deadNinja.getSize().y));
	m_deadAnim.setScale({ m_hitbox.width / 300, m_hitbox.height / 400 });
	m_deadAnim.setOrigin(482 / 2.f, 498 / 2.f);
	m_deadAnim.setPosition(m_hitbox.width * 0.5f, m_hitbox.height * 0.5f);
	m_deadAnim.setSpeed(10);
	m_deadAnim.reset();

	m_vel = { 0, 0 };
}

void Player::reset()
{
	m_playerDead = false;
	m_playerDying = false;
	m_currState = IDLE;
	m_grounded = false;
	m_currAnim.reset();
	m_currAnim.setTexture(m_ninja);
	m_deadAnim.reset();
	m_deadAnim.setTexture(m_deadNinja);

	m_vel = { 0,0 };
}

sf::FloatRect Player::getLocalBounds() const
{
	return {0, 0, m_hitbox.width, m_hitbox.height};
}

sf::FloatRect Player::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

sf::FloatRect& Player::getHitbox()
{
	return m_hitbox;
}

void Player::jump()
{
	m_vel.y = -1.5;
}

bool Player::isGrounded() const
{
	return m_grounded;
}

void Player::update(float dt)
{
	sf::Vector2f moveDir(0, 1);

	// Gravity 
	this->m_vel.y += moveDir.y * this->m_gravity;

	if (!m_playerDying && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		moveDir.x = 1;
		this->m_vel.x += moveDir.x * this->m_acc; 
		if (std::abs(this->m_vel.x) > this->m_max_vel.x) {
			this->m_vel.x = this->m_max_vel.x * ((this->m_vel.x < 0.f) ? -1.f : 1.f);
		}
	}
	if (!m_playerDying && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		moveDir.x = -1;
		this->m_vel.x += moveDir.x * this->m_acc;
		if (std::abs(this->m_vel.x) > this->m_max_vel.x) {
			this->m_vel.x = this->m_max_vel.x * ((this->m_vel.x < 0.f) ? -1.f : 1.f);
		}
	}

	if (m_playerDying) m_vel.x = 0;

	// Deaccelerating
	this->m_vel *= this->m_friction.x;
	if (!m_playerDying && std::abs(this->m_vel.x) < this->m_min_vel.x) this->m_vel.x = 0.f;
	if (std::abs(this->m_vel.y) < this->m_min_vel.y) this->m_vel.y = 0.f;

	
	//std::cout << "player : " <<  this->m_vel.y << " ";

	move(this->m_vel);

	// new velocity is zero but state was RUNNING
	if (abs(m_vel.x) < 0.1 && m_currState != IDLE) {
		m_currAnim.setTextureRect_allFrames(sf::IntRect(0, 0, m_ninja.getSize().x, m_ninja.getSize().y / 2));
		m_currAnim.reset();
		m_currState = IDLE;
	}
	// new velocity is non zero but state was IDLE
	else if (abs(m_vel.x) >= 0.1) {
		m_currAnim.setTextureRect_allFrames(sf::IntRect(0, m_ninja.getSize().y / 2, m_ninja.getSize().x, m_ninja.getSize().y / 2));
		m_currState = RUNNING;
		m_currAnim.setInverted(m_vel.x < 0, 0);
	}

	// checking collison for red platforms 
	for (auto it = Platform::m_redPlatforms.begin(); it != Platform::m_redPlatforms.end(); it++) {
		if (areColliding(getGlobalBounds(), **it)) {
			m_playerDying = true;
		}
	}
		
	//check Collision
	move(-this->m_vel);
	{
		for (auto it = Enemy::m_enemies.begin(); it != Enemy::m_enemies.end(); it++) {
			move(this->m_vel);
			bool haveCollided = (*it)->getGlobalBounds().intersects(this->getGlobalBounds()) && (!(*it)->get_dead()) && !m_playerDying;
			move(-this->m_vel);
			if (!haveCollided)continue;
			
			sf::FloatRect enemy = (*it)->getGlobalBounds();
			sf::FloatRect playerBox(getGlobalBounds());

			bool regionTOP = false;
			if (playerBox.top + playerBox.height <= enemy.top)
			{
				if (playerBox.left > enemy.left + enemy.width)
				{
					sf::Vector2f ratio = this->m_vel / sf::Vector2f(playerBox.left - enemy.left - enemy.width, enemy.top - playerBox.top - playerBox.height);
					if (ratio.x < ratio.y) regionTOP = true;
				}
				else if (playerBox.left + playerBox.width < enemy.left)
				{
					sf::Vector2f ratio = this->m_vel / sf::Vector2f(enemy.left - playerBox.left - playerBox.width, enemy.top - playerBox.top - playerBox.height);
					if (ratio.x < ratio.y) regionTOP = true;
				}
				else regionTOP = true;
			}
			if (regionTOP) {
				m_vel.y = -1.0f;
				(*it)->die();
			}
			else {
				m_playerDying = true;
			}
		}
		for (auto it = Collidable::allCollidables.begin(); it != Collidable::allCollidables.end(); it++) {
			move(this->m_vel);
			bool haveCollided = areColliding(getGlobalBounds(), **it);
			move(-this->m_vel);
			if (!haveCollided)continue;

			sf::FloatRect plat = (*it)->getGlobalBounds();
			sf::FloatRect playerBox(getGlobalBounds());

			// 0-top, 1-right, 2-bottom, 3-left
			enum Region { TOP, RIGHT, BOTTOM, LEFT } region;
			//region = BOTTOM;

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
				this->m_vel.x = 0;
				if (region == LEFT) setPosition(plat.left - playerBox.width, getPosition().y);
				else if (region == RIGHT) setPosition(plat.left + plat.width, getPosition().y);
			}
		}
	}

	move(this->m_vel.x, this->m_vel.y);
	
	m_grounded = m_vel.y == 0;  // correction 

	if (m_playerDying) {
		m_deadAnim.animate(dt);
		if (m_deadAnim.isAnimationComp()) m_playerDead = true;
	}
	else m_currAnim.animate(dt);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	states.transform *= getTransform();
	if (m_playerDying) m_deadAnim.draw(target, states);
	else m_currAnim.draw(target, states);
	//target.draw(m_shape, states);
	
	
}

bool Player::isPlayerDead()
{
	return m_playerDead;
}
