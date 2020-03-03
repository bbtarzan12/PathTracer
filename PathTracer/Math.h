#pragma once
#include <glm/glm.hpp>
#include <tuple>

namespace PathTracing
{
	// Normal을 Up Vector로 하는 Local Space 좌표계 만들기
	std::tuple<glm::vec3, glm::vec3> BuildLocalSpace(const glm::vec3 normal);

	glm::vec3 WorldToLocal(const glm::vec3& vector, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& biTangent);

	// Local Space에서 World Space로 변환
	glm::vec3 LocalToWorld(const glm::vec3& vector, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& biTangent);

};
