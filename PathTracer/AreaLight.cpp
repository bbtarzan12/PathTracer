#include "AreaLight.h"
#include <utility>

#include "LightVisitor.h"
#include "Shape.h"

AreaLight::AreaLight(std::shared_ptr<Shape> shape)
	: shape(std::move(shape))
{
}

void AreaLight::Accept(LightVisitor& visitor)
{
	visitor.Visit(*this);
}

glm::vec3 AreaLight::Sample(const glm::vec3& point, glm::vec3& lightDir, float& pdf, float& distance) const
{
	const auto[randomPoint, randomNormal] = shape->GetRandomPointOnSurface();
	
	lightDir = glm::normalize(randomPoint - point);
	pdf = shape->GetPDF(point, lightDir);
	distance = glm::distance(point, randomPoint);

	return shape->color * shape->emit;
}

bool AreaLight::Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const
{
	return shape->Intersect(ray, tHit, normal, rayEpsilon);
}

const std::shared_ptr<Shape>& AreaLight::GetShape() const
{
	return shape;
}
