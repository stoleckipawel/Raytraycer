#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Scene/Camera/Camera.h"
#include "Walnut/Timer.h"
#include "Walnut/Image.h"
#include "Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Geometry/Sphere.h"
#include "Scene/Lights/DirectionalLight.h"
#include "glm/gtc/type_ptr.hpp"

#pragma once

using namespace Walnut;

class RaytraycerApp : public Layer
{
public:
	RaytraycerApp();
	virtual void OnUpdate(float ts) override;
	virtual void OnUIRender() override;
private:
	void Render();
	void RegisterMaterials();
	void RegisterPrimitives();
	void RegisterLights();
private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
	std::vector<Material> m_Materials;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0.0f;
};