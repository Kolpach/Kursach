#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <ctime>
#include "Evacuation.h"

inline void Evacuation::changeConfig() {//Функция изменяет параметры окна при изменении размера окна
	sf::Vector2u sizeVector = window.getSize();
	windowWidth = sizeVector.x;
	windowHeight = sizeVector.y;
	centerX = windowWidth / 2;
	centerY = windowHeight / 2;
	max_count_of_cells = 60;
	gridWidth = windowWidth / max_count_of_cells;
	gridHeight = gridWidth;
}

void Evacuation::drawObjects()//рисует все обьекты в objectStorage
{
	for (auto a : objectStorage) {
		window.draw(a.second->getShape());
	}
	window.display();
}

void Evacuation::drawBackground()
{
	sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));
	background.setFillColor(backgroundColor);
	window.draw(background);
}

void Evacuation::drawArrows()
{
	sf::RectangleShape drawableArrow;
	drawableArrow.setPosition(0.f, 0.f);
	drawableArrow.setOrigin(0.f, 1.5);//выставляем ось вращения
	drawableArrow.setFillColor(arrowColor);

	sf::CircleShape triangle(gridWidth / 2, 3);
	triangle.setOrigin(triangle.getRadius(), 0.f);
	triangle.setFillColor(arrowColor);
	for (auto arrow : akaGraph) {
		sf::Vector2i point1 = arrow.first;
		sf::Vector2i point2 = arrow.second;

		float length = sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2)) * gridWidth;//задаём длинну линии
		drawableArrow.setSize(sf::Vector2f(length, 3.f));
		drawableArrow.setPosition(point2.x * gridWidth + centerX, point2.y * gridHeight + centerY);
		triangle.setPosition(point2.x * gridWidth + centerX, point2.y * gridHeight + centerY);

		float additionalDeg = (point2.x - point1.x) > 0 ? 180 : 0;//сложные условия для правильного поворота
		if ((point2.x - point1.x) == 0 && (point2.y - point1.y) != 0) {
			drawableArrow.setRotation(90 * -(point2.y - point1.y) / abs(point2.y - point1.y));
			triangle.setRotation(90 * -(point2.y - point1.y) / abs(point2.y - point1.y) - 90);
		}
		else if ((point2.y - point1.y) != 0) {
			drawableArrow.setRotation(atan(float(point2.y - point1.y) / float(point2.x - point1.x)) * (180.f / 3.14159265) + additionalDeg);
			triangle.setRotation(atan(float(point2.y - point1.y) / float(point2.x - point1.x)) * (180.f / 3.14159265) + additionalDeg - 90);
		}
		else {
			drawableArrow.setRotation(0 + additionalDeg);
			triangle.setRotation(0 + additionalDeg - 90);
		}
		window.draw(drawableArrow);
		window.draw(triangle);
	}
	window.display();
}

template<class iterType, class vecValue>
inline iterType Evacuation::findInVector(iterType begin, iterType end, const vecValue value)//шаблонная функция для поиска в векторе
{
	for (iterType i = begin; i != end; ++i) {
		if (*i == value)
			return i;
	}
	return end;
}

void Evacuation::loadFromFile(std::string name)
{
	std::ifstream file(name);
	objectStorage.clear();
	akaGraph.clear();
	int size = 0;
	file >> size;
	if (size != 0) {
		for (int i = 0; i < size; ++i) {
			sf::Vector2i begin, end;
			file >> begin.x;
			file >> begin.y;
			file >> end.x;
			file >> end.y;
			akaGraph.push_back(std::pair<sf::Vector2i, sf::Vector2i>(begin, end));
		}
	}
	while (!file.eof()) {
		int num = 0;
		file >> num;
		if (num == 0)
			continue;
		mapObject::type locType = static_cast<mapObject::type>(num);
		switch (locType)
		{
		case mapObject::wall: {
			std::vector<sf::Vector2f> EMPTY_OBJECTS_CANT_BE_CREATED;
			wall* locObject = new wall(EMPTY_OBJECTS_CANT_BE_CREATED);
			file >> *locObject;
			objectStorage.emplace(locObject->getId(), locObject);
			break;
		}
		case mapObject::human: {
			human* locObject = new human(sf::Vector2f(0.f, 0.f), 2);
			file >> *locObject;
			objectStorage.emplace(locObject->getId(), locObject);
			break;
		}
		default:
			break;
		}
		file.ignore(1);
	}
	std::cout << "fineshed" << std::endl;
}

