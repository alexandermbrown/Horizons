#include "pch.h"
#include "TerrainRenderer.h"

#include "Horizons.h"
#include "Horizons/Core/Math.h"
#include "glm/gtc/matrix_transform.hpp"

TerrainRenderer::TerrainRenderer(TerrainStore* store, int render_width)
	: m_Store(store), RenderWidth(render_width)
{
	m_RenderChunks = new RenderChunk[RenderWidth * RenderWidth];

	m_Framebuffer = Li::Framebuffer::Create(ChunkWidthInPixels * RenderWidth, ChunkHeightInPixels * RenderWidth);
	
	m_AtlasBoundsUB = Li::UniformBuffer::Create("Terrain", 3, Li::ShaderType::Fragment, {
		{ "u_AtlasBounds0", Li::ShaderDataType::Float4 },
		{ "u_AtlasBounds1", Li::ShaderDataType::Float4 },
		{ "u_AtlasBounds2", Li::ShaderDataType::Float4 },
		{ "u_AtlasBounds3", Li::ShaderDataType::Float4 },
		{ "u_NoiseWeights", Li::ShaderDataType::Float3 },
		{ "u_BlendWidths", Li::ShaderDataType::Float3 }
	});
	m_AtlasBoundsUB->BindToSlot();

	m_TerrainShader = Li::ResourceManager::Get<Li::Shader>("shader_terrain");

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

	Li::Ref<Li::VertexBuffer> positionBuffer = Li::VertexBuffer::Create((float*)vertices, sizeof(vertices), Li::BufferUsage::StaticDraw);
	positionBuffer->SetLayout({
		{ Li::ShaderDataType::Float2, "POSITION", 0 },
		{ Li::ShaderDataType::Float2, "TEXCOORD", 1 }
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
	Li::Ref<Li::IndexBuffer> indexBuffer = Li::IndexBuffer::Create(indices, ChunkHeight * ChunkWidth * 6, Li::BufferUsage::StaticDraw);

	for (int i = 0; i < RenderWidth * RenderWidth; i++)
	{
		auto& chunk = m_RenderChunks[i];
		chunk.VertexArray = Li::VertexArray::Create();

		chunk.AlphaVB = Li::VertexBuffer::Create(sizeof(TerrainStore::AlphaValuesArray), Li::BufferUsage::DynamicDraw);
		chunk.AlphaVB->SetLayout({
			{ Li::ShaderDataType::Float3, "ALPHAVALUES", 2 }
		});

		chunk.VertexArray->AddVertexBuffer(positionBuffer);
		chunk.VertexArray->AddVertexBuffer(chunk.AlphaVB);
		chunk.VertexArray->SetIndexBuffer(indexBuffer);
		chunk.VertexArray->Finalize(m_TerrainShader);
	}

	int HalfRenderWidth = RenderWidth / 2;
	switch (Li::Application::Get().GetAPI())
	{
	case Li::RendererAPI::OpenGL:
		m_TerrainCamera = Li::MakeUnique<Li::OrthographicCamera>((float)-HalfRenderWidth, (float)HalfRenderWidth + 1.0f, (float)-HalfRenderWidth, (float)HalfRenderWidth + 1.0f);
		break;
	case Li::RendererAPI::D3D11:
		m_TerrainCamera = Li::MakeUnique<Li::OrthographicCamera>((float)-HalfRenderWidth, (float)HalfRenderWidth + 1.0f, (float)HalfRenderWidth + 1.0f, (float)-HalfRenderWidth);
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
		const TerrainType& terrain0 = Li::Application::Get<Horizons>().GetTerrainData().GetTerrainPrototype(chunk.Tiles[0]);
		const TerrainType& terrain1 = Li::Application::Get<Horizons>().GetTerrainData().GetTerrainPrototype(chunk.Tiles[1]);
		const TerrainType& terrain2 = Li::Application::Get<Horizons>().GetTerrainData().GetTerrainPrototype(chunk.Tiles[2]);
		const TerrainType& terrain3 = Li::Application::Get<Horizons>().GetTerrainData().GetTerrainPrototype(chunk.Tiles[3]);
		Li::Ref<Li::TextureAtlas> atlas = Li::ResourceManager::Get<Li::TextureAtlas>(terrain0.Atlas);

		m_AtlasBoundsUB->SetFloat4("u_AtlasBounds0", atlas->GetBounds(terrain0.Name));
		m_AtlasBoundsUB->SetFloat4("u_AtlasBounds1", atlas->GetBounds(terrain1.Name));
		m_AtlasBoundsUB->SetFloat4("u_AtlasBounds2", atlas->GetBounds(terrain2.Name));
		m_AtlasBoundsUB->SetFloat4("u_AtlasBounds3", atlas->GetBounds(terrain3.Name));
		m_AtlasBoundsUB->SetFloat3("u_NoiseWeights", { terrain1.NoiseWeight, terrain2.NoiseWeight, terrain3.NoiseWeight });
		m_AtlasBoundsUB->SetFloat3("u_BlendWidths", { terrain1.BlendWidth,terrain2.BlendWidth,terrain3.BlendWidth });
		m_AtlasBoundsUB->UploadData();

		m_TerrainShader->Bind();

		auto& transform_ub = Li::Renderer::GetTransformUniformBuffer();
		transform_ub->SetMat4("u_Transform", chunk.Transform);
		transform_ub->UploadData();

		auto& viewproj_ub = Li::Renderer::GetViewProjUniformBuffer();
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

		Li::Renderer::GetViewProjUniformBuffer()->Bind();
		Li::Renderer::GetTransformUniformBuffer()->Bind();
		m_AtlasBoundsUB->Bind();
		chunk.VertexArray->Bind();
		Li::Application::Get().GetWindow().GetContext()->SetDrawMode(Li::DrawMode::Triangles);
		Li::Application::Get().GetWindow().GetContext()->DrawIndexed(chunk.VertexArray->GetIndexBuffer()->GetCount());
		chunk.VertexArray->Unbind();
	}

	Li::Application::Get().GetWindow().GetContext()->BindDefaultRenderTarget();
}

void TerrainRenderer::SubmitQuad()
{
	Li::Renderer::Submit(m_Framebuffer->GetTexture(), m_QuadTransform);
}

void TerrainRenderer::PrepareRenderChunks()
{
	if (m_ReloadRenderChunks || m_Store->ReloadRenderChunks())
	{
		int HalfRenderWidth = RenderWidth / 2;
		for (int y = -HalfRenderWidth; y <= HalfRenderWidth; y++)
		{
			for (int x = -HalfRenderWidth; x <= HalfRenderWidth; x++)
			{
				glm::ivec2 display_coord = m_Center + glm::ivec2{ x, y };
				// Account for any overflow past world bounds.
				glm::ivec2 world_size = m_Store->GetWorldSize();
				glm::ivec2 store_coord = {
					Math::PositiveMod(display_coord.x, world_size.x),
					Math::PositiveMod(display_coord.y, world_size.y)
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
		glm::ivec2 world_size = m_Store->GetWorldSize();
		int left_bound = Math::PositiveMod(m_Center.x - radius, world_size.x);
		int right_bound = Math::PositiveMod(m_Center.x + radius, world_size.x);

		int bottom_bound = Math::PositiveMod(m_Center.y - radius, world_size.y);
		int top_bound = Math::PositiveMod(m_Center.y + radius, world_size.y);

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
					Math::PositiveMod(display_coord.x, world_size.x),
					Math::PositiveMod(display_coord.y, world_size.y)
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
