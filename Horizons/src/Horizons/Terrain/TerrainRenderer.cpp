#include "pch.h"
#include "TerrainRenderer.h"

#include "Horizons/Core/Math.h"
#include "Horizons/Scripting/TerrainPrototypes.h"
#include "glm/gtc/matrix_transform.hpp"

TerrainRenderer::TerrainRenderer(TerrainStore* store, int render_width)
	: m_Store(store), RenderWidth(render_width)
{
	m_RenderChunks = new RenderChunk[RenderWidth * RenderWidth];

	m_Framebuffer = li::Framebuffer::Create(ChunkWidthInPixels * RenderWidth, ChunkHeightInPixels * RenderWidth);
	
	m_AtlasBoundsUB = li::UniformBuffer::Create("Terrain", 3, li::ShaderType::Fragment, {
		{ "u_AtlasBounds0", li::ShaderDataType::Float4 },
		{ "u_AtlasBounds1", li::ShaderDataType::Float4 },
		{ "u_AtlasBounds2", li::ShaderDataType::Float4 },
		{ "u_AtlasBounds3", li::ShaderDataType::Float4 },
		{ "u_NoiseWeights", li::ShaderDataType::Float3 },
		{ "u_BlendWidths", li::ShaderDataType::Float3 }
	});
	m_AtlasBoundsUB->BindToSlot();

	m_TerrainShader = li::ResourceManager::Get<li::Shader>("shader_terrain");

	struct ChunkVertex
	{
		glm::vec2 Position;
		glm::vec2 TexCoord;
	};
	ChunkVertex vertices[ChunkHeight + 1][ChunkWidth + 1];

	for (int y = 0; y <= ChunkHeight; y++)
	{
		for (int x = 0; x <= ChunkWidth; x++)
		{
			// 0,0 at bottom left.
			vertices[y][x].Position = { (float)x / (float)ChunkWidth, (float)y / (float)ChunkHeight };
			vertices[y][x].TexCoord = { (float)x / (float)ChunkWidth, (float)y / (float)ChunkHeight };
		}
	}

	li::Ref<li::VertexBuffer> positionBuffer = li::VertexBuffer::Create((float*)vertices, sizeof(vertices), li::BufferUsage::StaticDraw);
	positionBuffer->SetLayout({
		{ li::ShaderDataType::Float2, "POSITION", 0 },
		{ li::ShaderDataType::Float2, "TEXCOORD", 1 }
	});

	// Triangulate chunk.
	uint32_t indices[ChunkHeight * ChunkWidth * 6];
	for (int y = 0; y < ChunkHeight; y++)
	{
		for (int x = 0; x < ChunkWidth; x++)
		{
			int index = (x + y * ChunkWidth) * 6;
			indices[index] = y * (ChunkWidth + 1) + x;
			indices[index + 1] = indices[index] + 1;
			indices[index + 2] = indices[index] + (ChunkWidth + 1);

			indices[index + 3] = indices[index + 1];
			indices[index + 4] = indices[index + 2] + 1;
			indices[index + 5] = indices[index + 2];
		}
	}
	li::Ref<li::IndexBuffer> indexBuffer = li::IndexBuffer::Create(indices, ChunkHeight * ChunkWidth * 6, li::BufferUsage::StaticDraw);

	for (int i = 0; i < RenderWidth * RenderWidth; i++)
	{
		auto& chunk = m_RenderChunks[i];
		chunk.VertexArray = li::VertexArray::Create();

		chunk.AlphaVB = li::VertexBuffer::Create(sizeof(TerrainStore::AlphaValuesArray), li::BufferUsage::DynamicDraw);
		chunk.AlphaVB->SetLayout({
			{ li::ShaderDataType::Float3, "ALPHAVALUES", 2 }
		});

		chunk.VertexArray->AddVertexBuffer(positionBuffer);
		chunk.VertexArray->AddVertexBuffer(chunk.AlphaVB);
		chunk.VertexArray->SetIndexBuffer(indexBuffer);
		chunk.VertexArray->Finalize(m_TerrainShader);
	}

	int HalfRenderWidth = RenderWidth / 2;
	switch (li::Application::Get()->GetAPI())
	{
	case li::RendererAPI::OpenGL:
		m_TerrainCamera = li::CreateScope<li::OrthographicCamera>((float)-HalfRenderWidth, (float)HalfRenderWidth + 1.0f, (float)-HalfRenderWidth, (float)HalfRenderWidth + 1.0f);
		break;
	case li::RendererAPI::D3D11:
		m_TerrainCamera = li::CreateScope<li::OrthographicCamera>((float)-HalfRenderWidth, (float)HalfRenderWidth + 1.0f, (float)HalfRenderWidth + 1.0f, (float)-HalfRenderWidth);
		break;
	}
}

