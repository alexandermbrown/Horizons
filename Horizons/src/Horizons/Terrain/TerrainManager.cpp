#include "pch.h"
#include "TerrainManager.h"

#include "glm/gtc/matrix_transform.hpp"

li::Scope<TerrainManager::TerrainData> TerrainManager::s_Data;

static int positive_mod(int a, int b)
{
	return ((a % b) + b) % b;
}

void TerrainManager::Init()
{
	LI_ASSERT(s_Data == nullptr, "Init already called!");
	s_Data = li::CreateScope<TerrainData>();

	s_Data->Framebuffer = li::Framebuffer::Create(ChunkWidthInPixels * RenderWidth, ChunkHeightInPixels * RenderWidth);

	s_Data->AtlasBoundsUB = li::UniformBuffer::Create("AtlasBounds", 3, li::ShaderType::Fragment, {
		{ "u_AtlasBounds0", li::ShaderDataType::Float4 },
		{ "u_AtlasBounds1", li::ShaderDataType::Float4 },
		{ "u_AtlasBounds2", li::ShaderDataType::Float4 },
		{ "u_AtlasBounds3", li::ShaderDataType::Float4 }
		});
	s_Data->AtlasBoundsUB->BindToSlot();

	s_Data->TerrainShader = li::ResourceManager::Get<li::Shader>("shader_terrain");
	s_Data->TerrainShader->AddUniformBuffer(li::Renderer::GetViewProjUniformBuffer());
	s_Data->TerrainShader->AddUniformBuffer(li::Renderer::GetTransformUniformBuffer());
	s_Data->TerrainShader->AddUniformBuffer(s_Data->AtlasBoundsUB);

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

	for (int c = 0; c < s_Data->RenderChunks.size(); c++)
	{
		auto& chunk = s_Data->RenderChunks[c];
		chunk.VertexArray = li::VertexArray::Create();

		chunk.AlphaVB = li::VertexBuffer::Create(sizeof(StoreChunk::AlphaValues), li::BufferUsage::DynamicDraw);
		chunk.AlphaVB->SetLayout({
			{ li::ShaderDataType::Float3, "ALPHAVALUES", 2 }
		});

		chunk.VertexArray->AddVertexBuffer(positionBuffer);
		chunk.VertexArray->AddVertexBuffer(chunk.AlphaVB);
		chunk.VertexArray->SetIndexBuffer(indexBuffer);
		chunk.VertexArray->Finalize(s_Data->TerrainShader);
	}

	constexpr int HalfRenderWidth = RenderWidth / 2;

	switch (li::RendererAPI::GetAPI())
	{
	case li::RendererAPI::API::OpenGL:
		s_Data->TerrainCamera = li::CreateScope<li::OrthographicCamera>((float)-HalfRenderWidth, (float)HalfRenderWidth + 1.0f, (float)-HalfRenderWidth, (float)HalfRenderWidth + 1.0f);
		break;
	case li::RendererAPI::API::D3D11:
		s_Data->TerrainCamera = li::CreateScope<li::OrthographicCamera>((float)-HalfRenderWidth, (float)HalfRenderWidth + 1.0f, (float)HalfRenderWidth + 1.0f, (float)-HalfRenderWidth);
		break;
	}
}

void TerrainManager::Shutdown()
{
	s_Data.reset();
}

void TerrainManager::LoadWorld(const std::string& path, glm::ivec2 center)
{
	s_Data->ReloadRenderChunks = true;

	// Load terrain file.
	s_Data->TerrainPath = path;

	s_Data->TerrainFile.open(path, std::ios::in | std::ios::binary);

	if (!s_Data->TerrainFile.is_open())
	{
		LI_ERROR("Failed to open file {}", path);
		return;
	}

	uint16_t width, height;
	s_Data->TerrainFile.read((char*)&width, sizeof(width));
	s_Data->TerrainFile.read((char*)&height, sizeof(height));

	s_Data->Width = width;
	s_Data->Height = height;

	LI_INFO("Opening world at {}, size is ({},{})", path, width, height);

	LoadInitialChunks(center);

	s_Data->QuadTransform = glm::translate(glm::mat4(1.0f), { (-1.0f + (float)center.x) * MetersPerChunk, (-1.0f + (float)center.y) * MetersPerChunk, 0.0f })
		* glm::scale(glm::mat4(1.0f), { MetersPerChunk * RenderWidth, MetersPerChunk * RenderWidth, 1.0f });

	// Start load thread.
	s_Data->RunLoadThread = true;
	s_Data->LoadThread = std::thread(LoadThreadEntryPoint, center);
}

