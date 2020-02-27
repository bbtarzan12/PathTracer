#pragma once
#include <memory>
#include <glm/glm.hpp>

class Shape;

struct IntersectInfo
{
	std::weak_ptr<Shape> shape;
	float t = 0;
	glm::vec3 normal;
};