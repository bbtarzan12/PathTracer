#pragma once
#include <glm/common.hpp>
#include <glm/common.hpp>
#include <glm/common.hpp>
#include <glm/common.hpp>
#include <glm/common.hpp>
#include <glm/common.hpp>

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(const glm::vec3& position, const glm::vec3& color, const float& intensity);

	virtual void Accept(LightVisitor& visitor) override;
	virtual glm::vec3 Sample(const glm::vec3& point, glm::vec3& lightDir, float& pdf, float& distance) const override;
	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const override;

	
private:
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
};
