#include "lipch.h"
#include "Font.h"

#include "Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

#include "hb-ot.h"

namespace li
{
	Font::Font(const std::string& name, const FontProperties& props, std::unordered_map<uint32_t, glm::vec2>&& textureOffsets, Ref<Texture2D> texture, const char* ttfData, uint32_t ttfSize)
		: m_Name(name), m_Properties(props), m_TextureOffsets(textureOffsets), m_Texture(texture)
	{
		hb_blob_t* blob = hb_blob_create(ttfData, ttfSize, HB_MEMORY_MODE_DUPLICATE, nullptr, 0);
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
		if (m_TextureOffsets.find(character) != m_TextureOffsets.end())
			return m_TextureOffsets.at(character);
		else
			return { 0.0f, 0.0f };
	}

	hb_font_t* Font::GetHBFont(int pointSize)
	{
		if (m_Fonts.find(pointSize) == m_Fonts.end())
		{
			hb_font_t* font = hb_font_create(m_Face);
			hb_ot_font_set_funcs(font);
			hb_font_set_scale(font, pointSize * 64, pointSize * 64);
			m_Fonts[pointSize] = font;
			return font;
		}
		return m_Fonts.at(pointSize);
	}

	Label::Label(const char* utf8_text, int pointSize, Ref<Font> font, bool dynamic, int excess)
		: m_PointSize(pointSize), m_Font(font), m_BufferLength(0u), m_Dynamic(dynamic), m_Excess(excess)
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
			LI_CORE_ERROR("Label must be dynamic to set text!");
			return;
		}

		hb_buffer_clear_contents(m_Buffer);
		m_GlyphVertices.clear();
		m_GlyphIndices.clear();

		bool resize = LoadLayout(utf8_text);
		if (resize)
		{
			LI_CORE_WARN("Increasing text buffer size.");
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

		const FontProperties& fontInfo = m_Font->GetProperties();
		double current_x = 0;
		double current_y = 0;

		for (unsigned int i = 0; i < buffer_length; i++)
		{
			double x_position = current_x + glyph_position[i].x_offset / 64.0;
			double y_position = current_y + glyph_position[i].y_offset / 64.0;

			float left = (float)x_position - (4.0f * (float)m_PointSize / (float)fontInfo.GlyphWidth);
			float right = left + (float)m_PointSize * 1.33f;
			float bottom = (float)y_position - (4.0f * (float)m_PointSize / (float)fontInfo.GlyphWidth);
			float top = bottom + (float)m_PointSize * 1.33f;

			const glm::vec2& textureOffset = m_Font->GetTextureOffset(glyph_info[i].codepoint);
			float textureLeft = textureOffset.x + 0.002f;
			float textureBottom = textureOffset.y + 0.002f;
			float textureRight = textureOffset.x + fontInfo.GlyphWidth / (float)fontInfo.TextureWidth - 0.002f;
			float textureTop = textureOffset.y + fontInfo.GlyphWidth / (float)fontInfo.TextureWidth - 0.002f;

			int indexOffset = i * 4;
			m_GlyphIndices.push_back(indexOffset);
			m_GlyphIndices.push_back(indexOffset + 1);
			m_GlyphIndices.push_back(indexOffset + 2);
			m_GlyphIndices.push_back(indexOffset);
			m_GlyphIndices.push_back(indexOffset + 2);
			m_GlyphIndices.push_back(indexOffset + 3);

			GlyphVertex vertex;
			vertex.Position = glm::vec3(left, bottom, (float)i / (float)buffer_length * 0.001f);
			vertex.TexCoords = glm::vec2(textureLeft, textureBottom);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec3(right, bottom, (float)i / (float)buffer_length * 0.001f);
			vertex.TexCoords = glm::vec2(textureRight, textureBottom);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec3(right, top, (float)i / (float)buffer_length * 0.001f);
			vertex.TexCoords = glm::vec2(textureRight, textureTop);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec3(left, top, (float)i / (float)buffer_length * 0.001f);
			vertex.TexCoords = glm::vec2(textureLeft, textureTop);
			m_GlyphVertices.push_back(vertex);

			current_x += glyph_position[i].x_advance / 64.0;
			current_y += glyph_position[i].y_advance / 64.0;
		}
		return resize;
	}

	void Label::CreateRenderingBuffers(bool dynamic)
	{
		m_VertexArray = VertexArray::Create();

		BufferLayout charLayout({
			{ ShaderDataType::Float3, "POSITION", 0 },
			{ ShaderDataType::Float2, "TEXCOORD", 1 }
		});

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

		m_VertexBuffer->SetLayout(charLayout);
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