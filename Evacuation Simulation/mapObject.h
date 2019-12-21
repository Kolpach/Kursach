#pragma once
#include "Shapes.h"
class mapObject
{
protected:
	mapObject() = default;
	static int16_t current_id;
	int16_t id;
public:
	virtual hateYouRectangle::Shapes* getColliderAndSpritre();
	int16_t getId();
	virtual ~mapObject() = default;
};

class wall : public mapObject
{
public:
	virtual hateYouRectangle::Shapes* getColliderAndSpritre() override;
	hateYouRectangle::Rectangle colliderAndSprite;
	wall(const std::vector<sf::Vector2f>& vectorOfPoints);

};
class human : public mapObject
{
public:
	virtual hateYouRectangle::Shapes* getColliderAndSpritre() override;
	hateYouRectangle::Circle colliderAndSprite;
	human(float radius);
	bool hero = false;
};