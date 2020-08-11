#include "lipch.h"
#include "FontLoader.h"

namespace li
{
	Ref<Font> FontLoader::Load(const Assets::Font* font)
	{
		FontProperties props;
		props.GlyphWidth = font->glyph_width();
		props.TextureWidth = font->texture_width();
		props.EmSize = font->em_size();
		props.AscenderY = font->ascender_y();
		props.DescenderY = font->descender_y();
		props.LineHeight = font->line_height();
		props.UnderlineY = font->underline_y();
		props.UnderlineThickness = font->underline_thickness();

		const auto* saved_glyphs = font->glyphs();
		std::unordered_map<wchar_t, Glyph> glyphs(saved_glyphs->size());
		for (const Assets::GlyphEntry* entry : *saved_glyphs)
		{
			Glyph& glyph = glyphs[entry->character()];
			glyph.Character = entry->character();

			const Assets::Vec2* texture_offset = entry->texture_offset();
			glyph.TextureOffset = { texture_offset->x(), texture_offset->y() };

			glyph.Width = entry->width();
			glyph.Height = entry->height();
			glyph.HorizontalAdvance = entry->horizontal_advance();
			glyph.BearingX = entry->bearing_x();
			glyph.BearingY = entry->bearing_y();
		}

		const auto* image = font->image();
		Ref<Texture2D> texture = Texture2D::Create(image->size(), image->data(), WrapType::ClampToEdge, WrapType::ClampToEdge, FilterType::Linear, FilterType::Linear);

		return CreateRef<Font>(font->name()->str(), props, std::move(glyphs), texture);
	}
}