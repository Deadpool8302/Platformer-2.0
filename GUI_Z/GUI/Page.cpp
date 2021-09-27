#include "Page.hpp"
#include "Dropdown.hpp"

#include <iostream>

using namespace gui;
Page::Page(const sf::Vector2f& size)
	:Entity(GUI_ID_PAGE)
{
	m_maxSize = size;
	m_activeRegion = sf::FloatRect(0, 0, size.x, size.y);
	m_lastActiveRegion = sf::FloatRect(0, 0, m_maxSize.x, m_maxSize.y);
	m_background.setSize(size);

	for (int i = 0; i < 4; i++)
		m_connectedScroll[i].setInactive();

	setHeader(false);

	m_functional_object = FunctionalObject::PAGE;
	
}
void Page::addEntity(Entity& entity)
{
	m_entities.push_back(&entity);

	setFunctionalParent(entity, this);
}
void Page::removeEntity(const Entity& entity)
{
	std::string name = getName(entity.getID());
	if (name != "")Frame::removeName(name);
	for (int i = 0; i < m_entities.size(); i++) {
		if (m_entities[i]->getID() == entity.getID()) {
			m_entities.erase(m_entities.begin() + i);
			break;
		}
	}
}
void Page::removeEntity(unsigned int id)
{
	for (int i = 0; i < m_entities.size(); i++) {
		if (m_entities[i]->getID() == id) {
			m_entities.erase(m_entities.begin() + i);
			break;
		}
	}
}
void Page::setName(const Entity& entity, const std::string& name)
{
	Frame::setName(entity, name);
}
Entity* Page::getByID(unsigned int id) const
{
	for (int i = 0; i < m_entities.size(); i++) {
		if (m_entities[i]->getID() == id) {
			return m_entities[i];
		}
	}
	return nullptr;
}
Entity* Page::getByName(const std::string& name) const
{
	return getByID(Frame::getIDByName(name));
}
std::string Page::getName(unsigned int id)
{
	return Frame::getName(id);
}
void Page::setFillColor(sf::Color color)
{
	m_background.setFillColor(color);
}
sf::Color Page::getFillColor() const
{
	return m_background.getFillColor();
}
void Page::setTexture(const sf::Texture* texture, bool resetRect)
{
	m_background.setTexture(texture, resetRect);
}
void Page::setTextureRect(const sf::IntRect& rect)
{
	m_background.setTextureRect(rect);
}
const sf::Texture* gui::Page::getTexture() const
{
	return m_background.getTexture();;
}
void Page::setActiveRegion(const sf::FloatRect& region)
{
	m_activeRegion = region;
	limitActiveRegion();

	for (int i = 0; i < 4; i++) {
		if (m_connectedScroll[i].isActive())setScroll(ScrollPlacement(i));
	}

	m_header.setPointCount(4);
	m_header.setPoint(0, sf::Vector2f(0, 0));
	m_header.setPoint(1, sf::Vector2f(m_activeRegion.width, 0));
	m_header.setPoint(2, sf::Vector2f(m_activeRegion.width, 15));
	m_header.setPoint(3, sf::Vector2f(0, 15));
	m_minimise.setPosition(m_activeRegion.width - 15, -15);
	m_maximise.setPosition(m_activeRegion.width - 33, -15);
}
sf::FloatRect Page::getActiveRegion() const
{
	return m_activeRegion;
}
void Page::moveActiveRegion(float offsetX, float offsetY)
{
	m_activeRegion.left += offsetX;
	m_activeRegion.top += offsetY;
	limitActiveRegion();
}
void Page::moveActiveRegion(const sf::Vector2f& offset)
{
	m_activeRegion.left += offset.x;
	m_activeRegion.top += offset.y;
	limitActiveRegion();
}
void Page::setMaxSize(const sf::Vector2f& size)
{
	m_maxSize = size;
	m_lastActiveRegion = sf::FloatRect(0, 0, m_maxSize.x, m_maxSize.y);
	m_background.setSize(getMaxSize());
}
sf::Vector2f Page::getMaxSize() const
{
	return m_maxSize;
}
sf::Vector2f Page::getSize() const
{
	return sf::Vector2f(m_activeRegion.width, m_activeRegion.height);
}
void Page::setScroll(ScrollPlacement place)
{
	// construct scroll
	m_connectedScroll[place].createScroll(getSize(), getMaxSize(), place);
	m_connectedScroll[place].mapBarPosition(sf::Vector2f(m_activeRegion.left, m_activeRegion.top));
	m_connectedScroll[place].setActive();

	// calculate complementary placement
	ScrollPlacement comp((ScrollPlacement)(((int)place + 2) % 4));

	/*
		1 - inverse transform mouse position
		2 - scroll to new mouse position
		3 - scroll complement by returned offset
		4 - move active region of page by returned offset
	*/

	m_connectedScroll[place].m_bar.setAction([this, place]() {
		scrollBy(place, getLastMouseOffset());
		});
	m_connectedScroll[place].m_rect.setAction([this, place, comp]() {
		sf::Vector2f offset = m_connectedScroll[place].scrollTo(getLocalMousePosition());
		if (m_connectedScroll[comp].isActive())m_connectedScroll[comp].scrollBy(offset);
		moveActiveRegion(m_connectedScroll[place].mapPosition(offset));
		});
}
void Page::removeScroll(ScrollPlacement place)
{
	m_connectedScroll[place].setInactive();
}
void Page::scrollBy(ScrollPlacement place, const sf::Vector2f& offset)
{
	// calculate complement placement
	ScrollPlacement comp((ScrollPlacement)(((int)place + 2) % 4));

	sf::Vector2f newOffset;

	// scroll by offset and scroll complement by returned offset
	if (m_connectedScroll[place].isActive()) {
		newOffset = m_connectedScroll[place].scrollBy(offset);
		if (m_connectedScroll[comp].isActive())newOffset = m_connectedScroll[comp].scrollBy(newOffset);
	}

	if (m_connectedScroll[place].isActive())
		moveActiveRegion(m_connectedScroll[place].mapPosition(newOffset));
}
void Page::setHeader(bool hasHeader, bool isMovable, bool isMinimisable, bool isMaximisable)
{
	/*
		has Header :
			false -> dont display header or set header & minimise inactive

			true->
				isMovable :
					true -> set action of header
				isMinimisable :
					true -> set action of minimise and set minise to active or display minimise
					false -> set minimise to inactive or dont display minimise
				isMaximisable :
					true -> set action of maximise and set maximise to active or display maximise
					false -> set maximise to inactive or dont display maximise
	*/

	if (hasHeader) {
		m_header.setPointCount(4);
		m_header.setPoint(0, sf::Vector2f(0, 0));
		m_header.setPoint(1, sf::Vector2f(m_activeRegion.width, 0));
		m_header.setPoint(2, sf::Vector2f(m_activeRegion.width, 15));
		m_header.setPoint(3, sf::Vector2f(0, 15));
		m_header.setPosition(0, -15);
		m_header.setFillColor(sf::Color::White);
		m_header.setSelectionFillColor(sf::Color::White);
		m_header.actionEvent = ActionEvent::MOUSEHELD;
		if (isMovable)m_header.setAction([this] {
			move(getFunctionalParent()->getLastMouseOffset());
			});
		else m_header.setAction(nullptr);
		m_header.setActive();

		if (isMinimisable) {
			m_minimise.setPointCount(4);
			m_minimise.setPoint(0, sf::Vector2f(0, 0));
			m_minimise.setPoint(1, sf::Vector2f(15, 0));
			m_minimise.setPoint(2, sf::Vector2f(15, 15));
			m_minimise.setPoint(3, sf::Vector2f(0, 15));
			m_minimise.setPosition(m_activeRegion.width - 15, -15);
			m_minimise.setFillColor(sf::Color::Red);
			m_minimise.setSelectionFillColor(sf::Color::Red);
			m_minimise.setSelectionOutlineColor(sf::Color::Red);
			m_minimise.setSelectionOutlineThickness(3);
			m_minimise.actionEvent = ActionEvent::RELEASE;
			m_minimise.setAction([this] { isActive() ? setInactive() : setActive(); });
			m_minimise.setActive();
		}
		else {
			m_minimise.setInactive();
		}

		if (isMaximisable) {
			m_maximise.setPointCount(4);
			m_maximise.setPoint(0, sf::Vector2f(0, 0));
			m_maximise.setPoint(1, sf::Vector2f(15, 0));
			m_maximise.setPoint(2, sf::Vector2f(15, 15));
			m_maximise.setPoint(3, sf::Vector2f(0, 15));
			m_maximise.setPosition(m_activeRegion.width - 33, -15);
			m_maximise.setFillColor(sf::Color::Blue);
			m_maximise.setSelectionFillColor(sf::Color::Blue);
			m_maximise.setSelectionOutlineColor(sf::Color::Blue);
			m_maximise.setSelectionOutlineThickness(3);
			m_maximise.actionEvent = ActionEvent::RELEASE;
			m_maximise.setAction([this] { 
				sf::FloatRect rect = m_lastActiveRegion;
				m_lastActiveRegion = getActiveRegion();
				setActiveRegion(rect);
				});
			m_maximise.setActive();
		}
		else {
			m_maximise.setInactive();
		}
	}
	else {
		m_header.setInactive();
		m_minimise.setInactive();
		m_maximise.setInactive();
	}
}
sf::Vector2f Page::getLocalMousePosition() const
{
	return getInverseTransform().transformPoint(getFunctionalParent()->getMousePosition());
}
sf::Vector2f Page::getMousePosition() const
{
	return  getInverseTransform().transformPoint(getFunctionalParent()->getMousePosition()) + sf::Vector2f(m_activeRegion.left, m_activeRegion.top);
}
sf::Vector2f Page::getLastMouseOffset() const
{
	sf::Transform transform = getInverseTransform();
	transform.translate(getPosition());
	
	return transform.transformPoint(getFunctionalParent()->getLastMouseOffset());
}
sf::FloatRect gui::Page::getLocalBounds() const
{
	return sf::FloatRect(0, 0, getSize().x, getSize().y);
}
sf::FloatRect gui::Page::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}
bool Page::contains(const sf::Vector2f& point) const
{
	return getLocalBounds().contains(getInverseTransform().transformPoint(point));
}
Entity* Page::isHit(const sf::Vector2f& point)
{
	Entity* entity = nullptr;

	sf::Vector2f inv_trans_point = getInverseTransform().transformPoint(point);

	// check for m_header and minimise and maximise
	if (m_header.isActive()) {
		entity = m_minimise.isHit(inv_trans_point);
		if (entity != nullptr)return entity;

		entity = m_maximise.isHit(inv_trans_point);
		if (entity != nullptr)return entity;

		entity = m_header.isHit(inv_trans_point);
		if (entity != nullptr)return entity;	
	}


	if (isActive()) {
		//if point doesnt lie in page
		if (!contains(point))return nullptr;

		// if point is in one of he scrolls
		for (int i = 0; i < 4 && entity == nullptr; i++) {
			if (m_connectedScroll[i].isActive())entity = m_connectedScroll[i].isHit(inv_trans_point);
		}

		// if point is in any entity
		for (auto it = m_entities.begin(); it != m_entities.end() && entity == nullptr; it++) {
			entity = (*it)->isHit(inv_trans_point + sf::Vector2f(m_activeRegion.left, m_activeRegion.top));
		}

		// else point is in page
		if (entity == nullptr)
			entity = this;

		return entity;
	}
	return nullptr;
}
void Page::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (isActive()) {

		// x or y attribute of maxSize is zero then error if thrown
		if (m_maxSize.x && m_maxSize.y) {
			sf::RenderTexture rt;
			rt.create(getSize().x, getSize().y);
			rt.clear(sf::Color::Transparent);

			rt.draw(m_background);

			for (auto it = m_entities.begin(); it != m_entities.end(); it++) {
				(*it)->move(-sf::Vector2f(m_activeRegion.left, m_activeRegion.top));
				(*it)->draw(rt);
				(*it)->move(sf::Vector2f(m_activeRegion.left, m_activeRegion.top));
			}
			rt.display();
			sf::Sprite s;
			s.setTexture(rt.getTexture());
			target.draw(s, states);
		}

		for (int i = 0; i < 4; i++) {
			if (m_connectedScroll[i].isActive()) {
				m_connectedScroll[i].draw(target, states);
			}
		}
	}

	if (m_header.isActive()) {
		m_header.draw(target, states);
		m_minimise.draw(target, states);
		m_maximise.draw(target, states);
	}
}
void Page::activateSelection()
{
	m_selected = true;
}
void Page::deactivateSelection()
{
	m_selected = false;
}
bool Page::pollEvents(sf::Event event)
{
	bool wasEventPolled = false;
	if (isActive()) {
		for (int i = 0; i < m_entities.size(); i++) {
			if (getClassID(*m_entities[i]) == GUI_ID_PAGE && m_entities[i]->contains(getMousePosition()))
				wasEventPolled = ((Page*)m_entities[i])->pollEvents(event);
			else if (getClassID(*m_entities[i]) == GUI_ID_DROPDOWN && ((Dropdown*)m_entities[i])->containsExcludingHeader(getMousePosition()))
				wasEventPolled = ((Dropdown*)m_entities[i])->pollEvents(event);
		}

		if (event.type == sf::Event::MouseWheelScrolled && !wasEventPolled) {
			if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
				if (m_connectedScroll[TOP].isActive())
					this->scrollBy(TOP, sf::Vector2f(-5 * event.mouseWheelScroll.delta, 0));
				else if (m_connectedScroll[BOTTOM].isActive())
					this->scrollBy(BOTTOM, sf::Vector2f(-5 * event.mouseWheelScroll.delta, 0));
			}
			else {
				if (m_connectedScroll[LEFT].isActive())
					this->scrollBy(LEFT, sf::Vector2f(0, -5 * event.mouseWheelScroll.delta));
				else if (m_connectedScroll[RIGHT].isActive())
					this->scrollBy(RIGHT, sf::Vector2f(0, -5 * event.mouseWheelScroll.delta));
			}
			return true;
		}
	}
	return wasEventPolled;
}
void Page::limitActiveRegion()
{
	// left and top must not be negative or exceed the maxSize
	m_activeRegion.left = std::max(0.f, m_activeRegion.left);
	m_activeRegion.left = std::min(m_maxSize.x, m_activeRegion.left);

	m_activeRegion.top = std::max(0.f, m_activeRegion.top);
	m_activeRegion.top = std::min(m_maxSize.y, m_activeRegion.top);

	//if activeRegion is spreading out of the bounds
	m_activeRegion.width = std::max(0.f, m_activeRegion.width);
	if (m_activeRegion.left + m_activeRegion.width > m_maxSize.x)m_activeRegion.width = m_maxSize.x - m_activeRegion.left;
	m_activeRegion.height = std::max(0.f, m_activeRegion.height);
	if (m_activeRegion.top + m_activeRegion.height > m_maxSize.y)m_activeRegion.height = m_maxSize.y - m_activeRegion.top;
}

void Page::setFunctionalParentForSubVariables(Functional* parent)
{
	setFunctionalParent(m_header, parent);
	setFunctionalParent(m_minimise, parent);
	setFunctionalParent(m_maximise, parent);
	for (int i = 0; i < 4; i++) {
		setFunctionalParent(m_connectedScroll[i].m_rect, parent);
		setFunctionalParent(m_connectedScroll[i].m_bar, parent);
	}
}


