
#include "import-font.h"

#include <cstdlib>
#include <queue>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

//#ifdef _WIN32
//    #pragma comment(lib, "freetype.lib")
//#endif

namespace msdfgen {

#define REQUIRE(cond) { if (!(cond)) return false; }
#define F26DOT6_TO_DOUBLE(x) (1/64.*double(x))
#define F26DOT6_TO_FLOAT(x) (1/64.f*float(x))

    class FreetypeHandle {
        friend FreetypeHandle * initializeFreetype();
        friend void deinitializeFreetype(FreetypeHandle *library);
        friend FontHandle * loadFont(FreetypeHandle *library, const char *filename);
        friend FontHandle* loadFontFromMemory(FreetypeHandle* library, const unsigned char* data, size_t size);

        FT_Library library;

    };

    class FontHandle {
        friend FontHandle * loadFont(FreetypeHandle *library, const char *filename);
        friend FontHandle* loadFontFromMemory(FreetypeHandle* library, const unsigned char* data, size_t size);
        friend void destroyFont(FontHandle *font);
        friend bool getFontMetrics(FontMetrics &metrics, FontHandle *font);
        friend bool getFontWhitespaceWidth(double &spaceAdvance, double &tabAdvance, FontHandle *font);
        friend bool loadGlyph(Shape &output, FontHandle *font, unicode_t unicode, double *advance);
        friend bool getKerning(double &output, FontHandle *font, unicode_t unicode1, unicode_t unicode2);
        friend long getGlyphCount(FontHandle* font);
        friend unsigned long getFirstChar(FontHandle* font, uint32_t* index);
        friend unsigned long getNextChar(FontHandle* font, unsigned long charcode, uint32_t* index);
        friend bool getGlyphMetrics(GlyphMetrics& metrics, FontHandle* font);

        FT_Face face;

    };

    struct FtContext {
        Point2 position;
        Shape *shape;
        Contour *contour;
    };

    static Point2 ftPoint2(const FT_Vector &vector) {
        return Point2(F26DOT6_TO_DOUBLE(vector.x), F26DOT6_TO_DOUBLE(vector.y));
    }

    static int ftMoveTo(const FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        if (!(context->contour && context->contour->edges.empty()))
            context->contour = &context->shape->addContour();
        context->position = ftPoint2(*to);
        return 0;
    }

    static int ftLineTo(const FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        Point2 endpoint = ftPoint2(*to);
        if (endpoint != context->position) {
            context->contour->addEdge(new LinearSegment(context->position, endpoint));
            context->position = endpoint;
        }
        return 0;
    }

    static int ftConicTo(const FT_Vector *control, const FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        context->contour->addEdge(new QuadraticSegment(context->position, ftPoint2(*control), ftPoint2(*to)));
        context->position = ftPoint2(*to);
        return 0;
    }

    static int ftCubicTo(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        context->contour->addEdge(new CubicSegment(context->position, ftPoint2(*control1), ftPoint2(*control2), ftPoint2(*to)));
        context->position = ftPoint2(*to);
        return 0;
    }

    FreetypeHandle * initializeFreetype() {
        FreetypeHandle *handle = new FreetypeHandle;
        FT_Error error = FT_Init_FreeType(&handle->library);
        if (error) {
            delete handle;
            return NULL;
        }
        return handle;
    }

    void deinitializeFreetype(FreetypeHandle *library) {
        FT_Done_FreeType(library->library);
        delete library;
    }

    FontHandle * loadFont(FreetypeHandle *library, const char *filename) {
        if (!library)
            return NULL;
        FontHandle *handle = new FontHandle;
        FT_Error error = FT_New_Face(library->library, filename, 0, &handle->face);
        if (error) {
            delete handle;
            return NULL;
        }
        return handle;
    }

    FontHandle* loadFontFromMemory(FreetypeHandle* library, const unsigned char* data, size_t size) {
        if (!library)
            return NULL;
        FontHandle* handle = new FontHandle;
        FT_Error error = FT_New_Memory_Face(library->library, data, size, 0, &handle->face);
        if (error) {
            delete handle;
            return NULL;
        }
        return handle;
    }

    void destroyFont(FontHandle *font) {
        FT_Done_Face(font->face);
        delete font;
    }

