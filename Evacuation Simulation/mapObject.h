#pragma once
class mapObject
{
protected:
	mapObject() = default;
	static int16_t current_id;
	int16_t id;
	sf::ConvexShape drawable;//-------
	sf::Vector2f position;
	std::vector<sf::Vector2f> points;//-------
public:
	void setFillColor(const sf::Color & col);//-------
	void addPosition(float x, float y);//-------
	sf::ConvexShape getShape() const;//-------
	std::pair<double, double> pointsProjectToLine(const sf::Vector2f& A, const sf::Vector2f& Normal) const;//-------

	enum type{ wall = 1, human};
	type myType;
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();
	virtual void updatePosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight) {};
	int16_t getId();
	virtual ~mapObject() = default;
	friend std::ostream& operator<<(std::ostream& stream, mapObject& object);
	friend std::istream& operator>>(std::istream& stream, mapObject& object);
};


class wall : public mapObject
{
public:
	virtual void updatePosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight) override;
	wall(const std::vector<sf::Vector2f>& vectorOfPoints);
};
class human : public mapObject
{
private:
	double radius;
public:
	double getRadius();
	bool needToChangeDirection = true;
	sf::Vector2f direction = sf::Vector2f(0.f, 0.f);
	virtual void updatePosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight) override;
	void move(float, int16_t gridWidth);
	human(sf::Vector2f position, float radius);
	bool hero = false;
};

std::ostream& operator<<(std::ostream& stream, mapObject& object);
std::istream& operator>>(std::istream& stream, mapObject& object);