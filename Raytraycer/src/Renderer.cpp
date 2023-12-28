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

	if (m_ActiveScene->Primitives.size() == 0.0)
		return;

	UpdateGuids();

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

	if (m_Settings.Accumulate)
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

	glm::vec3 light_sum = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 hit_contribution = glm::vec3(1.0f, 1.0f, 1.0f);

	int bounces = 3;
	for (int i = 0; i < bounces; i++)
	{
		Trace trace = TraceScene(ray);

		if (trace.Result == TraceResult::Miss)//Sky hit
		{
			light_sum += SampleFallback(ray) * hit_contribution;
			break;
		}

		const Primitive* primitive = m_ActiveScene->Primitives[trace.PrimitiveId].get();
		light_sum += primitive->Material->Emmisive * hit_contribution;
		hit_contribution *= primitive->Material->Albedo;

		//Evaluate Directional Lights
		for (int i = 0; i < m_ActiveScene->Lights.size(); i++)
		{
			const Light* light = m_ActiveScene->Lights[i].get();
			glm::vec3 lightDir = glm::normalize(light->Direction);

			Ray directSunRay;
			directSunRay.Origin = trace.WorldPosition + (lightDir * 0.01f);
			directSunRay.Direction = lightDir;

			Trace sunRayPayload = TraceScene(directSunRay);

			if (sunRayPayload.Result == TraceResult::Miss)//If sun rays doesnt hit anything
			{
				light_sum += light->CalculateShading(trace.WorldNormal, hit_contribution);
			}
		}

		//Set New Ray properties for next bounce
		ray.Origin = trace.WorldPosition + trace.WorldNormal * 0.01f;//biased
		glm::vec3 cone = Walnut::Random::Vec3(-0.5f, 0.5f) * primitive->Material->Roughness;
		ray.Direction = glm::reflect(ray.Direction, trace.WorldNormal + cone);
	}

	return glm::vec4(light_sum, 1.0f);
}

Trace Renderer::TraceScene(const Ray& ray)
{
	Trace best_trace = Trace();
	for (uint32_t i = 0; i < m_ActiveScene->Primitives.size(); i++)
	{
		const Primitive* primitive = m_ActiveScene->Primitives[i].get();
		Trace trace = primitive->TraceRay(ray);
		if (trace.Result == TraceResult::Hit)
		{
			if (trace.HitDistance > 0.0f && trace.HitDistance < best_trace.HitDistance)
			{
				best_trace = trace;
			};
		}
	}

	if (best_trace.Result == TraceResult::Hit)
	{
		const Primitive* primitive = m_ActiveScene->Primitives[best_trace.PrimitiveId].get();
		return primitive->ResolveTracePayload(best_trace, ray);
	}
	else
	{
		return best_trace.Miss();
	}
}

glm::vec3 Renderer::SampleFallback(const Ray& ray)
{
	glm::vec3 sky = glm::vec3(0.7f, 0.8f, 1.0f);
	glm::vec3 ground_albedo = glm::vec3(0.31f, 0.31f, 0.31f);

	//Compute ground shading assuming it's point up and is only lit with directional light
	glm::vec3 ground = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < m_ActiveScene->Lights.size(); i++)
	{
		const Light* light = m_ActiveScene->Lights[i].get();
		ground += light->CalculateShading(glm::vec3(0.0f, 1.0f, 0.0f), ground_albedo);
	}

	float sky_opacity = ray.Direction.y * 0.5f + 0.5f;
	return ground * (1.0f - sky_opacity) + sky * sky_opacity;//lerp
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