TerrainRenderer::~TerrainRenderer()
{
	delete[] m_RenderChunks;
}

bool TerrainRenderer::LoadTerrain(const std::string& path, glm::ivec2 center)
{
	if (m_Store->LoadTerrain(path, center))
	{
		m_ReloadRenderChunks = true;
		m_Center = center;
		m_PrevCenter = center;

		float HalfRenderWidth = (float)(RenderWidth / 2);
		m_QuadTransform = glm::translate(glm::mat4(1.0f), { (-HalfRenderWidth + (float)center.x) * MetersPerChunk, (-HalfRenderWidth + (float)center.y) * MetersPerChunk, 0.0f })
			* glm::scale(glm::mat4(1.0f), { MetersPerChunk * RenderWidth, MetersPerChunk * RenderWidth, 1.0f });
		return true;
	}
	else return false;
}

void TerrainRenderer::UnloadTerrain()
{
	m_Store->UnloadTerrain();
}

void TerrainRenderer::UpdateCenter(glm::ivec2 center)
{
	if (m_Center != center)
	{
		m_RenderCenterChanged = true;
		m_PrevCenter = m_Center;
		m_Center = center;

		float HalfRenderWidth = (float)(RenderWidth / 2);
		m_QuadTransform = glm::translate(glm::mat4(1.0f), { (-HalfRenderWidth + (float)center.x) * MetersPerChunk, (-HalfRenderWidth + (float)center.y) * MetersPerChunk, 0.0f })
			* glm::scale(glm::mat4(1.0f), { MetersPerChunk * RenderWidth, MetersPerChunk * RenderWidth, 1.0f });

		m_Store->UpdateCenter(center);
	}
}

void TerrainRenderer::RenderFramebuffer()
{
	PrepareRenderChunks();

	// Render terrain to framebuffer to get per pixel interpolation.
	m_Framebuffer->Bind();
	m_Framebuffer->Clear();

	for (int i = 0; i < RenderWidth * RenderWidth; i++)
	{
		auto& chunk = m_RenderChunks[i];
		const TerrainPrototype& terrain0 = TerrainPrototypes::GetTerrainPrototype(chunk.Tiles[0]);
		const TerrainPrototype& terrain1 = TerrainPrototypes::GetTerrainPrototype(chunk.Tiles[1]);
		const TerrainPrototype& terrain2 = TerrainPrototypes::GetTerrainPrototype(chunk.Tiles[2]);
		const TerrainPrototype& terrain3 = TerrainPrototypes::GetTerrainPrototype(chunk.Tiles[3]);
		li::Ref<li::TextureAtlas> atlas = li::ResourceManager::Get<li::TextureAtlas>(terrain0.Atlas);

		m_AtlasBoundsUB->SetFloat4("u_AtlasBounds0", atlas->GetBounds(terrain0.Name));
		m_AtlasBoundsUB->SetFloat4("u_AtlasBounds1", atlas->GetBounds(terrain1.Name));
		m_AtlasBoundsUB->SetFloat4("u_AtlasBounds2", atlas->GetBounds(terrain2.Name));
		m_AtlasBoundsUB->SetFloat4("u_AtlasBounds3", atlas->GetBounds(terrain3.Name));
		m_AtlasBoundsUB->SetFloat3("u_NoiseWeights", { terrain1.NoiseWeight, terrain2.NoiseWeight, terrain3.NoiseWeight });
		m_AtlasBoundsUB->SetFloat3("u_BlendWidths", { terrain1.BlendWidth,terrain2.BlendWidth,terrain3.BlendWidth });
		m_AtlasBoundsUB->UploadData();

		m_TerrainShader->Bind();

		auto& transform_ub = li::Renderer::GetTransformUniformBuffer();
		transform_ub->SetMat4("u_Transform", chunk.Transform);
		transform_ub->UploadData();

		auto& viewproj_ub = li::Renderer::GetViewProjUniformBuffer();
		viewproj_ub->SetMat4("u_ViewProj", m_TerrainCamera->GetViewProjectionMatrix());
		viewproj_ub->UploadData();

		// Bind textures.
		atlas->Bind(0);
		terrain1.NoiseTexture->Bind(1);
		terrain2.NoiseTexture->Bind(2);
		terrain3.NoiseTexture->Bind(3);

		m_TerrainShader->SetTexture("u_Texture", 0);
		m_TerrainShader->SetTexture("u_Noise1", 1);
		m_TerrainShader->SetTexture("u_Noise2", 2);
		m_TerrainShader->SetTexture("u_Noise3", 3);

		li::Renderer::GetViewProjUniformBuffer()->Bind();
		li::Renderer::GetTransformUniformBuffer()->Bind();
		m_AtlasBoundsUB->Bind();
		chunk.VertexArray->Bind();
		li::Application::Get()->GetWindow()->GetContext()->SetDrawMode(li::DrawMode::Triangles);
		li::Application::Get()->GetWindow()->GetContext()->DrawIndexed(chunk.VertexArray->GetIndexBuffer()->GetCount());
		chunk.VertexArray->Unbind();
	}
}

