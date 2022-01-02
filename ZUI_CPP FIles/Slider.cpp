

#include <ZUI/Slider.hpp>
using namespace zui;

Slider::Slider(const sf::Vector2f& size)
	:Textbox(size, ZUI_ID_SLIDER)
{
	m_offset = 0;
	m_variable = nullptr;
	m_limits[0] = m_limits[1] = 0;

	actionEvent = ActionEvent::MOUSEHELD;
}

Slider::~Slider()
{
}

void Slider::setVariable(float& var, float lower_limit, float upper_limit)
{
	m_variable = &var;
	m_limits[0] = lower_limit;
	m_limits[1] = upper_limit;

	// change the offset if value is between limits
	if ((var >= m_limits[0] && var <= m_limits[1]) || (var >= m_limits[1] && var <= m_limits[0]))
		m_offset = (var - m_limits[0]) * (m_box.getSize().x - m_bar.getLocalBounds().width) / (m_limits[1] - m_limits[0]);
	// else keep it at lower limit
	else var = m_limits[0];

	action = [this] {
		m_offset = getInverseTransform().transformPoint(getFunctionalParent()->getMousePosition()).x;

		shiftOffset(0);
	};
}

void Slider::shiftOffset(float shift)
{
	m_offset += shift;

	// check if offset is in range
	m_offset = std::max(m_offset, 0.f);
	m_offset = std::min(m_offset, m_box.getSize().x - m_bar.getLocalBounds().width);

	// map offset to variable
	*m_variable = m_limits[0] + (m_offset / (m_box.getSize().x - m_bar.getLocalBounds().width)) * (m_limits[1] - m_limits[0]);
}

void Slider::setBarFillColor(sf::Color color)
{
	m_bar.setFillColor(color);
}
void Slider::setBarSelectionColor(sf::Color color)
{
	m_bar.setSelectionFillColor(color);
}
void Slider::setSize(const sf::Vector2f& size)
{
	m_box.setSize(size);

	sf::Vector2f barSize(15, size.y * 0.75);
	m_bar.setPointCount(4);
	m_bar.setPoint(0, sf::Vector2f(0, 0));
	m_bar.setPoint(1, sf::Vector2f(barSize.x, 0));
	m_bar.setPoint(2, sf::Vector2f(barSize.x, barSize.y));
	m_bar.setPoint(3, sf::Vector2f(0, barSize.y));
}
void Slider::reset()
{
	m_offset = 0;
	if (m_variable != nullptr) *m_variable = m_limits[0];
}
Entity* Slider::isHit(const sf::Vector2f& point)
{
	if (isActive()) {
		// if point is inside box return this, nullptr otherwise
		return contains(point) ? this : nullptr;
	}

	return nullptr;
}
void Slider::activateSelection()
{
	m_selected = true;
	m_bar.activateSelection();
}
void Slider::deactivateSelection()
{
	m_selected = false;
	m_bar.deactivateSelection();
}
void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive()) {
		// save transform
		sf::Transform transform = states.transform;

		// apply transform
		states.transform *= getTransform();

		// draw midLine with ranging colors
		float halfWidth = 1.5f;
		sf::Vertex midLine[4] = {
			{states.transform.transformPoint(0, m_box.getSize().y * 0.5f - halfWidth), sf::Color::Green},
			{states.transform.transformPoint(m_box.getSize().x * 0.6f, m_box.getSize().y * 0.5 - halfWidth), sf::Color::Yellow },
			{states.transform.transformPoint(m_box.getSize().x * 0.6f, m_box.getSize().y * 0.5 + halfWidth), sf::Color::Yellow },
			{states.transform.transformPoint(0, m_box.getSize().y * 0.5 + halfWidth), sf::Color::Green}
		};
		target.draw(midLine, 4, sf::Quads);

		midLine[0] = { states.transform.transformPoint(m_box.getSize().x * 0.3f, m_box.getSize().y * 0.5 - halfWidth), sf::Color::Yellow };
		midLine[1] = { states.transform.transformPoint(m_box.getSize().x, m_box.getSize().y * 0.5 - halfWidth), sf::Color::Red };
		midLine[2] = { states.transform.transformPoint(m_box.getSize().x, m_box.getSize().y * 0.5 + halfWidth), sf::Color::Red };
		midLine[3] = { states.transform.transformPoint(m_box.getSize().x * 0.3f, m_box.getSize().y * 0.5 + halfWidth), sf::Color::Yellow };
		target.draw(midLine, 4, sf::Quads);

		// draw box
		target.draw(m_box, states);

		// apply transform to text
		// since translation is last operation, text is translated w.r.t box
		states.transform.translate(
			(m_box.getSize().x - m_text.getGlobalBounds().width) * 0.5f * (int)m_alignment,
			m_box.getSize().y * 0.5f - m_text.getGlobalBounds().height
		);
		//draw text
		target.draw(m_text, states);

		// reuse transform
		states.transform = transform * getTransform();

		//apply transform to bar
		states.transform.translate(m_offset, getSize().y * 0.25 * 0.5);

		//draw bar
		m_bar.draw(target, states);
	}
}