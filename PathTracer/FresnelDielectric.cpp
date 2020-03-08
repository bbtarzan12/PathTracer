#include "FresnelDielectric.h"

FresnelDielectric::FresnelDielectric(const float etai, const float etat)
	:etai(etai), etat(etat)
{
}

glm::vec3 FresnelDielectric::Calculate(float cosi) const
{
	cosi = glm::clamp(cosi, -1.0f, 1.0f);

	const bool bEntering = cosi > 0.0f;
	float ei = etai;
	float et = etat;

	if (!bEntering)
		std::swap(ei, et);

	const float sint = ei / et * glm::sqrt(glm::max(0.0f, 1.0f - cosi * cosi));
	
	if(sint >= 1.0f)
	{
		return glm::vec3(1.0f);
	}

	const float cost = glm::sqrt(glm::max(0.0f, 1.0f - sint * sint));
	cosi = glm::abs(cosi);

	const float etatCosi = etat * cosi;
	const float etatCost = etat * cost;
	const float etaiCosi = etai * cosi;
	const float etaiCost = etai * cost;
	
	const float lRNum = etatCosi - etaiCost;
	const float lRDenom = etatCosi + etaiCost;
	const float rRNum = etaiCosi - etatCost;
	const float rRDenom = etaiCosi + etatCost;

	const glm::vec3 lR = glm::vec3(lRNum / lRDenom);
	const glm::vec3 rR = glm::vec3(rRNum / rRDenom);

	return (lR * lR + rR * rR) * 0.5f;
}

