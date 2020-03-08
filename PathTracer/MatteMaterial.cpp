#include "MatteMaterial.h"
#include "LambertianBRDF.h"

MatteMaterial::MatteMaterial(const glm::vec3& R)
{
	bxdfs.push_back(std::make_unique<LambertianBrdf>(R));
}