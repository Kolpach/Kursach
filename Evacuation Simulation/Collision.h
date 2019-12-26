#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "mapObject.h"
#include "Evacuation.h"

class BroadPhase {
	
	bool intersect(const Evacuation::projections& a, const Evacuation::projections& b);
	bool narrowCollision(const mapObject& a, const mapObject& b);
	std::pair<mapObject*, mapObject*> tryThisVariant(std::vector<Evacuation::projections>& constObjects, std::vector<Evacuation::projections>& movableObjects, bool alarm);
public:
	std::pair<mapObject*, mapObject*> checkCollision(std::vector<Evacuation::projections>& constObjects, std::vector<Evacuation::projections>& movableObjects, bool alarm, float , int32_t centerX, int32_t centerY, int16_t gridWidth, int16_t gridHeight);//должно быть по значению
};