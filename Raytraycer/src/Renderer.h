#pragma once

#include "Walnut/Image.h"
#include <memory>
#include "Scene/Camera/Camera.h"
#include "Scene/Ray.h"
#include "glm/glm.hpp"
#include "Scene/Scene.h"



class Renderer
{
public:
	struct Settings
	{
		bool Accumulate = true;
		int Bounces = 10;
		bool DebugNormal = false;
	};

public:
	void Render(const Scene& scene, const Camera& camera);
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFrontBuffer() const { return m_FrontBuffer; }

	void Reset() { m_FrameIndex = 1; };
	uint32_t GetFrameIndex() { return m_FrameIndex; };
	Settings& GetSettings() { return m_Settings; };
private:
	Trace TraceScene(const Ray& ray);
	glm::vec4 RayGen(uint32_t x, uint32_t y);
	void CleanHistoryBuffer();
	void UpdateGuids();
	void UpdateTransforms();
private:
	std::shared_ptr<Walnut::Image> m_FrontBuffer;
	uint32_t* m_FrontBufferData = nullptr;

	Settings m_Settings;
	const Camera* m_ActiveCamera = nullptr;
	const Scene* m_ActiveScene = nullptr;

	glm::vec4* m_HistoryBufferData = nullptr;
	uint32_t m_FrameIndex = 1;

	const float m_epsilon = 1.0f / 255.0f;
	const float AIR_IOR = 1.0003f;

	std::vector<uint32_t> m_HorizontalPixelsIterator;
	std::vector<uint32_t> m_VerticalPixelsIterator;
};


