#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/TextureAtlas.h"
#include "Lithium/Renderer/OrthographicCamera.h"
#include "Lithium/Renderer/VertexArray.h"
#include "Lithium/Renderer/Buffer.h"
#include "Lithium/Renderer/Shader.h"

#include "glm/glm.hpp"

#define LI_MAX_BATCH_INSTANCES 16384

namespace li
{
	struct BatchData
	{
		glm::mat4 Transform;
		glm::vec4 AtlasBounds;
		glm::vec4 Color;
	};

	struct Batch
	{
		Batch(Ref<TextureAtlas> atlas, const glm::vec2& quadOrigin);

		Ref<TextureAtlas> Atlas;
		uint32_t InstanceCount;
		std::array<BatchData, LI_MAX_BATCH_INSTANCES> InstanceData;
		Ref<VertexBuffer> InstanceBuffer;
		Ref<VertexArray> InstanceVA;
	};

	class BatchRenderer
	{
	public:
		BatchRenderer(glm::vec2 quadOrigin = glm::vec2(1.0f));
		virtual ~BatchRenderer() = default;

		void AddTextureAtlas(Ref<TextureAtlas> atlas);
		void SetUniformBuffer(Ref<UniformBuffer> viewProjBuffer);

		void BeginScene(OrthographicCamera* camera);
		void EndScene();
		void Submit(
			const std::string& textureAlias, 
			const glm::vec4& color, 
			const glm::mat4& transform);

	private:
		std::vector<Ref<Batch>> m_Batches;
		std::unordered_map<std::string, uint32_t> m_TextureIndices;

		glm::vec2 m_QuadOrigin;
		
		Ref<Shader> m_Shader;
	};
}