#pragma once
#include "glm/glm.hpp"

namespace Utils 
{
	static uint32_t ConvertColorToUInt32(glm::vec4 color)
	{
		color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
		color *= 255.0f;
		return ((uint8_t)color.x) | ((uint8_t)color.y << 8) | ((uint8_t)color.z << 16) | ((uint8_t)color.w << 24);
	}
}
