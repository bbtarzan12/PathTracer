#include "AreaLight.h"
#include "LightVisitor.h"
#include "Shape.h"
#include "Material.h"
#include "IntersectInfo.h"

#include <utility>

AreaLight::AreaLight(std::unique_ptr<Shape> shape, const glm::vec3& color, const float intensity)
	: color(color), intensity(intensity), shape(std::move(shape))
{
}

void AreaLight::Accept(LightVisitor& visitor)
{
	visitor.Visit(*this);
}

glm::vec3 AreaLight::Sample(const glm::vec3& point, const glm::vec3& worldWo, const IntersectInfo& info, const Material* material, float& distance, glm::vec3& worldWi) const
{
	const auto[randomPoint, randomNormal] = shape->GetRandomPointOnSurface();
	
	worldWi = glm::normalize(randomPoint - point);
	distance = glm::distance(point, randomPoint);
	
	const float pdf = shape->GetPDF(point, worldWi);
	const float area = shape->GetArea();
	const float distance2 = distance * distance;
	const glm::vec3 f = material->CalculateF(info, worldWo, worldWi);
	const glm::vec3 radiance = color * intensity;
	const float cosTheta = glm::max(0.0f, glm::dot(info.normal, worldWi));

	return f * radiance * cosTheta / pdf * (area / distance2);
}

bool AreaLight::Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const
{
	return shape->Intersect(ray, tHit, normal, rayEpsilon);
}

glm::vec3 AreaLight::Emit() const
{
	return color * intensity;
}

const Shape* AreaLight::GetShape() const
{
	return shape.get();
}
