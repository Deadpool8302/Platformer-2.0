

#include <ZUI/ZUIFrame.hpp>
#include <ZUI/Page.hpp>
#include <ZUI/Slider.hpp>
#include <ZUI/Textbox.hpp>
#include <ZUI/TextButton.hpp>
#include <ZUI/Dropdown.hpp>
#include <ZUI/Inputbox.hpp>

using namespace zui;

// set the initial count to 0
uint64_t Entity::item_count = 0;

Entity::Entity(uint64_t class_id)
{
	/*
	* generate unique id
	* first 8 bits -> class id
	* last 24 bits item count at the time of construction of entity
	*/
	m_id = (class_id << 24) + ++item_count;

	// set defaults
	m_selected = false;
	action = nullptr;
	actionEvent = ActionEvent::NONE;
	m_active = true;
	m_functionalParent = nullptr;
}

Entity::Entity(const Entity& entity)
{
	m_id = (getClassID(entity) << 24) + ++item_count;

	if(getClassID(entity) != ZUI_ID_INPUTBOX)
		action = entity.action;

	m_selected = entity.m_selected;
	actionEvent = entity.actionEvent;
	m_active = entity.m_active;
	m_functionalParent = entity.m_functionalParent;

	setPosition(entity.getPosition());
	setScale(entity.getScale());
	setRotation(entity.getRotation());
	setOrigin(entity.getOrigin());
}

Entity& Entity::operator=(const Entity& entity)
{
	if (getClassID(entity) != ZUI_ID_INPUTBOX)
		action = entity.action;

	m_selected = entity.m_selected;
	actionEvent = entity.actionEvent;
	m_active = entity.m_active;
	m_functionalParent = entity.m_functionalParent;

	setPosition(entity.getPosition());
	setScale(entity.getScale());
	setRotation(entity.getRotation());
	setOrigin(entity.getOrigin());

	return *this;
}


Entity::~Entity()
{

}
uint64_t Entity::getID() const
{
	return m_id;
}
uint64_t Entity::getClassID(const Entity& entity)
{
	// get the 8 most significan bits
	return entity.getID() >> 24;
}
Functional* zui::Entity::getFunctionalParent() const
{
	return m_functionalParent;
}
bool zui::Entity::isSelected() const
{
	return m_selected;
}
void Entity::setActive()
{
	m_active = true;
}
void Entity::setInactive()
{
	m_active = false;
}
bool Entity::isActive() const
{
	return m_active;
}
void Entity::callAction() const
{
	if(isActive())
		action();
}
bool Entity::hasAction() const
{
	return action != nullptr;
}
void Entity::setAction(std::function<void()> func)
{
	action = func;
}

bool zui::Entity::hasFunctionalParent() const
{
	return m_functionalParent != nullptr;
}

void Entity::setFunctionalParentForSubVariables(Functional* parent)
{

}

void zui::setFunctionalParent(Entity& entity, Functional* parent)
{
	entity.m_functionalParent = parent;

	entity.setFunctionalParentForSubVariables(parent);
}


bool operator==(const Entity& first, const Entity& second)
{
	return first.getID() == second.getID();
}

bool operator!=(const Entity& first, const Entity& second)
{
	return first.getID() != second.getID();
}

template <typename T>
bool isEntityOfType(Entity* entity) {
	return false;
}
template<> bool isEntityOfType<Textbox>(Entity* entity) {
	return Entity::getClassID(*entity) == ZUI_ID_TEXTBOX;
}
template<> bool isEntityOfType<Button>(Entity* entity) {
	return Entity::getClassID(*entity) == ZUI_ID_BUTTON;
}
template<> bool isEntityOfType<TextButton>(Entity* entity) {
	return Entity::getClassID(*entity) == ZUI_ID_TEXTBUTTON;
}
template<> bool isEntityOfType<Slider>(Entity* entity) {
	return Entity::getClassID(*entity) == ZUI_ID_SLIDER;
}
template<> bool isEntityOfType<Inputbox>(Entity* entity) {
	return Entity::getClassID(*entity) == ZUI_ID_INPUTBOX;
}
template<> bool isEntityOfType<Page>(Entity* entity) {
	return Entity::getClassID(*entity) == ZUI_ID_PAGE;
}
template<> bool isEntityOfType<Dropdown>(Entity* entity) {
	return Entity::getClassID(*entity) == ZUI_ID_DROPDOWN;
}
template<> bool isEntityOfType<Scroll>(Entity* entity) {
	return Entity::getClassID(*entity) == ZUI_ID_SCROLL;
}


