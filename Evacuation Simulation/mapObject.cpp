#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "mapObject.h"
int16_t mapObject::current_id = 1;
double human::getRadius()
{
	return radius;
}
void human::updatePosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight)
{
	sf::Vector2f circlePos = position;
	circlePos.x = circlePos.x * gridWidth + centerX;
	circlePos.y = circlePos.y * gridHeight + centerY;
	for (size_t i = 0; i < drawable.getPointCount(); ++i) {
		sf::Vector2f pos = points[i];
		pos.x = pos.x * gridWidth / 2 * radius + circlePos.x;
		pos.y = pos.y * gridHeight / 2 * radius + circlePos.y;
		drawable.setPoint(i, pos);
	}
}
void human::move(float path, int16_t gridWidth)
{
	position.x = position.x + direction.x * path / gridWidth;
	position.y = position.y + direction.y * path / gridWidth;
	size_t count = drawable.getPointCount();
	for (size_t i = 0; i < count; ++i) {
		sf::Vector2f curPos = drawable.getPoint(i);
		drawable.setPoint(i, sf::Vector2f(curPos.x + direction.x * path, curPos.y + direction.y * path));
	}
}
human::human(sf::Vector2f pos,  float rad) :  radius(rad)
{
	position = pos;
	myType = type::human;
	id = current_id * 10 + 2;//последн€€ цифра говорит о том, что обьект - человек
	++current_id;

	sf::CircleShape localCircle(1, 12);
	size_t countOfPoints = localCircle.getPointCount();

	drawable.setPointCount(countOfPoints);
	drawable.setPosition(0.f, 0.f);
	for (size_t i = 0; i < countOfPoints; ++i) {
		points.push_back(localCircle.getPoint(i));
		drawable.setPoint(i, points[i]);
	}

}

void mapObject::setFillColor(const sf::Color& col)
{
	drawable.setFillColor(col);
}

void mapObject::addPosition(float x, float y)
{
	size_t countOfPoints = points.size();
	drawable.setPosition(0.f, 0.f);//ќно тут нужно?
	for (size_t i = 0; i < countOfPoints; ++i) {
		sf::Vector2f pos = drawable.getPoint(i);
		drawable.setPoint(i, sf::Vector2f(pos.x + x, pos.y + y));
	}
}

sf::ConvexShape mapObject::getShape() const
{
	return drawable;
}

std::pair<double, double> mapObject::pointsProjectToLine(const sf::Vector2f& Line, const sf::Vector2f& Normal) const
{
	double max = 0.9, min = 0.9;
	if (Line.x != 0) {
		for (sf::Vector2f locPoint : points) {//ќбщий случай
			double locX = (locPoint.x - locPoint.y * Normal.x / Normal.y) / (1 - Line.y * Normal.x / (Line.x * Normal.y));


			if (min == 0.9 || locX < min) {
				min = locX;
			}
			if (max == 0.9 || locX > max) {
				max = locX;
			}
		}
	}
	else {
		for (sf::Vector2f locPoint : points) {//„астный случай дл€ пр€мой перпендикул€рной оси X
			double locY = (locPoint.y - locPoint.x * Normal.y / Normal.x) / (1 - Line.x * Normal.y / (Line.y * Normal.x));


			if (min == 0.9 || locY < min)
				min = locY;
			if (max == 0.9 || locY > max)
				max = locY;
		}
	}
	return std::pair<double, double>(min, max);
}

void mapObject::setPosition(sf::Vector2f pos)
{
	position = pos;
}

sf::Vector2f mapObject::getPosition()
{
	return position;
}

int16_t mapObject::getId()
{
	return id;
}

//переводит рисунок в точные кординаты
void wall::updatePosition(int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight)
{
	sf::Vector2f wallPos = position;
	wallPos.x = wallPos.x * gridWidth + centerX;
	wallPos.y = wallPos.y * gridHeight + centerY;
	for (size_t i = 0; i < drawable.getPointCount(); ++i) {
		sf::Vector2f pos = points[i];
		pos.x = pos.x * gridWidth + wallPos.x;
		pos.y = pos.y * gridHeight + wallPos.y;
		drawable.setPoint(i, pos);
	}
}

wall::wall(const std::vector<sf::Vector2f>& vectorOfPoints) 
{
	points = vectorOfPoints;
	size_t countOfPoints = points.size();
	drawable.setPointCount(countOfPoints);
	drawable.setPosition(0.f, 0.f);
	for (size_t i = 0; i < countOfPoints; ++i) {
		drawable.setPoint(i, points[i]);
	}

	myType = type::wall;
	id = current_id * 10 + 1;//последн€€ цифра говорит о том, что обьект - стена
	++current_id;
}

std::ostream& operator<<(std::ostream& stream, mapObject& object)
{
	stream << object.myType << '\n';
	stream << object.id << '\n';
	stream << object.position.x << '\n';
	stream << object.position.y << '\n';
	stream << object.points.size() << '\n';
	for (sf::Vector2f loc : object.points) {
		stream << loc.x << '\n';
		stream << loc.y << '\n';
	}
	stream << object.drawable.getPointCount() << '\n';
	for (size_t i = 0; i < object.drawable.getPointCount(); ++i) {
		sf::Vector2f loc = object.drawable.getPoint(i);
		stream << loc.x << '\n';
		stream << loc.y << '\n';
	}
	return stream;
}
std::istream& operator>>(std::istream& stream, mapObject& object)
{
	stream >> object.id;
	sf::Vector2f pos;
	stream >> pos.x;
	stream >> pos.y;
	object.position = pos;

	size_t sizeOfPoints;
	stream >> sizeOfPoints;
	for (size_t i = 0; i < sizeOfPoints; ++i) {
		sf::Vector2f loc;
		stream >> loc.x;
		stream >> loc.y;
		object.points.push_back(loc);
	}

	size_t countOfDrawablePoints;
	stream >> countOfDrawablePoints;
	object.drawable.setPointCount(countOfDrawablePoints);
	for (size_t i = 0; i < countOfDrawablePoints; ++i) {
		sf::Vector2f loc;
		stream >> loc.x;
		stream >> loc.y;
		object.drawable.setPoint(i, loc);
	}
	return stream;
}