    bool getFontMetrics(FontMetrics &metrics, FontHandle *font) {
        metrics.emSize = F26DOT6_TO_DOUBLE(font->face->units_per_EM);
        metrics.ascenderY = F26DOT6_TO_DOUBLE(font->face->ascender);
        metrics.descenderY = F26DOT6_TO_DOUBLE(font->face->descender);
        metrics.lineHeight = F26DOT6_TO_DOUBLE(font->face->height);
        metrics.underlineY = F26DOT6_TO_DOUBLE(font->face->underline_position);
        metrics.underlineThickness = F26DOT6_TO_DOUBLE(font->face->underline_thickness);
        return true;
    }

    bool getFontWhitespaceWidth(double &spaceAdvance, double &tabAdvance, FontHandle *font) {
        FT_Error error = FT_Load_Char(font->face, ' ', FT_LOAD_NO_SCALE);
        if (error)
            return false;
        spaceAdvance = F26DOT6_TO_DOUBLE(font->face->glyph->advance.x);
        error = FT_Load_Char(font->face, '\t', FT_LOAD_NO_SCALE);
        if (error)
            return false;
        tabAdvance = F26DOT6_TO_DOUBLE(font->face->glyph->advance.x);
        return true;
    }

    bool loadGlyph(Shape &output, FontHandle *font, unicode_t unicode, double *advance) {
        if (!font)
            return false;
        FT_Error error = FT_Load_Char(font->face, unicode, FT_LOAD_NO_SCALE);
        if (error)
            return false;
        output.contours.clear();
        output.inverseYAxis = false;
        if (advance)
            *advance = F26DOT6_TO_DOUBLE(font->face->glyph->advance.x);

        FtContext context = { };
        context.shape = &output;
        FT_Outline_Funcs ftFunctions;
        ftFunctions.move_to = &ftMoveTo;
        ftFunctions.line_to = &ftLineTo;
        ftFunctions.conic_to = &ftConicTo;
        ftFunctions.cubic_to = &ftCubicTo;
        ftFunctions.shift = 0;
        ftFunctions.delta = 0;
        error = FT_Outline_Decompose(&font->face->glyph->outline, &ftFunctions, &context);
        if (error)
            return false;
        if (!output.contours.empty() && output.contours.back().edges.empty())
            output.contours.pop_back();
        return true;
    }

    bool getKerning(double &output, FontHandle *font, unicode_t unicode1, unicode_t unicode2) {
        FT_Vector kerning;
        if (FT_Get_Kerning(font->face, FT_Get_Char_Index(font->face, unicode1), FT_Get_Char_Index(font->face, unicode2), FT_KERNING_UNSCALED, &kerning)) {
            output = 0;
            return false;
        }
        output = F26DOT6_TO_DOUBLE(kerning.x);
        return true;
    }

    long getGlyphCount(FontHandle* font) {
        return font->face->num_glyphs;
    }

    unsigned long getFirstChar(FontHandle* font, uint32_t* index) {
        return FT_Get_First_Char(font->face, index);
    }

    unsigned long getNextChar(FontHandle* font, unsigned long charcode, uint32_t* index) {
        return FT_Get_Next_Char(font->face, charcode, index);
    }

    bool getGlyphMetrics(GlyphMetrics& metrics, FontHandle* font)
    {
        metrics.width = F26DOT6_TO_FLOAT(font->face->glyph->metrics.width);
        metrics.height = F26DOT6_TO_FLOAT(font->face->glyph->metrics.height);

        metrics.horiBearingX = F26DOT6_TO_FLOAT(font->face->glyph->metrics.horiBearingX);
        metrics.horiBearingY = F26DOT6_TO_FLOAT(font->face->glyph->metrics.horiBearingY);
        metrics.horiAdvance = F26DOT6_TO_FLOAT(font->face->glyph->metrics.horiAdvance);

        metrics.vertBearingX = F26DOT6_TO_FLOAT(font->face->glyph->metrics.vertBearingX);
        metrics.vertBearingY = F26DOT6_TO_FLOAT(font->face->glyph->metrics.vertBearingY);
        metrics.vertAdvance = F26DOT6_TO_FLOAT(font->face->glyph->metrics.vertAdvance);

        return true;
    }
}
