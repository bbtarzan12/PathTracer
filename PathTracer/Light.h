#pragma once
#include <glm/glm.hpp>

class Ray;

class Light
{
public:
	virtual ~Light() = default;
	virtual glm::vec3 Sample(const glm::vec3& point, glm::vec3& lightDir, float& pdf, float& distance) const = 0;
	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const = 0;
};
