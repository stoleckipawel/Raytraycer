#pragma once
#include "glm/glm.hpp"

struct Ray
{
	glm::vec3 Origin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Transmission = glm::vec3(1.0f, 1.0f, 1.0f);
};

