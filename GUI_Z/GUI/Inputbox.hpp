#pragma once
#include "TextButton.hpp"

namespace gui {

class Inputbox : public TextButton {

public:
	Inputbox(const sf::Vector2f& size = sf::Vector2f(0,0));

	bool isInInputMode() const;


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	////////////////////////////////////////////////////////////
	/// \brief Actions to take upon selection of the object
	///
	////////////////////////////////////////////////////////////
	virtual void activateSelection() override;

	////////////////////////////////////////////////////////////
	/// \brief Actions to take upon deselection of the object
	///
	////////////////////////////////////////////////////////////
	virtual void deactivateSelection() override;

private:

	void updateProperties(bool shouldHighlight);

	bool m_input_mode;
	bool m_highlighted;
};

} // namespace gui
