#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "mapObject.h"
int16_t mapObject::current_id = 1;
hateYouRectangle::Shapes* human::getColliderAndSpritre()
{
	return &colliderAndSprite;
}
human::human(float radius) : colliderAndSprite(radius)
{
	id = current_id * 10 + 2;//последн€€ цифра говорит о том, что обьект - человек
	++current_id;
}

hateYouRectangle::Shapes* mapObject::getColliderAndSpritre()
{
	return nullptr;
}

int16_t mapObject::getId()
{
	return id;
}


hateYouRectangle::Shapes* wall::getColliderAndSpritre()
{
	return &colliderAndSprite;
}

wall::wall(const std::vector<sf::Vector2f>& vectorOfPoints) : colliderAndSprite(vectorOfPoints)
{
	id = current_id * 10 + 1;//последн€€ цифра говорит о том, что обьект - стена
	++current_id;
}
