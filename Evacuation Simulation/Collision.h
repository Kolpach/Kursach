#pragma once
#include "Shapes.h"
#include "mapObject.h"

class BroadPhase {
	struct projections
	{
		std::pair<float, float> X, Y;
		mapObject* owner;
	};
	bool intersect(projections a, projections b);
	bool narrowCollision(const hateYouRectangle::Shapes& a, const hateYouRectangle::Shapes& b);
public:
	std::vector<projections> constObjects;
	std::vector<projections> movableObjects;
	std::pair<int16_t, int16_t> checkCollision();
};