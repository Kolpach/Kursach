#include "Collision.h"

//колизия по принципу obb
bool collision(const hateYouRectangle::Shapes& one, const hateYouRectangle::Shapes& two) {
	if (one.points.size() < 2 && two.points.size() < 2)
		throw "unpredictable count of points in rectangle";
	//проверка сторон первого многоугольника на разделяющую ось
	for (size_t i = 0; i < one.points.size(); i++) {
		sf::Vector2f normal;
		if (i == one.points.size() - 1) {
			normal = sf::Vector2f(one.points[0].x - one.points[i].x, one.points[0].y - one.points[i].y);
		}
		else {
			normal = sf::Vector2f(one.points[i + 1].x - one.points[i].x, one.points[i + 1].y - one.points[i].y);
		}

		
		sf::Vector2f line;
		if (normal.y != 0) {//общий случай
			line.x = 1;
			line.y = -normal.x / normal.y;
		}
		else {//случай для прямой совпадающей с осью X по направлению
			line.x = 0;
			line.y = 1;
		}

		std::pair<double, double> intervalCoords = one.pointsProjectToLine( line, normal);
		std::pair<double, double> intervalCoords2 = two.pointsProjectToLine( line, normal);

		if (intervalCoords.second < intervalCoords2.first)
			return false;
		if (intervalCoords2.second < intervalCoords.first)
			return false;
	}
	//проверка сторон второго многоугольника на разделяющую ось
	for (size_t i = 0; i < two.points.size(); i++) {
		sf::Vector2f normal;
		if (i == two.points.size() - 1) {
			normal = sf::Vector2f(two.points[0].x - two.points[i].x, two.points[0].y - two.points[i].y);
		}
		else {
			normal = sf::Vector2f(two.points[i + 1].x - two.points[i].x, two.points[i + 1].y - two.points[i].y);
		}


		sf::Vector2f line;
		if (normal.y != 0) {//общий случай
			line.x = 1;
			line.y = -normal.x / normal.y;
		}
		else {//случай для прямой совпадающей с осью X по направлению
			line.x = 0;
			line.y = 1;
		}

		std::pair<double, double> intervalCoords = one.pointsProjectToLine(line, normal);
		std::pair<double, double> intervalCoords2 = two.pointsProjectToLine(line, normal);

		if (intervalCoords.second < intervalCoords2.first)
			return false;
		if (intervalCoords2.second < intervalCoords.first)
			return false;
	}
	return true;//Если не нашлось ни одной разделяющей оси
}