void TerrainManager::UnloadWorld()
{
	s_Data->RunLoadThread = false;
	s_Data->LoadThread.join();
	s_Data->TerrainFile.close();
}

void TerrainManager::UpdateCenter(glm::ivec2 center)
{
	// Note: s_Data->Center can only be written by this function.
	// The mutex is to ensure LoadThread does not read while this function is writing.
	// s_Data->PrevCenter can only be read by the main render thread (ie. not the loading thread).
	if (s_Data->Center != center)
	{
		s_Data->RenderCenterChanged = true;
		s_Data->PrevCenter = s_Data->Center;

		s_Data->QuadTransform = glm::translate(glm::mat4(1.0f), { (-1.0f + (float)center.x) * MetersPerChunk, (-1.0f + (float)center.y) * MetersPerChunk, 0.0f })
			* glm::scale(glm::mat4(1.0f), { MetersPerChunk * RenderWidth, MetersPerChunk * RenderWidth, 1.0f });

		std::lock_guard<std::mutex> lock(s_Data->CenterMutex);
		s_Data->Center = center;
	}
}

void TerrainManager::RenderFramebuffer()
{
	if (s_Data->ReloadRenderChunks)
	{
		constexpr int HalfRenderWidth = RenderWidth / 2;
		for (int y = -HalfRenderWidth; y <= HalfRenderWidth; y++)
		{
			for (int x = -HalfRenderWidth; x <= HalfRenderWidth; x++)
			{
				glm::ivec2 display_coord = s_Data->Center + glm::ivec2{ x, y };
				// Account for any overflow past world bounds.
				glm::ivec2 store_coord = glm::ivec2{
					positive_mod(display_coord.x, s_Data->Width),
					positive_mod(display_coord.y, s_Data->Height)
				};

				auto& chunk = s_Data->RenderChunks[(x + HalfRenderWidth) + (y + HalfRenderWidth) * (size_t)RenderWidth];
				chunk.CenterOffset = { x, y };
				chunk.Transform = glm::translate(glm::mat4(1.0f), {
					(float)chunk.CenterOffset.x,
					(float)chunk.CenterOffset.y, 0.0f
				});
				LoadChunkFromStore(store_coord, &chunk);
			}
		}
		s_Data->ReloadRenderChunks = false;
	}
	else if (s_Data->RenderCenterChanged)
	{
		// test if current chunk is within radius of center.
		constexpr int radius = RenderWidth / 2;

		int left_bound = positive_mod(s_Data->Center.x - radius, s_Data->Width);
		int right_bound = positive_mod(s_Data->Center.x + radius, s_Data->Width);

		int bottom_bound = positive_mod(s_Data->Center.y - radius, s_Data->Height);
		int top_bound = positive_mod(s_Data->Center.y + radius, s_Data->Height);

		// Remove chunks which are no longer near the center.
		for (auto& chunk : s_Data->RenderChunks)
		{
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
				glm::ivec2 display_coord = s_Data->Center - chunk.CenterOffset;
				chunk.CenterOffset = -chunk.CenterOffset;
				chunk.Transform = glm::translate(glm::mat4(1.0f), {
					chunk.CenterOffset.x,
					chunk.CenterOffset.y, 0.0f
				});

				// Account for any overflow past world bounds.
				glm::ivec2 store_coord = {
					positive_mod(display_coord.x, s_Data->Width),
					positive_mod(display_coord.y, s_Data->Height)
				};

				// Load RenderChunk From Store
				LoadChunkFromStore(store_coord, &chunk);
			}
			else
			{
				chunk.CenterOffset -= s_Data->Center - s_Data->PrevCenter;
				chunk.Transform = glm::translate(glm::mat4(1.0f), {
					chunk.CenterOffset.x,
					chunk.CenterOffset.y, 0.0f
				});
			}
		}
		s_Data->RenderCenterChanged = false;
	}

	// Render terrain to framebuffer to get per pixel interpolation.
	s_Data->Framebuffer->Bind();
	s_Data->Framebuffer->SetClearColor({ 0.1f, 0.0f, 0.1f, 1.0f });
	s_Data->Framebuffer->Clear();
	li::RendererAPI::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	// TODO: setup textures.
	li::Ref<li::Texture> texture = li::ResourceManager::Get<li::Texture2D>("texture_terrain_test");
	s_Data->AtlasBoundsUB->SetFloat4("u_AtlasBounds0", { 0.0f, 0.0f, 0.5f, 0.25f });
	s_Data->AtlasBoundsUB->SetFloat4("u_AtlasBounds1", { 0.5f, 0.0f, 0.5f, 0.25f });
	s_Data->AtlasBoundsUB->SetFloat4("u_AtlasBounds2", { 0.0f, 0.25f, 0.5f, 0.25f });
	s_Data->AtlasBoundsUB->SetFloat4("u_AtlasBounds3", { 0.5f, 0.25f, 0.5f, 0.25f });
	s_Data->AtlasBoundsUB->UploadData();

	for (auto& chunk : s_Data->RenderChunks)
	{
		s_Data->TerrainShader->Bind();

		auto& transform_ub = li::Renderer::GetTransformUniformBuffer();
		transform_ub->SetMat4("u_Transform", chunk.Transform);
		transform_ub->UploadData();

		auto& viewproj_ub = li::Renderer::GetViewProjUniformBuffer();
		viewproj_ub->SetMat4("u_ViewProj", s_Data->TerrainCamera->GetViewProjectionMatrix());
		viewproj_ub->UploadData();

		s_Data->TerrainShader->SetTexture("u_Texture", 0);
		texture->Bind();
		chunk.VertexArray->Bind();
		li::RendererAPI::SetDrawMode(li::DrawMode::Triangles);
		li::RendererAPI::DrawIndexed(chunk.VertexArray->GetIndexBuffer()->GetCount());
		chunk.VertexArray->Unbind();
	}
}

