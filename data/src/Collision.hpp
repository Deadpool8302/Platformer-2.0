#pragma once
#include<SFML/Graphics.hpp>
#include<vector>

namespace Collision {
	////////////////////////////////////////////////////////////
	/// \brief Test for a collision between two sprites by comparing 
	/// the alpha values of overlapping pixels
	/// 
	/// Supports scaling and rotation
	/// AlphaLimit: The threshold at which a pixel becomes "solid". If AlphaLimit is 127, a pixel with
	/// alpha value 128 will cause a collision and a pixel with alpha value 126 will not.
	///
	/// This functions creates bitmasks of the textures of the two sprites by
	/// downloading the textures from the graphics card to memory -> SLOW!
	/// You can avoid this by using the "CreateTextureAndBitmask" function
	/// 
	/// \param Object1 sprite for the first object
	/// \param Object2 sprite for the second object 
	/// 
	/// \return True if collision occurs 
	////////////////////////////////////////////////////////////
	bool PixelPerfectTest(const sf::Sprite& Object1, const sf::Sprite& Object2, sf::Uint8 AlphaLimit = 0);

	////////////////////////////////////////////////////////////
	/// \brief Replaces Texture::loadFromFile
	/// 
	/// Load an imagefile into the given texture and create a bitmask for it
	/// 
	/// This is much faster than creating the bitmask for a texture 
	/// on the first run of "PixelPerfectTest"
	///
	/// \return Returns false if the file could not be opened for some reason
	////////////////////////////////////////////////////////////
	bool CreateTextureAndBitmask(sf::Texture& LoadInto, const std::string& Filename);
}

class Collidable : public sf::Transformable {
public:

	enum class ID { OBSTACLE, PLATFORM, MOVING_PlAT };
	using enum ID;

	////////////////////////////////////////////////////////////
	/// \brief Default Constructor
	///	Adds object to set of all Collidables
	///
	///////////////////////////////////////////////////////////
	Collidable();

	////////////////////////////////////////////////////////////
	/// \brief Default virtual Constructor
	/// Removes object from set of all Collidables
	///
	///////////////////////////////////////////////////////////
	virtual ~Collidable();
	
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
	/// \brief Get the global (non-minimal) bounding rectangle of the entity
	///
	/// The returned rectangle is in global coordinates, which means
	/// that it takes into account the transformations (translation,
	/// rotation, scale, ...) that are applied to the entity.
	/// In other words, this function returns the bounds of the
	/// shape in the global 2D world's coordinate system.
	///
	/// This function does not necessarily return the \a minimal
	/// bounding rectangle. It merely ensures that the returned
	/// rectangle covers all the vertices (but possibly more).
	/// This allows for a fast approximation of the bounds as a
	/// first check; you may want to use more precise checks
	/// on top of that.
	///
	/// \return Global bounding rectangle of the entity
	///
	////////////////////////////////////////////////////////////
	sf::FloatRect getGlobalBounds() const;

	sf::Vector2f getCollidableSpeed();

	ID getID();

	double m_distance;
	sf::Vector2f m_speed;
	sf::Vector2f m_initialPos;
	int m_moveDir;

protected:
	void updateHitbox(const sf::Vector2f& size);
	void setID(ID id);

private:
	friend bool areColliding(const sf::FloatRect& obj1, const Collidable& obj2);
	ID m_id;

public:
	static std::vector<Collidable*> allCollidables;

private:
	sf::FloatRect m_hitbox;							/// < hitbox of the object

};

bool areColliding(const sf::FloatRect& obj1, const Collidable& obj2);