void Evacuation::addObject(mapObject* object)//добавить обьект
{
	objectStorage.emplace(object->getId(), object);
}

void Evacuation::deleteObject(const int16_t key)//удалить обьект
{
	delete objectStorage[key];
	objectStorage.erase(key);
}

void Evacuation::setDirection()
{
	if (!alarm) {
		int degree;
		for (auto loc : movableObjects) {
			human* object = dynamic_cast<human*>(loc.owner);
			if (object->needToChangeDirection) {
				if (object->direction.x == 0) {
					degree = 90 * object->direction.y / abs(object->direction.y);
				}
				else {
					degree = atan(object->direction.y / object->direction.x) * 180.0 / 3.14159265 * object->direction.x / abs(object->direction.x);
				}
				degree += std::rand() % 180;
				object->direction.x = cos(float(degree) * 3.14159265 / 180.0);
				object->direction.y = sin(float(degree) * 3.14159265 / 180.0);
				object->needToChangeDirection = false;
			}
		}
	}
}
//Каждый фрейм
void Evacuation::eachFrame(sf::Int32 elapced)
{
	setDirection();
	std::pair<mapObject*, mapObject*> objects = checkCollision(float(elapced) * gridWidth * 0.0001 * humanSpeed);//так не должно быть
	/*if (objects.first != nullptr) {
		human* obj = dynamic_cast<human*>(objects.first);
		sf::Vector2f backDir = obj->direction;
		backDir.x = backDir.x * -1;
		backDir.y = backDir.y * -1;
		dynamic_cast<human*>(objects.first)->needToChangeDirection = true;
	}*/
	
	//двигаем объекты
	for (auto& obj : movableObjects) {
		if (dynamic_cast<human*>(obj.owner)->direction != sf::Vector2f(0.f, 0.f)) {
			dynamic_cast<human*>(obj.owner)->move(float(elapced) * gridWidth * 0.0001 * humanSpeed, gridWidth);
			obj = projectToAxis(obj.owner);
		}
	}
	window.clear();
	drawBackground();
	drawObjects();
}

void Evacuation::start()
{

	int seed = 13;
	std::srand(seed);

	window.clear();
	changeConfig();//сохраняем текущие параметры окна
	for (auto a : objectStorage) {//обновляем положение объектов
		a.second->updatePosition(centerX, centerY, gridWidth, gridHeight);
	}
	drawBackground();
	drawObjects();//рисуем объекты
	drawArrows();//рисуем направления движения

	for (auto a : objectStorage) {//заполняем текущие проекции на оси
		projections local = projectToAxis(a.second);
		local.owner = a.second;
		if (a.second->myType == mapObject::type::wall) {
			constObjects.push_back(local);
		}
		else {
			movableObjects.push_back(local);
		}
	}

	sf::Clock timer;
	while (window.isOpen())
	{
		sf::Time elapsed = timer.restart();
		eachFrame(elapsed.asMilliseconds());
		sf::Event eve;
		while (window.pollEvent(eve))
		{
			if (eve.type == sf::Event::Resized) {//изменение размера
				uint32_t dy = eve.size.height;
				uint32_t dx = eve.size.width;
				bool needToBeChanged = false;
				if (dy < 400) {//Ограничения на размер окна
					dy = 400;
					needToBeChanged = true;
				}
				if (dx < 600) {
					dx = 600;
					needToBeChanged = true;
				}
				if (needToBeChanged) {
					window.setSize(sf::Vector2u(dx, dy));
				}
				window.setView(sf::View(sf::FloatRect(0, 0, dx, dy)));
				changeConfig();//сохраняем текущие параметры окна
				drawBackground();
				for (auto a : objectStorage) {//обновляем положение объектов
					a.second->updatePosition(centerX, centerY, gridWidth, gridHeight);
				}
				//обновляем проекции
				for (auto& obj : constObjects) {
					obj = projectToAxis(obj.owner);
				}
				for (auto& obj : movableObjects) {
					obj = projectToAxis(obj.owner);
				}
				drawObjects();//рисуем объекты
				drawArrows();//рисуем направления движения
			}
			if (eve.type == sf::Event::Closed) {//закрытие окна
				window.close();
			}
			if (eve.type == sf::Event::KeyPressed) {//реакция на нажатия клавиш
				
				/*if (eve.key.code == sf::Keyboard::B)
				{
					sf::CircleShape dot(5);
					window.clear();
					for (auto a : objectStorage) {
						projections local = projectToAxis(a.second);
						dot.setPosition(sf::Vector2f(local.X.first - 5, local.Y.first - 5));
						window.draw(dot);
						dot.setPosition(sf::Vector2f(local.X.first - 5, local.Y.second - 5));
						window.draw(dot);
						dot.setPosition(sf::Vector2f(local.X.second - 5, local.Y.second - 5));
						window.draw(dot);
						dot.setPosition(sf::Vector2f(local.X.second - 5, local.Y.first - 5));
						window.draw(dot);
					}
					window.display();
				}*/
				if (eve.key.code == sf::Keyboard::Q)
				{
					window.close();
				}
			}
			
		}
	}
}

