#pragma once
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Slider.hpp"
#include "ZUIFrame.hpp"
#include <vector>

namespace zui {
class Scroll;

class Page : public Entity, public Functional {
public:

	////////////////////////////////////////////////////////////
	/// \brief Enumeration for scroll position / direction
	///
	////////////////////////////////////////////////////////////
	enum ScrollPlacement { LEFT, TOP, RIGHT, BOTTOM, NONE };

	////////////////////////////////////////////////////////////
	/// \brief Default Constructor
	/// 
	/// \param size maximum size of the page
	///
	////////////////////////////////////////////////////////////
	Page(const sf::Vector2f& size = sf::Vector2f(0, 0));

	////////////////////////////////////////////////////////////
	/// \brief Default virtual destrcutor
	///
	////////////////////////////////////////////////////////////
	virtual ~Page();

	////////////////////////////////////////////////////////////
	/// \brief Attach any entity to page
	///
	////////////////////////////////////////////////////////////
	void addEntity(Entity& entity);

	////////////////////////////////////////////////////////////
	/// \brief Attach any entity to page
	///
	////////////////////////////////////////////////////////////
	void addEntity(Entity* entity);

	////////////////////////////////////////////////////////////
	/// \brief remove any already connected entity from page
	///
	////////////////////////////////////////////////////////////
	void removeEntity(Entity& entity);

	////////////////////////////////////////////////////////////
	/// \brief remove any already connected entity from page
	///
	////////////////////////////////////////////////////////////
	void removeEntity(Entity* entity);

	////////////////////////////////////////////////////////////
	/// \brief remove any already connected entity from page by its Id
	///
	////////////////////////////////////////////////////////////
	void removeEntity(uint64_t id);

	////////////////////////////////////////////////////////////
	/// \brief Get the entity attached to the object by Id
	/// 
	/// \return Pointer to Entity
	///
	////////////////////////////////////////////////////////////
	Entity* getByID(uint64_t id) const;

	////////////////////////////////////////////////////////////
	/// \brief Set the background color of the object
	/// 
	/// \param color -> New fill color
	///
	////////////////////////////////////////////////////////////
	void setFillColor(sf::Color color);

	////////////////////////////////////////////////////////////
	/// \brief Get the background color of the object
	/// 
	/// \return background color
	///
	////////////////////////////////////////////////////////////
	sf::Color getFillColor() const;

	////////////////////////////////////////////////////////////
	/// \brief Change the source texture of the shape
	///
	/// The \a texture argument refers to a texture that must
	/// exist as long as the shape uses it. Indeed, the shape
	/// doesn't store its own copy of the texture, but rather keeps
	/// a pointer to the one that you passed to this function.
	/// If the source texture is destroyed and the shape tries to
	/// use it, the behavior is undefined.
	/// \a texture can be NULL to disable texturing.
	/// If \a resetRect is true, the TextureRect property of
	/// the shape is automatically adjusted to the size of the new
	/// texture. If it is false, the texture rect is left unchanged.
	///
	/// \param texture   New texture
	/// \param resetRect Should the texture rect be reset to the size of the new texture?
	///
	////////////////////////////////////////////////////////////
	void setTexture(const sf::Texture* texture, bool resetRect = false);

	////////////////////////////////////////////////////////////
	/// \brief Set the sub-rectangle of the texture that the shape will display
	///
	/// The texture rect is useful when you don't want to display
	/// the whole texture, but rather a part of it.
	/// By default, the texture rect covers the entire texture.
	///
	/// \param rect Rectangle defining the region of the texture to display
	///
	////////////////////////////////////////////////////////////
	void setTextureRect(const sf::IntRect& rect);

	////////////////////////////////////////////////////////////
	/// \brief Get the source texture of the shape
	///
	/// If the shape has no source texture, a NULL pointer is returned.
	/// The returned pointer is const, which means that you can't
	/// modify the texture when you retrieve it with this function.
	///
	/// \return Pointer to the shape's texture
	///
	////////////////////////////////////////////////////////////
	const sf::Texture* getTexture() const;

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
	sf::FloatRect getLocalBounds() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the global bounding rectangle of the entity
	///
	/// The returned rectangle is in global coordinates, which means
	/// that it takes into account the transformations (translation,
	/// rotation, scale, ...) that are applied to the entity.
	/// In other words, this function returns the bounds of the
	/// text in the global 2D world's coordinate system.
	///
	/// \return Global bounding rectangle of the entity
	///
	////////////////////////////////////////////////////////////
	sf::FloatRect getGlobalBounds() const;

