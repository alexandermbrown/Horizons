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

	Label::Label(const std::wstring& text, float pointSize, Ref<Font> font, const glm::vec4 color, int maxChars)
		: m_Text(text), m_PointSize(pointSize), m_Font(font), m_MaxChars(maxChars)
	{
		Init();
		Calculate();
	}

	void Label::Init()
	{
		if (m_MaxChars >= 0)
		{
			m_GlyphVertices.reserve((size_t)m_MaxChars * 4);
			m_GlyphIndices.reserve((size_t)m_MaxChars * 6);
		}
		else
		{
			m_GlyphVertices.reserve(m_Text.length() * 4);
			m_GlyphIndices.reserve(m_Text.length() * 6);
		}

		m_VertexArray = VertexArray::Create();

		BufferLayout charLayout({
			{ ShaderDataType::Float2, "a_Position", 0 },
			{ ShaderDataType::Float2, "a_TexCoord", 1 }
		});

		m_VertexBuffer = VertexBuffer::Create((m_MaxChars ? m_MaxChars : m_Text.length()) * charLayout.GetStride() * 4,
			m_MaxChars >= 0 ? BufferUsage::DynamicDraw : BufferUsage::StaticDraw);
		m_VertexBuffer->SetLayout(charLayout);

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create((m_MaxChars ? m_MaxChars : m_Text.length()) * 6 * sizeof(uint32_t),
			m_MaxChars >= 0 ? BufferUsage::DynamicDraw : BufferUsage::StaticDraw);

		m_VertexArray->SetIndexBuffer(indexBuffer);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	}

	void Label::Calculate()
	{
		const FontProperties& fontInfo = m_Font->GetProperties();
		float xOffset = 0;
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

			//float textureLeft = 0.0f;
			//float textureBottom = 0.0f;
			//float textureRight = 1.0f;
			//float textureTop = 1.0f;

			//float textureOffset = 4 * (fontInfo.GlyphWidth / fontInfo.EmSize) / fontInfo.TextureWidth;
			//float textureLeft = glyphInfo.TextureOffset.x + textureOffset + (glyphInfo.BearingX * scale / fontInfo.TextureWidth);
			//float textureBottom = glyphInfo.TextureOffset.y + textureOffset + (glyphInfo.BearingY * scale / fontInfo.TextureWidth) - (glyphInfo.Height * scale / fontInfo.TextureWidth);
			//float textureRight = glyphInfo.TextureOffset.x + (glyphInfo.BearingX * scale / fontInfo.TextureWidth) + (glyphInfo.Width * scale / fontInfo.TextureWidth);
			//float textureTop = glyphInfo.TextureOffset.y + (glyphInfo.BearingY * scale / fontInfo.TextureWidth);

			float textureLeft = glyphInfo.TextureOffset.x + 0.002f;
			float textureBottom = glyphInfo.TextureOffset.y + 0.002f;
			float textureRight = glyphInfo.TextureOffset.x + fontInfo.GlyphWidth / (float)fontInfo.TextureWidth - 0.002f;
			float textureTop = glyphInfo.TextureOffset.y + fontInfo.GlyphWidth / (float)fontInfo.TextureWidth - 0.002f;

			//float textureLeft = ((fontInfo.GlyphWidth * character) % fontInfo.TextureWidth) / (float)fontInfo.TextureWidth;
			//float textureBottom = std::floor((float)(fontInfo.GlyphWidth * character) / (float)fontInfo.TextureWidth) * fontInfo.GlyphWidth / (float)fontInfo.TextureWidth;
			//float textureRight = textureLeft + fontInfo.GlyphWidth / fontInfo.TextureWidth;
			//float textureTop = textureBottom + fontInfo.GlyphWidth / fontInfo.TextureWidth;

			int count = m_GlyphVertices.size();
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

		m_VertexArray->GetIndexBuffer()->SetSubData((uint32_t*)&m_GlyphIndices[0], m_GlyphIndices.size() * sizeof(uint32_t), 0);
		m_VertexBuffer->SetSubData((float*)&m_GlyphVertices[0], sizeof(GlyphVertex) * m_GlyphVertices.size(), 0);
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