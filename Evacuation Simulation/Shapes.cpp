#include <cmath>
#include "Shapes.h"
namespace hateYouRectangle {
	Rectangle::Rectangle(const std::vector<sf::Vector2f>& a) 
	{
		points = a;
		size_t countOfPoints = points.size();
		drawable.setPointCount(countOfPoints);
		drawable.setPosition(0.f, 0.f);
		for (size_t i = 0; i < countOfPoints; ++i) {
			drawable.setPoint(i, points[i]);
		}
	}
	void Shapes::setFillColor(const sf::Color& col)
	{
		drawable.setFillColor(col);
	}
	void Shapes::addPosition(float x, float y)
	{
		size_t countOfPoints = points.size();
		drawable.setPosition(0.f, 0.f);
		for (size_t i = 0; i < countOfPoints; ++i) {
			points[i].x += x;
			points[i].y += y;
			drawable.setPoint(i, points[i]);
		}
	}
	sf::ConvexShape Shapes::getShape()
	{
		return drawable;
	}
	//���������� ���������� - �������� ������ �� �����, ��� �������� �� ����������� ��� ���������� ������ ���� ���������� � ����� �������� x ��� y
	std::pair<double, double> Shapes::pointsProjectToLine(const sf::Vector2f& Line, const sf::Vector2f& Normal) const
	{
		double max = 0, min = 0;
		if (Line.x != 0) {
			for (sf::Vector2f locPoint : points) {//����� ������
				double locX = (locPoint.x - locPoint.y * Normal.x / Normal.y) / (1 - Line.y * Normal.x / (Line.x * Normal.y));
			

				if (min == 0.0 || locX < min) {
					min = locX;
				}
				if (max == 0.0 || locX > max) {
					max = locX;
				}
			}
		}
		else {
			for (sf::Vector2f locPoint : points) {//������� ������ ��� ������ ���������������� ��� X
				double locY = (locPoint.y - locPoint.x * Normal.y / Normal.x) / (1 - Line.x * Normal.y / (Line.y * Normal.x));
				

				if (min == 0 || locY < min)
					min = locY;
				if (max == 0 || locY > max)
					max = locY;
			}
		}
		return std::pair<double, double>(min, max);
	}

	Circle::Circle( float r) : radius(r)
	{
		sf::CircleShape localCircle(r, 12);
		size_t countOfPoints = localCircle.getPointCount();

		drawable.setPointCount(countOfPoints);
		drawable.setPosition(0.f, 0.f);
		for (size_t i = 0; i < countOfPoints; ++i) {
			points.push_back(localCircle.getPoint(i));
			drawable.setPoint(i, points[i]);
		}

	}
	
}