	////////////////////////////////////////////////////////////
	/// \brief Set Active Region of the object
	///
	/// \param region -> New active region
	/// 
	////////////////////////////////////////////////////////////
	void setActiveRegion(const sf::FloatRect& region);

	////////////////////////////////////////////////////////////
	/// \brief Get current active region of the object
	/// 
	/// \return current active region
	///
	////////////////////////////////////////////////////////////
	sf::FloatRect getActiveRegion() const;

	////////////////////////////////////////////////////////////
	/// \brief Get current position of active region of the object
	/// 
	/// \return current position of active region
	///
	////////////////////////////////////////////////////////////
	sf::Vector2f getActiveRegionPosition() const;

	////////////////////////////////////////////////////////////
	/// \brief move active Region of the object
	///
	////////////////////////////////////////////////////////////
	void moveActiveRegion(float offsetX, float offsetY);

	////////////////////////////////////////////////////////////
	/// \brief move active Region of the object
	///
	////////////////////////////////////////////////////////////
	void moveActiveRegion(const sf::Vector2f& offset);

	////////////////////////////////////////////////////////////
	/// \brief set Maximum Size of the object
	///
	/// \param size -> New maximum size of the object
	/// 
	////////////////////////////////////////////////////////////
	void setMaxSize(const sf::Vector2f& size);

	////////////////////////////////////////////////////////////
	/// \brief Get maximum size of the object
	/// 
	/// \return current maximum size of the object
	///
	////////////////////////////////////////////////////////////
	sf::Vector2f getMaxSize() const;

	////////////////////////////////////////////////////////////
	/// \brief get size of the active region of the object
	///
	/// \return current size of the active region
	/// 
	////////////////////////////////////////////////////////////
	sf::Vector2f getSize() const;

	////////////////////////////////////////////////////////////
	/// \brief Set Scroll by its position / direction i.e. Left, Right, Top, Bottom
	///
	////////////////////////////////////////////////////////////
	void setScroll(ScrollPlacement place);

	////////////////////////////////////////////////////////////
	/// \brief Remove Scroll by its position / direction i.e. Left, Right, Top, Bottom
	///
	////////////////////////////////////////////////////////////
	void removeScroll(ScrollPlacement place);

	////////////////////////////////////////////////////////////
	/// \brief scroll the object by offset in specified diretion
	/// 
	/// \param place -> placement of Scroll
	/// \param offset -> offset of scrolling
	///
	////////////////////////////////////////////////////////////
	void scrollBy(ScrollPlacement place, const sf::Vector2f& offset);

	////////////////////////////////////////////////////////////
	/// \brief Set the header of the object
	/// 
	/// \param hasHeader -> true for setting header, false for removing header
	/// \param isMovable -> true if object should move if mouseheld on header
	/// \param isMinimisable -> true for visible minimise button
	/// \param isMaximisable -> true for visible maximise button
	///
	////////////////////////////////////////////////////////////
	void setHeader(bool hasHeader, bool isMovable = true, bool isMinimisable = true, bool isMaximisable = true);

	////////////////////////////////////////////////////////////
	/// \brief Read Events from event
	/// 
	/// \param event -> polled event of a window 
	///		   Different polled window and current window may produce unexpected results
	/// 
	////////////////////////////////////////////////////////////
	bool pollEvents(sf::Event event) override;

	////////////////////////////////////////////////////////////
	/// \brief Get the mouse position with respect to the current page
	/// 
	////////////////////////////////////////////////////////////
	sf::Vector2f getLocalMousePosition() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the mouse position with respect to the active region of current page
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
	/// \brief Get the entity if the point is over the object
	/// 
	/// \return Pointer to the entity if pointed by the point, nullptr otherwise
	/// 
	/// \param point -> Position of the point
	/// 
	////////////////////////////////////////////////////////////
	Entity* isHit(const sf::Vector2f& point) override;

	////////////////////////////////////////////////////////////
	/// \brief Draws the object to the target applying its states
	/// 
	/// \param target -> Render target to draw to
	/// \param states -> Current render states
	/// 
	////////////////////////////////////////////////////////////
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	////////////////////////////////////////////////////////////
	/// \brief Actions to take upon selection of the object
	///
	////////////////////////////////////////////////////////////
	void activateSelection() override;

	////////////////////////////////////////////////////////////
	/// \brief Actions to take upon deselection of the object
	///
	////////////////////////////////////////////////////////////
	void deactivateSelection() override;

	////////////////////////////////////////////////////////////
	/// \brief Object's action is set by default
	///
	////////////////////////////////////////////////////////////
	void setAction(std::function<void()> func) = delete;

private:
	////////////////////////////////////////////////////////////
	/// \brief check if new active region is inside the max region
	///
	////////////////////////////////////////////////////////////
	virtual void limitActiveRegion();

