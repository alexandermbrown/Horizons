#include "lipch.h"
#include "Font.h"

#include "Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

#include "hb-ot.h"

namespace Li
{
	Font::Font(const std::string& name, const FontProperties& props, std::unordered_map<uint32_t, glm::vec2>&& texture_offsets, Ref<Texture2D> texture, const char* ttf_data, uint32_t ttf_size)
		: m_Name(name), m_Properties(props), m_TextureOffsets(texture_offsets), m_Texture(texture)
	{
		hb_blob_t* blob = hb_blob_create(ttf_data, ttf_size, HB_MEMORY_MODE_DUPLICATE, nullptr, 0);
		m_Face = hb_face_create(blob, 0);
		hb_blob_destroy(blob);
	}

	Font::~Font()
	{
		for (auto [pt, font] : m_Fonts)
		{
			hb_font_destroy(font);
		}
		
		hb_face_destroy(m_Face);
	}

	glm::vec2 Font::GetTextureOffset(int16_t character) const
	{
		auto iter = m_TextureOffsets.find(character);
		if (iter != m_TextureOffsets.end())
			return iter->second;
		else
			return { 0.0f, 0.0f };
	}

	hb_font_t* Font::GetHBFont(int pointSize)
	{
		auto iter = m_Fonts.find(pointSize);
		if (iter != m_Fonts.end())
		{
			return iter->second;
		}
		else
		{
			hb_font_t* font = hb_font_create(m_Face);
			hb_ot_font_set_funcs(font);
			hb_font_set_scale(font, pointSize * 64, pointSize * 64);
			m_Fonts[pointSize] = font;
			return font;
		}
	}

	Label::Label(const char* utf8_text, int point_size, Ref<Font> font, bool dynamic, int excess)
		: m_PointSize(point_size), m_Font(font), m_BufferLength(0u), m_Dynamic(dynamic), m_Excess(excess)
	{
		m_DistanceFactor = font->GetProperties().DistanceGradient * (float)m_PointSize + 2.0f;
		m_Buffer = hb_buffer_create();

		LoadLayout(utf8_text);
		CreateRenderingBuffers(dynamic);
	}

	Label::~Label()
	{
		hb_buffer_destroy(m_Buffer);
	}

	void Label::SetText(const char* utf8_text)
	{
		if (!m_Dynamic)
		{
			Log::CoreError("Label must be dynamic to set text!");
			return;
		}

		hb_buffer_clear_contents(m_Buffer);
		m_GlyphVertices.clear();
		m_GlyphIndices.clear();

		bool resize = LoadLayout(utf8_text);
		if (resize)
		{
			Log::CoreWarn("Increasing text buffer size.");
			CreateRenderingBuffers(true);
		}
		else
		{
			UpdateRenderingBuffers();
		}
	}

