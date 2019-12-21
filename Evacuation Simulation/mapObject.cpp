#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "mapObject.h"
int16_t mapObject::current_id = 1;
sf::Vector2f human::getPosition()
{
	return position;
}
hateYouRectangle::Shapes* human::getColliderAndSprite()
{
	return &colliderAndSprite;
}
std::vector<sf::Vector2f> human::getGlobalPosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight)
{
	std::vector<sf::Vector2f> local = getColliderAndSprite()->points;
	sf::Vector2f pos = position;
	pos.x = pos.x * gridWidth + centerX;
	pos.y = pos.y * gridHeight + centerY;
	for (size_t i = 0; i < local.size(); ++i) {
		local[i].x = local[i].x * gridWidth / 2 * 2 + pos.x;
		local[i].y = local[i].y * gridHeight / 2 * 2 + pos.y;
	}
	return local;
}
human::human(sf::Vector2f pos,  float radius) : position(pos), colliderAndSprite(radius)
{
	myType = type::human;
	id = current_id * 10 + 2;//последн€€ цифра говорит о том, что обьект - человек
	++current_id;
}

hateYouRectangle::Shapes* mapObject::getColliderAndSprite()
{
	return nullptr;
}

sf::Vector2f mapObject::getPosition()
{
	return sf::Vector2f(0, 0);
}

std::vector<sf::Vector2f> mapObject::getGlobalPosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight)
{
	return std::vector<sf::Vector2f>(NULL);
}

int16_t mapObject::getId()
{
	return id;
}


sf::Vector2f wall::getPosition()
{
	return sf::Vector2f(0, 0);
}

hateYouRectangle::Shapes* wall::getColliderAndSprite()
{
	return &colliderAndSprite;
}
//преобразовывает в точные кординаты
std::vector<sf::Vector2f> wall::getGlobalPosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight)
{
	std::vector<sf::Vector2f> local = getColliderAndSprite()->points;
	for (size_t i = 0; i < local.size(); ++i) {
		local[i].x = local[i].x * gridWidth + centerX;
		local[i].y = local[i].y * gridHeight + centerY;
	}
	return local;
}

wall::wall(const std::vector<sf::Vector2f>& vectorOfPoints) : colliderAndSprite(vectorOfPoints)
{
	myType = type::wall;
	id = current_id * 10 + 1;//последн€€ цифра говорит о том, что обьект - стена
	++current_id;
}
