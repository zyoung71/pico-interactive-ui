#include <interactive-ui/graphics/Font.hpp>

bool FontOrder::operator()(const Font& f1, const Font& f2) const
{
    return f1.character_begin < f2.character_begin;
}

const Font* Font::FindFont(char32_t codepoint, const FontGroup &font_group)
{
    for (const Font& f : font_group)
    {
        if (codepoint >= f.character_begin && codepoint <= f.character_end)
            return &f;
    }
    return &fonts::default_font;
}