void TerrainManager::RenderQuad()
{
	// Render the framebuffer's texture to the window.
	li::Renderer::Submit(s_Data->Framebuffer->GetTexture(), s_Data->QuadTransform);
}

void TerrainManager::LoadInitialChunks(glm::ivec2 center)
{
	s_Data->Center = center;

	constexpr int HalfStoreWidth = StoreWidth / 2;
	for (int y = -HalfStoreWidth; y <= HalfStoreWidth; y++)
	{
		for (int x = -HalfStoreWidth; x <= HalfStoreWidth; x++)
		{
			glm::ivec2 coord = center + glm::ivec2{ x, y };
			// Account for any overflow past world bounds.
			coord = glm::ivec2{
				positive_mod(coord.x, s_Data->Width),
				positive_mod(coord.y, s_Data->Height)
			};
			auto& chunk = s_Data->StoreChunks[(x + HalfStoreWidth) + (y + HalfStoreWidth) * (size_t)StoreWidth];
			chunk.CenterOffset = { x, y };
			LoadChunkFromDisk(coord, &chunk);
		}
	}
}

void TerrainManager::LoadThreadEntryPoint(glm::ivec2 first_center)
{
	glm::ivec2 saved_center = first_center;
	while (s_Data->RunLoadThread.load())
	{
		glm::ivec2 center;
		{
			std::lock_guard<std::mutex> lock(s_Data->CenterMutex);
			center = s_Data->Center;
		}

		if (saved_center != center)
		{
			// test if current chunk is within radius of center.
			constexpr int radius = StoreWidth / 2;

			int left_bound = positive_mod(center.x - radius, s_Data->Width);
			int right_bound = positive_mod(center.x + radius, s_Data->Width);

			int bottom_bound = positive_mod(center.y - radius, s_Data->Height);
			int top_bound = positive_mod(center.y + radius, s_Data->Height);

			// Remove chunks which are no longer near the center.
			for (auto& chunk : s_Data->StoreChunks)
			{
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
					glm::ivec2 display_coord = center - chunk.CenterOffset;
					chunk.CenterOffset = -chunk.CenterOffset;

					// Account for any overflow past world bounds.
					glm::ivec2 store_coord = glm::ivec2{
						positive_mod(display_coord.x, s_Data->Width),
						positive_mod(display_coord.y, s_Data->Height)
					};

					LoadChunkFromDisk(store_coord, &chunk);
				}
				else
				{
					chunk.CenterOffset -= center - saved_center;
				}
			}

			saved_center = center;
		}
	}
}

