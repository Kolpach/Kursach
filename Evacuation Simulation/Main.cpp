#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <map>
#include <cmath>
struct windowConfig {
	int32_t width;
	int32_t height;
	int32_t centerX;
	int32_t centerY;
	int16_t gridWidth;
	int16_t gridHeight;

	sf::Color gridColor;
	sf::Color backgroundColor;
	sf::Color selectColor;
	windowConfig(const sf::RenderWindow& window) {
		changeConfig(window);
		backgroundColor = sf::Color((int)255, (int)222, (int)111, (int)255);
		gridColor = sf::Color((int)34, (int)0, (int)0, (int)180);
		selectColor = sf::Color(204, 255, 204, 255);
	}
	void changeConfig(const sf::RenderWindow& window) {
		sf::Vector2u sizeVector = window.getSize();
		width = sizeVector.x;
		height = sizeVector.y;
		centerX = width / 2;
		centerY = height / 2;
		gridWidth = width / 60;
		gridHeight = gridWidth;
	}
};


void drawGrid(sf::RenderWindow& window, const windowConfig& config) {
	
		
	sf::RectangleShape lineX(sf::Vector2f(config.width, 2.f));
	sf::RectangleShape lineY(sf::Vector2f(2.f, config.height));
	sf::RectangleShape background(sf::Vector2f(config.width, config.height));
	
		
	background.setFillColor(config.backgroundColor);
	window.draw(background);

	lineY.setFillColor(config.gridColor);
	lineY.setPosition(config.centerX - 1, 0);
	window.draw(lineY);
	for (int i = 1; config.centerX - 1 + config.gridWidth * i < config.width ; ++i) {
		lineY.setPosition(config.centerX - 1 + config.gridWidth * i, 0);
		window.draw(lineY);
		lineY.setPosition(config.centerX - 1 - config.gridWidth * i, 0);
		window.draw(lineY);
	}

	lineX.setFillColor(config.gridColor);
	lineX.setPosition(0, config.centerY - 1);
	window.draw(lineX);
	for (int i = 1; config.centerY - 1 + config.gridHeight * i < config.height; ++i) {
		lineX.setPosition(0, config.centerY - 1 - config.gridHeight * i);
		window.draw(lineX);
		lineX.setPosition(0, config.centerY - 1 + config.gridHeight * i);
		window.draw(lineX);
	}
	window.display();
}

std::vector<sf::Vector2f>::const_iterator findInVector(const std::vector<sf::Vector2f>& vec, const sf::Vector2f value) {
	for (std::vector<sf::Vector2f>::const_iterator i = vec.begin(); i != vec.end(); ++i) {
		if (*i == value)
			return i;
	}
	return vec.end();
}

sf::Vector2f getPositionOnGrid(double x, double y, const windowConfig& config) {
	double deltaX = (x - config.centerX) / abs(x - config.centerX);
	double posX = config.centerX;
	while (deltaX * (posX - config.centerX) < deltaX * (x - config.centerX)) {
		posX += config.gridWidth * deltaX;
	}
	posX -= deltaX > 0 ? config.gridWidth : 0;

	double deltaY = (y - config.centerY) / abs(y - config.centerY);
	double posY = config.centerY;
	while (deltaY * (posY - config.centerY) < deltaY * (y - config.centerY)) {
		posY += config.gridHeight * deltaY;
	}
	posY -= deltaY > 0 ? config.gridHeight : 0;
	return sf::Vector2f(posX, posY);
}

bool selectCheck(sf::Vector2f position, std::vector<sf::Vector2f>& vec) {
	std::vector<sf::Vector2f>::const_iterator a = findInVector(vec, position);
	if (a != vec.end()) {
		vec.erase(a);
		return false;
	}
	else {
		vec.push_back(position);
		return true;
	}
}

int main() {

	std::vector<sf::Vector2f> selectedChecks;


	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(600, 400), "Plan disigner!", sf::Style::Default, settings);

	window.setFramerateLimit(60);
	window.setSize(sf::Vector2u(600, 400));

	windowConfig config(window);
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);


	window.clear();
		
	drawGrid(window, config);
	//window.display();
	while (window.isOpen())
	{
		sf::Event eve;
		while (window.pollEvent(eve))
		{
			if (eve.type == sf::Event::Resized) {
				uint32_t dy = eve.size.height;
				uint32_t dx = eve.size.width;
				bool needToBeChanged = false;
				if (dy < 400) {
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
				config.changeConfig(window);
				drawGrid(window, config);
			}
			if (eve.type == sf::Event::Closed)
				window.close();
			if (eve.type == sf::Event::MouseButtonPressed)
			{
				if (eve.mouseButton.button == sf::Mouse::Left)
				{

					sf::Vector2f pos = getPositionOnGrid(eve.mouseButton.x, eve.mouseButton.y, config);
					if (selectCheck(pos, selectedChecks)) {

						sf::RectangleShape cell(sf::Vector2f(pos.x + config.gridWidth, pos.y - config.gridHeight)) ;
						cell.setPosition(pos);
						cell.setFillColor(config.selectColor);
						
						window.draw(cell);
						window.display();
					}
					else
					{
						window.clear();
						drawGrid(window, config);
					}
					
					
				}
			}
		}
	}
	return 0;
}