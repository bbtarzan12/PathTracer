#include "SpecularBRDF.h"
#include "Fresnel.h"

SpecularBRDF::SpecularBRDF(const glm::vec3& R, std::unique_ptr<Fresnel> fresnel)
	:Bxdf(BxdfType::REFLECTION), R(R), fresnel(std::move(fresnel))
{
	
}

glm::vec3 SpecularBRDF::CalculateF(const glm::vec3& wo, const glm::vec3& wi) const
{
	return glm::vec3(0);
}

glm::vec3 SpecularBRDF::SampleF(const glm::vec3& wo, glm::vec3& wi, float& pdf) const
{
	wi = glm::vec3(-wo.x, -wo.y, wo.z); // 어짜피 Local Space에서 계산이 될태니 (0, 0, 1)에 대해 wo를 pi만큼 회전시킨다고 생각

	pdf = 1.0f;

	return fresnel->Calculate(CosTheta(wo)) * R / glm::abs(CosTheta(wi));
}

const glm::vec3& SpecularBRDF::GetR() const
{
	return R;
}
