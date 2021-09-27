#include "TextButton.hpp"

using namespace gui;

TextButton::TextButton(const sf::Vector2f size, int class_id)
	:Textbox(size, class_id)
{
	actionEvent = ActionEvent::RELEASE;

	m_second_ScaleFactor.x = m_second_ScaleFactor.y = 1;
	m_second_OutlineThickness = 0;
	m_second_FillColor = sf::Color::White;
	m_second_OutlineColor = sf::Color::White;
}

TextButton::TextButton(const sf::Vector2f& size):
	Textbox(size, GUI_ID_TEXTBUTTON)
{
	actionEvent = ActionEvent::RELEASE;
	
	m_second_ScaleFactor.x = m_second_ScaleFactor.y = 1;
	m_second_OutlineThickness = 0;
	m_second_FillColor = sf::Color::White;
	m_second_OutlineColor = sf::Color::White;
}

TextButton::~TextButton()
{
}

void TextButton::switchProperties()
{
	auto fillColor = m_box.getFillColor();
	auto outlineColor = m_box.getOutlineColor();
	auto outlineThickness = m_box.getOutlineThickness();
	auto scaleFactor = getScale();

	m_box.setFillColor(m_second_FillColor);
	m_box.setOutlineColor(m_second_OutlineColor);
	m_box.setOutlineThickness(m_second_OutlineThickness);
	setScale(m_second_ScaleFactor);

	m_second_FillColor = fillColor;
	m_second_OutlineColor = outlineColor;
	m_second_OutlineThickness = outlineThickness;
	m_second_ScaleFactor = scaleFactor;
}

void TextButton::setFillColor(const sf::Color& color)
{
	if (m_selected) m_second_FillColor = color;
	else m_box.setFillColor(color);
}
void TextButton::setOutlineColor(const sf::Color& color)
{
	if (m_selected) m_second_OutlineColor = color;
	else m_box.setOutlineColor(color);
}
void TextButton::setOutlineThickness(float thickness)
{
	if (m_selected) m_second_OutlineThickness = thickness;
	else m_box.setOutlineThickness(thickness);
}
void TextButton::setSelectionFillColor(const sf::Color& color)
{
	if (m_selected) m_box.setFillColor(color);
	else m_second_FillColor = color;
}
void TextButton::setSelectionOutlineColor(const sf::Color& color)
{
	if (m_selected) m_box.setOutlineColor(color);
	else m_second_OutlineColor = color;
}
void TextButton::setSelectionOutlineThickness(float thickness)
{
	if (m_selected) m_box.setOutlineThickness(thickness);
	else m_second_OutlineThickness = thickness;
}
void TextButton::setSelectionScale(const sf::Vector2f& scaleFactor)
{
	if (m_selected) setScale(scaleFactor);
	else m_second_ScaleFactor = scaleFactor;
}
void TextButton::activateSelection()
{
	if (!m_selected) switchProperties();
	m_selected = true;
}
void TextButton::deactivateSelection()
{
	if (m_selected) switchProperties();
	m_selected = false;
}



