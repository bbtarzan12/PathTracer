#pragma once
#include "Fresnel.h"

class FresnelDielectric : public Fresnel
{
public:

	//tex:
	//굴절률 $\eta_i$, $\eta_t$
	FresnelDielectric(const float etai, const float etat);
	
	glm::vec3 Calculate(float cosi) const override;

private:
	float etai, etat;
};
