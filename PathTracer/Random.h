#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <tuple>

namespace PathTracing
{
	float RandomFloat();

	std::tuple<float, float> RandomFloat2();

	// [0, 1]���簢���� ���������� ���
	glm::vec2 ConcentricSampleDisk(const glm::vec2& point);

	// �ΰ��� ������ �����ϰ� �������� �ݱ��� ������ ����
	glm::vec3 UniformSampleHemisphere(const float r1, const float r2);

	glm::vec3 UniformSampleSphere(const float r1, const float r2);

	glm::vec3 UniformSampleCone(const float r1, const float r2, const float cosThetaMax);

	glm::vec3 UniformSampleCone(const float r1, const float r2, const float cosThetaMax, const glm::vec3& x, const glm::vec3& y, const glm::vec3& z);

	constexpr float UniformHemispherePDF = glm::one_over_two_pi<float>();

}
