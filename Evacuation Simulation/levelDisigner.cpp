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
		for (sf::Vector2f pos : selectedChecks) {
			cell.setPosition(sf::Vector2f(pos.x * gridWidth + centerX, pos.y * gridHeight + centerY));
			cell.setFillColor(selectColor);
			window.draw(cell);
			std::cout << "draw: " << pos.x << ' ' << pos.y << std::endl;
		}
		window.display();
	}
}

inline std::vector<sf::Vector2f>::const_iterator levelDisigner::findInVector(const std::vector<sf::Vector2f>& vec, const sf::Vector2f value) {//поиск в векторе
	for (std::vector<sf::Vector2f>::const_iterator i = vec.begin(); i != vec.end(); ++i) {
		if (*i == value)
			return i;
	}
	return vec.end();
}

inline sf::Vector2f levelDisigner::getPositionOnGrid(double x, double y) {//устанавливает позицию, полученную от клика, в левый верхний левый угол
	double deltaX = (x - centerX) / abs(x - centerX);
	double posX = centerX;
	while (deltaX * (posX - centerX) < deltaX * (x - centerX)) {
		posX += gridWidth * deltaX;
	}
	posX -= deltaX > 0 ? gridWidth : 0;

	double deltaY = (y - centerY) / abs(y - centerY);
	double posY = centerY;
	while (deltaY * (posY - centerY) < deltaY * (y - centerY)) {
		posY += gridHeight * deltaY;
	}
	posY -= deltaY > 0 ? gridHeight : 0;
	return sf::Vector2f(posX, posY);
}

inline bool levelDisigner::selectCheck(const sf::Vector2f& position) {// Функция меняет значение клетки и возвращает результат
	sf::Vector2f pos = sf::Vector2f((position.x - centerX) / gridWidth, (position.y - centerY) / gridHeight);//Внутри вектора хранятся не реальные координаты, а кординаты относительно центра
	std::vector<sf::Vector2f>::const_iterator a = findInVector(selectedChecks, pos);//поиск в векторе
	if (a != selectedChecks.end()) {
		selectedChecks.erase(a);
		return false;
	}
	else {
		selectedChecks.push_back(pos);
		return true;
	}
}

void levelDisigner::start()
{
	drawGrid();
	while (window.isOpen())
	{
		sf::Event eve;
		while (window.pollEvent(eve))
		{
			if (eve.type == sf::Event::Resized) {
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
			if (eve.type == sf::Event::Closed)
				window.close();
			if (eve.type == sf::Event::MouseButtonPressed)
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
						sf::RectangleShape grid(sf::Vector2f(gridWidth, gridHeight));
						grid.setPosition(sf::Vector2f(pos.x, pos.y));
						grid.setFillColor(gridColor);

						window.draw(grid);

						sf::RectangleShape cell(sf::Vector2f(gridWidth - 2, gridHeight - 2));//Это не "чё за тупой код?" это оптимизация
						cell.setPosition(sf::Vector2f(pos.x + 1, pos.y + 1));
						cell.setFillColor(backgroundColor);

						window.draw(cell);
						window.display();
					}

					for (auto a : selectedChecks) {
						std::cout << a.x << ' ' << a.y << std::endl;
					}
					std::cout << std::endl;
				}
			}
		}
	}
}

levelDisigner::~levelDisigner()
{
	/*std::ofstream file;
	file.open("123", std::ios::binary);
	file.write(selectedChecks,)*/
}