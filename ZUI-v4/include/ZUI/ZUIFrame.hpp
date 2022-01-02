#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>

namespace zui {

////////////////////////////////////////////////////////////
/// ZUI Class Ids 
/// Used for generating entity Id, makes up for the 8 most significant bits of Id 
////////////////////////////////////////////////////////////
constexpr int ZUI_ID_TEXTBOX =		8;
constexpr int ZUI_ID_INPUTBOX =		7;
constexpr int ZUI_ID_SLIDER =		6;
constexpr int ZUI_ID_BUTTON =		5;
constexpr int ZUI_ID_TEXTBUTTON =	4;
constexpr int ZUI_ID_SCROLL =		3;
constexpr int ZUI_ID_DROPDOWN =		2;
constexpr int ZUI_ID_PAGE =			1;

class Functional {
public:

	////////////////////////////////////////////////////////////
	/// \brief Get the mouse position with respect to the current window
	/// 
	////////////////////////////////////////////////////////////
	virtual sf::Vector2f getMousePosition() const = 0;

	////////////////////////////////////////////////////////////
	/// \brief Get the differnce between last and current mouse positions
	/// 
	/// \return Differnce between last and current mouse positions
	///
	////////////////////////////////////////////////////////////
	virtual sf::Vector2f getLastMouseOffset() const = 0;

	////////////////////////////////////////////////////////////
	/// \brief Check if the point is inside the object
	///		   Definition of if the point is inside the object may vary with different zui objects
	/// 
	/// \return true if point is inside the object, false otherwise
	/// 
	/// \param point -> Position of the point
	/// 
	////////////////////////////////////////////////////////////
	virtual bool contains(const sf::Vector2f& point) const = 0;

	////////////////////////////////////////////////////////////
	/// \brief Get the local bounding rectangle of the entity
	///
	/// The returned rectangle is in local coordinates, which means
	/// that it ignores the transformations (translation, rotation,
	/// scale, ...) that are applied to the entity.
	/// In other words, this function returns the bounds of the
	/// entity in the entity's coordinate system.
	///
	/// \return Local bounding rectangle of the entity
	///
	////////////////////////////////////////////////////////////
	virtual sf::FloatRect getLocalBounds() const = 0;

	////////////////////////////////////////////////////////////
	/// \brief Read Events from event
	/// 
	/// \param event -> polled event of a window 
	/// 
	////////////////////////////////////////////////////////////
	virtual bool pollEvents(sf::Event event) = 0;


	enum FunctionalObject {
		FRAME = 0,
		PAGE = ZUI_ID_PAGE,
		DROPDOWN = ZUI_ID_DROPDOWN
	};

	////////////////////////////////////////////////////////////
	/// \brief return Functional object Type (Frame, Page, Dropdown)
	/// 
	/// \return type of functional object
	/// 
	////////////////////////////////////////////////////////////
	FunctionalObject getFunctionalFrame();

	////////////////////////////////////////////////////////////
	// Member Data
	////////////////////////////////////////////////////////////

protected: 

	////////////////////////////////////////////////////////////
	/// \brief Set Record of wheter derived object is frame or page
	///
	////////////////////////////////////////////////////////////
	FunctionalObject m_functional_object;
};

class Frame;

class Entity : public sf::Transformable {
	friend Frame;

protected:

	////////////////////////////////////////////////////////////
	/// \brief Protected constructor, Sets the unique ID of the object
	/// 
	/// \param class_id -> Globally defined ZUI CLASS IDs
	/// 
	////////////////////////////////////////////////////////////
	Entity(uint64_t class_id);

	///////////////////////////////////////////////////////////
	/// \brief Protected Copy Constructor, Sets the unique ID of the object
	///
	/// \param entity -> object to copy from
	/// 
	///////////////////////////////////////////////////////////
	Entity(const Entity& entity);

	Entity& operator=(const Entity& entity);

public:

	////////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	////////////////////////////////////////////////////////////
	virtual ~Entity();

	////////////////////////////////////////////////////////////
	/// \brief Get the unique ID of the object
	///
	/// \return Unique ID of the object
	/// 
	////////////////////////////////////////////////////////////
	uint64_t getID() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the unique Class ID of the object
	///
	/// \return Unique Class ID of the object
	/// 
	////////////////////////////////////////////////////////////
	static uint64_t getClassID(const Entity& entity);

	////////////////////////////////////////////////////////////
	/// \brief Get the Frame attached to object
	/// 
	/// \return Pointer to current Frame
	/// 
	////////////////////////////////////////////////////////////
	Functional* getFunctionalParent() const;

	////////////////////////////////////////////////////////////
	/// \brief Check if the point is inside the object
	///		   Definition of if the point is inside the object may vary with different zui objects
	/// 
	/// \return true if point is inside the object, false otherwise
	/// 
	/// \param point -> Position of the point
	/// 
	////////////////////////////////////////////////////////////
	virtual bool contains(const sf::Vector2f& point) const = 0;