	////////////////////////////////////////////////////////////
	/// \brief Set the Functional parent of entity's sub variables
	///
	////////////////////////////////////////////////////////////
	void setFunctionalParentForSubVariables(Functional* parent) override;

	
	////////////////////////////////////////////////////////////
	/// \brief Structure for Scolling
	///
	////////////////////////////////////////////////////////////
	struct Scroll : public Entity {
		////////////////////////////////////////////////////////////
		/// \brief Default Constructor
		///
		////////////////////////////////////////////////////////////
		Scroll();

		////////////////////////////////////////////////////////////
		/// \brief Set the size of the scroll based on its length
		/// 
		/// \param region_size -> size of the region in control of scroll
		/// \param max_size -> maximum size of scrollable field
		/// \param place -> placement of scroll
		///
		////////////////////////////////////////////////////////////
		void createScroll(sf::Vector2f region_size, sf::Vector2f max_size, ScrollPlacement place);

		////////////////////////////////////////////////////////////
		/// \brief Get the size of the scroll body
		/// 
		/// \return current size of the scroll
		///
		////////////////////////////////////////////////////////////
		sf::Vector2f getSize() const;

		////////////////////////////////////////////////////////////
		/// \brief map the position of to new position in scroll
		/// 
		/// \position unmapped Position of scroll
		///
		////////////////////////////////////////////////////////////
		sf::Vector2f mapPosition(const sf::Vector2f& position);

		////////////////////////////////////////////////////////////
		/// \brief map the position of bar to scroll to new position
		/// 
		/// \position unmapped Position of scroll
		///
		////////////////////////////////////////////////////////////
		void mapBarPosition(const sf::Vector2f& position);
		
		////////////////////////////////////////////////////////////
		/// \brief Scroll the bar to the new position if possible
		/// 
		/// \param position -> position to scroll to
		///
		////////////////////////////////////////////////////////////
		sf::Vector2f scrollTo(const sf::Vector2f& position);

		////////////////////////////////////////////////////////////
		/// \brief Scroll the bar by offset
		/// 
		////////////////////////////////////////////////////////////
		sf::Vector2f scrollBy(const sf::Vector2f& offset);

		////////////////////////////////////////////////////////////
		/// \brief Check if the point is inside the object
		///		   Definition of if the point is inside the object may vary with different zui objects
		/// 
		/// \return true if point is inside the object, false otherwise
		/// 
		/// \param point -> Position of the point
		/// 
		////////////////////////////////////////////////////////////
		bool contains(const sf::Vector2f& point) const;

		////////////////////////////////////////////////////////////
		/// \brief Get the entity if the point is over the entity
		/// 
		/// \return Pointer to the entity if pointed by the point, nullptr otherwise
		/// 
		/// \param point -> Position of the point
		/// 
		////////////////////////////////////////////////////////////
		Entity* isHit(const sf::Vector2f& point);

		////////////////////////////////////////////////////////////
		/// \brief Draws the object to the target applying its states
		/// 
		/// \param target -> Render target to draw to
		/// \param states -> Current render states
		/// 
		////////////////////////////////////////////////////////////
		void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

		////////////////////////////////////////////////////////////
		/// \brief Actions to take upon selection of the object
		///
		////////////////////////////////////////////////////////////
		void activateSelection();

		////////////////////////////////////////////////////////////
		/// \brief Actions to take upon deselection of the object
		///
		////////////////////////////////////////////////////////////
		void deactivateSelection();

		////////////////////////////////////////////////////////////
		/// \brief Object's action is set by default
		///
		////////////////////////////////////////////////////////////

		Button m_bar;						/// < movable part of scroll
		Button m_rect;						/// < rectangle as the body of scroll
		float m_length;						/// < maximum length of scroll
		float m_region_length;				/// < length of visible region
		ScrollPlacement m_place;			/// < placement of scroll
	};

	////////////////////////////////////////////////////////////
	// Member Data 
	////////////////////////////////////////////////////////////

private:
	sf::Vector2f m_maxSize;					/// < maximum size of the page
	sf::FloatRect m_activeRegion;			/// < active region of the page

	std::vector<Entity*> m_entities;		/// < list of all entities connected to page

	Scroll m_connectedScroll[4];			/// < list of connected scrolls

	Button m_header;						/// < header of the page
	Button m_minimise;						/// < minimise button for the page
	Button m_maximise;						/// < maximise button for the page
	sf::FloatRect m_lastActiveRegion;		/// < stores the last active region for m_maximise
	sf::RectangleShape m_background;		/// < background of the page
};


typedef std::unique_ptr<zui::Page> Page_ptr;


} // namespace zui