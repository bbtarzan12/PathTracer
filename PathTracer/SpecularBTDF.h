#pragma once
#include "Bxdf.h"

class FresnelDielectric;

class SpecularBTDF : public Bxdf
{
public:
	SpecularBTDF(const glm::vec3& T, const float etai, const float etat);

	virtual glm::vec3 CalculateF(const glm::vec3& wo, const glm::vec3& wi) const override;
	virtual glm::vec3 SampleF(const glm::vec3& wo, glm::vec3& wi, float& pdf) const override;

	const glm::vec3& GetT() const;


private:
	glm::vec3 T;
	float etai, etat;
	std::unique_ptr<FresnelDielectric> fresnel;
	
};