void TerrainManager::LoadChunkFromDisk(glm::ivec2 coord, StoreChunk* destination)
{
	std::lock_guard<std::mutex> lock(destination->Mutex);

	destination->Coord = coord;

	s_Data->TerrainFile.seekg(sizeof(TerrainFileHeader) 
		+ (size_t)coord.x * sizeof(TerrainFileChunk) 
		+ (size_t)coord.y * (size_t)s_Data->Width * sizeof(TerrainFileChunk), s_Data->TerrainFile.beg);

	TerrainFileChunk temp_chunk;
	s_Data->TerrainFile.read((char*)temp_chunk.Tiles, sizeof(temp_chunk.Tiles));

	// Load data in main (bottom left) chunk.
	s_Data->TerrainFile.read((char*)temp_chunk.AlphaValues, sizeof(temp_chunk.AlphaValues));

	for (int y = 0; y < ChunkHeight; y++)
	{
		for (int x = 0; x < ChunkWidth; x++)
		{
			for (int i = 0; i < NumTilesPerChunk - 1; i++)
			{
				destination->AlphaValues[y][x][i] = (float)temp_chunk.AlphaValues[y][x][i] / 255.0f;
			}
		}
	}

	int right_x = (coord.x + 1) % s_Data->Width;
	int top_y = (coord.y + 1) % s_Data->Height;

	// Load data right.
	{
		for (int y = 0; y < ChunkHeight; y++)
		{
			s_Data->TerrainFile.seekg(sizeof(TerrainFileHeader)
				+ (size_t)right_x * sizeof(TerrainFileChunk)
				+ (size_t)coord.y * (size_t)s_Data->Width * sizeof(TerrainFileChunk)
				+ sizeof(TerrainFileChunk::Tiles)
				+ ((size_t)y * ChunkWidth * (NumTilesPerChunk - 1))
				, s_Data->TerrainFile.beg);

			uint8_t bytes[NumTilesPerChunk - 1];
			s_Data->TerrainFile.read((char*)&bytes, sizeof(bytes));

			for (int i = 0; i < NumTilesPerChunk - 1; i++)
			{
				destination->AlphaValues[y][ChunkWidth][i] = (float)bytes[i] / 255.0f;
			}
		}
	}

	// Load data top.
	{
		s_Data->TerrainFile.seekg(sizeof(TerrainFileHeader)
			+ (size_t)coord.x * sizeof(TerrainFileChunk)
			+ (size_t)top_y * (size_t)s_Data->Width * sizeof(TerrainFileChunk)
			+ sizeof(TerrainFileChunk::Tiles), s_Data->TerrainFile.beg);

		uint8_t row[ChunkWidth][NumTilesPerChunk - 1];
		s_Data->TerrainFile.read((char*)row, sizeof(row));

		for (int x = 0; x < ChunkWidth; x++)
		{
			for (int i = 0; i < NumTilesPerChunk - 1; i++)
			{
				destination->AlphaValues[ChunkHeight][x][i] = (float)row[x][i] / 255.0f;
			}
		}
	}

	// Load data top right.
	{
		s_Data->TerrainFile.seekg(sizeof(TerrainFileHeader)
			+ (size_t)right_x * sizeof(TerrainFileChunk)
			+ (size_t)top_y * (size_t)s_Data->Width * sizeof(TerrainFileChunk)
			+ sizeof(TerrainFileChunk::Tiles), s_Data->TerrainFile.beg);

		uint8_t bytes[3];
		s_Data->TerrainFile.read((char*)&bytes, sizeof(bytes));

		for (int i = 0; i < NumTilesPerChunk - 1; i++)
		{
			destination->AlphaValues[ChunkHeight][ChunkWidth][i] = (float)bytes[i] / 255.0f;
		}
	}
}

void TerrainManager::LoadChunkFromStore(glm::ivec2 store_coord, RenderChunk* destination)
{
	destination->Coord = store_coord;
	for (auto& store_chunk : s_Data->StoreChunks)
	{
		if (store_chunk.Mutex.try_lock())
		{
			std::lock_guard<std::mutex> lock(store_chunk.Mutex, std::adopt_lock);
			if (store_chunk.Coord == store_coord)
			{
				destination->AlphaVB->SetSubData((float*)store_chunk.AlphaValues, sizeof(store_chunk.AlphaValues), 0, true);
				return;
			}
		}
	}

	// Fill the alpha buffer with 0 because the chunk has not been loaded into storage yet.
	{
		LI_WARN("Chunk ({}, {}) not loaded in store.", store_coord.x, store_coord.y);

		float alpha_values[ChunkHeight + 1][ChunkWidth + 1][NumTilesPerChunk - 1] = { 0.0f };
		destination->AlphaVB->SetSubData((float*)alpha_values, sizeof(alpha_values), 0, true);
	}
}
