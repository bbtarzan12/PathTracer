#pragma once
#include <limits>

namespace PathTracing
{
	template<typename T>
	constexpr T INFINITE = std::numeric_limits<T>::infinity();

	constexpr float GAMMA_DECODING = 2.2f;
	constexpr float GAMMA_COMPRESSION = 1.0f / GAMMA_DECODING;

}
