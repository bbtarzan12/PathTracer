#pragma once
#include <glm/glm.hpp>

#include "bitmask.hpp"

/*
 *                  ^
 *                  │ z
 *                  │
 *                  │
 *                  │        x
 *                  /──────────>
 *                 /
 *                /
 *             y /
 *              └
 *
 *  BxDF에서는 이 로컬 좌표계를 사용한다
 */

enum class BxdfType
{
	REFLECTION = 0x01,
	TRANSMISSION = 0x02,
	ALL = REFLECTION | TRANSMISSION,
	_bitmask_value_mask = ALL
};
BITMASK_DEFINE(BxdfType)

using namespace Bitmask;

class Bxdf
{
public:
	explicit Bxdf(const BxdfType& type);
	virtual ~Bxdf() = default;

	// wo와 wi가 주어졌을 때 F계산
	virtual glm::vec3 CalculateF(const glm::vec3& wo, const glm::vec3& wi) const = 0;

	// wo만 주어졌을 때 wi, pdf, F계산
	// 기본 F를 Sample하는 것은 반구를 Uniform하게 샘플링한다고 가정
	virtual glm::vec3 SampleF(const glm::vec3& wo, glm::vec3& wi, float& pdf) const;

	// 기본 PDF는 반구를 Uniform하게 샘플링한다고 가정
	virtual float PDF(const glm::vec3& wo, const glm::vec3& wi) const;

	bool MatchType(const bitmask<BxdfType>& other) const;
	const bitmask<BxdfType>& GetType() const;

protected:
	static float CosTheta(const glm::vec3& v);
	static float SinTheta(const glm::vec3& v);
	static float CosPhi(const glm::vec3& v);
	static float SinPhi(const glm::vec3& v);

protected:
	BxdfType type;
};