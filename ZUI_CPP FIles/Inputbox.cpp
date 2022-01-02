

#include <ZUI/Inputbox.hpp>

using namespace zui;

Inputbox::Inputbox(const sf::Vector2f& size)
	:TextButton(size, ZUI_ID_INPUTBOX)
{
	actionEvent = ActionEvent::RELEASE;

	m_input_mode = false;
	m_highlighted = false;

	setAction([this]
		{
			m_input_mode = !m_input_mode;

			if (m_input_mode)
				updateProperties(true);

			else if (!m_input_mode && !m_selected) {
				updateProperties(false);
			}
		}
	);

	m_blink_counter = 0;
	m_blink_limit = 40;
}

Inputbox::~Inputbox()
{

}

bool Inputbox::isInInputMode() const
{
	return m_input_mode;
}

void Inputbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
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

		// apply cursor
		sf::String str = m_text.getString();
		if (m_input_mode && m_blink_counter <= m_blink_limit && m_blink_counter >= 0) {
			m_text.setString(str + '|');
		}

		target.draw(m_text, states);

		// remove cursor
		if (m_input_mode) {
			m_text.setString(str);
			m_blink_counter++;
		}
		if (m_blink_counter > m_blink_limit)m_blink_counter = -m_blink_limit;
	}
}

void Inputbox::updateProperties(bool shouldHighlight)
{
	if (shouldHighlight ^ m_highlighted)
		switchProperties();
	
	m_highlighted = shouldHighlight;
}

void Inputbox::activateSelection()
{
	updateProperties(true);
	m_selected = true;
}

void Inputbox::deactivateSelection()
{
	if (!m_input_mode)
		updateProperties(false);
	
	m_selected = false;
}
