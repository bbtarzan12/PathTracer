#include "MirrorMaterial.h"
#include "IntersectInfo.h"
#include "Math.h"
#include "SpecularBRDF.h"

MirrorMaterial::MirrorMaterial(const glm::vec3& R)
	: mirror(std::make_unique<SpecularBRDF>(R))
{
	
}

glm::vec3 MirrorMaterial::CalculateF(const IntersectInfo& info, const glm::vec3& worldWo, const glm::vec3& worldWi) const
{
	return glm::vec3(0);
}

glm::vec3 MirrorMaterial::SampleF(const IntersectInfo& info, const glm::vec3& worldWo, glm::vec3& worldWi, float& pdf) const
{
	const glm::vec3 wo = PathTracing::WorldToLocal(worldWo, info.normal, info.tangent, info.biTangent);
	glm::vec3 wi(0);
	const glm::vec3 f = mirror->SampleF(wo, wi, pdf);
	worldWi = PathTracing::LocalToWorld(wi, info.normal, info.tangent, info.biTangent);
	return f;
}

glm::vec3 MirrorMaterial::GetDefaultColorForDrawOpenGL() const
{
	return mirror->GetR();
}
