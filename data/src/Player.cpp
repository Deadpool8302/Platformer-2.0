#include "Player.hpp"

#include "Utility.hpp"

Player::Player()
	: m_grounded(false)
{
	m_currState = IDLE;
	m_hitbox.width = m_hitbox.height = 70;

	m_shape.setFillColor(sf::Color::Transparent);
	m_shape.setSize({70, 70 });

	m_shape.setOutlineColor(sf::Color::Green);
	m_shape.setOutlineThickness(-2);

	m_ninja.loadFromFile("data/assets/images/ninja.png");

	m_currAnim.setTexture(m_ninja);
	m_currAnim.setFrameLimit(9);
	m_currAnim.setFrameSize(sf::Vector2i(m_ninja.getSize().x / 10, m_ninja.getSize().y / 2));
	m_currAnim.setTextureRect_allFrames(sf::IntRect(0, 0, m_ninja.getSize().x , m_ninja.getSize().y / 2));
	m_currAnim.setScale({ m_hitbox.width / 363, m_hitbox.height / 458 });
	m_currAnim.setOrigin(363/2.f, 458/2.f);
	m_currAnim.setPosition(m_hitbox.width * 0.5f, m_hitbox.height * 0.5f);
	m_currAnim.setSpeed(20);
	m_currAnim.reset();

	m_acc.x = m_acc.y = m_vel.x = m_vel.y = m_force.x = m_force.y = 0;
}

void Player::reset()
{
	m_currState = IDLE;
	m_grounded = false;
	m_currAnim.reset();

	m_acc.x = m_acc.y = m_vel.x = m_vel.y = m_force.x = m_force.y = 0;
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

void Player::addForce(const sf::Vector2f& magnitude)
{
	// acc += force * unit dir

	m_force.x += magnitude.x;
	m_force.y += magnitude.y;
}

void Player::jump()
{
	m_vel.y = -15;
}

bool Player::isGrounded() const
{
	return m_grounded;
}

void Player::update(float dt)
{
	sf::Vector2f moveDir(0, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir.x = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveDir.x = -1;

	addForce(moveDir * sf::Vector2f(5, 5));

	// reinitialise acceleration
	m_acc.x = 0;
	m_acc.y = m_gravity.y;

	// add force, reinitialise force
	m_acc += m_force;
	m_force = { 0, 0 };

	{ // stop oscillations
		sf::Vector2f initAccDir = unitVec(m_acc);

		m_acc.x -= m_vel.x * m_friction.x; // apply friction wrt to velocity in opposite direction to accelreation

		sf::Vector2f finAccDir = unitVec(m_acc);
		if (initAccDir.x * finAccDir.x < 0)m_acc.x = 0;
	}

	sf::Vector2f velDir(unitVec(m_vel)); // direction of velocity
	m_vel.y = velDir.y * std::min(abs(m_vel.y), m_max_vel.y); // limit velocity to maximum
	m_vel.x = velDir.x * std::min(abs(m_vel.x), m_max_vel.x); // limit velocity to maximum

	sf::Vector2f dis = m_vel + (m_acc * dt * dt) / 2.0f; // calculate displacement

	if (abs(m_vel.x) < 0.015) m_vel.x = 0;
	if (abs(m_vel.y) < 0.015) m_vel.y = 0;

	// new velocity is zero but state was RUNNING
	if (abs(m_vel.x) < 0.1  && m_currState != IDLE)  {
		m_currAnim.setTextureRect_allFrames(sf::IntRect(0, 0, m_ninja.getSize().x , m_ninja.getSize().y / 2));
		m_currAnim.reset();
		m_currState = IDLE;
	}
	// new velocity is non zero but state was IDLE
	else if(abs(m_vel.x) >= 0.1 && m_currState != RUNNING){
		m_currAnim.setTextureRect_allFrames(sf::IntRect(0, m_ninja.getSize().y / 2, m_ninja.getSize().x , m_ninja.getSize().y / 2));
		m_currAnim.reset();
		m_currState = RUNNING;
		m_currAnim.setInverted(m_vel.x < 0, 0);
	}

	m_vel += m_acc * dt; // calculate velocity for next frame

	//check Collision
	{
		for (auto it = Collidable::allCollidables.begin(); it != Collidable::allCollidables.end(); it++) {
			move(dis);
			bool haveCollided = areColliding(getGlobalBounds(), **it);
			move(-dis);
			if (!haveCollided)continue;

			sf::FloatRect plat = (*it)->getGlobalBounds();
			sf::FloatRect playerBox(getGlobalBounds());

			// 0-top, 1-right, 2-bottom, 3-left
			enum Region { TOP, RIGHT, BOTTOM, LEFT } region;

			if (playerBox.top >= plat.top + plat.height)
			{
				if (playerBox.left > plat.left + plat.width)
				{
					sf::Vector2f ratio = dis / sf::Vector2f(playerBox.left - plat.left - plat.width, playerBox.top - plat.top - plat.height);
					if (ratio.x < ratio.y) region = BOTTOM;
					else region = RIGHT;
				}
				else if (playerBox.left + playerBox.width < plat.left)
				{
					sf::Vector2f ratio = dis / sf::Vector2f(plat.left - playerBox.left - playerBox.width, playerBox.top - plat.top - plat.height);
					if (ratio.x < ratio.y) region = BOTTOM;
					else region = LEFT;
				}
				else region = BOTTOM;
			}
			else if (playerBox.top + playerBox.height <= plat.top)
			{
				if (playerBox.left > plat.left + plat.width)
				{
					sf::Vector2f ratio = dis / sf::Vector2f(playerBox.left - plat.left - plat.width, plat.top - playerBox.top - playerBox.height);
					if (ratio.x < ratio.y) region = TOP;
					else region = RIGHT;
				}
				else if (playerBox.left + playerBox.width < plat.left)
				{
					sf::Vector2f ratio = dis / sf::Vector2f(plat.left - playerBox.left - playerBox.width, plat.top - playerBox.top - playerBox.height);
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
				dis.y = 0;
				m_vel.y = 0;
				if (region == TOP) setPosition(getPosition().x, plat.top - playerBox.height);
				else if (region == BOTTOM) setPosition(getPosition().x, plat.top + plat.height);
			}
			else if (region == LEFT || region == RIGHT)
			{
				dis.x = 0;
				m_vel.x = 0;
				if (region == LEFT) setPosition(plat.left - playerBox.width, getPosition().y);
				else if (region == RIGHT) setPosition(plat.left + plat.width, getPosition().y);
			}
		}
	}

	move(dis.x, dis.y);

	m_grounded = m_vel.y == 0;


	m_currAnim.animate(dt);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	m_currAnim.draw(target, states);
	target.draw(m_shape, states);
}


