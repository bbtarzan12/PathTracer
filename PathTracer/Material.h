#pragma once
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "Bxdf.h"

struct IntersectInfo;

class Material
{
public:
	virtual ~Material() = default;

	// wo와 wi가 주어졌을 때 brdf 계산
	glm::vec3 CalculateF(const IntersectInfo& info, const glm::vec3& worldWo, const glm::vec3& worldWi) const;

	// wo가 주어졌을 때 BxDF에 맞는 wi를 생성하고 brdf 계산
	glm::vec3 SampleF(const IntersectInfo& info, const glm::vec3& worldWo, glm::vec3& worldWi, float& pdf, bitmask<BxdfType>& type) const;
	
protected:
	mutable std::vector<std::unique_ptr<Bxdf>> bxdfs;
	
};
