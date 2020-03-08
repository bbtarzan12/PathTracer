#include "SpecularBTDF.h"
#include <glm/glm.hpp>

#include "FresnelDielectric.h"

SpecularBTDF::SpecularBTDF(const glm::vec3& T, const float etai, const float etat)
	:Bxdf(BxdfType::TRANSMISSION), T(T), etai(etai), etat(etat), fresnel(std::make_unique<FresnelDielectric>(etai, etat))
{
}

glm::vec3 SpecularBTDF::CalculateF(const glm::vec3& wo, const glm::vec3& wi) const
{
	return glm::vec3(0);
}

glm::vec3 SpecularBTDF::SampleF(const glm::vec3& wo, glm::vec3& wi, float& pdf) const
{
	const bool bEntering = CosTheta(wo) > 0.0f;
	float ei = etai;
	float et = etat;

	if (!bEntering)
		std::swap(ei, et);

	const float eta = ei / et;
	const float eta2 = eta * eta;
	
	const float sini = SinTheta(wo);
	const float sini2 = sini * sini;
	const float sint2 = eta2 * sini2;

	if (sint2 >= 1.0f)
		return glm::vec3(0);

	float cost = glm::sqrt(glm::max(0.0f, 1.0f - sint2));

	if (bEntering)
		cost = -cost;

	const float sintOverSini = eta; // 이걸 밑에서 나누는 이유는 wi가 원래 식이 무엇인지 보면 알 수 있다.
	wi = glm::vec3(-wo.x * sintOverSini, -wo.y * sintOverSini, cost);
	pdf = 1.0f;
	const glm::vec3 f = fresnel->Calculate(CosTheta(wo));
	return (glm::vec3(1.0f) - f) * T / glm::abs(CosTheta(wi));
}

const glm::vec3& SpecularBTDF::GetT() const
{
	return T;
}
