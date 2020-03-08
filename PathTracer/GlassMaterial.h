#pragma once
#include "Material.h"

class GlassMaterial : public Material
{
public:
	GlassMaterial(const glm::vec3& R, const glm::vec3& T, const float ior);
	
};
