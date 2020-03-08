#include "MirrorMaterial.h"

#include "FresnelNoOp.h"
#include "SpecularBRDF.h"

MirrorMaterial::MirrorMaterial(const glm::vec3& R)
{
	bxdfs.push_back(std::make_unique<SpecularBRDF>(R, std::make_unique<FresnelNoOp>()));
}
