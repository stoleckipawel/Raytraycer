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
	struct HitPayload 
	{
		float HitDistance = 0.0f;
		glm::vec3 WorldNormal = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 WorldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		uint32_t ObjectIndex = 0;
	};

	HitPayload TraceRay(const Ray& ray);
	glm::vec4 RayGen(uint32_t x, uint32_t y);
	HitPayload ClosestHit(const Ray& ray, uint32_t objectIndex, float hitDistance);
	HitPayload Miss(const Ray& ray);

	std::shared_ptr<Walnut::Image> m_FrontBuffer;
	uint32_t* m_FrontBufferData = nullptr;

	const Camera* m_ActiveCamera;
	const Scene* m_ActiveScene;
};

