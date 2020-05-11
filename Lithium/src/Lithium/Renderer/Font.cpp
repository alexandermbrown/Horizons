#include "lipch.h"
#include "Font.h"

#include "glm/gtc/matrix_transform.hpp"

namespace li
{
	Font::Font(const std::string& name, const FontProperties& props, const std::vector<Glyph> glyphs, Ref<Texture2D> texture)
		: m_Name(name), m_Properties(props), m_Glyphs(), m_Texture(texture)
	{
		for (const Glyph& glyph : glyphs)
		{
			m_Glyphs[glyph.Character] = glyph;
		}
	}

	Label::Label(const std::wstring& text, float pointSize, Ref<Font> font, const glm::vec4 color, uint32_t maxChars)
		: m_Text(text), m_PointSize(pointSize), m_Font(font), m_MaxChars(maxChars)
	{
		Init();
		Calculate();
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
			{ ShaderDataType::Float2, "a_Position", 0 },
			{ ShaderDataType::Float2, "a_TexCoord", 1 }
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
	}

	void Label::Calculate()
	{
		const FontProperties& fontInfo = m_Font->GetProperties();
		float xOffset = 0.0f;
		m_GlyphVertices.clear();
		m_GlyphIndices.clear();

		for (wchar_t character : m_Text)
		{
			const Glyph& glyphInfo = m_Font->GetGlyph(character);
			float scale = m_PointSize / fontInfo.EmSize;

			if (character == L' ') {
				xOffset += glyphInfo.HorizontalAdvance * scale * 1.6f;
				continue;
			}

			float left = xOffset + (glyphInfo.BearingX - 4.0f) * scale;
			float right = left + m_PointSize * 1.25f;
			float bottom = -8.0f;
			float top = bottom + m_PointSize * 1.25f;

			float textureLeft = glyphInfo.TextureOffset.x + 0.002f;
			float textureBottom = glyphInfo.TextureOffset.y + 0.002f;
			float textureRight = glyphInfo.TextureOffset.x + fontInfo.GlyphWidth / (float)fontInfo.TextureWidth - 0.002f;
			float textureTop = glyphInfo.TextureOffset.y + fontInfo.GlyphWidth / (float)fontInfo.TextureWidth - 0.002f;

			uint32_t count = (uint32_t)m_GlyphVertices.size();
			m_GlyphIndices.push_back(count);
			m_GlyphIndices.push_back(count + 1);
			m_GlyphIndices.push_back(count + 2);
			m_GlyphIndices.push_back(count);
			m_GlyphIndices.push_back(count + 2);
			m_GlyphIndices.push_back(count + 3);

			GlyphVertex vertex;
			vertex.Position = glm::vec2(left, bottom);
			vertex.TexCoords = glm::vec2(textureLeft, textureBottom);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec2(right, bottom);
			vertex.TexCoords = glm::vec2(textureRight, textureBottom);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec2(right, top);
			vertex.TexCoords = glm::vec2(textureRight, textureTop);
			m_GlyphVertices.push_back(vertex);

			vertex.Position = glm::vec2(left, top);
			vertex.TexCoords = glm::vec2(textureLeft, textureTop);
			m_GlyphVertices.push_back(vertex);

			xOffset += glyphInfo.HorizontalAdvance * scale;
		}

		m_VertexArray->GetIndexBuffer()->SetSubData(
			(uint32_t*)&m_GlyphIndices[0], 
			static_cast<uint32_t>(m_GlyphIndices.size() * sizeof(uint32_t)), 
			0
		);
		m_VertexBuffer->SetSubData(
			(float*)&m_GlyphVertices[0], 
			static_cast<uint32_t>(sizeof(GlyphVertex) * m_GlyphVertices.size()),
			0
		);
	}

	void Label::Set(const std::wstring& text, float pointSize, const glm::vec3& position, Ref<Font> font)
	{
		m_Text = text;
		m_PointSize = pointSize;
		m_Font = font;
		Calculate();
	}

	void Label::Set(const std::wstring& text, float pointSize, const glm::mat4& transform, Ref<Font> font)
	{
		m_Text = text;
		m_PointSize = pointSize;
		m_Font = font;
		Calculate();
	}

	void Label::SetText(const std::wstring& text)
	{
		m_Text = text;
		Calculate();
	}

	void Label::SetPointSize(float pointSize)
	{
		m_PointSize = pointSize;
		Calculate();
	}

	void Label::SetFont(Ref<Font> font)
	{
		m_Font = font;
		Calculate();
	}
}