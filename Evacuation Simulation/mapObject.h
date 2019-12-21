#pragma once
#include "Shapes.h"
class mapObject
{
protected:
	mapObject() = default;
	static int16_t current_id;
	int16_t id;
public:
	enum type{ wall = 1, human};
	type myType;
	virtual hateYouRectangle::Shapes* getColliderAndSprite();
	virtual sf::Vector2f getPosition();
	virtual std::vector<sf::Vector2f> getGlobalPosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight);
	int16_t getId();
	virtual ~mapObject() = default;
};

class wall : public mapObject
{
public:
	virtual sf::Vector2f getPosition() override;
	virtual hateYouRectangle::Shapes* getColliderAndSprite() override;
	virtual std::vector<sf::Vector2f> getGlobalPosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight) override;
	hateYouRectangle::Rectangle colliderAndSprite;
	wall(const std::vector<sf::Vector2f>& vectorOfPoints);

};
class human : public mapObject
{
private:
	sf::Vector2f position;//можно удалить, но пока оставлю
public:
	virtual sf::Vector2f getPosition() override;
	virtual hateYouRectangle::Shapes* getColliderAndSprite() override;
	virtual std::vector<sf::Vector2f> getGlobalPosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight) override;
	hateYouRectangle::Circle colliderAndSprite;
	human(sf::Vector2f position, float radius);
	bool hero = false;
};