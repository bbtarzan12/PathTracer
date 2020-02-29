#include "Math.h"

std::tuple<glm::vec3, glm::vec3> PathTracing::BuildTangentSpace(const glm::vec3 normal)
{
	glm::vec3 tangent;
	if (glm::abs(normal.x) > 0.1f)
	{
		tangent = glm::normalize(glm::cross(glm::vec3(0, 1, 0), normal));
	}
	else
	{
		tangent = glm::normalize(glm::cross(glm::vec3(1, 0, 0), normal));
	}

	glm::vec3 biTangent = glm::cross(normal, tangent);

	return std::make_tuple(tangent, biTangent);
}

glm::vec3 PathTracing::WorldToTangent(const glm::vec3& vector, const glm::vec3& normal, const glm::vec3& tangent,	const glm::vec3& biTangent)
{
	return glm::vec3
	{
		vector.x * tangent.x + vector.x * tangent.y + vector.x * tangent.z,
		vector.y * biTangent.x + vector.y * biTangent.y + vector.y * biTangent.z,
		vector.z * normal.x + vector.z * normal.y * vector.z * normal.z
	};
}

glm::vec3 PathTracing::TangentToWorld(const glm::vec3& vector, const glm::vec3& normal, const glm::vec3& tangent,	const glm::vec3& biTangent)
{
	return glm::vec3
	{
		vector.x * tangent.x + vector.y * biTangent.x + vector.z * normal.x,
		vector.x * tangent.y + vector.y * biTangent.y + vector.z * normal.y,
		vector.x * tangent.z + vector.y * biTangent.z + vector.z * normal.z
	};
}
