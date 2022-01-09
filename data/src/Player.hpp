#pragma once
#include<SFML/Graphics.hpp>
#include "Collision.hpp"
#include "Platform.hpp"
#include "Animation.hpp"
#include "Utility.hpp"
#include "Enemy.hpp"
#include "Obstacle.hpp"
#include <vector>
#include<iostream>

class Player : public sf::Transformable {
public:

	enum class State { IDLE = 0, RUNNING };
	using enum State;

	Player();

	void reset();

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

	sf::FloatRect& getHitbox();

	void jump();
	bool isGrounded() const;

	void update(float dt);

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

	bool isPlayerDead(); 

private:
	State m_currState;
	sf::FloatRect m_hitbox;
	sf::RectangleShape m_shape; // temporary

	sf::Texture m_ninja;
	sf::Texture m_deadNinja;

	Animation m_currAnim;
	Animation m_deadAnim;

	bool m_grounded;
	bool m_playerDead;    // to check wether player is dead
	bool m_playerDying;

	sf::Vector2f m_vel;

	const sf::Vector2f m_max_vel = { 0.2f, 0.7f };
	const sf::Vector2f m_min_vel = { 0.09f, 0.001f };
	const float m_acc = 0.1f;          
	const float m_gravity = 0.002f;
	const sf::Vector2f m_friction = {0.9975f, 1.f };


	// battery saver settings 
	/*const sf::Vector2f m_max_vel = { 0.7f, 0.1f };
	const sf::Vector2f m_min_vel = { 0.4f, 0.7f };
	const float m_acc = 0.45f;
	const float m_gravity = 0.95f;
	const sf::Vector2f m_friction = { 0.9975f, 0.977f };
	sf::Vector2f m_force;*/
	void boundRectX();
	void boundRectY();

	sf::RectangleShape horizontal, vertical;  // temp
		
};