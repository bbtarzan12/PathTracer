#include "LambertianBRDF.h"

#include <glm/gtc/constants.inl>

LambertianBrdf::LambertianBrdf(const glm::vec3& R)
	:Bxdf(BxdfType::REFLECTION), R(R)
{
	
}

glm::vec3 LambertianBrdf::CalculateF(const glm::vec3& wo, const glm::vec3& wi) const
{
	return R * glm::one_over_pi<float>();
}

const glm::vec3& LambertianBrdf::GetR() const
{
	return R;
}
