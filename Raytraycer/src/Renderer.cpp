#include "Renderer.h"
#include "Walnut/Random.h"
#include <execution>
#include "Utils/Utils.h"

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (!m_FrontBuffer)
	{
		m_FrontBuffer = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	else if (width != m_FrontBuffer->GetWidth() || height != m_FrontBuffer->GetHeight())
	{
		m_FrontBuffer->Resize(width, height);
	}
	else
	{
		return;
	}

	delete[] m_FrontBufferData;
	m_FrontBufferData = new uint32_t[width * height];

	delete[] m_HistoryBufferData;
	m_HistoryBufferData = new glm::vec4[width * height];

	m_FrameIndex = 1;

	m_VerticalPixelsIterator.resize(height);
	for (uint32_t i = 0; i < height; i++)
	{
		m_VerticalPixelsIterator[i] = i;
	}
	m_HorizontalPixelsIterator.resize(width);
	for (uint32_t i = 0; i < width; i++)
	{
		m_HorizontalPixelsIterator[i] = i;
	}
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	m_ActiveCamera = &camera;
	m_ActiveScene = &scene;

	UpdateGuids();
	UpdateTransforms();

	//Clean Buffer
	if (m_FrameIndex == 1)
	{
		CleanHistoryBuffer();
	}

	//Multihreaded
	std::for_each(std::execution::par, m_VerticalPixelsIterator.begin(), m_VerticalPixelsIterator.end(), 
		[this](uint32_t y) 
		{
			std::for_each(std::execution::par, m_HorizontalPixelsIterator.begin(), m_HorizontalPixelsIterator.end(),
			[this, y](uint32_t x)
			{
				uint32_t pixel_id = x + y * m_FrontBuffer->GetWidth();
				//Trace One sample
				m_HistoryBufferData[pixel_id] += RayGen(x, y);
				//Resolve samples
				glm::vec4 accumulated_color = m_HistoryBufferData[pixel_id] / (float)m_FrameIndex;
				//float4 -> INT32 conversion
				m_FrontBufferData[pixel_id] = Utils::ConvertColorToUInt32(accumulated_color);
			});
		});

	m_FrontBuffer->SetData(m_FrontBufferData);

	if (m_Settings.Accumulate && !m_Settings.DebugNormal)
	{
		m_FrameIndex++;
	}
	else
	{
		m_FrameIndex = 1;
	}
}

glm::vec4 Renderer::RayGen(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = glm::normalize(m_ActiveCamera->GetRayDirections()[x + y * m_FrontBuffer->GetWidth()]);

	glm::vec3 incomingLight = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 transmissionContribution = glm::vec3(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < m_Settings.Bounces; i++)
	{
		Trace trace = TraceScene(ray);

		if (trace.Result == TraceResult::Miss)
		{
			glm::vec3 environmentLight = m_ActiveScene->EnvironmentLight.SampleEnvironment(ray, m_ActiveScene->GetDirectionalLights());
			incomingLight += environmentLight * transmissionContribution;
			break;
		}

		if (m_Settings.DebugNormal)
		{
			//Red - Right
			//Green - UP
			//Blue  Front
			incomingLight = trace.WorldNormal * 0.5f + 0.5f;
			break;
		}

		const Primitive* primitive = m_ActiveScene->Primitives[trace.HitGuid].get();
		//Surface Properties
		float opacity = primitive->Material->Opacity;
		glm::vec3 diffuseAlbedo = primitive->Material->Albedo;
		float specularAlbedo = primitive->Material->Specular;
		glm::vec3 emmisive = primitive->Material->Emmisive;// *opacity;

		//Emmisive
		incomingLight += emmisive * transmissionContribution;
		
		float fresnel = Utils::FresnelSchlick(specularAlbedo, AIR_IOR, 1.0f, ray.Direction, trace.WorldNormal);
		bool isSpecularBounce = fresnel >= Walnut::Random::Float();//assumes temporal accumulation

		glm::vec3 hitTranssmission = isSpecularBounce ? glm::vec3(fresnel, fresnel, fresnel) : diffuseAlbedo;
		transmissionContribution *= hitTranssmission;

		glm::vec3 diffuseDir = Utils::RandomHemisphereDir(trace.WorldNormal);

		glm::vec3 reflectedDir = glm::reflect(ray.Direction, trace.WorldNormal);
		float reflectedDirOpacity = isSpecularBounce * (1.0 - primitive->Material->Roughness);//Roughnes & Specular Integration
		ray.Direction = diffuseDir * (1.0f - reflectedDirOpacity) + reflectedDir * reflectedDirOpacity;//lerp

		ray.Origin = trace.WorldPosition + trace.WorldNormal * m_epsilon;//bias
	}

	return glm::vec4(incomingLight, 1.0f);
}

Trace Renderer::TraceScene(const Ray& ray)
{
	Trace best_trace = Trace();
	for (uint32_t i = 0; i < m_ActiveScene->Primitives.size(); i++)
	{
		const Primitive* primitive = m_ActiveScene->Primitives[i].get();
		//Perform Intersection Test
		Trace trace = primitive->Intersect(ray);
		if (trace.Result == TraceResult::Hit)
		{
			//Pick Closest Hit
			if (trace.HitDistance > 0.0f && trace.HitDistance < best_trace.HitDistance)
			{
				best_trace = trace;
			};
		}
	}

	if (best_trace.Result == TraceResult::Hit)
	{
		const Primitive* primitive = m_ActiveScene->Primitives[best_trace.HitGuid].get();
		return primitive->ResolveTracePayload(best_trace, ray);
	}
	else
	{
		return best_trace.Miss();
	}
}

void Renderer::CleanHistoryBuffer()
{
	memset(m_HistoryBufferData, 0, m_FrontBuffer->GetWidth() * m_FrontBuffer->GetHeight() * sizeof(glm::vec4));//clear buffer
}

void Renderer::UpdateGuids()
{
	for (int i = 0; i < m_ActiveScene->Primitives.size(); i++)
	{
		m_ActiveScene->Primitives[i]->Guid = i;
	}
}

void Renderer::UpdateTransforms()
{
	for (int i = 0; i < m_ActiveScene->Primitives.size(); i++)
	{
		m_ActiveScene->Primitives[i]->UpdateTransforms();;
	}
}
