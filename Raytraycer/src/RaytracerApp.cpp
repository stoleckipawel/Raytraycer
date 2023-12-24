#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Camera.h"
#include "Walnut/Timer.h"
#include "Walnut/Image.h"
#include "Renderer.h"
#include "Scene.h"
#include "glm/gtc/type_ptr.hpp"

using namespace Walnut;

class ExampleLayer : public Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.0001f, 1000.0f)
	{
		{
			Material material;
			material.Albedo = glm::vec3(0.0f, 1.0f, 0.0f) * 0.5f;
			material.Roughness = 0.03f;
			m_Scene.Materials.push_back(material);
		}

		{
			Material material;
			material.Albedo = glm::vec3(1.0f, 0.0f, 0.0f) * 0.5f;
			material.Roughness = 0.070f;
			m_Scene.Materials.push_back(material);
		}

		{
			Material material;
			material.Albedo = glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f;
			material.Roughness = 0.160f;
			m_Scene.Materials.push_back(material);
		}

		{
			Sphere sphere;
			sphere.MaterialIndex = 0;
			sphere.Position = glm::vec3(-1.0f, 0.0f, 0.0f);
			sphere.Radius = 0.6;
			m_Scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.MaterialIndex = 1;
			sphere.Position = glm::vec3(1.0f, -0.470f, -5.0f);
			sphere.Radius = 1.5;
			m_Scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.MaterialIndex = 2;
			sphere.Position = glm::vec3(1.0f, -1002.0f, 0.0f);
			sphere.Radius = 1000.0f;
			m_Scene.Spheres.push_back(sphere);
		}

		{
			//moon
			DirectionalLight directionalLight;
			directionalLight.Direction = glm::vec3(-0.6f, -0.5f, 1.0f);
			directionalLight.Color = glm::vec3(0.25, 0.9, 1.0);
			directionalLight.Intensity = 0.15f;
			m_Scene.DirectionalLights.push_back(directionalLight);
		}

		{
			//sun
			DirectionalLight directionalLight;
			directionalLight.Direction = glm::vec3(0.45f, -1.0f, -1.0f);
			directionalLight.Color = glm::vec3(1.0, 0.9, 0.3);
			directionalLight.Intensity = 0.75f;
			m_Scene.DirectionalLights.push_back(directionalLight);
		}
		
	}
	virtual void OnUpdate(float ts) override
	{
		if (m_Camera.OnUpdate(ts))
		{
			m_Renderer.Reset();
		};
	}
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render Time: %.3fms", m_LastRenderTime);
		ImGui::Text("Frame Index: %.i", m_Renderer.GetFrameIndex());
		ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulate);
		
		if (ImGui::Button("Reset"))
		{
			m_Renderer.Reset();
		}
		ImGui::End();

		ImGui::Begin("Lights");
		for (size_t i = 0; i < m_Scene.DirectionalLights.size(); i++)
		{
			ImGui::PushID(i);

			DirectionalLight& directionalLight = m_Scene.DirectionalLights[i];
			ImGui::DragFloat3("Direction", glm::value_ptr(directionalLight.Direction), 0.01f);
			ImGui::ColorEdit3("Color", glm::value_ptr(directionalLight.Color));
			ImGui::DragFloat("Intensity", &directionalLight.Intensity, 0.01f);
			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::End();

		ImGui::Begin("Geometry");
		for(size_t i = 0; i < m_Scene.Spheres.size(); i++)
		{ 
			ImGui::PushID(i);

			Sphere& sphere = m_Scene.Spheres[i];
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.01f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.01f);
			ImGui::DragInt("MaterialIndex", &sphere.MaterialIndex, 1.0f, 0.0f, m_Scene.Materials.size());

			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::End();

		ImGui::Begin("Materials");
		for (size_t i = 0; i < m_Scene.Materials.size(); i++)
		{
			ImGui::PushID(i);
			Material& material = m_Scene.Materials[i];
			ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
			ImGui::ColorEdit3("Emmisive", glm::value_ptr(material.Emmisive));
			ImGui::DragFloat("Metalic", &material.Metalic, 1.0f, 0.0f, 1.0f);
			ImGui::DragFloat("Specular", &material.Specular, 0.01f);
			ImGui::DragFloat("Roughness", &material.Roughness, 0.01f, 0.0f, 1.0f);
			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Viewport");

		m_ViewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

		auto FrontBuffer = m_Renderer.GetFrontBuffer();
		if (FrontBuffer)
		{
			ImGui::Image(FrontBuffer->GetDescriptorSet(), { (float)FrontBuffer->GetWidth(), (float)FrontBuffer->GetHeight() },
				ImVec2(0,1), ImVec2(1,0));
		}
		
		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Raytraycer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}