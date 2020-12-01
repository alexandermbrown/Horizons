#include "lipch.h"
#include "FontLoader.h"

namespace li
{
	Ref<Font> FontLoader::Load(const Assets::Font* font)
	{
		FontProperties props;
		props.GlyphWidth = font->glyph_width();
		props.TextureWidth = font->texture_width();
		props.DistanceGradient = font->distance_gradient();

		const auto* saved_glyphs = font->glyphs();
		std::unordered_map<uint32_t, glm::vec2> textureOffsets(saved_glyphs->size());
		for (const Assets::GlyphEntry* entry : *saved_glyphs)
		{
			const Assets::Vec2* texture_offset = entry->texture_offset();
			textureOffsets[entry->codepoint()] = { texture_offset->x(), texture_offset->y() };
		}

		const auto* image = font->image();
		Ref<Texture2D> texture = Texture2D::Create(image->size(), image->data(), 4, WrapType::ClampToEdge, WrapType::ClampToEdge, FilterType::Linear, FilterType::Linear);

		const auto* ttf = font->ttf();
		return CreateRef<Font>(font->name()->str(), props, std::move(textureOffsets), texture, (const char*)ttf->data(), (uint32_t)ttf->size());
	}
}