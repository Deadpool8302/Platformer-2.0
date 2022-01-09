#include "Platform.hpp"
#include "Utility.hpp"
#include "Enemy.hpp"

std::vector<std::unique_ptr<Platform>> Platform::m_platforms;
std::vector<std::unique_ptr<Platform>> Platform::m_movingPlatforms;

Platform::Platform()
{
	m_distance = 0;
	m_speed = sf::Vector2f(0, 0);
	m_moveDir = 0;
}

Platform::~Platform()
{
	
}

void Platform::setSize(const sf::Vector2f& size)
{
	updateHitbox(size);
}

sf::Vector2f Platform::getSize() const
{
	auto rect = getLocalBounds();
	return { rect.width, rect.height };
}

void Platform::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	states.transform *= getTransform();
	for(int i = 0; i < m_imgs.size(); i++){
		target.draw(m_imgs[i], states);
	}
}

void Platform::loadTileTextures(const sf::Vector2i& tileCount, const sf::Vector2f& tileSize, const std::array<sf::Texture, 12>& tileTextures)
{
	// emplace back a single tile into m_imgs
	// platTilePos -> position of tile wrt platform tiles 
	auto loadTileSingle = [this, &tileSize](const sf::Vector2i& platTilePos, const sf::Texture& texture) {
		m_imgs.emplace_back(texture);
		// size of whole image -> textureSize
		// size of image on screen -> tileSize
		// mapping image size into tileSize
		// image size(pixels) -> tileSize
		// 1(pixels) -> tileSize / image size									<<< scaleFactor
		// final size of image(pixels) -> current size of image * scaleFactor	<<< result
		m_imgs.back().setScale(tileSize / texture.getSize());// scaleFactor

		// final image position in pixels
		m_imgs.back().setPosition(platTilePos * tileSize);
	};

	// emplace back multiple tiles using a single texture and single sprite into m_imgs
	// platTilePos -> position of first tile wrt platform tiles
	// tileCount -> total number of tiles across and below in region of collective tiles
	auto loadTileMultiple = [this, &tileSize](const sf::Vector2i& platTilePos, const sf::Vector2i& tileCount, const sf::Texture& texture) {
		m_imgs.emplace_back(texture);

		// when rect size is greater than texture size, texture repeats if setRepeat is enabled
		// size of texture used in sprite -> tileCount * texture size
		m_imgs.back().setTextureRect({ 0, 0, tileCount.x * (int)texture.getSize().x, tileCount.y * (int)texture.getSize().y });

		// size of whole image -> tilecount * textureSize
		// size of image on screen -> tileCount * tileSize
		// for mapping see above in loadTileSingle
		m_imgs.back().setScale(tileSize / texture.getSize());
		
		// final image position in pixels
		m_imgs.back().setPosition(platTilePos * tileSize);
	}; 
	
	// load a two single tiles using loadTileSingle with first & last texture
	// load a multiple tiles using loadTileMultiple with middle texture
	auto loadTileLine = [&loadTileSingle, &loadTileMultiple, this, &tileSize](
		const sf::Vector3<sf::Vector2i> platTilePositions,		// positions of each tile(first, mid, last) wrt platform tiles
		const sf::Vector2i& midTileCount,						// number of tiles across and below in middle tile region
		const sf::Texture& first,								// texture used for first tile
		const sf::Texture& mid,									// texture used for middle tiles
		const sf::Texture& last									// texture used for last tile
		) {
			loadTileSingle(platTilePositions.x, first);
			if (midTileCount.x != 0 && midTileCount.y != 0) { // when midTileCount is non zero, middle tiles and last tile use thier respective positions
				loadTileMultiple(platTilePositions.y, midTileCount, mid);
				loadTileSingle(platTilePositions.z, last);
			}
			else { // when midTileCount is zero, last tile uses positions of middle tile
				loadTileSingle(platTilePositions.y, last);
			}
	};

	if (tileCount.x == 1 && tileCount.y == 1)
		loadTileSingle({ 0, 0 }, tileTextures[10]); // mid top 

	else if (tileCount.x == 1)
		loadTileLine({ {0,0} ,{0,1} , {0 , tileCount.y - 1} }, { 1, tileCount.y - 2 }, tileTextures[1], tileTextures[4], tileTextures[7]); // mid line

	else if (tileCount.y == 1)
		loadTileLine({ {0,0}, {1,0}, {tileCount.x - 1 , 0} }, { tileCount.x - 2, 1 }, tileTextures[9], tileTextures[10], tileTextures[11]); // top edge
	
	else {
		loadTileLine({{0, 0}, {1, 0}, {tileCount.x - 1, 0}}, { tileCount.x - 2, 1 }, tileTextures[0], tileTextures[1], tileTextures[2]); // top edge
		
		loadTileMultiple({0, 1}, { 1, tileCount.y - 2 }, tileTextures[3]); // left edge

		loadTileMultiple({1, 1}, { tileCount.x - 2, tileCount.y - 2 }, tileTextures[4]); // center

		loadTileMultiple({tileCount.x - 1, 1}, { 1, tileCount.y - 2 }, tileTextures[5]); // right edge

		loadTileLine({
				{0, tileCount.y - 1},
				{1, tileCount.y - 1},
				{tileCount.x - 1, tileCount.y - 1}},
			{ tileCount.x - 2, 1 }, tileTextures[6], tileTextures[7], tileTextures[8]
		); // bottom edge
	}
}

