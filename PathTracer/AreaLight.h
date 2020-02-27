#pragma once
#include <memory>

#include "Light.h"

class Shape;

class AreaLight : public Light
{
public:
	AreaLight(std::shared_ptr<Shape> shape);

	virtual glm::vec3 Sample(const glm::vec3& point, glm::vec3& lightDir, float& pdf, float& distance) const override;
	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const override;

	const std::shared_ptr<Shape>& GetShape() const;
	
private:
	std::shared_ptr<Shape> shape;
};
