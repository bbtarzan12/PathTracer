#include "PointLight.h"

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, const float& intensity)
	: position(position), color(color), intensity(intensity)
{
}

glm::vec3 PointLight::Sample(const glm::vec3& point, glm::vec3& lightDir, float& pdf, float& distance) const
{
	lightDir = glm::normalize(position - point);
	pdf = 1;
	distance = glm::distance(point, position);
	
	const float distanceSquared = distance * distance;
	
	return color * (intensity / distanceSquared);
}

bool PointLight::Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const
{
	return false;
}
