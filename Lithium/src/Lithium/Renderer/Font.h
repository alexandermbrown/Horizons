#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Renderer/VertexArray.h"

#include "glm/glm.hpp"
#include "hb.h"

#include <unordered_map>

namespace li
{
	struct FontProperties
	{
		uint16_t GlyphWidth = 0;
		uint16_t TextureWidth = 0;
	};

	class Font
	{
	public:

		Font(const std::string& name, const FontProperties& props, std::unordered_map<uint32_t, glm::vec2>&& textureOffsets, Ref<Texture2D> texture, const char* ttfData, uint32_t ttfSize);
		virtual ~Font();

		inline Ref<Texture2D> GetTexture() const { return m_Texture; }
		inline const FontProperties& GetProperties() const { return m_Properties; }
		inline const glm::vec2& GetTextureOffset(int16_t character) const { return m_TextureOffsets.at(character); }

		hb_font_t* GetHBFont(int pointSize);

	private:

		std::string m_Name;
		FontProperties m_Properties;
		Ref<Texture2D> m_Texture;
		std::unordered_map<uint32_t, glm::vec2> m_TextureOffsets;

		std::unordered_map<int, hb_font_t*> m_Fonts;

		hb_face_t* m_Face;
	};


	struct GlyphVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
	};

	class Label
	{
	public:

		Label(const std::u16string& text, int pointSize, Ref<Font> font, uint32_t maxChars = 0);
		virtual ~Label();

		inline int GetPointSize() const { return m_PointSize; }
		inline const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }
		inline const Ref<Font>& GetFont() { return m_Font; }

	private:
		void Init();

	private:

		std::u16string m_Text;
		int m_PointSize;
		Ref<Font> m_Font;

		uint32_t m_MaxChars;
		std::vector<GlyphVertex> m_GlyphVertices;
		std::vector<uint32_t> m_GlyphIndices;

		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;

		hb_buffer_t* m_Buffer;
	};
}