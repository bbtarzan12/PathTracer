#include "FresnelNoOp.h"

glm::vec3 FresnelNoOp::Calculate(const float cosi) const
{
	return glm::vec3(1.0f);
}
