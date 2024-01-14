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
#define MaterialIDBlue 7
#define MaterialIDTop 8

RaytraycerApp::RaytraycerApp()
	: m_Camera(86.0f, 0.0001f, 1000.0f)
{
	RegisterMaterials();
	CornellBox();

}

void RaytraycerApp::CornellBox()
{
	std::unique_ptr<DirectionalLight> Sun = std::make_unique<DirectionalLight>();
	Sun->Intensity = 0.0f;
	Sun->Color = glm::vec3(1.0f, 0.90f, 0.58f);
	Sun->Direction = glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f));
	m_Scene.Lights.push_back(std::move(Sun));

	const float box_size = 30.0f;
	const float box_size_halved = box_size * 0.5f;
	
	float light_scale = box_size * 0.1;
	float glossy_sphere_radius = box_size * 0.2;
	float rough_sphere_radius = glossy_sphere_radius * 0.5;
	glm::vec3 rough_box_size = glm::vec3(0.7f, 1.75f, 0.7f) * box_size * 0.145f;

	std::unique_ptr<Plane> plane_bottom = std::make_unique<Plane>();
	plane_bottom->Material = &m_Materials[MaterialIDWhite];
	plane_bottom->Position = glm::vec3(0.0f, -box_size_halved, 0.0f);
	m_Scene.Primitives.push_back(std::move(plane_bottom));

	std::unique_ptr<Plane> plane_top = std::make_unique<Plane>();
	plane_top->Material = &m_Materials[MaterialIDTop];
	plane_top->Position = glm::vec3(0.0f, box_size_halved, 0.0f);
	plane_top->Rotation = glm::vec3(0.0f, -1.0f, 0.0f);
	m_Scene.Primitives.push_back(std::move(plane_top));

	std::unique_ptr<Plane> plane_left = std::make_unique<Plane>();
	plane_left->Material = &m_Materials[MaterialIDRed];
	plane_left->Position = glm::vec3(-box_size_halved, 0.0f, 0.0f);
	plane_left->Rotation = glm::vec3(1.0f, 0.0f, 0.0f);
	m_Scene.Primitives.push_back(std::move(plane_left));
	
	std::unique_ptr<Plane> plane_right = std::make_unique<Plane>();
	plane_right->Material = &m_Materials[MaterialIDGreen];
	plane_right->Position = glm::vec3(box_size_halved, 0.0f, 0.0f);
	plane_right->Rotation = glm::vec3(-1.0f, 0.0f, 0.0f);
	m_Scene.Primitives.push_back(std::move(plane_right));

	std::unique_ptr<Plane> plane_back = std::make_unique<Plane>();
	plane_back->Material = &m_Materials[MaterialIDBlue];
	plane_back->Position = glm::vec3(0.0, 0.0f, box_size_halved);
	plane_back->Rotation = glm::vec3(0.0f, 0.0f, -1.0f);
	m_Scene.Primitives.push_back(std::move(plane_back));

	std::unique_ptr<Plane> plane_front = std::make_unique<Plane>();
	plane_front->Material = &m_Materials[MaterialIDWhite];
	plane_front->Position = glm::vec3(0.0, 0.0f, -box_size_halved);
	plane_front->Rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Scene.Primitives.push_back(std::move(plane_front));

	std::unique_ptr<Box> box_top_emmisive = std::make_unique<Box>();
	box_top_emmisive->Material = &m_Materials[MaterialIDEmmisive];
	box_top_emmisive->Position = glm::vec3(0.0f, box_size_halved, 0.0f);
	box_top_emmisive->Scale = glm::vec3(light_scale, 0.1f, light_scale);
	m_Scene.Primitives.push_back(std::move(box_top_emmisive));
	
	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();
	sphere->Material = &m_Materials[MaterialIDMirror];
	sphere->Position = glm::vec3(-box_size_halved, -box_size_halved, -box_size_halved) + glossy_sphere_radius * glm::vec3(1.2f, 1.0, 1.2f);
	sphere->Radius = glossy_sphere_radius;
	m_Scene.Primitives.push_back(std::move(sphere));

	std::unique_ptr<Box> box_rough = std::make_unique<Box>();
	glm::vec3 offset = rough_box_size * glm::sqrt(2.0f) * 1.4f;
	box_rough->Material = &m_Materials[MaterialIDRough];
	box_rough->Position = glm::vec3(box_size_halved - offset.x, -box_size_halved + rough_box_size.y, -box_size_halved + offset.z);
	box_rough->Rotation = glm::vec3(0.0, 33.0f, 0.0f);
	box_rough->Scale = rough_box_size;
	m_Scene.Primitives.push_back(std::move(box_rough));
}

void RaytraycerApp::SceneOutdoor()
{
}

void RaytraycerApp::RegisterMaterials()
{
	float roughness = 0.36f;
	float specular = 0.29f;
	float albedo_lum = 0.967f;

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
	material_emmisive.Emmisive = glm::vec3(1.0f, 0.90f, 0.58f) * 20.0f;
	material_emmisive.Roughness = roughness;
	material_emmisive.Specular = specular;
	m_Materials.push_back(material_emmisive);

	Material material_mirror;
	material_mirror.Roughness = 0.03f;
	material_mirror.Specular = 0.7f;
	m_Materials.push_back(material_mirror);

	Material material_rough;
	material_rough.Albedo = glm::vec3(albedo_lum, albedo_lum, albedo_lum) * 0.8f;
	material_rough.Roughness = 0.3f;
	material_rough.Specular = 0.4f;
	m_Materials.push_back(material_rough);

	Material material_blue;
	material_blue.Albedo = glm::vec3(0.0f, 0.0f, albedo_lum);
	material_blue.Roughness = roughness;
	material_blue.Specular = specular;
	m_Materials.push_back(material_blue);

	Material material_top;
	material_top.Albedo = glm::vec3(albedo_lum, albedo_lum, albedo_lum);
	material_top.Roughness = 0.3;
	material_top.Specular = 0.7;
	m_Materials.push_back(material_top);
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