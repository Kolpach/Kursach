#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
namespace hateYouRectangle {
	class Shapes
	{
	protected:
		sf::ConvexShape drawable;
	public:
		std::vector<sf::Vector2f> points;
		void setFillColor(const sf::Color& col);
		void addPosition(float x, float y);
		sf::ConvexShape getShape();
		virtual std::pair<double, double> pointsProjectToLine(const sf::Vector2f& A, const sf::Vector2f& Normal) const;
	};

	class Rectangle : public Shapes {
	public:
		Rectangle(const std::vector<sf::Vector2f>& a);
	};

	class Circle : public Shapes {
	private:
		double radius;
	public:
		Circle(float radius);
	};
}

/*
hateYouRectangle::Circle one(50.f);
	one.addPosition(318,190);
	window.draw(one.getShape());


	std::vector<sf::Vector2f> masRec;
	masRec.push_back(sf::Vector2f(0.f, 0.f));
	masRec.push_back(sf::Vector2f(150.f, 10.f));
	masRec.push_back(sf::Vector2f(120.f, 90.f));
	masRec.push_back(sf::Vector2f(30.f, 100.f));
	masRec.push_back(sf::Vector2f(0.f, 50.f));

	hateYouRectangle::Rectangle two(masRec);
	two.addPosition(210, 120);
	window.draw(two.getShape());

	std::vector<sf::Vector2f> masRec2;

	masRec2.push_back(sf::Vector2f(0.f, 0.f));
	masRec2.push_back(sf::Vector2f(150.f, 10.f));
	masRec2.push_back(sf::Vector2f(120.f, 90.f));
	masRec2.push_back(sf::Vector2f(30.f, 100.f));
	masRec2.push_back(sf::Vector2f(0.f, 50.f));



	hateYouRectangle::Rectangle three(masRec2);
	three.addPosition(300, 400);
	three.setFillColor(sf::Color(255, 255, 0, 255));
	window.draw(three.getShape());

	window.display();
	std::cout << collision(one, two) << '\n';
*/