Frame::Frame()
{
	// set defaults
	m_window = nullptr;
	m_mouseHoveringOn = nullptr;
	m_clicked = nullptr;
	m_navigator = -1;

	m_functional_object = FunctionalObject::FRAME;
}
Frame::~Frame()
{

}

void Frame::setWindow(sf::RenderWindow& window)
{
	m_window = &window;
}

void Frame::addEntity(Entity& entity)
{
	// insert in map
	m_entityMap.insert(std::make_pair(entity.getID(), &entity));

	if(isEntityOfType<Page>(&entity)){
		m_functionalParents.insert(std::make_pair(entity.getID(), (Functional*)((Page*)(&entity))));
	}
	if(isEntityOfType<Dropdown>(&entity)){
		m_functionalParents.insert(std::make_pair(entity.getID(), (Functional*)((Dropdown*)(&entity))));
	}
	
	setFunctionalParent(entity, this);
}

void Frame::addEntity(Entity* entity)
{
	addEntity(*entity);
}

void Frame::removeEntity(Entity& entity)
{
	removeEntity(entity.getID());
}

void Frame::removeEntity(Entity* entity)
{
	removeEntity(entity->getID());
}

void Frame::removeEntity(uint64_t id)
{
	// erase entity from map
	m_entityMap.erase(id);
}

void Frame::push_in_navigationOrder(Entity& entity) 
{
	m_navigationOrder.push_back(&entity);
}

