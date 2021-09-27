#include "Button.hpp"

using namespace gui;
Button::Button()
	:Entity(GUI_ID_BUTTON)
{
	actionEvent = ActionEvent::RELEASE;

	m_second_ScaleFactor.x = m_second_ScaleFactor.y = 1;
	m_second_OutlineThickness = 0;
	m_second_FillColor = sf::Color::White;
	m_second_OutlineColor = sf::Color::White;
}

void Button::setPointCount(std::size_t count)
{
	m_shape.setPointCount(count);
}
std::size_t Button::getPointCount() const
{
	return m_shape.getPointCount();
}
void Button::setPoint(std::size_t index, const sf::Vector2f& point)
{
	m_shape.setPoint(index, point);
}
sf::Vector2f Button::getPoint(std::size_t index) const
{
	return m_shape.getPoint(index);
}
void Button::switchProperties()
{
	auto fillColor = m_shape.getFillColor();
	auto outlineColor = m_shape.getOutlineColor();
	auto outlineThickness = m_shape.getOutlineThickness();
	auto scaleFactor = getScale();

	m_shape.setFillColor(m_second_FillColor);
	m_shape.setOutlineColor(m_second_OutlineColor);
	m_shape.setOutlineThickness(m_second_OutlineThickness);
	setScale(m_second_ScaleFactor);

	m_second_FillColor = fillColor;
	m_second_OutlineColor = outlineColor;
	m_second_OutlineThickness = outlineThickness;
	m_second_ScaleFactor = scaleFactor;
}

void Button::setFillColor(const sf::Color& color)
{
	if (m_selected) m_second_FillColor = color;
	else m_shape.setFillColor(color);
}
void Button::setOutlineColor(const sf::Color& color)
{
	if (m_selected) m_second_OutlineColor = color;
	else m_shape.setOutlineColor(color);
}
void Button::setOutlineThickness(float thickness)
{
	if (m_selected) m_second_OutlineThickness = thickness;
	else m_shape.setOutlineThickness(thickness);
}
void Button::setSelectionFillColor(const sf::Color& color)
{
	if (m_selected) m_shape.setFillColor(color);
	else m_second_FillColor = color;
}
void Button::setSelectionOutlineColor(const sf::Color& color)
{
	if (m_selected) m_shape.setOutlineColor(color);
	else m_second_OutlineColor = color;
}
void Button::setSelectionOutlineThickness(float thickness)
{
	if (m_selected) m_shape.setOutlineThickness(thickness);
	else m_second_OutlineThickness = thickness;
}
void Button::setSelectionScale(const sf::Vector2f& scaleFactor)
{
	if (m_selected) setScale(scaleFactor);
	else m_second_ScaleFactor = scaleFactor;
}
void Button::setTexture(const sf::Texture* texture, bool resetRect)
{
	m_shape.setTexture(texture, resetRect);
}
void Button::setTextureRect(const sf::IntRect& rect)
{
	m_shape.setTextureRect(rect);
}
sf::Color gui::Button::getFillColor()
{
	if (m_selected) return m_second_FillColor;
	else return m_shape.getFillColor();
}
sf::Color gui::Button::getOutlineColor()
{
	if (m_selected) return m_second_OutlineColor;
	else return m_shape.getOutlineColor();
}
float gui::Button::getOutlineThickness()
{
	if (m_selected) return m_second_OutlineThickness;
	else return m_shape.getOutlineThickness();
}
sf::Color gui::Button::getSelectionFillColor()
{
	if (!m_selected) return m_second_FillColor;
	else return m_shape.getFillColor();
}
sf::Color gui::Button::getSelectionOutlineColor()
{
	if (!m_selected) return m_second_OutlineColor;
	else return m_shape.getOutlineColor();
}
float gui::Button::getSelectionOutlineThickness()
{
	if (!m_selected) return m_second_OutlineThickness;
	else return m_shape.getOutlineThickness();
}
sf::Vector2f gui::Button::getSelectionScale()
{
	if (!m_selected) return m_second_ScaleFactor;
	else return getScale();
}
const sf::Texture* Button::getTexture() const
{
	return m_shape.getTexture();
}
const sf::IntRect& Button::getTextureRect() const
{
	return m_shape.getTextureRect();
}
sf::FloatRect Button::getLocalBounds() const
{
	return m_shape.getLocalBounds();
}
sf::FloatRect Button::getGlobalBounds() const
{
	return getTransform().transformRect(m_shape.getGlobalBounds());
}
bool Button::contains(const sf::Vector2f& point) const
{
	return getLocalBounds().contains(getInverseTransform().transformPoint(point));
}
Entity* Button::isHit(const sf::Vector2f& point)
{
	if (isActive()) {
		return contains(point) ? this : nullptr;
	}
}
void Button::activateSelection()
{
	if(!m_selected) switchProperties();
	m_selected = true;
}
void Button::deactivateSelection()
{
	if (m_selected) switchProperties();
	m_selected = false;
}
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive()) {
		states.transform *= getTransform();
		target.draw(m_shape, states);
	}
}