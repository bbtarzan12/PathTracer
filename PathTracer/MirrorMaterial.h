#pragma once
#include <memory>

#include "Material.h"

class SpecularBRDF;

class MirrorMaterial : public Material
{
public:
	MirrorMaterial(const glm::vec3& R);

	virtual glm::vec3 CalculateF(const IntersectInfo& info, const glm::vec3& worldWo, const glm::vec3& worldWi) const override;
	virtual glm::vec3 SampleF(const IntersectInfo& info, const glm::vec3& worldWo, glm::vec3& worldWi, float& pdf) const override;
	virtual glm::vec3 GetDefaultColorForDrawOpenGL() const override;
	
private:
	std::unique_ptr<SpecularBRDF> mirror;
	
};
