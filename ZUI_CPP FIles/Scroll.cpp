

#include <ZUI/Page.hpp>

using namespace zui;

Page::Scroll::Scroll()
	:Entity(ZUI_ID_SCROLL)
{
	m_length = 0;
	m_place = NONE;

	m_rect.setPointCount(4);
	m_rect.setFillColor(sf::Color(46, 46, 46));
	m_rect.setSelectionFillColor(sf::Color(86, 86, 86));
	m_rect.setSelectionOutlineColor(sf::Color::Transparent);

	m_bar.setPointCount(4);
	m_bar.setFillColor(sf::Color(153, 153, 153));
	m_bar.setSelectionFillColor(sf::Color(191, 191, 191));
	m_bar.setSelectionOutlineColor(sf::Color::Transparent);

	m_rect.actionEvent = ActionEvent::PRESS;
	m_bar.actionEvent = ActionEvent::MOUSEHELD;
}

void Page::Scroll::createScroll(sf::Vector2f region_size, sf::Vector2f max_size, ScrollPlacement place)
{
	sf::Vector2f barSize;

	if (place == LEFT || place == RIGHT) {
		m_length = max_size.y;
		m_region_length = region_size.y;
		m_place = place;
		
		m_rect.setPoint(0, sf::Vector2f(0, 0));
		m_rect.setPoint(1, sf::Vector2f(15, 0));
		m_rect.setPoint(2, sf::Vector2f(15, m_region_length));
		m_rect.setPoint(3, sf::Vector2f(0, m_region_length));

		barSize = sf::Vector2f(15, m_region_length * m_region_length / m_length);

		if (place == RIGHT) m_rect.setPosition(region_size.x - 15, 0);
		
	}
	else if (place == TOP || place == BOTTOM) {
		m_length = max_size.x;
		m_region_length = region_size.x;
		m_place = place;

		m_rect.setPoint(0, sf::Vector2f(0, 0));
		m_rect.setPoint(1, sf::Vector2f(0, 15));
		m_rect.setPoint(2, sf::Vector2f(m_region_length, 15));
		m_rect.setPoint(3, sf::Vector2f(m_region_length, 0));

		barSize = sf::Vector2f(m_region_length * m_region_length / m_length, 15);

		if (place == BOTTOM) m_rect.setPosition(0, region_size.y - 15);
	}

	m_bar.setPoint(0, sf::Vector2f(0, 0));
	m_bar.setPoint(1, sf::Vector2f(barSize.x, 0));
	m_bar.setPoint(2, barSize);
	m_bar.setPoint(3, sf::Vector2f(0, barSize.y));

	m_bar.setPosition(m_rect.getPosition());
}

sf::Vector2f Page::Scroll::getSize() const
{
	return{ m_rect.getLocalBounds().width, m_rect.getLocalBounds().height };
}

sf::Vector2f Page::Scroll::mapPosition(const sf::Vector2f& position)
{
	if (m_place == LEFT || m_place == RIGHT)
		return sf::Vector2f(0.f, position.y * m_length / m_region_length);
	if (m_place == TOP || m_place == BOTTOM)
		return sf::Vector2f(position.x * m_length / m_region_length, 0.f);
}

void Page::Scroll::mapBarPosition(const sf::Vector2f& position)
{
	m_bar.setPosition(m_rect.getPosition() + mapPosition(position));
}

sf::Vector2f Page::Scroll::scrollTo(const sf::Vector2f& position)
{
	return scrollBy(position - m_bar.getPosition() - sf::Vector2f(m_bar.getLocalBounds().width, m_bar.getLocalBounds().height) * 0.5f);
}

sf::Vector2f Page::Scroll::scrollBy(const sf::Vector2f& offset)
{
	sf::Vector2f oldPos = m_bar.getPosition();
	sf::Vector2f newPos = oldPos + offset;

	// check if in range and return total offset moved
	if (m_place == TOP || m_place == BOTTOM) {
		newPos.x = std::max(m_rect.getPosition().x, newPos.x);
		newPos.x = std::min(m_rect.getPosition().x + m_rect.getLocalBounds().width - m_bar.getLocalBounds().width, newPos.x);
		
		m_bar.setPosition(newPos.x, oldPos.y);
		return sf::Vector2f(newPos.x - oldPos.x, 0);
	}
	else if(m_place == LEFT || m_place == RIGHT) {
		newPos.y = std::max(m_rect.getPosition().y, newPos.y);
		newPos.y = std::min(m_rect.getPosition().y + m_rect.getLocalBounds().height - m_bar.getLocalBounds().height, newPos.y);
		
		m_bar.setPosition(oldPos.x, newPos.y);
		return sf::Vector2f(0, newPos.y - oldPos.y);
	}
}

bool Page::Scroll::contains(const sf::Vector2f& point) const
{
	return m_rect.contains(point);
}

Entity* Page::Scroll::isHit(const sf::Vector2f& point)
{
	return m_bar.contains(point) ? &m_bar : m_rect.isHit(point);
}

void Page::Scroll::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive()) {
		m_rect.draw(target, states);
		m_bar.draw(target, states);
	}
}

void Page::Scroll::activateSelection()
{
	m_selected = true;
}

void Page::Scroll::deactivateSelection()
{
	m_selected = false;
}