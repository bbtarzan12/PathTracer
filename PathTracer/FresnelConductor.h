#pragma once
#include "Fresnel.h"

class FresnelConductor : public Fresnel
{
public:

	//tex:
	//굴절률 $\eta$ 흡수 계수 $k$
	FresnelConductor(const glm::vec3& eta, const glm::vec3& k);

	
	virtual glm::vec3 Calculate(float cosi) const override;

private:
	glm::vec3 eta, k;
};
