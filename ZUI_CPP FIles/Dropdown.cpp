

#include <ZUI/Dropdown.hpp>

using namespace zui;

Dropdown::Dropdown(const sf::Vector2f& headerSize):
	Entity(ZUI_ID_DROPDOWN)
{
	m_itemCount = 0;
	m_itemLimit = 3;
	m_itemSize.y = headerSize.y;
	setHeader();
	setHeaderSize(headerSize);

	m_page.setFillColor(sf::Color::Transparent);

	m_functional_object = FunctionalObject::DROPDOWN;

}
Dropdown::Dropdown(const sf::Vector2f& headerSize, float itemHeight, int itemLimit): 
	Entity(ZUI_ID_DROPDOWN)
{
	m_itemCount = 0;
	m_itemLimit = itemLimit;
	m_itemSize.y = itemHeight;
	setHeader();
	setHeaderSize(headerSize);

	m_page.setFillColor(sf::Color::Transparent);
}

Dropdown::~Dropdown()
{
}

void Dropdown::setHeader()
{
	m_header.actionEvent = ActionEvent::MOUSEHELD;
	m_header.setAction([this] {
		move(getFunctionalParent()->getLastMouseOffset()); 
		});

	m_minimise.setOutlineThickness(2);
	m_minimise.setSelectionOutlineThickness(2);
	m_minimise.setSelectionScale(sf::Vector2f(1.1, 1.1));
	m_minimise.actionEvent = ActionEvent::RELEASE;
	m_minimise.setAction([this] {
		if (m_page.isActive()) {
			m_page.setInactive();
			m_minimise.setRotation(90);
		}
		else {
			m_page.setActive();
			m_minimise.setRotation(0);
		}
		});
}

