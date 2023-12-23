#pragma once

#include "Walnut/Image.h"
#include <memory>
#include "Camera.h"
#include "Ray.h"
#include "glm/glm.hpp"
#include "Scene.h"

class Renderer
{
public:
	Renderer() = default;
	void Render(const Scene& scene, const Camera& camera);
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFrontBuffer() const { return m_FrontBuffer; }
	
private:
	glm::vec4 TraceRay(const Scene& scene, const Ray& ray);
	std::shared_ptr<Walnut::Image> m_FrontBuffer;
	uint32_t* m_FrontBufferData = nullptr;
};

