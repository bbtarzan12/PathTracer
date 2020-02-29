#pragma once
#include <memory>

#include "Light.h"
class Shape;

class AreaLight : public Light
{
public:
	AreaLight(std::shared_ptr<Shape> shape, const glm::vec3& color, const float intensity);

	virtual void Accept(LightVisitor& visitor) override;
	virtual glm::vec3 Sample(const glm::vec3& point, const glm::vec3& worldWo, const IntersectInfo& info, const std::shared_ptr<Material>& material, float& distance, glm::vec3& worldWi) const override;
	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const override;
	virtual glm::vec3 Emit() const override;
	
	const std::shared_ptr<Shape>& GetShape() const;
	
private:
	glm::vec3 color;
	float intensity;
	std::shared_ptr<Shape> shape;
};
