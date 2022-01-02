#pragma once
#include "TextButton.hpp"

namespace zui {

class Inputbox : public TextButton {

public:

	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// \param size -> size of the box
	/// 
    ////////////////////////////////////////////////////////////
	Inputbox(const sf::Vector2f& size = sf::Vector2f(0,0));

	////////////////////////////////////////////////////////////
	/// \brief Default virtual destrcutor
	///
	////////////////////////////////////////////////////////////
	virtual ~Inputbox();

	////////////////////////////////////////////////////////////
	/// \brief check whether object is in input mode
	///
	/// \return true if in input mode, false otherwise
	/// 
	////////////////////////////////////////////////////////////
	bool isInInputMode() const;

	////////////////////////////////////////////////////////////
	/// \brief Draws the object to the target applying its states
	/// 
	/// \param target -> Render target to draw to
	/// \param states -> Current render states
	/// 
	////////////////////////////////////////////////////////////
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

	////////////////////////////////////////////////////////////
	/// \brief force properties to a state
	/// 
	/// \showHighlight true for selected state, false for original
	///
	////////////////////////////////////////////////////////////
	void updateProperties(bool shouldHighlight);

	bool m_input_mode;					/// < whether object is in input mode
	bool m_highlighted;					/// < whether object is highlighted or not
	mutable int m_blink_counter;		/// < blink oscillator from -blink limit to +blink limit
	int m_blink_limit = 40;				/// < blink time limit, total time taken by blink is 2*blink limit
};


typedef std::unique_ptr<zui::Inputbox> Inputbox_ptr;

} // namespace zui
