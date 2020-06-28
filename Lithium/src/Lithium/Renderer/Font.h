#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Renderer/VertexArray.h"

#include "glm/glm.hpp"

#include <map>

namespace li
{
	struct Glyph
	{
		wchar_t Character;
		glm::vec2 TextureOffset;

		float Width;
		float Height;

		float HorizontalAdvance;
		float BearingX;
		float BearingY;
	};

	struct FontProperties
	{
		uint16_t GlyphWidth = 0;
		uint16_t TextureWidth = 0;

		float EmSize;
		float AscenderY, DescenderY;
		float LineHeight;
		float UnderlineY, UnderlineThickness;
	};

	class Font
	{
	public:

		Font(const std::string& name, const FontProperties& props, const std::vector<Glyph>& glyphs, Ref<Texture2D> texture);
		virtual ~Font() = default;

		inline Ref<Texture2D> GetTexture() const { return m_Texture; }
		inline const FontProperties& GetProperties() const { return m_Properties; }
		inline const Glyph& GetGlyph(wchar_t character) const { return m_Glyphs.at(character); }

	private:

		std::string m_Name;
		FontProperties m_Properties;
		Ref<Texture2D> m_Texture;
		std::unordered_map<wchar_t, Glyph> m_Glyphs;
	};


	struct GlyphVertex
	{
		glm::vec2 Position;
		glm::vec2 TexCoords;
	};

	class Label
	{
	public:

		Label(const std::wstring& text, float pointSize, Ref<Font> font, const glm::vec4 color = glm::vec4(1.0f), uint32_t maxChars = 0);
		virtual ~Label() = default;

		void Set(const std::wstring& text, float pointSize, const glm::vec3& position, Ref<Font> font);
		void Set(const std::wstring& text, float pointSize, const glm::mat4& transform, Ref<Font> font);

		void SetText(const std::wstring& text);
		void SetPointSize(float pointSize);
		void SetFont(Ref<Font> font);

		inline const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }
		inline const Ref<Font>& GetFont() { return m_Font; }

	private:
		void Init();
		void Calculate();

	private:

		std::wstring m_Text;
		float m_PointSize;
		Ref<Font> m_Font;

		uint32_t m_MaxChars;
		std::vector<GlyphVertex> m_GlyphVertices;
		std::vector<uint32_t> m_GlyphIndices;

		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
	};
}