void Dropdown::setHeaderSize(const sf::Vector2f& headerSize)
{
	m_header.setSize(headerSize);

	float triWidth = m_header.getSize().y * 0.75f;

	m_minimise.setPointCount(3);
	m_minimise.setPoint(0, sf::Vector2f(0, 0));
	m_minimise.setPoint(1, sf::Vector2f(triWidth, 0));
	m_minimise.setPoint(2, sf::Vector2f(triWidth * 0.5, triWidth * 0.5f));
	m_minimise.setOrigin(triWidth * 0.5, triWidth * 0.25);
	m_minimise.setPosition(m_header.getPosition().x + m_header.getSize().x - m_header.getSize().y * 0.5f, m_header.getPosition().y + m_header.getSize().y * 0.5f);

	m_itemSize.x = headerSize.x - 30;

	m_page.setMaxSize({ headerSize.x, m_page.getMaxSize().y });
	m_page.setActiveRegion({ m_page.getActiveRegion().left, m_page.getActiveRegion().top, headerSize.x, m_page.getActiveRegion().height });
	m_page.setPosition(0, headerSize.y);
}
void Dropdown::setHeaderColor(const sf::Color& color)
{
	m_header.setFillColor(color);
	m_header.setSelectionFillColor(color);
}
void Dropdown::setHeaderOutlineThickness(float thickness)
{
	m_header.setOutlineThickness(thickness);
	m_header.setSelectionOutlineThickness(thickness);
}
void Dropdown::setHeaderOutlineColor(const sf::Color& color)
{
	m_header.setOutlineColor(color);
	m_header.setSelectionOutlineColor(color);
}
void Dropdown::setMinimiseButtonColor(const sf::Color& color)
{
	m_minimise.setFillColor(color);
	m_minimise.setSelectionFillColor(color);
}
void zui::Dropdown::setMinimiseButtonOutlineColor(const sf::Color& color)
{
	m_minimise.setOutlineColor(color);
	m_minimise.setSelectionOutlineColor(color);
}
void Dropdown::setItemLimit(int limit)
{
	m_itemLimit = limit;

	m_page.setActiveRegion({ m_page.getActiveRegion().left, m_page.getActiveRegion().top, m_page.getActiveRegion().width, std::min(m_itemLimit, m_itemCount) * (m_itemSize.y + 10) + 10 });
}
void Dropdown::setItemHeight(float height)
{
	m_itemSize.y = height;

	m_page.setMaxSize({ m_header.getSize().x, m_itemCount * (m_itemSize.y + 10) + 10 });
	m_page.setActiveRegion({ m_page.getActiveRegion().left, m_page.getActiveRegion().top, m_page.getActiveRegion().width, std::min(m_itemLimit, m_itemCount) * (m_itemSize.y + 10) + 10 });
}
void Dropdown::setTitle(const std::string& title, sf::Font& font, int charSize, const sf::Color& color)
{
	m_header.setFont(font);
	m_header.setString(title);
	m_header.setCharacterSize(charSize);
	m_header.setTextFillColor(color);
	m_header.alignText(Textbox::TextAlign::LEFT);
	m_header.setStyle(sf::Text::Bold);
}
void Dropdown::setFillColor(const sf::Color& color)
{
	m_page.setFillColor(color);
}
void Dropdown::setMovementEnabled(bool enable)
{
	if(!enable)m_header.setAction(nullptr);
	else m_header.setAction([this] {
		move(getFunctionalParent()->getLastMouseOffset());
	});
}
sf::Color Dropdown::getFillColor()
{
	return m_page.getFillColor();
}
sf::Vector2f Dropdown::getHeaderSize() const
{
	return m_header.getSize();
}
sf::Vector2f Dropdown::getMousePosition() const
{
	return getInverseTransform().transformPoint(getFunctionalParent()->getMousePosition());
}
sf::Vector2f Dropdown::getLastMouseOffset() const
{
	sf::Transform transform = getInverseTransform();
	transform.translate(getPosition());

	return transform.transformPoint(getFunctionalParent()->getLastMouseOffset());
}
sf::FloatRect Dropdown::getLocalBounds() const
{
	return sf::FloatRect(0, 0, getSize().x, getSize().y);
}
sf::FloatRect Dropdown::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}
sf::Vector2f Dropdown::getSize() const
{
	return m_header.getSize() + sf::Vector2f(0, m_page.getSize().y);
}
Entity* zui::Dropdown::getItemAt(int index) const
{
	return m_page.getByID(m_itemIDs.at(index));
}
void Dropdown::addItem(Textbox& textbox)
{
	textbox.setSize(m_itemSize);
	textbox.setOrigin(m_itemSize * 0.5f);
	textbox.setPosition((m_header.getSize().x - 10) / 2, m_itemCount++ * (m_itemSize.y + 10) + 10 + m_itemSize.y / 2.0f);
	
	m_page.addEntity(textbox);

	m_page.setMaxSize({ m_header.getSize().x, m_itemCount * (m_itemSize.y + 10) + 10 });
	m_page.setActiveRegion({ m_page.getActiveRegion().left, m_page.getActiveRegion().top, m_page.getActiveRegion().width, std::min(m_itemLimit, m_itemCount) * (m_itemSize.y + 10) + 10 });
	m_page.setScroll(Page::ScrollPlacement::RIGHT);

	m_itemIDs.push_back(textbox.getID());
}
void Dropdown::addItem(Textbox* textbox)
{
	addItem(*textbox);
}
void zui::Dropdown::addItem(Button& button)
{
	button.setPointCount(4);
	button.setPoint(0, { 0, 0 });
	button.setPoint(1, { m_itemSize.x, 0 });
	button.setPoint(2, m_itemSize);
	button.setPoint(3, { 0, m_itemSize.y });
	button.setOrigin(m_itemSize * 0.5f);
	button.setPosition((m_header.getSize().x - 10) / 2, m_itemCount++ * (m_itemSize.y + 10) + 10 + m_itemSize.y / 2.0f);
	m_page.addEntity(button);

	m_page.setMaxSize({ m_header.getSize().x, m_itemCount * (m_itemSize.y + 10) + 10 });
	m_page.setActiveRegion({ m_page.getActiveRegion().left, m_page.getActiveRegion().top, m_page.getActiveRegion().width, std::min(m_itemLimit, m_itemCount) * (m_itemSize.y + 10) + 10 });
	m_page.setScroll(Page::ScrollPlacement::RIGHT);

	m_itemIDs.push_back(button.getID());
}
void zui::Dropdown::addItem(Button* button)
{
	addItem(*button);
}
void Dropdown::insertItem(int where, Textbox& textbox)
{
	textbox.setSize(m_itemSize);
	textbox.setOrigin(m_itemSize * 0.5f);
	textbox.setPosition((m_header.getSize().x - 10) / 2, where * (m_itemSize.y + 10) + 10 + m_itemSize.y / 2.0f);
	m_page.addEntity(textbox);

	for (int i = where; i < m_itemCount; i++) {
		m_page.getByID(m_itemIDs[i])->move(0, m_itemSize.y + 10);
	}

	++m_itemCount;

	m_page.setMaxSize({ m_header.getSize().x, m_itemCount * (m_itemSize.y + 10) + 10 });
	m_page.setActiveRegion({ m_page.getActiveRegion().left, m_page.getActiveRegion().top, m_page.getActiveRegion().width, std::min(m_itemLimit, m_itemCount) * (m_itemSize.y + 10) + 10 });
	m_page.setScroll(Page::ScrollPlacement::RIGHT);

	m_itemIDs.insert(m_itemIDs.begin() + where, textbox.getID());
}
void Dropdown::insertItem(int where, Textbox* textbox)
{
	insertItem(where, *textbox);
}
void zui::Dropdown::insertItem(int where, Button& button)
{
	button.setPointCount(4);
	button.setPoint(0, { 0, 0 });
	button.setPoint(1, { m_itemSize.x, 0 });
	button.setPoint(2, m_itemSize);
	button.setPoint(3, { 0, m_itemSize.y });
	button.setOrigin(m_itemSize * 0.5f);
	button.setPosition((m_header.getSize().x - 10) / 2, where * (m_itemSize.y + 10) + 10 + m_itemSize.y / 2.0f);
	m_page.addEntity(button);

	for (int i = where; i < m_itemCount; i++) {
		m_page.getByID(m_itemIDs[i])->move(0, m_itemSize.y + 10);
	}

	++m_itemCount;

	m_page.setMaxSize({ m_header.getSize().x, m_itemCount * (m_itemSize.y + 10) + 10 });
	m_page.setActiveRegion({ m_page.getActiveRegion().left, m_page.getActiveRegion().top, m_page.getActiveRegion().width, std::min(m_itemLimit, m_itemCount) * (m_itemSize.y + 10) + 10 });
	m_page.setScroll(Page::ScrollPlacement::RIGHT);

	m_itemIDs.insert(m_itemIDs.begin() + where, button.getID());
}
void zui::Dropdown::insertItem(int where, Button* button)
{
	insertItem(where, *button);
}
void Dropdown::eraseItem(int where)
{
	m_page.removeEntity(m_itemIDs[where]);

	m_itemIDs.erase(m_itemIDs.begin() + where);

	--m_itemCount;

	for (int i = where; i < m_itemCount; i++) {
		m_page.getByID(m_itemIDs[i])->move(0, -m_itemSize.y - 10);
	}

	m_page.setMaxSize({ m_header.getSize().x, m_itemCount * (m_itemSize.y + 10) + 10 });
	m_page.setActiveRegion({ m_page.getActiveRegion().left, m_page.getActiveRegion().top, m_page.getActiveRegion().width, std::min(m_itemLimit, m_itemCount) * (m_itemSize.y + 10) + 10 });
	m_page.setScroll(Page::ScrollPlacement::RIGHT);
}
void Dropdown::expand()
{
	m_page.setActive();
	m_minimise.setRotation(0);
}
void Dropdown::collapse()
{
	m_page.setInactive();
	m_minimise.setRotation(90);
}
bool Dropdown::pollEvents(sf::Event event)
{
	if(isActive())
		return m_page.pollEvents(event);

	return false;
}
bool Dropdown::contains(const sf::Vector2f& point) const
{
	return getLocalBounds().contains(getInverseTransform().transformPoint(point));
}
bool Dropdown::containsExcludingHeader(const sf::Vector2f& point)
{
	return sf::FloatRect(m_page.getPosition().x, m_page.getPosition().y, m_page.getSize().x, m_page.getSize().y).contains(getInverseTransform().transformPoint(point));
}
Entity* Dropdown::isHit(const sf::Vector2f& point)
{
	if (isActive()) {
		Entity* entity = nullptr;

		sf::Vector2f inv_trans_point = getInverseTransform().transformPoint(point);

		entity = m_minimise.isHit(inv_trans_point);
		if (entity != nullptr) return entity;

		entity = m_header.isHit(inv_trans_point);
		if (entity != nullptr) return entity;

		if (m_page.isActive()) {
			entity = m_page.isHit(inv_trans_point);
			if (entity != nullptr) return entity;
		}

		if (contains(point))return this;
	}

	return nullptr;
}
void Dropdown::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive()) {
		states.transform *= getTransform();
		m_header.draw(target, states);
		m_minimise.draw(target, states);
		m_page.draw(target, states);
	}
}
void Dropdown::activateSelection()
{
	m_selected = true;
}
void Dropdown::deactivateSelection()
{
	m_selected = false;
}

void Dropdown::setFunctionalParentForSubVariables(Functional* parent)
{
	setFunctionalParent(m_page, this);
	setFunctionalParent(m_header, this);
	setFunctionalParent(m_minimise, this);
}