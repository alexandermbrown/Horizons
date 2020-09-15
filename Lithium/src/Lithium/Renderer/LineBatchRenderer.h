#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/OrthographicCamera.h"
#include "Lithium/Renderer/VertexArray.h"
#include "Lithium/Renderer/Buffer.h"
#include "Lithium/Renderer/Shader.h"

#include "glm/glm.hpp"

namespace li
{
	class LineBatchRenderer
	{
	public:
		LineBatchRenderer(const Ref<UniformBuffer>& viewProjBuffer);
		virtual ~LineBatchRenderer() = default;

		void BeginScene();
		void EndScene();
		void Submit(const glm::vec4& color, const glm::vec3& point1, const glm::vec3& point2);

	private:
		void Flush();
		void AddVertex(const glm::vec4& color, const glm::vec3& point);

		struct LineVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		static constexpr int MaxBatchVertices = 16384;

		Ref<Shader> m_Shader;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<VertexArray> m_VertexArray;
		Ref<UniformBuffer> m_ViewProjUB;

		uint32_t m_VertexCount;
		int m_BatchAtlasCount;

		std::array<LineVertex, MaxBatchVertices> m_VertexData;
	};
}