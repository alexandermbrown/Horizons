#include "lipch.h"
#include "Font.h"

#include "Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

#include <hb-ot.h>

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

	Label::Label(const std::u16string& text, int pointSize, Ref<Font> font, uint32_t maxChars)
		: m_Text(text), m_PointSize(pointSize), m_Font(font), m_MaxChars(maxChars)
	{
		m_DistanceFactor =  font->GetProperties().DistanceGradient * (float)pointSize + 2.0f;
		Init();
	}

	Label::~Label()
	{
		hb_buffer_destroy(m_Buffer);
	}

	void Label::Init()
	{
		if (m_MaxChars > 0)
		{
			m_GlyphVertices.reserve((size_t)m_MaxChars * 4ULL);
			m_GlyphIndices.reserve((size_t)m_MaxChars * 6ULL);
		}
		else
		{
			m_GlyphVertices.reserve(m_Text.length() * 4ULL);
			m_GlyphIndices.reserve(m_Text.length() * 6ULL);
		}

		m_VertexArray = VertexArray::Create();

		BufferLayout charLayout({
			{ ShaderDataType::Float3, "POSITION", 0 },
			{ ShaderDataType::Float2, "TEXCOORD", 1 }
		});

		m_VertexBuffer = VertexBuffer::Create(
			static_cast<uint32_t>(m_MaxChars ? m_MaxChars : m_Text.length()) * charLayout.GetStride() * 4U,
			m_MaxChars >= 0 ? BufferUsage::DynamicDraw : BufferUsage::StaticDraw);
		m_VertexBuffer->SetLayout(charLayout);

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(
			static_cast<uint32_t>((m_MaxChars ? m_MaxChars : m_Text.length()) * 6ULL * sizeof(uint32_t)),
			m_MaxChars >= 0 ? BufferUsage::DynamicDraw : BufferUsage::StaticDraw);

		m_VertexArray->SetIndexBuffer(indexBuffer);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->Finalize(Renderer::GetFontShader());

		m_VertexArray->Unbind();

		m_Buffer = hb_buffer_create();
		hb_buffer_add_utf16(m_Buffer, (const uint16_t*)m_Text.c_str(), -1, 0, -1);
		hb_segment_properties_t props;
		props.direction = HB_DIRECTION_LTR; // TODO: for other languages, set correct properties.
		props.language = hb_language_from_string("en", -1);
		props.script = HB_SCRIPT_LATIN;
		hb_buffer_set_segment_properties(m_Buffer, &props);

		hb_font_t* hb_font = m_Font->GetHBFont(m_PointSize);
		hb_shape(hb_font, m_Buffer, NULL, 0);

		unsigned int len = hb_buffer_get_length(m_Buffer);
		hb_glyph_info_t* info = hb_buffer_get_glyph_infos(m_Buffer, NULL);
		hb_glyph_position_t* pos = hb_buffer_get_glyph_positions(m_Buffer, NULL);

		const FontProperties& fontInfo = m_Font->GetProperties();
		double current_x = 0;
		double current_y = 0;

		int num_quads = 0;
		for (unsigned int i = 0; i < len; i++)
		{
			hb_codepoint_t gid = info[i].codepoint;
			double x_position = current_x + pos[i].x_offset / 64.;
			double y_position = current_y + pos[i].y_offset / 64.;

			char glyphname[32];
			hb_font_get_glyph_name(hb_font, gid, glyphname, sizeof(glyphname));

			float left = (float)x_position - (4.0f * (float)m_PointSize / (float)fontInfo.GlyphWidth);
			float right = left + (float)m_PointSize * 1.33f;
			float bottom = (float)y_position - (4.0f * (float)m_PointSize / (float)fontInfo.GlyphWidth);
			float top = bottom + (float)m_PointSize * 1.33f;

			const glm::vec2& textureOffset = m_Font->GetTextureOffset(gid);
			float textureLeft = textureOffset.x + 0.002f;
			float textureBottom = textureOffset.y + 0.002f;
			float textureRight = textureOffset.x + fontInfo.GlyphWidth / (float)fontInfo.TextureWidth - 0.002f;
			float textureTop = textureOffset.y + fontInfo.GlyphWidth / (float)fontInfo.TextureWidth - 0.002f;

			int indexOffset = num_quads * 4;
			m_GlyphIndices.push_back(indexOffset);
			m_GlyphIndices.push_back(indexOffset + 1);
			m_GlyphIndices.push_back(indexOffset + 2);
			m_GlyphIndices.push_back(indexOffset);
			m_GlyphIndices.push_back(indexOffset + 2);
			m_GlyphIndices.push_back(indexOffset + 3);

			GlyphVertex vertex;
			vertex.Position = glm::vec3(left, bottom, (float)i / (float)len * 0.01);
			vertex.TexCoords = glm::vec2(textureLeft, textureBottom);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec3(right, bottom, (float)i / (float)len * 0.01);
			vertex.TexCoords = glm::vec2(textureRight, textureBottom);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec3(right, top, (float)i / (float)len * 0.01);
			vertex.TexCoords = glm::vec2(textureRight, textureTop);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec3(left, top, (float)i / (float)len * 0.01);
			vertex.TexCoords = glm::vec2(textureLeft, textureTop);
			m_GlyphVertices.push_back(vertex);
			num_quads++;
			
			current_x += pos[i].x_advance / 64.;
			current_y += pos[i].y_advance / 64.;
		}

		m_VertexArray->GetIndexBuffer()->SetSubData(
			(uint32_t*)&m_GlyphIndices[0], 
			static_cast<uint32_t>(m_GlyphIndices.size() * sizeof(uint32_t)), 
			0, true
		);
		m_VertexBuffer->SetSubData(
			(float*)&m_GlyphVertices[0], 
			static_cast<uint32_t>(sizeof(GlyphVertex) * m_GlyphVertices.size()),
			0, true
		);
		LI_CORE_TRACE("DONE!");
	}
}