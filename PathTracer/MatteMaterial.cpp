#include "MatteMaterial.h"
#include "IntersectInfo.h"
#include "LambertianBRDF.h"
#include "Math.h"

MatteMaterial::MatteMaterial(const glm::vec3& R)
	:lambertian(std::make_shared<LambertianBrdf>(R))
{
}

glm::vec3 MatteMaterial::CalculateF(const IntersectInfo& info, const glm::vec3& worldWo, const glm::vec3& worldWi) const
{
	const glm::vec3 wo = PathTracing::WorldToTangent(worldWo, info.normal, info.tangent, info.biTangent);
	const glm::vec3 wi = PathTracing::WorldToTangent(worldWi, info.normal, info.tangent, info.biTangent);	
	return lambertian->CalculateF(wo, wi);
}

glm::vec3 MatteMaterial::SampleF(const IntersectInfo& info, const glm::vec3& worldWo, glm::vec3& worldWi, float& pdf) const
{
	const glm::vec3 wo = PathTracing::WorldToTangent(worldWo, info.normal, info.tangent, info.biTangent);
	glm::vec3 wi(0);
	const glm::vec3 f = lambertian->SampleF(wo, wi, pdf);
	worldWi = PathTracing::TangentToWorld(wi, info.normal, info.tangent, info.biTangent);
	return f;
}

glm::vec3 MatteMaterial::GetDefaultColorForDrawOpenGL() const
{
	return lambertian->GetR();
}