void TerrainRenderer::SubmitQuad()
{
	li::Renderer::Submit(m_Framebuffer->GetTexture(), m_QuadTransform);
}

void TerrainRenderer::PrepareRenderChunks()
{
	if (m_ReloadRenderChunks)
	{
		int HalfRenderWidth = RenderWidth / 2;
		for (int y = -HalfRenderWidth; y <= HalfRenderWidth; y++)
		{
			for (int x = -HalfRenderWidth; x <= HalfRenderWidth; x++)
			{
				glm::ivec2 display_coord = m_Center + glm::ivec2{ x, y };
				// Account for any overflow past world bounds.
				glm::ivec2 store_coord = {
					Math::PositiveMod(display_coord.x, m_Store->GetWorldWidth()),
					Math::PositiveMod(display_coord.y, m_Store->GetWorldHeight())
				};

				auto& chunk = m_RenderChunks[(x + HalfRenderWidth) + (y + HalfRenderWidth) * (size_t)RenderWidth];
				chunk.CenterOffset = { x, y };
				chunk.Transform = glm::translate(glm::mat4(1.0f), {
					(float)chunk.CenterOffset.x,
					(float)chunk.CenterOffset.y, 0.0f
					});
				m_Store->LoadRenderChunkData(store_coord, &chunk);
			}
		}
		m_ReloadRenderChunks = false;
	}
	else if (m_RenderCenterChanged)
	{
		// test if current chunk is within radius of center.
		int radius = RenderWidth / 2;

		int left_bound = Math::PositiveMod(m_Center.x - radius, m_Store->GetWorldWidth());
		int right_bound = Math::PositiveMod(m_Center.x + radius, m_Store->GetWorldWidth());

		int bottom_bound = Math::PositiveMod(m_Center.y - radius, m_Store->GetWorldHeight());
		int top_bound = Math::PositiveMod(m_Center.y + radius, m_Store->GetWorldHeight());

		// Remove chunks which are no longer near the center.
		for (int i = 0; i < RenderWidth * RenderWidth; i++)
		{
			auto& chunk = m_RenderChunks[i];
			bool in_x;
			bool in_y;

			if (left_bound < right_bound)
				in_x = chunk.Coord.x >= left_bound && chunk.Coord.x <= right_bound;
			else
				in_x = chunk.Coord.x >= left_bound || chunk.Coord.x <= right_bound;

			if (bottom_bound < top_bound)
				in_y = chunk.Coord.y >= bottom_bound && chunk.Coord.y <= top_bound;
			else
				in_y = chunk.Coord.y >= bottom_bound || chunk.Coord.y <= top_bound;

			// Swap the chunk if it is not in the radius of the center.
			if (!in_x || !in_y)
			{
				glm::ivec2 display_coord = m_Center - chunk.CenterOffset;
				chunk.CenterOffset = -chunk.CenterOffset;
				chunk.Transform = glm::translate(glm::mat4(1.0f), {
					chunk.CenterOffset.x,
					chunk.CenterOffset.y, 0.0f
				});

				// Account for any overflow past world bounds.
				glm::ivec2 store_coord = {
					Math::PositiveMod(display_coord.x, m_Store->GetWorldWidth()),
					Math::PositiveMod(display_coord.y, m_Store->GetWorldHeight())
				};

				// Load RenderChunk From Store
				m_Store->LoadRenderChunkData(store_coord, &chunk);
			}
			else
			{
				chunk.CenterOffset -= m_Center - m_PrevCenter;
				chunk.Transform = glm::translate(glm::mat4(1.0f), {
					chunk.CenterOffset.x,
					chunk.CenterOffset.y, 0.0f
				});
			}
		}
		m_RenderCenterChanged = false;
	}
}
