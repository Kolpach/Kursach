#pragma once
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

	std::vector<sf::Vector2f> selectedChecks;//Внутри вектора хранятся не реальные координаты, а кординаты относительно центра


	sf::ContextSettings settings;

	sf::RenderWindow window;

	sf::Color gridColor;
	sf::Color backgroundColor;
	sf::Color selectColor;
	
	void changeConfig();

	void drawGrid();

	void drawCells();

	std::vector<sf::Vector2f>::const_iterator findInVector(const std::vector<sf::Vector2f>& vec, const sf::Vector2f value);

	sf::Vector2f getPositionOnGrid(double x, double y);

	bool selectCheck(const sf::Vector2f& position);

	void saveToFile();
	
	void loadToFile();

	void buildWall();

	void deleteWall();


public:
	levelDisigner() : settings(0, 0, 8), window(sf::VideoMode(600, 400), "Plan disigner!", sf::Style::Default, settings) {//Задаём окошко-лукошко и его параметры
		window.setFramerateLimit(60);
		window.setSize(sf::Vector2u(600, 400));
		changeConfig();
		backgroundColor = sf::Color((int)255, (int)222, (int)111, (int)255);
		gridColor = sf::Color((int)34, (int)0, (int)0, (int)180);
		selectColor = sf::Color(204, 255, 204, 255);
	}
	void start();
	~levelDisigner();
};