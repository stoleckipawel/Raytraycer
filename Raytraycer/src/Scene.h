#pragma once
#include <vector>
#include "glm/glm.hpp"

struct Material
{
	glm::vec3 Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 Emmisive = glm::vec3(0.0f, 0.0f, 0.0f);
	float Specular = 0.5;
	float Roughness = 0.07;
	float Metalic = 0.0;
};

struct Sphere
{
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	float Radius = 0.5f;
	int MaterialIndex = 0;
};

struct DirectionalLight
{
	glm::vec3 Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
	float Intensity = 1.0f;
};

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
	std::vector<DirectionalLight> DirectionalLights;
};
