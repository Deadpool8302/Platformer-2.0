#pragma once
#include<SFML/Graphics.hpp>

class Animation : public sf::Transformable{
public:
	Animation();

	void setSpeed(int speed);
	void setTexture(sf::Texture& texture);
	void setFrameLimit(int limit);
	void setTextureRect_allFrames(const sf::IntRect& rect);
	void setFrameSize(const sf::Vector2i& size);

	void setInverted(bool in_x_dir, bool in_y_dir);
	sf::Vector2<bool> getInverted() const;

	int getCurrentFrame() const;
	bool isAnimationComp() const;

	void animate(float dt);
	void reset();

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

private:
	sf::Sprite m_img;
	sf::Vector2i m_frameSize;
	sf::IntRect m_frameRect;
	int m_frame;
	int m_frameLimit;
	float m_animSpeed;
	float m_count;

	bool m_AnimComplete;
	sf::Vector2<bool> m_xyFlip;
};