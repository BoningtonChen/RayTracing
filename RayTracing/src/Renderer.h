#pragma once

#include "Walnut/Image.h"

#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

#include <memory>

#include <glm/glm.hpp>

class Renderer
{
public:
	struct Settings
	{
		bool Accumulate = true;
		bool SlowRandom = true;
	};

	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera& camera);

	[[nodiscard]] std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

	void ResetFrameIndex() { m_FrameIndex = 1; }

	Settings& GetSettings() { return m_Settings; }

private:
	struct HitPayLoad
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};
	[[nodiscard]] glm::vec4 PerPixel(uint32_t x, uint32_t y) const;	// RayGen Shader

	[[nodiscard]] HitPayLoad TraceRay(const Ray& ray) const;
	[[nodiscard]] HitPayLoad ClosestHit(
		const Ray& ray, 
		float hitDistance, 
		int objectIndex
	) const;
	static HitPayLoad Miss(const Ray& ray);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;

	Settings m_Settings;

	std::vector<uint32_t> m_ImageHorizontalIterator, m_ImageVerticalIterator;

	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;

	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_AccumulationData = nullptr;

	uint32_t m_FrameIndex = 1;
};
