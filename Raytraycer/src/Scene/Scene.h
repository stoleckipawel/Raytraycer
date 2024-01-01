#pragma once
#include <vector>
#include "Lights/Light.h"
#include "Geometry/Primitive.h"
#include "Lights/EnvironmentLight.h"

struct Scene
{
	std::vector<std::unique_ptr<Primitive>> Primitives;
	std::vector<std::unique_ptr<Light>> Lights;
	EnvironmentLight EnvironmentLight;

	std::vector<const DirectionalLight*> GetDirectionalLights() const
	{
		std::vector<const DirectionalLight*> directionalLights;
		for (const auto& lightPtr : Lights) 
		{
			if (const auto directionalLightPtr = dynamic_cast<const DirectionalLight*>(lightPtr.get())) 
			{
				directionalLights.push_back(directionalLightPtr);
			}
		}
		return directionalLights;
	}
};


