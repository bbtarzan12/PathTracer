#pragma once
#include <memory>
#include <glm/glm.hpp>

class Shape;
class Material;

struct IntersectInfo
{
	std::weak_ptr<Shape> shape;
	std::weak_ptr<Material> material;
	float t = 0;
	glm::vec3 normal, tangent, biTangent;
	glm::vec3 emit;
};