Evacuation::~Evacuation()
{
	while (objectStorage.size()) {
		std::map<int16_t, mapObject*>::iterator iterToFirst = objectStorage.begin();
		deleteObject((*iterToFirst).first);
	}
}

Evacuation::projections Evacuation::projectToAxis(mapObject* object)
{
	sf::Vector2f position = object->getPosition();
	position.x = position.x * gridWidth + centerX;
	position.y = position.y * gridHeight + centerY;
	Evacuation::projections cords;
	cords.owner = object;
	if (object->myType == mapObject::type::human) {
		cords.X.first = position.x;
		cords.X.second = position.x + gridWidth * 2;//2 - радиус
		cords.Y.first = position.y;
		cords.Y.second = position.y + gridHeight * 2;//2 - радиус
	}
	else {
		cords.X.first = position.x;
		cords.Y.first = position.y;
		cords.X.second = position.x;
		cords.Y.second = position.y;
		sf::ConvexShape locShape = object->getShape();
		for (size_t i = 0; i < locShape.getPointCount(); ++i)
		{
			sf::Vector2f locP = locShape.getPoint(i);
			if (cords.X.second < locP.x)
				cords.X.second = locP.x;
			if (cords.Y.second < locP.y)
				cords.Y.second = locP.y;
		}
	}
	return cords;
}






//колизия по принципу obb
bool Evacuation::narrowCollision(const mapObject& firstObj, const mapObject& secondObj) {
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
std::pair<mapObject*, mapObject*> Evacuation::tryThisVariant(std::vector<Evacuation::projections>& constObjects, std::vector<Evacuation::projections>& movableObjects)
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
bool Evacuation::intersect(const Evacuation::projections& a, const Evacuation::projections& b)
{
	if (a.X.second < b.X.first || a.X.first > b.X.second)
		return false;
	if (a.Y.second < b.Y.first || a.Y.first > b.Y.second)
		return false;
	return true;
}

std::pair<mapObject*, mapObject*> Evacuation::checkCollision(float path)
{
	std::vector<Evacuation::projections> constObjectsLoc = constObjects;
	std::vector<Evacuation::projections> movableObjectsLoc = movableObjects;
	//симулируем следующий фрейм НО не двигаем объекты
	for (auto& obj : movableObjectsLoc) {
		//dynamic_cast<human*>(obj.owner)->move(path, gridWidth);
		//obj = projectToAxis(obj.owner);
		sf::Vector2f direction = dynamic_cast<human*>(obj.owner)->direction;
		obj.X.first += path * direction.x;
		obj.X.second += path * direction.x;
		obj.Y.first += path * direction.y;
		obj.Y.second += path * direction.y;
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
					dynamic_cast<human*>(locM.owner)->direction = sf::Vector2f(0.f, 0.f);
					dynamic_cast<human*>(locM.owner)->needToChangeDirection = true;
				}
			}
		}
	}
	for (std::vector<Evacuation::projections>::iterator i = movableObjects.begin(); i != movableObjects.end(); ++i) {
		for (std::vector<Evacuation::projections>::iterator j = i + 1; j != movableObjects.end(); ++j) {
			if (intersect(*i, *j)) {
				if (narrowCollision(*(i->owner), *(j->owner))) {
					if (alarm) {

					}
					else {
						dynamic_cast<human*>(i->owner)->direction = sf::Vector2f(0.f, 0.f);
						dynamic_cast<human*>(i->owner)->needToChangeDirection = true;
						dynamic_cast<human*>(j->owner)->direction = sf::Vector2f(0.f, 0.f);
						dynamic_cast<human*>(j->owner)->needToChangeDirection = true;
					}

				}
				
			}
		}
	}
	return std::pair<mapObject*, mapObject*>(nullptr, nullptr);
}
