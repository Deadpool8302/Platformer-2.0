#pragma once

#include <SFML/Graphics.hpp>

template <typename U, typename V>
sf::Vector2f operator*(const sf::Vector2<U>& a, const sf::Vector2<V>& b)
{
	return sf::Vector2f(a.x * b.x, a.y * b.y);
}

template <typename U, typename V>
sf::Vector2f operator/(const sf::Vector2<U>& a, const sf::Vector2<V>& b)
{
	return sf::Vector2f(a.x / b.x, a.y / b.y);
}

template <typename U>
sf::Vector2f unitVec(const sf::Vector2<U>& vec)
{
	sf::Vector2f unit(0, 0);
	if (vec.x != 0) unit.x = vec.x / std::abs(vec.x);
	if (vec.y != 0) unit.y = vec.y / std::abs(vec.y);
	return unit;
}