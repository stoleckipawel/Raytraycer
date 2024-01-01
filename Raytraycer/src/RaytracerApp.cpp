#include "RaytraycerApp.h"
#include "Scene/Geometry/Sphere.h"
#include "Scene/Geometry/Plane.h"

RaytraycerApp::RaytraycerApp()
	: m_Camera(45.0f, 0.0001f, 1000.0f)
{
	RegisterMaterials();
	RegisterPrimitives();
	RegisterLights();
}

void RaytraycerApp::RegisterMaterials()
{
	Material material_ground;
	material_ground.Albedo = glm::vec3(0.68f, 0.74f, 0.67f) * 0.6f;
	material_ground.Roughness = 0.5f;
	material_ground.Specular = 0.04f;
	m_Materials.push_back(material_ground);
	
	Material material_red;
	material_red.Albedo = glm::vec3(1.0f, 0.5f, 0.5f) * 0.5f;
	material_red.Roughness = 1.0f;
	material_red.Opacity = 0.9f;
	material_red.IOR = 1.125f;
	material_red.Specular = 0.1f;
	m_Materials.push_back(material_red);

	Material material_white;
	material_white.Albedo = glm::vec3(0.5f, 1.0f, 1.0f) * 0.75f;
	material_white.Roughness = 0.0f;
	material_white.Opacity = 0.9f;
	material_white.IOR = 1.125f;
	material_white.Specular = 1.0f;
	m_Materials.push_back(material_white);

	Material material_emmisive;
	material_emmisive.Albedo = glm::vec3(0.0f, 0.0f, 0.0f);
	material_emmisive.Emmisive = glm::vec3(1.0f, 0.9f, 0.9f) * 10.0f;
	material_emmisive.Roughness = 0.75f;
	material_emmisive.Specular = 0.04f;
	m_Materials.push_back(material_emmisive);
	
}

void RaytraycerApp::RegisterPrimitives()
{
	std::unique_ptr<Plane> plane_green = std::make_unique<Plane>();
	plane_green->Material = &m_Materials[0];
	plane_green->Position = glm::vec3(0.0f, -2.0f, 0.0f);
	m_Scene.Primitives.push_back(std::move(plane_green));

	std::unique_ptr<Sphere> sphere_green = std::make_unique<Sphere>();
	sphere_green->Material = &m_Materials[1];
	sphere_green->Position = glm::vec3(-1.0f, 0.0f, 0.0f);
	sphere_green->Radius = 1.0f;
	m_Scene.Primitives.push_back(std::move(sphere_green));


	std::unique_ptr<Sphere> sphere_red = std::make_unique<Sphere>();
	sphere_red->Material = &m_Materials[2];
	sphere_red->Position = glm::vec3(1.0f, 0.670f, -5.0f);
	sphere_red->Radius = 2.5;
	m_Scene.Primitives.push_back(std::move(sphere_red));
	
	std::unique_ptr<Sphere> sphere_emmisive = std::make_unique<Sphere>();
	sphere_emmisive->Material = &m_Materials[3];
	sphere_emmisive->Position = glm::vec3(16.0f, 17.0f, -100.0f);
	sphere_emmisive->Radius = 20.0f;
	m_Scene.Primitives.push_back(std::move(sphere_emmisive));	
}

void RaytraycerApp::RegisterLights()
{
	std::unique_ptr<DirectionalLight> sun = std::make_unique<DirectionalLight>();
	sun->Direction = glm::vec3(-1.0f, 0.360f, 0.0f);
	sun->Color = glm::vec3(1.0f, 0.905f, 0.7f);
	sun->Intensity = 120.0f;
	m_Scene.Lights.push_back(std::move(sun));
}

void RaytraycerApp::OnUpdate(float ts)
{
	if (m_Camera.OnUpdate(ts))
	{
		m_Renderer.Reset();
	};
}

void RaytraycerApp::OnUIRender()
{
	ImGui::Begin("Settings");
	ImGui::Text("Last Render Time: %.3fms", m_LastRenderTime);
	ImGui::Text("Frame Index: %.i", m_Renderer.GetFrameIndex());
	ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulate);
	ImGui::DragInt("Light Bounce Count", &m_Renderer.GetSettings().Bounces, 1.0f, 1, 8);
		
	if (ImGui::Button("Reset"))
	{
		m_Renderer.Reset();
	}
	ImGui::End();

	ImGui::Begin("Lights");
	m_Scene.EnvironmentLight.BuildUI(655628);
	for (int32_t i = 0; i < m_Scene.Lights.size(); i++)
	{
		m_Scene.Lights[i]->BuildUI(i);
	}
	ImGui::End();

	ImGui::Begin("Geometry");
	for(uint32_t i = 0; i < m_Scene.Primitives.size(); i++)
	{ 
		m_Scene.Primitives[i]->BuildUI(i);
	}
	ImGui::End();

	ImGui::Begin("Materials");
	for (uint32_t i = 0; i < m_Materials.size(); i++)
	{
		m_Materials[i].BuildUI(i);
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

void RaytraycerApp::Render()
{
	Timer timer;

	m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
	m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
	m_Renderer.Render(m_Scene, m_Camera);

	m_LastRenderTime = timer.ElapsedMillis();
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Raytraycer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<RaytraycerApp>();
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