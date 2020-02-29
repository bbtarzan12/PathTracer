#pragma once
#include <glm/glm.hpp>

class Shape;
class Material;

struct IntersectInfo
{
	const Shape* shape;
	const Material* material;
	float t = 0;
	glm::vec3 normal, tangent, biTangent;
	glm::vec3 emit;
};