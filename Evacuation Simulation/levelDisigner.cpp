#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
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
			std::cout << "draw: " << pos.x << ' ' << pos.y << std::endl;
		}
		window.display();
	}
}

void levelDisigner::drawObjects()//рисует все обьекты в objectStorage
{
	for (auto a : objectStorage) {
		sf::ConvexShape local = a.second->getColliderAndSpritre()->getShape();
		for (size_t i = 0; i < local.getPointCount(); ++i) { // в objectStorage хранятся относительные координаты, поэтому преобразовываем их в реальные
			sf::Vector2f point = local.getPoint(i);
			local.setPoint(i, sf::Vector2f(point.x * gridWidth + centerX, point.y * gridHeight + centerY));
		}
		window.draw(local);
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
				changeConfig();
				drawGrid();
				drawCells();
			}
			if (eve.type == sf::Event::Closed)//закрытие окна
				window.close();
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

					for (auto a : selectedChecks) {
						std::cout << a.x << ' ' << a.y << std::endl;
					}
					std::cout << std::endl;
				}
			}
			if (eve.type == sf::Event::KeyPressed) {//реакция на нажатия клавиш
				if (eve.key.code == sf::Keyboard::W) {
					wall* local = nullptr;
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
						points.push_back(sf::Vector2f(minX, minY));
						points.push_back(sf::Vector2f(maxX, minY));
						points.push_back(sf::Vector2f(maxX, maxY));
						points.push_back(sf::Vector2f(minX, maxY));
						
						local = new wall(points);
						addObject(local);
					}
					selectedChecks.clear();
					drawGrid();
					drawObjects();
				}
				if (eve.key.code == sf::Keyboard::D)
				{
					drawObjects();
				}
			}
		}
	}
}

levelDisigner::~levelDisigner()
{
	for (auto a : objectStorage) {
		deleteObject(a.first);
	}
}