	////////////////////////////////////////////////////////////
	/// \brief Get the entity if the point is over the entity
	/// 
	/// \return Pointer to the entity if pointed by the point, nullptr otherwise
	/// 
	/// \param point -> Position of the point
	/// 
	////////////////////////////////////////////////////////////
	virtual Entity* isHit(const sf::Vector2f& point) = 0;

	////////////////////////////////////////////////////////////
	/// \brief Draws the object to the target applying its states
	/// 
	/// \param target -> Render target to draw to
	/// \param states -> Current render states
	/// 
	////////////////////////////////////////////////////////////
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;

	////////////////////////////////////////////////////////////
	/// \brief Actions to take upon selection of the object
	///
	////////////////////////////////////////////////////////////
	virtual void activateSelection() = 0;

	////////////////////////////////////////////////////////////
	/// \brief Actions to take upon deselection of the object
	///
	////////////////////////////////////////////////////////////
	virtual void deactivateSelection() = 0;

	////////////////////////////////////////////////////////////
	/// \brief Get the selection of object
	/// 
	/// \return True if object is selected
	///
	////////////////////////////////////////////////////////////
	bool isSelected() const;

	////////////////////////////////////////////////////////////
	/// \brief Set the active state of the object to true
	///
	////////////////////////////////////////////////////////////
	void setActive();

	////////////////////////////////////////////////////////////
	/// \brief Set the active state of the object to false
	///
	////////////////////////////////////////////////////////////
	void setInactive();

	////////////////////////////////////////////////////////////
	/// \brief Get the current active state of the object
	///
	/// \return Current active State of the object
	/// 
	////////////////////////////////////////////////////////////
	bool isActive() const;

	////////////////////////////////////////////////////////////
	/// \brief call the action attached to the object
	///
	////////////////////////////////////////////////////////////
	void callAction() const;

	////////////////////////////////////////////////////////////
	/// \brief check if action of the object is nullptr
	///
	/// \return false if action is nullptr else true
	/// 
	////////////////////////////////////////////////////////////
	bool hasAction() const;

	////////////////////////////////////////////////////////////
	/// \brief set the action of the object
	/// 
	/// \param func -> Lambda function
	///
	////////////////////////////////////////////////////////////
	void setAction(std::function<void()> func);

	////////////////////////////////////////////////////////////
	/// \brief Event which triggers action of the object
	///
	////////////////////////////////////////////////////////////
	enum class ActionEvent {
		NONE,			/// < No event
		RELEASE,		/// < If Mouse button is released
		PRESS,			/// < If Mouse button is pressed
		MOUSEHELD,		/// < If Mouse button is pressed and held
		MOUSEHOVER,		/// < If Mouse button has mouse over it
	};


private:

	////////////////////////////////////////////////////////////
	/// \brief check if action of the object is nullptr
	///
	/// \return false if action is null else true
	/// 
	////////////////////////////////////////////////////////////
	bool hasFunctionalParent() const;

	////////////////////////////////////////////////////////////
	/// \brief Set the Functional parent of entity
	///
	////////////////////////////////////////////////////////////
	friend void setFunctionalParent(Entity& entity, Functional* parent);

	////////////////////////////////////////////////////////////
	/// \brief Set the Functional parent of entity's sub variables
	///
	////////////////////////////////////////////////////////////
	virtual void setFunctionalParentForSubVariables(Functional* parent);


	////////////////////////////////////////////////////////////
	// Member data
	////////////////////////////////////////////////////////////
public:
	ActionEvent actionEvent;								/// < Type of the Action Event

protected:
	bool m_selected;										/// < Selection State of the entity
	std::function<void()> action;							/// < Job of the entity upon reaching specified event state 

private:
	uint64_t m_id;										/// < Unique id for every entity
	bool m_active;										/// < State of the entity
	Functional* m_functionalParent;						/// < Frame attached to entity, can be nullptr

	static uint64_t item_count;							/// < Strictly increasing count of all constructed entities, Used for generating Ids, \
																  makes up for the 24 least significant bits of Id
};


template<typename T>
std::unique_ptr<T> create()
{
	return std::make_unique<T>();
}

template<typename T>
std::unique_ptr<T> copy(T& copy)
{
	return std::make_unique<T>(copy);
}

template<typename T>
std::unique_ptr<T> copy(std::unique_ptr<T>& copy)
{
	return std::make_unique<T>(*copy);
}



class Frame : Functional {
public:
	////////////////////////////////////////////////////////////
	/// \brief Default Constructor
	///
	////////////////////////////////////////////////////////////
	Frame();

	Frame(Frame&) = delete;
	Frame(Frame&&) = delete;

	////////////////////////////////////////////////////////////
	/// \brief Default Destructor
	///
	////////////////////////////////////////////////////////////
	~Frame();

	////////////////////////////////////////////////////////////
	/// \brief Set the Current Window
	/// 
	/// \param window -> Current Window used for Events and Drawing Fucntions
	/// 
	////////////////////////////////////////////////////////////
	void setWindow(sf::RenderWindow& window);

	////////////////////////////////////////////////////////////
	/// \brief attach an entity to object
	///
	////////////////////////////////////////////////////////////
	void addEntity(Entity& entity);

