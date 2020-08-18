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
		uint16_t GlyphWidth;
		uint16_t TextureWidth;
		float DistanceGradient;
	};

	class Font
	{
	public:

		Font(const std::string& name, const FontProperties& props, std::unordered_map<uint32_t, glm::vec2>&& textureOffsets, Ref<Texture2D> texture, const char* ttfData, uint32_t ttfSize);
		virtual ~Font();

		inline Ref<Texture2D> GetTexture() const { return m_Texture; }
		inline const FontProperties& GetProperties() const { return m_Properties; }
		glm::vec2 GetTextureOffset(int16_t character) const;
		

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

		Label(const char* utf8_text, int point_size, Ref<Font> font, bool dynamic, int excess = 0);
		virtual ~Label();

		inline int GetPointSize() const { return m_PointSize; }
		inline float GetDistanceFactor() const { return m_DistanceFactor; }

		inline const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }
		inline const Ref<Font>& GetFont() { return m_Font; }

		void SetText(const char* utf8_text);

	private:
		bool LoadLayout(const char* utf8_text);
		void CreateRenderingBuffers(bool dynamic);
		void UpdateRenderingBuffers();

		int m_PointSize;
		float m_DistanceFactor;
		Ref<Font> m_Font;

		bool m_Dynamic;
		int m_Excess;

		unsigned int m_BufferLength;
		std::vector<GlyphVertex> m_GlyphVertices;
		std::vector<uint32_t> m_GlyphIndices;

		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;

		hb_buffer_t* m_Buffer;
	};
}