void addPlatform(const sf::Vector2i& tilePos, const sf::Vector2i& tileCount, const sf::Vector2f& tileSize, const std::array<sf::Texture, 12>& tileTextures)
{
	Platform::m_platforms.push_back(std::make_unique<Platform>());
	Platform::m_platforms.back()->setSize(tileCount * tileSize);
	Platform::m_platforms.back()->setPosition(tilePos * tileSize);
	Platform::m_platforms.back()->setID(Collidable::PLATFORM);
	Platform::m_platforms.back()->loadTileTextures(tileCount, tileSize, tileTextures);
}

void addMovingPlatform(const sf::Vector2i& tilePos, const sf::Vector2i& tileCount, const sf::Vector2f& tileSize, const double distance, const sf::Vector2f& speed, const int movingDir, const std::array<sf::Texture, 12>& tileTextures)
{
	Platform::m_movingPlatforms.push_back(std::make_unique<Platform>());
	Platform::m_movingPlatforms.back()->setSize(tileCount * tileSize);
	Platform::m_movingPlatforms.back()->setPosition(tilePos * tileSize);
	Platform::m_movingPlatforms.back()->m_initialPos = sf::Vector2f({ tilePos * tileSize });
	Platform::m_movingPlatforms.back()->loadTileTextures(tileCount, tileSize, tileTextures);
	Platform::m_movingPlatforms.back()->m_distance = distance * tileSize.x;
	Platform::m_movingPlatforms.back()->m_speed = speed;
	Platform::m_movingPlatforms.back()->m_moveDir = movingDir;
	Platform::m_movingPlatforms.back()->setID(Collidable::MOVING_PlAT); 

}
void updateAllMovingPlatforms(float dt)
{
	for (int i = 0; i < Platform::m_movingPlatforms.size(); i++) {
		Platform::m_movingPlatforms[i]->updateMovingPlatform(dt);
	}
}

void Platform::updateMovingPlatform(float dt)
{
	move(this->m_speed.x * m_moveDir, this->m_speed.y * m_moveDir);
	double x, y, x1, y1;
	x = m_initialPos.x;
	y = m_initialPos.y;
	x1 = getPosition().x;
	y1 = getPosition().y;
	double dis = std::sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y));
	if (dis >= m_distance) m_moveDir *= -1;
	else if (dis <= 0) m_moveDir *= -1;
}

void removeAllPlatforms()
{
	Platform::m_platforms.clear();
	Platform::m_movingPlatforms.clear();
}

void drawAllPlatforms(sf::RenderTarget& target, sf::RenderStates states)
{
	for(int i = 0; i < Platform::m_platforms.size(); i++){
		Platform::m_platforms[i]->draw(target, states);
	}
	for (int i = 0; i < Platform::m_movingPlatforms.size(); i++) {
		Platform::m_movingPlatforms[i]->draw(target, states);
	}
}