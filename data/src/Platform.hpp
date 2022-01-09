#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <array>
#include <memory>
#include "Collision.hpp"

class Platform : public Collidable
{
public:

	/////////////////////////////////////////////////
	/// \brief Default Constructor
	///
	/////////////////////////////////////////////////
	Platform();

	/////////////////////////////////////////////////
	/// \brief Default Destructor
	///
	/////////////////////////////////////////////////
	~Platform();

	/////////////////////////////////////////////////
	/// \brief set the size of the hitbox of the object
	///
	/// \param size new size of the hitbox of the object 
	///
	/////////////////////////////////////////////////
	void setSize(const sf::Vector2f& size);

	/////////////////////////////////////////////////
	/// \brief get the size of the hitbox of the object 
	///
	/// \return size of the hitbox of the object 
	///
	/////////////////////////////////////////////////
	sf::Vector2f getSize() const;

	////////////////////////////////////////////////////////////
	/// \brief draw the object to a render target
	///
	/// \param target render target to draw to
	/// \param states current render states
	///
	////////////////////////////////////////////////////////////
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

	void updateMovingPlatform(float dt);
private:

	/////////////////////////////////////////////////
	/// \brief load the textures of the tiles of the object 
	///
	/// \param tileCount number of tiles accross and below in the object
	/// \param tileSize size of one tile 
	/// \param tileTextures set of all tile textures 
	///
	/////////////////////////////////////////////////
	void loadTileTextures(const sf::Vector2i& tileCount, const sf::Vector2f& tileSize, const std::array<sf::Texture, 12>& tileTextures);
	
	// to add platforms 
	friend void addPlatform (
		const sf::Vector2i& tilePos, 
		const sf::Vector2i& tileCount, 
		const sf::Vector2f& tileSize, 
		const std::array<sf::Texture, 12>& tileTextures
	);

	friend void addMovingPlatform (
		const sf::Vector2i& tilePos, 
		const sf::Vector2i& tileCount, 
		const sf::Vector2f& tileSize, 
		const double distance,
		const sf::Vector2f& speed,
		const int movingDir,
		const std::array<sf::Texture, 12>& tileTextures
	);
	friend void updateAllMovingPlatforms(float dt);
	friend void drawAllPlatforms(sf::RenderTarget& target, sf::RenderStates states);
	friend void removeAllPlatforms();

private:
	
	/////////////////////////////////////////////////
	// Member Data
	/////////////////////////////////////////////////

	std::vector<sf::Sprite> m_imgs;                 /// < set of sprites of tiles of the object 
	static std::vector<std::unique_ptr<Platform>> m_platforms;		/// < set of all the platforms created 
	static std::vector<std::unique_ptr<Platform>> m_movingPlatforms;		/// < set of all the moving platforms createed
};

/////////////////////////////////////////////////
/// \brief add a platform to the set of all platforms 
///
/// \param tilePos position of platform on the basis of tile map
/// \param tileCount number of tiles accross and below in the object
/// \param tileSize size of one tile 
/// \param tileTextures set of all tile textures 
///
/////////////////////////////////////////////////
void addPlatform(const sf::Vector2i& tilePos, const sf::Vector2i& tileCount, const sf::Vector2f& tileSize, const std::array<sf::Texture, 12>& tileTextures);

void addMovingPlatform(const sf::Vector2i& tilePos, const sf::Vector2i& tileCount, const sf::Vector2f& tileSize, const double distance, const sf::Vector2f& speed, const int movingDir, const std::array<sf::Texture, 12>& tileTextures);
void updateAllMovingPlatforms(float dt);

/////////////////////////////////////////////////
/// \brief clear the set of all platforms 
///
/////////////////////////////////////////////////
void removeAllPlatforms();

////////////////////////////////////////////////////////////
/// \brief draw all platforms to a render target
///
/// \param target render target to draw to
/// \param states current render states
///
////////////////////////////////////////////////////////////
void drawAllPlatforms(sf::RenderTarget& target, sf::RenderStates states);