#pragma once
#include "Fresnel.h"

class FresnelNoOp : public Fresnel
{
public:
	glm::vec3 Calculate(const float cosi) const override;
};
