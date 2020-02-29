#include "PointLight.h"

#include "LightVisitor.h"

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, const float& intensity)
	: position(position), color(color), intensity(intensity)
{
}

void PointLight::Accept(LightVisitor& visitor)
{
	visitor.Visit(*this);
}

glm::vec3 PointLight::Sample(const glm::vec3& point, const glm::vec3& worldWo, const IntersectInfo& info, const Material* material, float& distance, glm::vec3& worldWi) const
{
	//lightDir = glm::normalize(position - point);
	//pdf = 1;
	//distance = glm::distance(point, position);
	//
	//const float distanceSquared = distance * distance;
	//
	//return color * (intensity / distanceSquared);

	// Todo 다시 구현하기
	
	return glm::vec3(0);
}

bool PointLight::Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const
{
	return false;
}
