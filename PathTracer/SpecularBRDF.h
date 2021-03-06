﻿#pragma once
#include "Bxdf.h"

class Fresnel;

class SpecularBRDF : public Bxdf
{
public:
	SpecularBRDF(const glm::vec3& R, std::unique_ptr<Fresnel> fresnel);

	virtual glm::vec3 CalculateF(const glm::vec3& wo, const glm::vec3& wi) const override;
	virtual glm::vec3 SampleF(const glm::vec3& wo, glm::vec3& wi, float& pdf) const override;

	const glm::vec3& GetR() const;
	
private:
	glm::vec3 R;
	std::unique_ptr<Fresnel> fresnel;
	
};
