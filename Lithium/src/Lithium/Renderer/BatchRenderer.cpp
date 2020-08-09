#include "lipch.h"
#include "BatchRenderer.h"
#include "RendererAPI.h"
#include "Lithium/Resources/ResourceManager.h"

#include "glm/gtc/type_ptr.hpp"

namespace li
{
	Batch::Batch(Ref<TextureAtlas> atlas, const glm::vec2& quadOrigin, const Ref<Shader>& shader)
		: Atlas(atlas), InstanceCount(0), InstanceData(), InstanceVA(VertexArray::Create())
	{
		float quadVertices[16] = {
			-quadOrigin.x		, -quadOrigin.y			, 0.0f, 1.0f,
			-quadOrigin.x + 1.0f, -quadOrigin.y			, 1.0f, 1.0f,
			-quadOrigin.x + 1.0f, -quadOrigin.y + 1.0f	, 1.0f, 0.0f,
			-quadOrigin.x		, -quadOrigin.y + 1.0f	, 0.0f, 0.0f
		};

		Ref<VertexBuffer> quadVB = VertexBuffer::Create(quadVertices, sizeof(quadVertices), BufferUsage::StaticDraw);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		quadVB->SetLayout({
			{ ShaderDataType::Float2, "POSITION", 6 },
			{ ShaderDataType::Float2, "TEXCOORD", 7 }
		});

		InstanceVA->SetIndexBuffer(quadIB);
		InstanceVA->AddVertexBuffer(quadVB);
		
		static_assert(sizeof(BatchData) == sizeof(glm::mat4) + sizeof(glm::vec4) + sizeof(glm::vec4));

		InstanceBuffer = VertexBuffer::Create(LI_MAX_BATCH_INSTANCES * sizeof(BatchData), BufferUsage::DynamicDraw);
		InstanceBuffer->SetLayout({
			{ ShaderDataType::Mat4, "I_TRANSFORM", 0, false, 1 },
			{ ShaderDataType::Float4, "I_ATLASBOUNDS", 4, false, 1 },
			{ ShaderDataType::Float4, "COLOR", 5, false, 1 }
		});
		InstanceVA->AddVertexBuffer(InstanceBuffer);

		InstanceVA->Finalize(shader);
	}

	BatchRenderer::BatchRenderer(glm::vec2 quadOrigin)
		: m_QuadOrigin(quadOrigin), m_Batches(), m_TextureIndices()
	{
	}

	void BatchRenderer::PostResourceLoad()
	{
		m_Shader = ResourceManager::Get<Shader>("shader_instance");
	}

	void BatchRenderer::AddTextureAtlas(Ref<TextureAtlas> atlas)
	{
		uint32_t index = (uint32_t)m_Batches.size();
		for (auto& [alias, bounds]: atlas->GetEntries()) {
			m_TextureIndices[alias] = index;
		}

		m_Batches.push_back(CreateRef<Batch>(atlas, m_QuadOrigin, m_Shader));
	}

	void BatchRenderer::SetUniformBuffer(Ref<UniformBuffer> viewProjBuffer)
	{
		m_Shader->AddUniformBuffer(viewProjBuffer);
	}

	void BatchRenderer::BeginScene()
	{
		for (const Ref<Batch>& batch : m_Batches)
		{
			batch->InstanceCount = 0;
		}
	}

	void BatchRenderer::EndScene()
	{
		m_Shader->Bind();
		m_Shader->SetTexture("u_Texture", 0);

		// Loop through batches and render each to framebuffer.
		for (const Ref<Batch> batch : m_Batches)
		{
			if (batch->InstanceCount > 0)
			{
				batch->Atlas->Bind();

				batch->InstanceBuffer->SetSubData(
					(float*)batch->InstanceData.data(),
					sizeof(BatchData) * batch->InstanceCount,
					0, true, BufferTarget::ArrayBuffer
				);

				batch->InstanceVA->Bind();
				li::RendererAPI::SetDrawMode(li::DrawMode::Triangles);
				RendererAPI::DrawIndexedInstanced(batch->InstanceVA->GetIndexBuffer()->GetCount(), batch->InstanceCount);
				batch->InstanceVA->Unbind();
			}
		}
	}

	void BatchRenderer::Submit(
		const std::string& textureAlias, 
		const glm::vec4& color, 
		const glm::mat4& transform)
	{
		const Ref<Batch>& batch = m_Batches[m_TextureIndices[textureAlias]];
		// TODO: flush instances and start new batch.
		LI_CORE_ASSERT(batch->InstanceCount < LI_MAX_BATCH_INSTANCES, "Too many instanced submissions for this atlas!");

		batch->InstanceData[batch->InstanceCount].Transform = transform;
		batch->InstanceData[batch->InstanceCount].AtlasBounds = batch->Atlas->GetBounds(textureAlias);
		batch->InstanceData[batch->InstanceCount].Color = color;

		batch->InstanceCount++;
	}
}