#include "RaytraycerApp.h"
#include "Scene/Geometry/Sphere.h"
#include "Scene/Geometry/Plane.h"
#include "Scene/Geometry/Box.h"

#define MaterialIDGround 0
#define MaterialIDWhite 1
#define MaterialIDRed 2
#define MaterialIDGreen 3
#define MaterialIDEmmisive 4
#define MaterialIDMirror 5
#define MaterialIDRough 6

RaytraycerApp::RaytraycerApp()
	: m_Camera(45.0f, 0.0001f, 1000.0f)
{
	RegisterMaterials();
	CornellBox();
}

void RaytraycerApp::CornellBox()
{
	std::unique_ptr<Plane> plane = std::make_unique<Plane>();
	plane->Material = &m_Materials[MaterialIDWhite];
	plane->Position = glm::vec3(0.0f, -2.0f, 0.0f);
	m_Scene.Primitives.push_back(std::move(plane));

	//std::unique_ptr<Box> box = std::make_unique<Box>();
	//box->Material = &m_Materials[MaterialIDRed];
	//box->Position = glm::vec3(0.0f, 0.0f, 0.0);
	//box->Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	//m_Scene.Primitives.push_back(std::move(box));

	std::unique_ptr<Box> box_left = std::make_unique<Box>();
	box_left->Material = &m_Materials[MaterialIDRed];
	box_left->Position = glm::vec3(0.0f, -0.5f, 0.0);
	box_left->Scale = glm::vec3(0.1f, 3.0f, 3.0f);
	m_Scene.Primitives.push_back(std::move(box_left));

	std::unique_ptr<Box> box_right = std::make_unique<Box>();
	box_right->Material = &m_Materials[MaterialIDGreen];
	box_right->Position = glm::vec3(3.0f, -0.5f, 0.0);
	box_right->Scale = glm::vec3(0.1f, 3.0f, 3.0f);
	m_Scene.Primitives.push_back(std::move(box_right));

	std::unique_ptr<Box> box_back = std::make_unique<Box>();
	box_back->Material = &m_Materials[MaterialIDWhite];
	box_back->Position = glm::vec3(1.5f, -0.5f, -1.5f);
	box_back->Scale = glm::vec3(3.0f, 3.0f, 0.1f);
	m_Scene.Primitives.push_back(std::move(box_back));

	std::unique_ptr<Box> box_top = std::make_unique<Box>();
	box_top->Material = &m_Materials[MaterialIDWhite];
	box_top->Position = glm::vec3(1.5f, 1.0f, 0.0f);
	box_top->Scale = glm::vec3(3.1f, 0.1f, 3.0f);
	m_Scene.Primitives.push_back(std::move(box_top));

	std::unique_ptr<Box> box_top_emmisive = std::make_unique<Box>();
	box_top_emmisive->Material = &m_Materials[MaterialIDEmmisive];
	box_top_emmisive->Position = glm::vec3(1.5f, 0.9f, 0.0f);
	box_top_emmisive->Scale = glm::vec3(0.75f, 0.1f, 0.75f);
	m_Scene.Primitives.push_back(std::move(box_top_emmisive));

	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();
	sphere->Material = &m_Materials[MaterialIDMirror];
	sphere->Position = glm::vec3(1.0f, -1.5f, 0.5f);
	m_Scene.Primitives.push_back(std::move(sphere));

	std::unique_ptr<Box> box_rough = std::make_unique<Box>();
	box_rough->Material = &m_Materials[MaterialIDRough];
	box_rough->Position = glm::vec3(2.0f, -1.0f, 0.0f);
	box_rough->Rotation = glm::vec3(0.0, 45.0f, 0.0f);
	box_rough->Scale = glm::vec3(0.5f, 1.5f, 0.5f);
	m_Scene.Primitives.push_back(std::move(box_rough));
	
}

void RaytraycerApp::RegisterMaterials()
{
	float roughness = 1.0f;
	float specular = 0.04f;
	float albedo_lum = 0.94f;

	Material material_ground;
	material_ground.Albedo = glm::vec3(0.68f, 0.74f, 0.67f) * 0.6f;
	material_ground.Roughness = roughness;
	material_ground.Specular = specular;
	m_Materials.push_back(material_ground);

	Material material_white;
	material_white.Albedo = glm::vec3(albedo_lum, albedo_lum, albedo_lum);
	material_white.Roughness = roughness;
	material_white.Specular = specular;
	m_Materials.push_back(material_white);

	Material material_red;
	material_red.Albedo = glm::vec3(albedo_lum, 0.0f, 0.0f);
	material_red.Roughness = roughness;
	material_red.Specular = specular;
	m_Materials.push_back(material_red);

	Material material_green;
	material_green.Albedo = glm::vec3(0.0f, albedo_lum, 0.0f);
	material_green.Roughness = roughness;
	material_green.Specular = specular;
	m_Materials.push_back(material_green);

	Material material_emmisive;
	material_emmisive.Emmisive = glm::vec3(1.0f, 0.94f, 0.658f) * 15.0f;
	material_emmisive.Roughness = roughness;
	material_emmisive.Specular = specular;
	m_Materials.push_back(material_emmisive);

	Material material_mirror;
	material_mirror.Roughness = 0.0f;
	material_mirror.Specular = 0.5f;
	m_Materials.push_back(material_mirror);

	Material material_rough;
	material_rough.Roughness = 0.1f;
	material_rough.Specular = 0.5f;
	m_Materials.push_back(material_rough);
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
	ImGui::Checkbox("Debug Normal", &m_Renderer.GetSettings().DebugNormal);
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