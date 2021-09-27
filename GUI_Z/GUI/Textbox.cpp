#include "Textbox.hpp"

using namespace gui;

Textbox::Textbox(const sf::Vector2f size, int class_id)
	:Entity(class_id)
{
	m_alignment = TextAlign::CENTER;
	setSize(size);
}

Textbox::Textbox(const sf::Vector2f& size):
	Entity(GUI_ID_TEXTBOX)
{
	m_alignment = TextAlign::CENTER;
	setSize(size);
}

Textbox::~Textbox()
{
}

void Textbox::setSize(const sf::Vector2f& size)
{
	m_box.setSize(size);
}

const sf::Vector2f& Textbox::getSize() const
{
	return m_box.getSize();
}

void Textbox::setString(const std::string& string)
{
	m_text.setString(string);
}
void Textbox::setFont(const sf::Font& font)
{
	m_text.setFont(font);
}
void Textbox::setCharacterSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}
void Textbox::setLineSpacing(float spacingFactor)
{
	m_text.setLineSpacing(spacingFactor);
}
void Textbox::setLetterSpacing(float spacingFactor)
{
	m_text.setLetterSpacing(spacingFactor);
}
void Textbox::setStyle(sf::Uint32 style)
{
	m_text.setStyle(style);
}
void Textbox::setFillColor(const sf::Color& color)
{
	m_box.setFillColor(color);
}
void Textbox::setOutlineColor(const sf::Color& color)
{
	m_box.setOutlineColor(color);
}
void Textbox::setOutlineThickness(float thickness)
{
	m_box.setOutlineThickness(thickness);
}
void Textbox::setTexture(const sf::Texture* texture, bool resetRect)
{
	m_box.setTexture(texture, resetRect);
}
void Textbox::setTextureRect(const sf::IntRect& rect)
{
	m_box.setTextureRect(rect);
}
void Textbox::setTextFillColor(const sf::Color& color)
{
	m_text.setFillColor(color);
}
void Textbox::setTextOutlineColor(const sf::Color& color)
{
	m_text.setOutlineColor(color);
}
void Textbox::setTextOutlineThickness(float thickness)
{
	m_text.setOutlineThickness(thickness);
}
void Textbox::clearString()
{
	m_text.setString("");
}
std::string Textbox::getString() const
{
	return m_text.getString();
}
const sf::Font* Textbox::getFont() const
{
	return m_text.getFont();
}
unsigned int Textbox::getCharacterSize() const
{
	return m_text.getCharacterSize();
}
float Textbox::getLetterSpacing() const
{
	return m_text.getLetterSpacing();
}
float Textbox::getLineSpacing() const
{
	return m_text.getLineSpacing();
}
sf::Uint32 Textbox::getStyle() const
{
	return m_text.getStyle();
}
const sf::Color& Textbox::getFillColor() const
{
	return m_box.getFillColor();
}
const sf::Color& Textbox::getOutlineColor() const
{
	return m_box.getOutlineColor();
}
float Textbox::getOutlineThickness() const
{
	return m_box.getOutlineThickness();
}
const sf::Texture* Textbox::getTexture() const
{
	return m_box.getTexture();
}
const sf::IntRect& Textbox::getTextureRect() const
{
	return m_box.getTextureRect();
}
const sf::Color& Textbox::getTextFillColor() const
{
	return m_text.getFillColor();
}
const sf::Color& Textbox::getTextOutlineColor() const
{
	return m_text.getOutlineColor();
}
float Textbox::getTextOutlineThickness() const
{
	return m_text.getOutlineThickness();
}
sf::Vector2f Textbox::findCharacterPos(std::size_t index) const
{
	return m_text.findCharacterPos(index);
}
sf::FloatRect Textbox::getLocalBounds() const
{
	return { 0, 0, getSize().x, getSize().y };
}
sf::FloatRect Textbox::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}
void Textbox::alignText(TextAlign textAlign)
{
	m_alignment = textAlign;
}

bool Textbox::contains(const sf::Vector2f& point) const
{
	return getLocalBounds().contains(getInverseTransform().transformPoint(point));
}

Entity* Textbox::isHit(const sf::Vector2f& point)
{
	if (isActive()) {
		return contains(point) ? this : nullptr;
	}
	return nullptr;
}
void Textbox::activateSelection()
{
	m_selected = true;
}
void Textbox::deactivateSelection()
{
	m_selected = false;
}

void Textbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive()) {

		// apply tarnsform to box
		states.transform *= getTransform();
		target.draw(m_box, states);

		// apply transform to text
		// since translation is last operation, text is translated w.r.t box
		states.transform.translate(
			(m_box.getSize().x - m_text.getGlobalBounds().width) * 0.5f * (int)m_alignment,
			m_box.getSize().y * 0.5f - m_text.getGlobalBounds().height
		);

		target.draw(m_text, states);
	}
}

