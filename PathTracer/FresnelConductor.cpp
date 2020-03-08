#include "FresnelConductor.h"

FresnelConductor::FresnelConductor(const glm::vec3& eta, const glm::vec3& k)
	:eta(eta), k(k)
{
	
}

glm::vec3 FresnelConductor::Calculate(float cosi) const
{
	cosi = glm::abs(cosi);
	
	const glm::vec3 eta2_p_k2 = eta * eta + k * k;
	const float cosi2 = cosi * cosi;
	const glm::vec3 eta2_p_k2_m_cosi2 = eta2_p_k2 * cosi2;
	const glm::vec3 two_eta_cosi = glm::vec3(2.0f) * eta * cosi;

	const glm::vec3 lRNum = eta2_p_k2_m_cosi2 - two_eta_cosi + glm::vec3(1.0f);
	const glm::vec3 lRDenom = eta2_p_k2_m_cosi2 + two_eta_cosi + glm::vec3(1.0f);

	const glm::vec3 rRNum = eta2_p_k2 - two_eta_cosi + cosi2;
	const glm::vec3 rRDenom = eta2_p_k2 + two_eta_cosi + cosi2;

	return (lRNum / lRDenom + rRNum / rRDenom) * 0.5f;
}
