#include "Shape.h"

#include <glm/gtc/constants.inl>

#include "Ray.h"

Shape::~Shape()
{
	ClearOpenGL();
}

float Shape::GetPDF() const
{
	return 1.0f / GetArea();
}

float Shape::GetPDF(const glm::vec3& point, const glm::vec3& wi) const
{
	const Ray ray(point, wi, 0.0f);

	float tHit;
	glm::vec3 normal;
	if(!Intersect(ray, tHit, normal, 0.001f))
	{
		return 0.0f;
	}

	const glm::vec3 hitPoint = ray.origin + ray.direction * tHit;
	const float distance = glm::distance(point, hitPoint);
	const float cosTheta = glm::abs(glm::dot(normal, -wi));

	return (distance * distance) / (cosTheta * GetArea());
}

float Shape::UniformConePDF(const float cosThetaMax) const
{
	return 1.0f / (glm::two_pi<float>() * (1.0f - cosThetaMax));
}
