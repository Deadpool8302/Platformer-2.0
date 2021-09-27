#include "Animation.hpp"

Animation::Animation()
{
	m_xyFlip.x = m_xyFlip.y = false;
	m_frame = 0;
	m_frameLimit = 0;
	m_count = 0.f;
	m_animSpeed = 1;
}

void Animation::setSpeed(int speed)
{
	m_animSpeed = speed;
}

void Animation::setTexture(sf::Texture& texture)
{
	m_img.setTexture(texture);
}

void Animation::setFrameLimit(int limit)
{
	m_frameLimit = limit;
}

void Animation::setTextureRect_allFrames(const sf::IntRect& rect)
{
	m_frameRect = rect;
}

void Animation::setFrameSize(const sf::Vector2i& size)
{
	m_frameSize = size;
}

void Animation::setInverted(bool in_x_dir, bool in_y_dir)
{
	m_xyFlip.x = in_x_dir;
	m_xyFlip.y = in_y_dir;

	setScale(abs(getScale().x) * (m_xyFlip.x ? -1 : 1), abs(getScale().y) * (m_xyFlip.y ? -1 : 1));
}

sf::Vector2<bool> Animation::getInverted() const
{
	return m_xyFlip;
}

void Animation::animate(float dt)
{
	m_count += dt;
	if(m_count * m_animSpeed >= 1.f){
		m_count = 0;
		m_img.setTextureRect({ (m_frameRect.left + m_frameSize.x * m_frame) % m_frameRect.width, m_frameRect.top, m_frameSize.x, m_frameSize.y });
		m_frame++;
		if (m_frame > m_frameLimit) m_frame = 0;
	}
}

void Animation::reset()
{
	m_frame = 0;
	m_count = 0.f;
	m_img.setTextureRect({ (m_frameRect.left + m_frameSize.x * m_frame) % m_frameRect.width, m_frameRect.top, m_frameSize.x, m_frameSize.y });
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_img, states);
}
