#pragma once
#include <vector>
#include "glm/glm.hpp"

struct Sphere
{
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	float Radius = 0.5f;
	glm::vec3 Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct Scene
{
	std::vector<Sphere> Spheres;
};