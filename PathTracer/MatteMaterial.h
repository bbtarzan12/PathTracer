#pragma once
#include "Material.h"

class LambertianBrdf;

class MatteMaterial : public Material
{
public:
	MatteMaterial(const glm::vec3& R);

};
