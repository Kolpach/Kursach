#include "Collision.h"
//колизия по принципу obb
bool BroadPhase::narrowCollision(const mapObject& firstObj, const mapObject& secondObj) {
	sf::ConvexShape one = firstObj.getShape();
	sf::ConvexShape two = secondObj.getShape();
	if (one.getPointCount() < 2 && two.getPointCount() < 2)
		throw "unpredictable count of points in rectangle";
	//проверка сторон первого многоугольника на разделяющую ось
	for (size_t i = 0; i < one.getPointCount(); i++) {
		sf::Vector2f normal;
		sf::Vector2f localFirstPoint;
		sf::Vector2f localSecondPoint;
		localFirstPoint = one.getPoint(i);

		if (i == one.getPointCount() - 1) {
			localSecondPoint = one.getPoint(0);
			normal = sf::Vector2f(localSecondPoint.x - localFirstPoint.x, localSecondPoint.y - localFirstPoint.y);
		}
		else {
			localSecondPoint = one.getPoint(i + 1);
			normal = sf::Vector2f(localSecondPoint.x - localFirstPoint.x, localSecondPoint.y - localFirstPoint.y);
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
		std::pair<double, double> intervalCoords = firstObj.pointsProjectToLine(line, normal);
		std::pair<double, double> intervalCoords2 = secondObj.pointsProjectToLine(line, normal);

		if (intervalCoords.second < intervalCoords2.first)
			return false;
		if (intervalCoords2.second < intervalCoords.first)
			return false;
	}
	//проверка сторон второго многоугольника на разделяющую ось
	for (size_t i = 0; i < two.getPointCount(); i++) {
		sf::Vector2f normal;
		sf::Vector2f localFirstPoint;
		sf::Vector2f localSecondPoint;
		localFirstPoint = two.getPoint(i);

		if (i == two.getPointCount() - 1) {
			localSecondPoint = two.getPoint(0);
			normal = sf::Vector2f(localSecondPoint.x - localFirstPoint.x, localSecondPoint.y - localFirstPoint.y);
		}
		else {
			localSecondPoint = two.getPoint(i + 1);
			normal = sf::Vector2f(localSecondPoint.x - localFirstPoint.x, localSecondPoint.y - localFirstPoint.y);
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

		std::pair<double, double> intervalCoords = firstObj.pointsProjectToLine(line, normal);
		std::pair<double, double> intervalCoords2 = secondObj.pointsProjectToLine(line, normal);

		if (intervalCoords.second < intervalCoords2.first)
			return false;
		if (intervalCoords2.second < intervalCoords.first)
			return false;
	}
	return true;//Если не нашлось ни одной разделяющей оси
}
std::pair<mapObject*, mapObject*> BroadPhase::tryThisVariant(std::vector<Evacuation::projections>& constObjects, std::vector<Evacuation::projections>& movableObjects, bool alarm)
{
	for (Evacuation::projections locM : movableObjects) {
		for (Evacuation::projections locC : constObjects) {
			if (intersect(locM, locC)) {
				return std::pair<mapObject*, mapObject*>(locM.owner, locC.owner);//тут надо поменять возвращаемое значение
			}
		}
	}
	return std::pair<mapObject*, mapObject*>(nullptr, nullptr);
}
//смотрим на пересечение двух промежутков
bool BroadPhase::intersect(const Evacuation::projections& a, const Evacuation::projections& b)
{
	if (a.X.second < b.X.first || a.X.first > b.X.second)
		return false;
	if (a.Y.second < b.Y.first || a.Y.first > b.Y.second)
		return false;
	return true;
}

std::pair<mapObject*, mapObject*> BroadPhase::checkCollision(std::vector<Evacuation::projections>& constObjects, std::vector<Evacuation::projections>& movableObjects, bool alarm, float path, int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight)
{
	std::vector<Evacuation::projections> constObjectsLoc = constObjects;
	std::vector<Evacuation::projections> movableObjectsLoc = movableObjects;
	//симулируем следующий фрейм
	for (auto& obj : movableObjectsLoc) {
		dynamic_cast<human*>(obj.owner)->move(path, gridWidth);
		obj = projectToAxis(obj.owner, centerX, centerY, gridWidth, gridHeight);
	}

	//В этом цикле проверяем коллизии между стеной и людьми
	for (Evacuation::projections& locM : movableObjectsLoc) {
		sf::Vector2f direction = dynamic_cast<human*>(locM.owner)->direction;
		for (Evacuation::projections locC : constObjectsLoc) {
			if (intersect(locM, locC)) {
				//if (narrowCollision(*locM.owner, *locC.owner)) {
				//return std::pair<mapObject*, mapObject*>(locM.owner, locC.owner);//тут надо поменять возвращаемое значение
				//}
				if (alarm) {

				}
				else {

				}
			}
		}
	}
	return std::pair<mapObject*, mapObject*>(nullptr, nullptr);
}
