#include "Math.h"

std::tuple<glm::vec3, glm::vec3> PathTracing::BuildLocalSpace(const glm::vec3 normal)
{
	glm::vec3 tangent;
	if (glm::abs(normal.x) > glm::abs(normal.y))
	{
		tangent = glm::vec3(normal.z, 0.0f, -normal.x) / glm::sqrt(normal.x * normal.x + normal.z * normal.z);
	}
	else
	{
		tangent = glm::vec3(0.0f, -normal.z, normal.y) / glm::sqrt(normal.y * normal.y + normal.z * normal.z);
	}

	glm::vec3 biTangent = glm::cross(normal, tangent);

	return std::make_tuple(tangent, biTangent);
}

glm::vec3 PathTracing::WorldToLocal(const glm::vec3& vector, const glm::vec3& normal, const glm::vec3& tangent,	const glm::vec3& biTangent)
{
	return glm::vec3
	{
		vector.x * tangent.x + vector.y * tangent.y + vector.z * tangent.z,
		vector.x * biTangent.x + vector.y * biTangent.y + vector.z * biTangent.z,
		vector.x * normal.x + vector.y * normal.y + vector.z * normal.z
	};
}

glm::vec3 PathTracing::LocalToWorld(const glm::vec3& vector, const glm::vec3& normal, const glm::vec3& tangent,	const glm::vec3& biTangent)
{
	return glm::vec3
	{
		vector.x * tangent.x + vector.y * biTangent.x + vector.z * normal.x,
		vector.x * tangent.y + vector.y * biTangent.y + vector.z * normal.y,
		vector.x * tangent.z + vector.y * biTangent.z + vector.z * normal.z
	};
}
