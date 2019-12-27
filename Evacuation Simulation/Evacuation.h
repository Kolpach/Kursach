#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <ctime>
#include <random>
#include "mapObject.h"
class Evacuation
{
	struct projections;
private:
	int32_t windowWidth;
	int32_t windowHeight;
	int32_t centerX;
	int32_t centerY;
	int16_t gridWidth;
	int16_t gridHeight;
	int16_t max_count_of_cells;
	sf::ContextSettings settings;

	sf::RenderWindow window;

	sf::Color gridColor;
	sf::Color backgroundColor;
	sf::Color arrowColor;

	bool alarm = false;
	float humanSpeed = 30.f;

	std::map <int16_t, mapObject*> objectStorage;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> akaGraph;

	void changeConfig();

	void drawObjects();
	void drawBackground();
	void drawArrows();

	void loadFromFile(std::string name);

	bool intersect(const Evacuation::projections& a, const Evacuation::projections& b);
	bool narrowCollision(const mapObject& a, const mapObject& b);

	void addObject(mapObject* object);
	void deleteObject(const int16_t key);
	Evacuation::projections projectToAxis(mapObject* object);
	void checkCollision(float path);
	void Alarm();
	bool canBeSeen(const sf::Vector2f& one, const sf::Vector2f& two);
	std::vector<projections> constObjects;
	std::vector<projections> movableObjects;
	void setDirection();
	void eachFrame(sf::Int32);
	sf::ConvexShape border;
public:
	struct projections
	{
		std::pair<float, float> X, Y;
		mapObject* owner;
	};
	Evacuation(std::string name) : settings(0, 0, 8), window(sf::VideoMode(600, 400), "Simulation", sf::Style::Default, settings) {//Задаём окошко-лукошко и его параметры
		loadFromFile(name);
		window.setFramerateLimit(60);
		window.setSize(sf::Vector2u(600, 400));
		changeConfig();
		backgroundColor = sf::Color((int)255, (int)222, (int)111, (int)255);
		gridColor = sf::Color((int)34, (int)0, (int)0, (int)180);
		arrowColor = sf::Color(0, 150, 0, 255);
		sf::Vector2u loc = window.getSize();
		uint32_t Wide = loc.x;
		uint32_t Height = loc.y;
		border.setPointCount(10);
		border.setPoint(0, sf::Vector2f(0.f, 0.f));
		border.setPoint(1, sf::Vector2f(Wide, 0.f));
		border.setPoint(2, sf::Vector2f(Wide, Height));
		border.setPoint(3, sf::Vector2f(0.f, Height));
		border.setPoint(4, sf::Vector2f(0.f, gridHeight));
		border.setPoint(5, sf::Vector2f(gridWidth, gridHeight));
		border.setPoint(6, sf::Vector2f(gridWidth, Height - gridHeight));
		border.setPoint(7, sf::Vector2f(Wide - gridWidth, Height - gridHeight));
		border.setPoint(8, sf::Vector2f(Wide - gridWidth, gridHeight));
		border.setPoint(9, sf::Vector2f(0.f, gridHeight));
	}
	void start();
	~Evacuation();
};
