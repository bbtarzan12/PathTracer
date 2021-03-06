#pragma once
#include <glm/glm.hpp>
#include "bitmask.hpp"

struct IntersectInfo;
class Material;
class LightVisitor;
class Ray;
enum class BxdfType;

using namespace Bitmask;

class Light
{
public:
	virtual ~Light() = default;

	virtual void Accept(LightVisitor& visitor) = 0;
	virtual glm::vec3 Sample(const glm::vec3& point, const glm::vec3& worldWo, const IntersectInfo& info, const Material* material, float& distance, glm::vec3& worldWi) const = 0;
	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const = 0;
	virtual glm::vec3 Emit() const = 0;
};