	////////////////////////////////////////////////////////////
	/// \brief attach an entity to object
	///
	////////////////////////////////////////////////////////////
	void addEntity(Entity* entity);

	////////////////////////////////////////////////////////////
	/// \brief detach an entity to object
	///
	////////////////////////////////////////////////////////////
	void removeEntity(Entity& entity);

	////////////////////////////////////////////////////////////
	/// \brief detach an entity to object
	///
	////////////////////////////////////////////////////////////
	void removeEntity(Entity* entity);

	////////////////////////////////////////////////////////////
	/// \brief detach an entity to object by its unique Id
	///
	////////////////////////////////////////////////////////////
	void removeEntity(uint64_t id);


	///////////////////////////////////////////////////////////
	/// \brief Add entity to key press navigation order 
	///	
	/// Entities added in navigation order may be different from entities attached to frame
	/// 
	/// \param entity entity to be added in navigation order
	//////////////////////////////////////////////////////////
	void push_in_navigationOrder(Entity& entity);

	////////////////////////////////////////////////////////////
	/// \brief Get the entity attached to the object by Id
	/// 
	/// \return Pointer to Entity
	///
	////////////////////////////////////////////////////////////
	Entity* getByID(uint64_t id) const;

	////////////////////////////////////////////////////////////
	/// \brief Get the mouse position with respect to the current window
	/// 
	////////////////////////////////////////////////////////////
	sf::Vector2f getMousePosition() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the differnce between last and current mouse positions
	/// 
	/// \return Differnce between last and current mouse positions
	///
	////////////////////////////////////////////////////////////
	sf::Vector2f getLastMouseOffset() const;

	////////////////////////////////////////////////////////////
	/// \brief Check if the point is inside the object
	///		   Definition of if the point is inside the object may vary with different zui objects
	/// 
	/// \return true if point is inside the object, false otherwise
	/// 
	/// \param point -> Position of the point
	/// 
	////////////////////////////////////////////////////////////
	bool contains(const sf::Vector2f& point) const override;

	////////////////////////////////////////////////////////////
	/// \brief Get the local bounding rectangle of the object
	///
	/// The returned rectangle is in local coordinates, which means
	/// that it ignores the transformations (translation, rotation,
	/// scale, ...) that are applied to the entity.
	/// In other words, this function returns the bounds of the
	/// entity in the entity's coordinate system.
	///
	/// \return Local bounding rectangle of the object
	///
	////////////////////////////////////////////////////////////
	sf::FloatRect getLocalBounds() const override;

	////////////////////////////////////////////////////////////
	/// \brief Update mouse and entities attached to the object
	/// 
	////////////////////////////////////////////////////////////
	void update();

	////////////////////////////////////////////////////////////
	/// \brief Read Events from event
	/// 
	/// \param event -> polled event of a window 
	///		   Different polled window and current window may produce unexpected results
	/// 
	////////////////////////////////////////////////////////////
	bool pollEvents(sf::Event event);

	////////////////////////////////////////////////////////////
	/// \brief Draw every entity attached to the object on current window
	/// 
	////////////////////////////////////////////////////////////
	void draw();

	////////////////////////////////////////////////////////////
	/// \brief check if mouse is hovering over something
	///
	/// \return true if mouse is over somehting false otherwise
	/// 
	////////////////////////////////////////////////////////////
	bool isMouseOverSomething() const;

	////////////////////////////////////////////////////////////
	/// \brief check if something was clicked
	///
	/// \return true if something was clicked false otherwise
	/// 
	////////////////////////////////////////////////////////////
	bool wasSomethingClicked() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the entity which was clicked
	///
	/// \return Clicked entity
	/// 
	////////////////////////////////////////////////////////////
	Entity* getClickedEntity() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the entity over which mouse is hovering
	///
	/// \return Entity on which mouse hovers
	/// 
	////////////////////////////////////////////////////////////
	Entity* getMouseHoveringOnWhichEntity() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the entity which was navigated to by keys
	///
	/// \return Get navigated entity
	/// 
	////////////////////////////////////////////////////////////
	Entity* getNavigatedEntity() const;

	////////////////////////////////////////////////////////////
	// Member Data
	////////////////////////////////////////////////////////////
private:
	sf::RenderWindow* m_window;											/// < main window of the frame (handles mouse position and rendering)
	Entity* m_mouseHoveringOn;											/// < last Entity over which mouse button was
	Entity *m_clicked;													/// < last Entity on which mouse button was pressed
	sf::Vector2f m_lastMousePos;										/// < last mouse Position on the current window
	int m_navigator;													/// < iterator for navigation through key press

	std::map<uint64_t, Entity*> m_entityMap;							/// < maps zui Ids to their respective entities
	std::unordered_map<uint64_t, Functional*> m_functionalParents;		/// < maps zui Ids of functional parents to their respective entities (also in entityMap)
	std::vector<Entity*> m_navigationOrder;								/// < key press navigation order of entities
};

} // namespace zui

bool operator==(const zui::Entity& first, const zui::Entity& second);

bool operator!=(const zui::Entity& first, const zui::Entity& second);