	bool Label::LoadLayout(const char* utf8_text)
	{
		bool resize = false;

		hb_buffer_add_utf8(m_Buffer, utf8_text, -1, 0, -1);
		hb_buffer_guess_segment_properties(m_Buffer);

		hb_shape(m_Font->GetHBFont(m_PointSize), m_Buffer, NULL, 0);

		unsigned int buffer_length = hb_buffer_get_length(m_Buffer);

		if (buffer_length > m_BufferLength)
		{
			// Reserve extra length if specified.
			m_BufferLength = buffer_length + m_Excess;
			m_GlyphVertices.reserve((size_t)m_BufferLength * 4);
			m_GlyphIndices.reserve((size_t)m_BufferLength * 6);
			resize = true;
		}

		hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(m_Buffer, NULL);
		hb_glyph_position_t* glyph_position = hb_buffer_get_glyph_positions(m_Buffer, NULL);

		const FontProperties& font_info = m_Font->GetProperties();
		double current_x = 0;
		double current_y = 0;

		for (unsigned int i = 0; i < buffer_length; i++)
		{
			double x_position = current_x + glyph_position[i].x_offset / 64.0;
			double y_position = current_y + glyph_position[i].y_offset / 64.0;

			float left = (float)x_position - (4.0f * (float)m_PointSize / (float)font_info.GlyphWidth);
			float right = left + (float)m_PointSize * 1.33f;
			float bottom = (float)y_position - (4.0f * (float)m_PointSize / (float)font_info.GlyphWidth);
			float top = bottom + (float)m_PointSize * 1.33f;

			const glm::vec2& texture_offset = m_Font->GetTextureOffset(glyph_info[i].codepoint);
			float texture_left = texture_offset.x + 0.002f;
			float texture_bottom = texture_offset.y + 0.002f;
			float texture_right = texture_offset.x + font_info.GlyphWidth / (float)font_info.TextureWidth - 0.002f;
			float texture_top = texture_offset.y + font_info.GlyphWidth / (float)font_info.TextureWidth - 0.002f;

			int index_offset = i * 4;
			m_GlyphIndices.push_back(index_offset);
			m_GlyphIndices.push_back(index_offset + 1);
			m_GlyphIndices.push_back(index_offset + 2);
			m_GlyphIndices.push_back(index_offset);
			m_GlyphIndices.push_back(index_offset + 2);
			m_GlyphIndices.push_back(index_offset + 3);

			GlyphVertex vertex;
			vertex.Position = glm::vec3(left, bottom, (float)i / (float)buffer_length * 0.001f);
			vertex.TexCoords = glm::vec2(texture_left, texture_bottom);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec3(right, bottom, (float)i / (float)buffer_length * 0.001f);
			vertex.TexCoords = glm::vec2(texture_right, texture_bottom);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec3(right, top, (float)i / (float)buffer_length * 0.001f);
			vertex.TexCoords = glm::vec2(texture_right, texture_top);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec3(left, top, (float)i / (float)buffer_length * 0.001f);
			vertex.TexCoords = glm::vec2(texture_left, texture_top);
			m_GlyphVertices.push_back(vertex);

			current_x += glyph_position[i].x_advance / 64.0;
			current_y += glyph_position[i].y_advance / 64.0;
		}
		return resize;
	}

	void Label::CreateRenderingBuffers(bool dynamic)
	{
		m_VertexArray = VertexArray::Create();

		if (dynamic)
		{
			m_VertexBuffer = VertexBuffer::Create((uint32_t)(sizeof(GlyphVertex) * m_BufferLength * 4), BufferUsage::DynamicDraw);
			m_VertexBuffer->SetSubData((float*)m_GlyphVertices.data(), (uint32_t)(sizeof(GlyphVertex) * m_GlyphVertices.size()), 0, true);

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create((uint32_t)(sizeof(uint32_t) * m_BufferLength * 6), BufferUsage::DynamicDraw);
			indexBuffer->SetSubData(m_GlyphIndices.data(), (uint32_t)(sizeof(uint32_t) * m_GlyphIndices.size()), 0, true);

			m_VertexArray->SetIndexBuffer(indexBuffer);
		}
		else
		{
			m_VertexBuffer = VertexBuffer::Create((float*)m_GlyphVertices.data(),
				(uint32_t)(sizeof(GlyphVertex) * m_GlyphVertices.size()),
				BufferUsage::StaticDraw);

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(m_GlyphIndices.data(),
				(uint32_t)m_GlyphIndices.size(),
				BufferUsage::StaticDraw);

			m_VertexArray->SetIndexBuffer(indexBuffer);
		}

		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "POSITION", 0 },
			{ ShaderDataType::Float2, "TEXCOORD", 1 }
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->Finalize(Renderer::GetFontShader());
		m_VertexArray->Unbind();
	}

	void Label::UpdateRenderingBuffers()
	{
		m_VertexBuffer->SetSubData((float*)m_GlyphVertices.data(),
			(uint32_t)(sizeof(GlyphVertex) * m_GlyphVertices.size()),
			0, true);

		m_VertexArray->GetIndexBuffer()->SetSubData(m_GlyphIndices.data(),
			(uint32_t)(sizeof(uint32_t) * m_GlyphIndices.size()), 0, true);
	}
}