Entity* Frame::getByID(uint64_t id) const
{
	return m_entityMap.at(id);
}
sf::Vector2f Frame::getMousePosition() const
{
	return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
}
sf::Vector2f Frame::getLastMouseOffset() const
{
	return getMousePosition() - m_lastMousePos;
}
bool Frame::contains(const sf::Vector2f& point) const
{
	return getLocalBounds().contains(point);
}
sf::FloatRect Frame::getLocalBounds() const
{
	return sf::FloatRect(0, 0, m_window->getSize().x, m_window->getSize().y);
}
void Frame::update()
{
	// only work if window has focus
	if (m_window->hasFocus()) {
		// only work if mouse is inside the window
		if(!contains(getMousePosition())){
			if (isMouseOverSomething())m_mouseHoveringOn->deactivateSelection();
			m_mouseHoveringOn = nullptr;
			m_clicked = nullptr;
		}
		else {
			// if mouse is moved while being held on something
			if (wasSomethingClicked() && m_clicked->actionEvent == Entity::ActionEvent::MOUSEHELD && m_clicked->hasAction()) {
				// if functional parent is frame or nullptr then object does not depend bounds (control by other entities) 
				if(!m_clicked->hasFunctionalParent() || m_clicked->getFunctionalParent()->getFunctionalFrame() == FunctionalObject::FRAME)
					m_clicked->callAction();
				
				// if functional parent is page then its bounds are decided by the local bounds visible on the functional parent of its functional parent
				else if(m_clicked->getFunctionalParent()->getFunctionalFrame() == FunctionalObject::PAGE &&
						m_clicked->getFunctionalParent()->getLocalBounds().contains(
							((Page*)(m_clicked->getFunctionalParent()))->getInverseTransform().transformPoint(
								((Page*)(m_clicked->getFunctionalParent()))->getFunctionalParent()->getMousePosition()
							)
						)
					) 
					m_clicked->callAction();

				// if functional parent is dropdown then its bounds are decided by the local bounds visible on the functional parent of its functional parent
				else if (m_clicked->getFunctionalParent()->getFunctionalFrame() == FunctionalObject::DROPDOWN &&
						m_clicked->getFunctionalParent()->getLocalBounds().contains(
							((Dropdown*)(m_clicked->getFunctionalParent()))->getInverseTransform().transformPoint(
								((Dropdown*)(m_clicked->getFunctionalParent()))->getFunctionalParent()->getMousePosition()
							)
						)
					)
					m_clicked->callAction();

				// if out of bounds
				else {
					m_mouseHoveringOn->deactivateSelection();
					m_clicked = nullptr;
					m_mouseHoveringOn = nullptr;
				}
			}
			else {
				// search through all entities if mouse is in window
				Entity* currentMouseHoveringOn = nullptr;
				for (auto it = m_entityMap.begin(); it != m_entityMap.end() && currentMouseHoveringOn == nullptr; it++) {
					currentMouseHoveringOn = it->second->isHit(getMousePosition());
				}

				//if entity has action event as mouse hover
				if (currentMouseHoveringOn != nullptr && currentMouseHoveringOn->actionEvent == Entity::ActionEvent::MOUSEHOVER && currentMouseHoveringOn->hasAction())
					currentMouseHoveringOn->action();

				// if mouse leaves previously pointed entity
				if (m_mouseHoveringOn != currentMouseHoveringOn)
				{
					if (isMouseOverSomething()){
						// not selected by mouse but selected by navigator
						if(m_navigator == -1 || *getNavigatedEntity() != *m_mouseHoveringOn)
							m_mouseHoveringOn->deactivateSelection();
					}
					m_mouseHoveringOn = currentMouseHoveringOn;

					if (isMouseOverSomething()) {
						m_mouseHoveringOn->activateSelection();
					}
				}
			}
		}

		//update last mouse pos
		m_lastMousePos = getMousePosition();
	}
}
bool Frame::pollEvents(sf::Event e)
{
	// if an entity is clicked
	if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {

		if (m_navigator != -1){
			if (!isMouseOverSomething() || *getNavigatedEntity() != *m_mouseHoveringOn) {
				getNavigatedEntity()->deactivateSelection(); // deactivate navigated entity
			}
			m_navigator = -1; // necessary
		}

		// if inputbox then trigger action to turn off cursor
		if (wasSomethingClicked() && isEntityOfType<Inputbox>(m_clicked))
			m_clicked->callAction();

		m_clicked = m_mouseHoveringOn;
		
		// trigger entity's action
		if (wasSomethingClicked() && m_clicked->actionEvent == Entity::ActionEvent::PRESS && m_clicked->hasAction())
			m_clicked->callAction();

		return true; // event occurred
	}
	// if an entity is released
	else if (e.type == sf::Event::MouseButtonReleased) {

		if (m_navigator != -1) {
			if (!isMouseOverSomething() || *getNavigatedEntity() != *m_mouseHoveringOn) { 
				getNavigatedEntity()->deactivateSelection(); // deactivate navigated entity
			}
			m_navigator = -1; // necessary
		}

		// trigger action
		if (wasSomethingClicked() && m_clicked == m_mouseHoveringOn && e.mouseButton.button == sf::Mouse::Left){
			if (m_clicked->actionEvent == Entity::ActionEvent::RELEASE && m_clicked->hasAction()) {
				m_clicked->callAction();
			}
		}

		// inputboxes whose input is enabled dont lose thier click unless something else is clicked
		if (wasSomethingClicked() && !isEntityOfType<Inputbox>(m_clicked)) {
			m_clicked = nullptr;
				
			return true; // event occurred
		}
	}

	else if (e.type == sf::Event::KeyPressed){
		if(e.key.code == sf::Keyboard::Return){ 
			
			// if inputbox was clicked, set clicked to null 
			if (wasSomethingClicked() && isEntityOfType<Inputbox>(m_clicked)) {
				m_clicked->callAction();

				// clicked inputbox must not lose its highlight if it is also the navigated entity
				if (m_navigator != -1 && *getNavigatedEntity() == *m_clicked)
					m_clicked->activateSelection();

				m_clicked = nullptr;
				
				return true; // event occurred
			}

			// else set clicked to navigated entity
			else if (m_navigator != -1) {

				// if inputbox then trigger action to turn off cursor
				if (wasSomethingClicked() && isEntityOfType<Inputbox>(m_clicked))
					m_clicked->callAction();

				m_clicked = getNavigatedEntity();

				// slider is an exception
				if (isEntityOfType<Slider>(m_clicked))
					m_clicked = nullptr;

				return true; // event occurred
			}
		}
		else if (e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right){
			// move slider bar accordingly
			if (m_navigator != -1 && isEntityOfType<Slider>(getNavigatedEntity())){
				float shift = 2.5;
				if (e.key.code == sf::Keyboard::Left) shift *= -1;
				((Slider*)getNavigatedEntity())->shiftOffset(shift);

				return true; // event occurred
			}
		}

		// key navigation 
		else if (e.key.code == sf::Keyboard::Tab || e.key.code == sf::Keyboard::Down || e.key.code == sf::Keyboard::Up) {
			if (m_navigator != -1) {
				if(!isMouseOverSomething() || getNavigatedEntity()->getID() != m_mouseHoveringOn->getID())
					getNavigatedEntity()->deactivateSelection(); // deactivate navigated entity

				// if navigated entity is inputbox and is taking input, disable input
				if (isEntityOfType<Inputbox>(getNavigatedEntity()) && ((Inputbox*)getNavigatedEntity())->isInInputMode()) {
					getNavigatedEntity()->callAction();
				}
			}
			// remove cursor of inputbox
			else if (wasSomethingClicked() && isEntityOfType<Inputbox>(m_clicked))
				m_clicked->callAction();

			if (!m_navigationOrder.empty()) {
				int add = 1;
				if (e.key.code == sf::Keyboard::Up)add = -1;

				m_navigator = (m_navigator + add + m_navigationOrder.size()) % m_navigationOrder.size();

				getNavigatedEntity()->activateSelection();

				m_clicked = nullptr;

				return true; // event occurred
			}
		}
	}
	else if (e.type == sf::Event::KeyReleased){
		if (e.key.code == sf::Keyboard::Return){
			// if enter is pressed trigger action on navigated entity
			if (wasSomethingClicked() && m_navigator != -1 && *m_clicked == *getNavigatedEntity()) {

				if (!isMouseOverSomething() || *getNavigatedEntity() != *m_mouseHoveringOn)
					m_clicked->deactivateSelection(); // deactivate navigated entity

				if(m_clicked->hasAction())
					m_clicked->callAction();

				// set clicked to null, inputbox with input is exception
				if (!isEntityOfType<Inputbox>(m_clicked)) {
					m_clicked = nullptr;
					m_navigator = -1;
				}

				return true; // event occurred
			}
		}
	}

	// if text is entered while textbox is selected
	else if (e.type == sf::Event::TextEntered) {
			if (e.text.unicode != 9 && e.text.unicode != 13 // 9 -> Tab, 13 -> Enter 
				&& wasSomethingClicked() && isEntityOfType<Inputbox>(m_clicked)) {

			Inputbox* inputbox = ((Inputbox*)m_clicked);
			
			char c = e.text.unicode;
			if (c == 8) { // backspace is pressed
				std::string str(inputbox->getString());
				if (str.size() > 0) {
					str.erase(str.size() - 1);
					inputbox->setString(str);
				}
			}
			else inputbox->setString(inputbox->getString() + c); // normal characters or numbers

			return true; // event occurred
		}
	}

	else {
		bool wasEventPolled = false;
		// poll events in pages and dropdowns
		for (auto it = m_functionalParents.begin(); it != m_functionalParents.end() && !wasEventPolled; it++) {
			if (it->second->getFunctionalFrame() != 0 && it->second->contains(getMousePosition()))
				wasEventPolled = it->second->pollEvents(e);
		}

		return wasEventPolled; // return event
	}

	return false; // event did not occur
}

void Frame::draw()
{
	for (auto it = m_entityMap.begin(); it != m_entityMap.end(); it++) {
		it->second->draw(*m_window);
	}
}

bool zui::Frame::isMouseOverSomething() const
{
	return m_mouseHoveringOn != nullptr;
}

bool zui::Frame::wasSomethingClicked() const
{
	return m_clicked != nullptr;
}

Entity* zui::Frame::getClickedEntity() const
{
	return m_clicked;
}

Entity* zui::Frame::getMouseHoveringOnWhichEntity() const
{
	return m_mouseHoveringOn;
}

Entity* zui::Frame::getNavigatedEntity() const
{
	if (m_navigator == -1) return nullptr;
	return m_navigationOrder[m_navigator];
}

Functional::FunctionalObject Functional::getFunctionalFrame()
{
	return m_functional_object;
}
