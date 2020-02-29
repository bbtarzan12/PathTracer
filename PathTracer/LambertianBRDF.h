#pragma once
#include "Bxdf.h"

class LambertianBrdf : public Bxdf
{
public:
	LambertianBrdf(const glm::vec3& R);
	
	virtual glm::vec3 CalculateF(const glm::vec3& wo, const glm::vec3& wi) const override;
	
	const glm::vec3& GetR() const;
	
private:
	glm::vec3 R;
};
