#include "RaytraycerApp.h"

RaytraycerApp::RaytraycerApp()
	: m_Camera(45.0f, 0.0001f, 1000.0f)
{
	RegisterMaterials();
	RegisterPrimitives();
	RegisterLights();
}

void RaytraycerApp::RegisterMaterials()
{
	Material material_green;
	material_green.Albedo = glm::vec3(0.0f, 1.0f, 0.0f) * 0.5f;
	material_green.Roughness = 0.03f;
	m_Materials.push_back(material_green);
	
	Material material_red;
	material_red.Albedo = glm::vec3(1.0f, 0.0f, 0.0f) * 0.5f;
	material_red.Roughness = 0.070f;
	m_Materials.push_back(material_red);

	Material material_white;
	material_white.Albedo = glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f;
	material_white.Roughness = 0.160f;
	m_Materials.push_back(material_white);

	Material material_emmisive;
	material_emmisive.Albedo = glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f;
	material_emmisive.Emmisive = glm::vec3(0.3f, 0.3f, 0.3f);
	material_emmisive.Roughness = 0.2f;
	m_Materials.push_back(material_emmisive);
	
}

void RaytraycerApp::RegisterPrimitives()
{
	std::unique_ptr<Sphere> sphere_green = std::make_unique<Sphere>();
	sphere_green->Material = &m_Materials[0];
	sphere_green->Position = glm::vec3(-1.0f, 0.0f, 0.0f);
	sphere_green->Radius = 0.6f;
	m_Scene.Primitives.push_back(std::move(sphere_green));

	std::unique_ptr<Sphere> sphere_red = std::make_unique<Sphere>();
	sphere_red->Material = &m_Materials[1];
	sphere_red->Position = glm::vec3(1.0f, -0.470f, -5.0f);
	sphere_red->Radius = 1.5;
	m_Scene.Primitives.push_back(std::move(sphere_red));

	std::unique_ptr<Sphere> sphere_white = std::make_unique<Sphere>();
	sphere_white->Material = &m_Materials[2];
	sphere_white->Position = glm::vec3(1.0f, -1002.0f, 0.0f);
	sphere_white->Radius = 1000.0f;
	m_Scene.Primitives.push_back(std::move(sphere_white));
	
	std::unique_ptr<Sphere> sphere_emmisive = std::make_unique<Sphere>();
	sphere_emmisive->Material = &m_Materials[3];
	sphere_emmisive->Position = glm::vec3(16.0f, 2.0f, -52.0f);
	sphere_emmisive->Radius = 20.0f;
	m_Scene.Primitives.push_back(std::move(sphere_emmisive));
	
}

void RaytraycerApp::RegisterLights()
{
	std::unique_ptr<DirectionalLight> moon = std::make_unique<DirectionalLight>();
	moon->Direction = glm::vec3(0.6f, 0.5f, -1.0f);
	moon->Color = glm::vec3(0.25, 0.9, 1.0);
	moon->Intensity = 0.2f;
	m_Scene.Lights.push_back(std::move(moon));

	std::unique_ptr<DirectionalLight> sun = std::make_unique<DirectionalLight>();
	sun->Direction = glm::vec3(-0.45f, 1.0f, 1.0f);
	sun->Color = glm::vec3(1.0, 0.9, 0.3);
	sun->Intensity = 0.75f;
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
		
	if (ImGui::Button("Reset"))
	{
		m_Renderer.Reset();
	}
	ImGui::End();

	ImGui::Begin("Lights");
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