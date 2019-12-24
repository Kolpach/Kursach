#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <ctime>
#include "levelDisigner.h"


inline void levelDisigner::changeConfig() {//Функция изменяет параметры окна при изменении размера окна
	sf::Vector2u sizeVector = window.getSize();
	windowWidth = sizeVector.x;
	windowHeight = sizeVector.y;
	centerX = windowWidth / 2;
	centerY = windowHeight / 2;
	max_count_of_cells = 60;
	gridWidth = windowWidth / max_count_of_cells;
	gridHeight = gridWidth;
}

inline void levelDisigner::drawGrid() {//Функция рисует решётку


	sf::RectangleShape lineX(sf::Vector2f(windowWidth, 2.f));
	sf::RectangleShape lineY(sf::Vector2f(2.f, windowHeight));
	sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));


	background.setFillColor(backgroundColor);
	window.draw(background);

	lineY.setFillColor(gridColor);
	lineY.setPosition(centerX - 1, 0);
	window.draw(lineY);
	for (int i = 1; centerX - 1 + gridWidth * i < windowWidth; ++i) {//Ширина решётки определяется в зависимости от ширины окна, поэтому всегда будет 60 клеток по ширине
		lineY.setPosition(centerX - 1 + gridWidth * i, 0);
		window.draw(lineY);
		lineY.setPosition(centerX - 1 - gridWidth * i, 0);
		window.draw(lineY);
	}

	lineX.setFillColor(gridColor);
	lineX.setPosition(0, centerY - 1);
	window.draw(lineX);
	for (int i = 1; centerY - 1 + gridHeight * i < windowHeight; ++i) {//Количество решёток по горизонтали варьируется от высоты окна
		lineX.setPosition(0, centerY - 1 - gridHeight * i);
		window.draw(lineX);
		lineX.setPosition(0, centerY - 1 + gridHeight * i);
		window.draw(lineX);
	}
	window.display();
}

inline void levelDisigner::drawCells() {//закрашивает клетку
	if (selectedChecks.size() != 0) {
		sf::RectangleShape cell(sf::Vector2f(gridWidth, gridHeight));
		for (sf::Vector2i pos : selectedChecks) {
			cell.setPosition(sf::Vector2f(pos.x * gridWidth + centerX, pos.y * gridHeight + centerY));
			cell.setFillColor(selectColor);
			window.draw(cell);
		}
		window.display();
	}
}

void levelDisigner::drawObjects()//рисует все обьекты в objectStorage
{
	for (auto a : objectStorage) {
		window.draw(a.second->getShape());
	}
	window.display();
}

