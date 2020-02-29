#pragma once

#include <glm/glm.hpp>
#include "Common.h"

class Ray
{
public:
	Ray();
	Ray(const glm::vec3& origin, const glm::vec3& direction, float start, float end = PathTracing::INFINITE<float>, int depth = 0);
	Ray(const glm::vec3& origin, const glm::vec3& direction, const Ray& parent, float start, float end = PathTracing::INFINITE<float>);

public:
	glm::vec3 origin;
	glm::vec3 direction;
	int depth;
	mutable float mint, maxt;
};