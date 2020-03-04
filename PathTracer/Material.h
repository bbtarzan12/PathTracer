#pragma once
#include <glm/vec3.hpp>

struct IntersectInfo;

class Material
{
public:
	virtual ~Material() = default;

	// wo와 wi가 주어졌을 때 brdf 계산
	virtual glm::vec3 CalculateF(const IntersectInfo& info, const glm::vec3& worldWo, const glm::vec3& worldWi) const = 0;

	// wo가 주어졌을 때 BxDF에 맞는 wi를 생성하고 brdf 계산
	virtual glm::vec3 SampleF(const IntersectInfo& info, const glm::vec3& worldWo, glm::vec3& worldWi, float& pdf) const = 0;

	// OpenGL Debug Method
	virtual glm::vec3 GetDefaultColorForDrawOpenGL() const = 0;
};
