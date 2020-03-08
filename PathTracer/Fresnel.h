#pragma once
#include <glm/glm.hpp>

class Fresnel
{
public:
	virtual ~Fresnel() = default;
	virtual glm::vec3 Calculate(const float cosi) const = 0;
};
