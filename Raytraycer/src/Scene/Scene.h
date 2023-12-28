#pragma once
#include <vector>
#include "Lights/Light.h"
#include "Geometry/Primitive.h"

struct Scene
{
	std::vector<std::unique_ptr<Primitive>> Primitives;
	std::vector<std::unique_ptr<Light>> Lights;
};
