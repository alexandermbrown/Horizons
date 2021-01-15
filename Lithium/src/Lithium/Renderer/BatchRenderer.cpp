#include "lipch.h"
#include "BatchRenderer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Resources/ResourceManager.h"

#include "glm/gtc/type_ptr.hpp"

namespace Li
{
	BatchRenderer::BatchRenderer(glm::vec2 quadOrigin, const Ref<UniformBuffer>& viewProjBuffer, const Ref<UniformBuffer>& transformBuffer)
		: m_QuadCount(0), m_VertexArray(VertexArray::Create()),
		m_ViewProjUB(viewProjBuffer), m_TransformUB(transformBuffer), m_Vertices(nullptr)
	{
		static_assert(sizeof(BatchVertex) == sizeof(BatchVertex::Position) + sizeof(BatchVertex::TexCoord) + sizeof(BatchVertex::Color) + sizeof(BatchVertex::TexIndex));
		static_assert(sizeof(BatchVertex[MaxBatchQuads * 4]) == sizeof(BatchVertex) * 4 * MaxBatchQuads);

		m_Shader = ResourceManager::Get<Shader>("shader_batched");

		m_VertexPositions[0] = { -quadOrigin.x       , -quadOrigin.y       , 0.0f, 1.0f };
		m_VertexPositions[1] = { -quadOrigin.x + 1.0f, -quadOrigin.y       , 0.0f, 1.0f };
		m_VertexPositions[2] = { -quadOrigin.x + 1.0f, -quadOrigin.y + 1.0f, 0.0f, 1.0f };
		m_VertexPositions[3] = { -quadOrigin.x       , -quadOrigin.y + 1.0f, 0.0f, 1.0f };

		std::vector<uint32_t> indices;
		indices.resize(MaxBatchQuads * 6);

		int offset = 0;
		for (int index = 0; index < MaxBatchQuads * 6; index += 6)
		{
			indices[index]     = offset + 0;
			indices[index + 1] = offset + 1;
			indices[index + 2] = offset + 2;
			indices[index + 3] = offset + 0;
			indices[index + 4] = offset + 2;
			indices[index + 5] = offset + 3;
			offset += 4;
		}
		m_IndexBuffer = IndexBuffer::Create(indices.data(), MaxBatchQuads * 6, BufferUsage::StaticDraw);

		m_Vertices = new BatchVertex[MaxBatchQuads * 4];
		m_VertexBuffer = VertexBuffer::Create(sizeof(BatchVertex[MaxBatchQuads * 4]), BufferUsage::DynamicDraw);

		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "POSITION", 0 },
			{ ShaderDataType::Float, "TEXINDEX", 1 },
			{ ShaderDataType::Float2, "TEXCOORD", 2 },
			{ ShaderDataType::Float4, "COLOR", 3 }
		});

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->Finalize(m_Shader);
	}

	BatchRenderer::~BatchRenderer()
	{
		if (m_Vertices)
		{
			delete[] m_Vertices;
			m_Vertices = nullptr;
		}
	}

	void BatchRenderer::AddTextureAtlas(Ref<TextureAtlas> atlas)
	{
		for (auto&& [texture, bounds] : atlas->GetEntries())
		{
			m_Atlases[texture] = atlas;
		}
	}

	void BatchRenderer::BeginScene()
	{
		m_QuadCount = 0;
		m_BatchAtlasCount = 0;
	}

	void BatchRenderer::Submit(
		const std::string& texture_name,
		const glm::vec4& color,
		const glm::mat4& transform,
		bool crop)
	{
		if (m_QuadCount >= MaxBatchQuads)
			Flush();

		auto it = m_Atlases.find(texture_name);
		if (it == m_Atlases.end())
		{
			LI_CORE_ERROR("Texture {} not found.", texture_name);
			return;
		}

		float tex_index;
		bool found = false;
		TextureAtlas* target_atlas = it->second.get();
		for (int i = m_BatchAtlasCount - 1; i >= 0; --i)
		{
			TextureAtlas* batch_atlas = m_BatchAtlases[i];
			if (batch_atlas == target_atlas)
			{
				found = true;
				tex_index = (float)i;
				break;
			}
		}

		if (!found)
		{
			if (m_BatchAtlasCount >= m_BatchAtlases.size())
				Flush();

			m_BatchAtlases[m_BatchAtlasCount] = target_atlas;
			tex_index = static_cast<float>(m_BatchAtlasCount);
			m_BatchAtlasCount++;
		}
		
		glm::vec4 bounds = target_atlas->GetBounds(texture_name);
		if (crop)
		{
			float quad_width = transform[0][0];
			float quad_height = transform[1][1];

			float quad_ratio = quad_width / quad_height;
			float texture_ratio = bounds.z / bounds.w;

			glm::vec2 texture_scale;
			if (quad_ratio > texture_ratio)
				texture_scale = { 1.0f, texture_ratio / quad_ratio };
			else
				texture_scale = { quad_ratio / texture_ratio, 1.0f };

			bounds = {
				bounds.x + bounds.z / 2.0f * (1.0f - texture_scale.x),
				bounds.y + bounds.w / 2.0f * (1.0f - texture_scale.y),
				bounds.z * texture_scale.x,
				bounds.w * texture_scale.y
			};
		}

		// Loop through each quad vertice.
		for (int i = 0; i < 4; i++)
		{
			BatchVertex& vertex = m_Vertices[m_QuadCount * 4 + i];

			vertex.Position = transform * m_VertexPositions[i];
			vertex.TexCoord.x = bounds.x + bounds.z * m_TexCoords[i].x;
			vertex.TexCoord.y = bounds.y + bounds.w * m_TexCoords[i].y;
			vertex.Color = color;
			vertex.TexIndex = tex_index;
		}

		m_QuadCount++;
	}

	void BatchRenderer::Flush()
	{
		if (m_QuadCount > 0)
		{
			m_Shader->Bind();

			// Bind all batch atlas textures.
			for (int slot = 0; slot < m_BatchAtlasCount; slot++)
			{
				m_BatchAtlases[slot]->Bind(slot);
			}

			m_VertexBuffer->SetSubData(
				reinterpret_cast<float*>(m_Vertices),
				sizeof(BatchVertex) * m_QuadCount * 4,
				0, true
			);

			m_ViewProjUB->Bind();
			m_TransformUB->Bind();
			m_VertexArray->Bind();
			Application::Get().GetWindow().GetContext()->SetDrawMode(Li::DrawMode::Triangles);
			Application::Get().GetWindow().GetContext()->DrawIndexed(m_QuadCount * 6);
			m_VertexArray->Unbind();
		}
		m_QuadCount = 0;
		m_BatchAtlasCount = 0;
	}
}
