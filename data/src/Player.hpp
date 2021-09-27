#pragma once
#include<SFML/Graphics.hpp>
#include"Collision.hpp"
#include "Animation.hpp"
#include <vector>


class Player : public sf::Transformable {
public:

	enum class State { IDLE = 0, RUNNING };
	using enum State;

	Player();

	void reset();

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

	sf::FloatRect& getHitbox();

	void addForce(const sf::Vector2f& magnitude);
	void jump();
	bool isGrounded() const;

	void update(float dt);

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
private:
	State m_currState;
	sf::FloatRect m_hitbox;
	sf::RectangleShape m_shape; // temporary

	sf::Texture m_ninja;
	Animation m_currAnim;

	bool m_grounded;

	const sf::Vector2f m_gravity = { 0.0f, 10.f };
	const sf::Vector2f m_friction = {2.f, 0.0f };
	const sf::Vector2f m_max_vel = {2.0f, 3.0f };
	sf::Vector2f m_force;
	sf::Vector2f m_acc;
	sf::Vector2f m_vel;	
};