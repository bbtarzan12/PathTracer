#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <tuple>

namespace PathTracing
{
	uint32_t _random();

	float RandomFloat();

	std::tuple<float, float> RandomFloat2();

	// [0, 1]���簢���� ���������� ���
	glm::vec2 ConcentricSampleDisk(const glm::vec2& point);

	// �ΰ��� ������ �����ϰ� �������� �ݱ��� ������ ����
	glm::vec3 UniformSampleHemisphere(const float r1, const float r2);

	glm::vec3 UniformSampleSphere(const float r1, const float r2);

	constexpr float UniformHemispherePDF = glm::one_over_two_pi<float>();

}
