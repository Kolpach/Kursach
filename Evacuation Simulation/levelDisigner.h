#pragma once
#include "mapObject.h"
class levelDisigner
{
private:
	int32_t windowWidth;
	int32_t windowHeight;
	int32_t centerX;
	int32_t centerY;
	int16_t gridWidth;
	int16_t gridHeight;
	int16_t max_count_of_cells;

	std::vector<sf::Vector2i> selectedChecks;//������ ������� �������� �� �������� ����������, � ��������� ������������ ������


	sf::ContextSettings settings;

	sf::RenderWindow window;

	sf::Color gridColor;
	sf::Color backgroundColor;
	sf::Color selectColor;
	sf::Color arrowColor;
	
	std::map <int16_t, mapObject*> objectStorage;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> akaGraph;

	void changeConfig();

	void drawGrid();

	void drawCells();

	void drawObjects();

	void drawArrows();

	template<class iterType, class vecValue> iterType findInVector(iterType begin, iterType end, const vecValue value);

	sf::Vector2f getPositionOnGrid(double x, double y);

	bool selectCheck(const sf::Vector2f& position);

	void saveToFile();
	
	void loadFromFile();

	void addObject(mapObject* object);

	void deleteObject(const int16_t key);


public:
	levelDisigner() : settings(0, 0, 8), window(sf::VideoMode(600, 400), "Plan disigner!", sf::Style::Default, settings) {//����� ������-������� � ��� ���������
		window.setFramerateLimit(60);
		window.setSize(sf::Vector2u(600, 400));
		changeConfig();
		backgroundColor = sf::Color((int)255, (int)222, (int)111, (int)255);
		gridColor = sf::Color((int)34, (int)0, (int)0, (int)180);
		selectColor = sf::Color(204, 255, 204, 255);
		arrowColor = sf::Color(0, 150, 0, 255);
	}
	void start();
	~levelDisigner();
};