void levelDisigner::drawArrows()
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
		drawableArrow.setSize(sf::Vector2f(length , 3.f));
		drawableArrow.setPosition(point2.x * gridWidth + centerX, point2.y * gridHeight + centerY);
		triangle.setPosition(point2.x * gridWidth + centerX, point2.y * gridHeight + centerY);

		float additionalDeg = (point2.x - point1.x) > 0 ? 180 : 0;//сложные условия для правильного поворота
		if ((point2.x - point1.x) == 0 && (point2.y - point1.y) != 0) {
			drawableArrow.setRotation(90 * -(point2.y - point1.y) / abs(point2.y - point1.y));
			triangle.setRotation(90 * -(point2.y - point1.y) / abs(point2.y - point1.y) - 90);
		}
		else if ((point2.y - point1.y) != 0) {
			drawableArrow.setRotation(atan(float(point2.y - point1.y) / float(point2.x - point1.x)) * (180.f / 3.14) + additionalDeg);
			triangle.setRotation(atan(float(point2.y - point1.y) / float(point2.x - point1.x)) * (180.f / 3.14) + additionalDeg - 90);
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
inline iterType levelDisigner::findInVector(iterType begin, iterType end, const vecValue value)//шаблонная функция для поиска в векторе
{
	for (iterType i = begin; i != end; ++i) {
		if (*i == value)
			return i;
	}
	return end;
}

inline sf::Vector2f levelDisigner::getPositionOnGrid(double x, double y) {//устанавливает позицию, полученную от клика, в левый верхний левый угол
	double deltaX = (x - centerX) / abs(x - centerX);//может быть -1 при расположении левее относ. центра, и 1 при расположении правее относ. центра
	double posX = centerX;
	while (deltaX * (posX - centerX) < deltaX * (x - centerX)) {
		posX += gridWidth * deltaX;
	}
	posX -= deltaX > 0 ? gridWidth : 0;

	double deltaY = (y - centerY) / abs(y - centerY);//может быть -1 при расположении выше относ. центра, и 1 при расположении ниже относ. центра
	double posY = centerY;
	while (deltaY * (posY - centerY) < deltaY * (y - centerY)) {
		posY += gridHeight * deltaY;
	}
	posY -= deltaY > 0 ? gridHeight : 0;
	return sf::Vector2f(posX, posY);
}

inline bool levelDisigner::selectCheck(const sf::Vector2f& position) {// Функция меняет значение клетки и возвращает результат
	sf::Vector2i pos = sf::Vector2i((position.x - centerX) / gridWidth, (position.y - centerY) / gridHeight);//Внутри вектора хранятся не реальные координаты, а кординаты относительно центра
	std::vector<sf::Vector2i>::const_iterator a = findInVector(selectedChecks.begin(), selectedChecks.end(), pos);//поиск в векторе
	if (a != selectedChecks.end()) {
		selectedChecks.erase(a);
		return false;
	}
	else {
		selectedChecks.push_back(pos);
		return true;
	}
}

void levelDisigner::saveToFile()
{
	std::string name;
	std::cout << "Save as: ";
	std::cin >> name;
	std::ofstream file("saves\\" + name + ".txt");
	file << akaGraph.size() << '\n';
	for (std::pair<sf::Vector2i, sf::Vector2i> loc : akaGraph) {
		file << loc.first.x << '\n';
		file << loc.first.y << '\n';
		file << loc.second.x << '\n';
		file << loc.second.y << '\n';
	}
	for (std::pair<int16_t, mapObject*> loc : objectStorage) {
		file << *loc.second << std::endl;
	}
	std::cout << "saved" << std::endl;
}

void levelDisigner::loadFromFile()
{
	std::string name;
	std::cout << "File name: ";
	std::cin >> name;
	std::ifstream file("saves\\" + name + ".txt");
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
		case mapObject::wall:{
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
	window.clear();
	changeConfig();//сохраняем текущие параметры окна
	drawGrid();//рисуем решётку
	drawCells();//рисуем закрашенные клетки
	for (auto a : objectStorage) {//обновляем положение объектов
		a.second->updatePosition(centerX, centerY, gridWidth, gridHeight);
	}
	drawObjects();//рисуем оюъекты
	drawArrows();//рисуем направления движения
	std::cout << "fineshed" << std::endl;
}

void levelDisigner::addObject(mapObject* object)//добавить обьект
{
	objectStorage.emplace(object->getId(), object);
}

void levelDisigner::deleteObject(const int16_t key)//удалить обьект
{
	delete objectStorage[key];
	objectStorage.erase(key);
}

void levelDisigner::start()
{
	drawGrid();
	while (window.isOpen())
	{
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
				window.clear();
				changeConfig();//сохраняем текущие параметры окна
				drawGrid();//рисуем решётку
				drawCells();//рисуем закрашенные клетки
				for (auto a : objectStorage) {//обновляем положение объектов
					a.second->updatePosition(centerX, centerY, gridWidth, gridHeight);
				}
				drawObjects();//рисуем оюъекты
				drawArrows();//рисуем направления движения
			}
			if (eve.type == sf::Event::Closed) {//закрытие окна
				window.close();
			}
			if (eve.type == sf::Event::MouseButtonPressed)//клик мыши
			{
				if (eve.mouseButton.button == sf::Mouse::Left)//активируем и деактивируем клетку
				{
					//Рисует по одной клетке
					sf::Vector2f pos = getPositionOnGrid(eve.mouseButton.x, eve.mouseButton.y);
					if (selectCheck(pos)) { //внутри selectCheck данные заносятся в вектор
						sf::RectangleShape cell(sf::Vector2f(gridWidth, gridHeight));
						cell.setPosition(sf::Vector2f(pos.x, pos.y));
						cell.setFillColor(selectColor);

						window.draw(cell);
						window.display();
					}
					else {
						sf::RectangleShape grid(sf::Vector2f(gridWidth, gridHeight));//закрашиваем обратно один квадрат, вместо перерисовки всего окна
						grid.setPosition(sf::Vector2f(pos.x, pos.y));//рисуем решётку, вокруг клетки
						grid.setFillColor(gridColor);

						window.draw(grid);

						sf::RectangleShape cell(sf::Vector2f(gridWidth - 2, gridHeight - 2));
						cell.setPosition(sf::Vector2f(pos.x + 1, pos.y + 1));
						cell.setFillColor(backgroundColor);

						window.draw(cell);//рисуем клетку
						window.display();
					}
				}
			}
			if (eve.type == sf::Event::KeyPressed) {//реакция на нажатия клавиш
				if (eve.key.code == sf::Keyboard::W) {
					if (selectedChecks.size() > 1) {
						sf::Vector2i pos1 = selectedChecks[0];
						sf::Vector2i pos2 = selectedChecks[1];
						float minX, maxX, minY, maxY;
						if (pos1.x > pos2.x) {
							minX = pos2.x;
							maxX = pos1.x;
						}
						else {
							minX = pos1.x;
							maxX = pos2.x;
						}
						if (pos1.y > pos2.y) {
							minY = pos2.y;
							maxY = pos1.y;
						}
						else {
							minY = pos1.y;
							maxY = pos2.y;
						}
						minX = minX;
						maxX = maxX + 1;
						minY = minY;
						maxY = maxY + 1;
						std::vector<sf::Vector2f> points;
						points.push_back(sf::Vector2f(minX - minX, minY - minY));
						points.push_back(sf::Vector2f(maxX - minX, minY - minY));
						points.push_back(sf::Vector2f(maxX - minX, maxY - minY));
						points.push_back(sf::Vector2f(minX - minX, maxY - minY));
						
						wall* local = new wall(points);
						local->setPosition(sf::Vector2f(minX, minY));
						local->updatePosition(centerX, centerY, gridWidth, gridHeight);
						addObject(local);
					}
					selectedChecks.clear();
					drawGrid();
					drawObjects();
				}
				if (eve.key.code == sf::Keyboard::D)
				{
					while (objectStorage.size()) {
						std::map<int16_t, mapObject*>::iterator iterToFirst = objectStorage.begin();
						deleteObject((*iterToFirst).first);
					}
					akaGraph.clear();
					drawGrid();
				}
				if (eve.key.code == sf::Keyboard::H)
				{
					if (selectedChecks.size() > 0) {
						sf::Vector2i pos1 = selectedChecks[0];
						float x = pos1.x, y = pos1.y;
						human* local = new human(sf::Vector2f(x, y), 2);
						local->updatePosition(centerX, centerY, gridWidth, gridHeight);
						addObject(local);
					}
					selectedChecks.clear();
					drawGrid();
					drawObjects();
					drawArrows();
				}
				if (eve.key.code == sf::Keyboard::A)
				{
					if (selectedChecks.size() > 1) {
						sf::Vector2i pos1 = selectedChecks[0];
						sf::Vector2i pos2 = selectedChecks[1];
						akaGraph.push_back(std::pair<sf::Vector2i, sf::Vector2i>(pos1, pos2));
					}
					selectedChecks.clear();
					drawGrid();
					drawObjects();
					drawArrows();
				}
				if (eve.key.code == sf::Keyboard::S)
				{
					saveToFile();
				}
				if (eve.key.code == sf::Keyboard::L)
				{
					loadFromFile();
				}
				if (eve.key.code == sf::Keyboard::Q)
				{
					window.close();
				}
			}
		}
	}
}

levelDisigner::~levelDisigner()
{
	while(objectStorage.size()) {
		std::map<int16_t, mapObject*>::iterator iterToFirst = objectStorage.begin();
		deleteObject((*iterToFirst).first);
	}
}