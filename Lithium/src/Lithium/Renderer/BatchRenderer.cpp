#include "lipch.h"
#include "BatchRenderer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

#include "glm/gtc/type_ptr.hpp"

namespace li
{
	BatchRenderer::BatchRenderer(glm::vec2 quadOrigin, const Ref<UniformBuffer>& viewProjBuffer, const Ref<UniformBuffer>& transformBuffer)
		: m_QuadOrigin(quadOrigin), m_AtlasIndices(), m_InstanceCount(0), m_InstanceData(),
		m_ViewProjUB(viewProjBuffer), m_TransformUB(transformBuffer)
	{
		m_Shader = ResourceManager::Get<Shader>("shader_instance");

		m_InstanceVA = VertexArray::Create();

		float quadVertices[16] = {
			-quadOrigin.x		, -quadOrigin.y			, 0.0f, 0.0f,
			-quadOrigin.x + 1.0f, -quadOrigin.y			, 1.0f, 0.0f,
			-quadOrigin.x + 1.0f, -quadOrigin.y + 1.0f	, 1.0f, 1.0f,
			-quadOrigin.x		, -quadOrigin.y + 1.0f	, 0.0f, 1.0f
		};

		Ref<VertexBuffer> quadVB = VertexBuffer::Create(quadVertices, sizeof(quadVertices), BufferUsage::StaticDraw);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		quadVB->SetLayout({
			{ ShaderDataType::Float2, "POSITION", 7 },
			{ ShaderDataType::Float2, "TEXCOORD", 8 }
			});

		m_InstanceVA->SetIndexBuffer(quadIB);
		m_InstanceVA->AddVertexBuffer(quadVB);

		static_assert(sizeof(BatchData) == sizeof(glm::mat4) + sizeof(glm::vec4) + sizeof(glm::vec4) + sizeof(float));
		static_assert(sizeof(m_InstanceData) == sizeof(BatchData) * MaxBatchInstances);
		
		m_InstanceBuffer = VertexBuffer::Create(sizeof(m_InstanceData), BufferUsage::DynamicDraw);
		m_InstanceBuffer->SetLayout({
			{ ShaderDataType::Mat4, "I_TRANSFORM", 0, false, 1 },
			{ ShaderDataType::Float4, "I_ATLASBOUNDS", 4, false, 1 },
			{ ShaderDataType::Float4, "COLOR", 5, false, 1 },
			{ ShaderDataType::Float, "I_TEXINDEX", 6, false, 1 }
			});
		m_InstanceVA->AddVertexBuffer(m_InstanceBuffer);

		m_InstanceVA->Finalize(m_Shader);
	}

	void BatchRenderer::AddTextureAtlas(Ref<TextureAtlas> atlas)
	{
		for (auto& [texture, bounds] : atlas->GetEntries())
		{
			m_AtlasIndices[texture] = (int)m_Atlases.size();
		}
		m_Atlases.push_back(atlas);
	}

	void BatchRenderer::BeginScene()
	{
		m_InstanceCount = 0;
		m_BatchAtlasCount = 0;
	}

	void BatchRenderer::EndScene()
	{
		Flush();
	}

	void BatchRenderer::Submit(
		const std::string& texture_alias,
		const glm::vec4& color,
		const glm::mat4& transform,
		bool crop)
	{
		if (m_InstanceCount >= MaxBatchInstances)
			Flush();

		BatchData& instance = m_InstanceData[m_InstanceCount];

		Ref<TextureAtlas> search_atlas = nullptr;
		const Ref<TextureAtlas>& saved_atlas = m_Atlases[m_AtlasIndices[texture_alias]];
		for (int i = 0; i < m_BatchAtlasCount; i++)
		{
			const auto& batch_atlas = m_BatchAtlases[i];
			if (batch_atlas == saved_atlas)
			{
				search_atlas = batch_atlas;
				instance.TextureIndex = (float)i;
			}
		}

		if (search_atlas == nullptr)
		{
			if (m_BatchAtlasCount >= m_BatchAtlases.size())
				Flush();

			m_BatchAtlases[m_BatchAtlasCount] = saved_atlas;
			instance.TextureIndex = (float)m_BatchAtlasCount;
			m_BatchAtlasCount++;
		}
		
		instance.Transform = transform;
		const glm::vec4& bounds = saved_atlas->GetBounds(texture_alias);
		if (crop)
		{
			float quad_width = transform[0][0];
			float quad_height = transform[1][1];

			float quad_ratio = quad_width / quad_height;
			float texture_ratio = bounds.z / bounds.w;

			glm::vec2 texture_scale;
			if (quad_ratio > texture_ratio)
			{
				texture_scale = { 1.0f, texture_ratio / quad_ratio };
			}
			else
			{
				texture_scale = { quad_ratio / texture_ratio, 1.0f };
			}

			instance.AtlasBounds = {
				bounds.x + bounds.z / 2.0f * (1.0f - texture_scale.x),
				bounds.y + bounds.w / 2.0f * (1.0f - texture_scale.y),
				bounds.z * texture_scale.x,
				bounds.w * texture_scale.y
			};
		}
		else
		{
			instance.AtlasBounds = bounds;
		}
		instance.Color = color;

		m_InstanceCount++;
	}

	void BatchRenderer::Flush()
	{
		m_Shader->Bind();
		m_Shader->SetTexture("u_Texture", 0);

		if (m_InstanceCount > 0)
		{
			// Bind all batch atlas textures.
			for (int slot = 0; slot < m_BatchAtlasCount; slot++)
			{
				m_BatchAtlases[slot]->Bind(slot);
			}

			m_InstanceBuffer->SetSubData(
				(float*)m_InstanceData.data(),
				sizeof(BatchData) * m_InstanceCount,
				0, true
			);

			m_ViewProjUB->Bind();
			m_TransformUB->Bind();
			m_InstanceVA->Bind();
			Application::Get()->GetWindow()->GetContext()->SetDrawMode(li::DrawMode::Triangles);
			Application::Get()->GetWindow()->GetContext()->DrawIndexedInstanced(m_InstanceVA->GetIndexBuffer()->GetCount(), m_InstanceCount);
			m_InstanceVA->Unbind();
		}
		m_InstanceCount = 0;
		m_BatchAtlasCount = 0;
	}
}