#pragma once
#include <vector>
#include <iostream>
using std::vector;
using std::cout;
using std::cin;
using std::endl;

class Grath
{
private:
	class Point
	{
		int32_t x, y;
	};
	vector<vector<int32_t>> Matrix;
	vector<Point> Points;
	int32_t findPoint(int32_t x, int32_t y);
public:
	void makeLink(Point G1, Point G2);
	void destroyLink(Point G1, Point G2);
	void addPoint(int32_t x, int32_t y);
	void removePoint(int32_t x, int32_t y);
};

