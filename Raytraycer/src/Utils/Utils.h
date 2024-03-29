#pragma once
#include "glm/glm.hpp"
#include "Walnut/Random.h"

namespace Utils 
{
	static glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float alpha)
	{
		return a * (1.0f - alpha) + (b * alpha);
	}

	static uint32_t ConvertColorToUInt32(glm::vec4 color)
	{
		color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
		color *= 255.0f;
		return ((uint8_t)color.x) | ((uint8_t)color.y << 8) | ((uint8_t)color.z << 16) | ((uint8_t)color.w << 24);
	}


	static glm::vec3 CosineWeightedHemisphereSample(const glm::vec3& normal)
	{
		float u1 = Walnut::Random::Float();
		float u2 = Walnut::Random::Float();

		float cosTheta = sqrt(1.0f - u1);
		float sinTheta = sqrt(u1);

		float phi = 2.0f * 3.141592653589793 * u2;

		// Calculate the tangent and bitangent vectors using a more robust method
		glm::vec3 tangent, bitangent;
		if (std::abs(normal.x) > 0.1f)
			tangent = glm::normalize(glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f)));
		else
			tangent = glm::normalize(glm::cross(normal, glm::vec3(1.0f, 0.0f, 0.0f)));

		bitangent = glm::cross(normal, tangent);

		// Form a basis matrix using the tangent, bitangent, and normal vectors
		glm::mat3 basisMatrix = glm::mat3(tangent, bitangent, normal);

		// Transform the sampled direction from local coordinates to world coordinates
		glm::vec3 hemisphereDir = basisMatrix * glm::vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);

		return hemisphereDir;
	}

	static glm::vec3 RandomHemisphereDir(glm::vec3 Normal)
	{
		glm::vec3 random3 = Walnut::Random::Vec3(-1.0f, 1.0f);
		glm::vec3 random_dir = glm::normalize(random3);
		float hemisphereSign = glm::sign(glm::dot(random_dir, Normal));
		return hemisphereSign * random_dir;//makes sure it's half sphere dirs
	}

	static float FresnelSchlick(float f0, float ior_inside, float ior_outside, glm::vec3 normal, glm::vec3 incident)
	{
		//source:https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
		// Schlick aproximation
		float r0 = (ior_inside - ior_outside) / (ior_inside + ior_outside);
		r0 *= r0;
		float cosX = -dot(normal, incident);
		if (ior_inside > ior_outside)
		{
			float n = ior_inside / ior_outside;
			float sinT2 = n * n * (1.0 - cosX * cosX);
			// Total internal reflection
			if (sinT2 > 1.0)
				return 1.0;
			cosX = sqrt(1.0 - sinT2);
		}
		float x = 1.0 - cosX;
		float ret = r0 + (1.0 - r0) * x * x * x * x * x;

		// adjust reflect multiplier for f0
		return f0 + (1.0f - f0) * ret;
	}
}
