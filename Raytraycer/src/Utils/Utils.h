#pragma once
#include "glm/glm.hpp"
#include "Walnut/Random.h"

namespace Utils 
{
	static uint32_t ConvertColorToUInt32(glm::vec4 color)
	{
		color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
		color *= 255.0f;
		return ((uint8_t)color.x) | ((uint8_t)color.y << 8) | ((uint8_t)color.z << 16) | ((uint8_t)color.w << 24);
	}

	static glm::vec3 RandomHemisphereDir(glm::vec3 Normal)
	{
		glm::vec3 random_dir = glm::normalize(Walnut::Random::Vec3(-1.0f, 1.0f));
		return glm::sign(glm::dot(random_dir, Normal)) * random_dir;//makes sure it's half sphere dirs
	}
}
