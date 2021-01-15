#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/TextureAtlas.h"
#include "Lithium/Renderer/OrthographicCamera.h"
#include "Lithium/Renderer/VertexArray.h"
#include "Lithium/Renderer/Buffer.h"
#include "Lithium/Renderer/Shader.h"

#include "glm/glm.hpp"

namespace Li
{
	class BatchRenderer
	{
	public:
		BatchRenderer(glm::vec2 quadOrigin, const Ref<UniformBuffer>& viewProjBuffer, const Ref<UniformBuffer>& transformBuffer);
		virtual ~BatchRenderer();

		void AddTextureAtlas(Ref<TextureAtlas> atlas);

		void BeginScene();
		inline void EndScene() { Flush(); }

		void Submit(
			const std::string& texture_alias,
			const glm::vec4& color,
			const glm::mat4& transform,
			bool crop);

	private:
		void Flush();

		struct BatchVertex
		{
			glm::vec3 Position;
			float TexIndex;
			glm::vec2 TexCoord;
			glm::vec4 Color;
		};

		static constexpr int MaxBatchQuads = 16384;
		static constexpr int MaxBatchTextures = 8;
		
		std::unordered_map<std::string, Ref<TextureAtlas>> m_Atlases;

		Ref<Shader> m_Shader;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexArray> m_VertexArray;

		Ref<UniformBuffer> m_ViewProjUB;
		Ref<UniformBuffer> m_TransformUB;

		int m_QuadCount;
		int m_BatchAtlasCount;

		BatchVertex* m_Vertices;
		std::array<TextureAtlas*, MaxBatchTextures> m_BatchAtlases;

		glm::vec4 m_VertexPositions[4];
		constexpr static glm::vec2 m_TexCoords[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};
	};
}
