#include "GlassMaterial.h"

#include "FresnelDielectric.h"
#include "SpecularBRDF.h"
#include "SpecularBTDF.h"

GlassMaterial::GlassMaterial(const glm::vec3& R, const glm::vec3& T, const float ior)
{
	bxdfs.push_back(std::make_unique<SpecularBRDF>(R, std::make_unique<FresnelDielectric>(1.0f, ior)));
	bxdfs.push_back(std::make_unique<SpecularBTDF>(T, 1.0f, ior));
}