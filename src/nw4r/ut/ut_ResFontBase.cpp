// Ported From https://github.com/kiwi515/ogws/blob/master/src/nw4r/ut/ut_ResFontBase.cpp

#include "nw4r/ut.h"

namespace nw4r {
namespace ut {
namespace detail {

ResFontBase::ResFontBase() : mResource(NULL), mFontInfo(NULL), mLastCharCode(0), mLastGlyphIndex(-1) {}

ResFontBase::~ResFontBase() {}

void ResFontBase::SetResourceBuffer(void *buffer, FontInformation *info) {
    mResource = buffer;
    mFontInfo = info;
}

void *ResFontBase::RemoveResourceBuffer() {
    void *pUserData = mResource;
    mFontInfo = NULL;
    mResource = NULL;
    return pUserData;
}

int ResFontBase::GetWidth() const {
    return mFontInfo->width;
}

int ResFontBase::GetHeight() const {
    return mFontInfo->height;
}

int ResFontBase::GetAscent() const {
    return mFontInfo->ascent;
}

int ResFontBase::GetDescent() const {
    return mFontInfo->height - mFontInfo->ascent;
}

int ResFontBase::GetBaselinePos() const {
    return mFontInfo->fontGlyph->baselinePos;
}

int ResFontBase::GetCellHeight() const {
    return mFontInfo->fontGlyph->cellHeight;
}

int ResFontBase::GetCellWidth() const {
    return mFontInfo->fontGlyph->cellWidth;
}

int ResFontBase::GetMaxCharWidth() const {
    return mFontInfo->fontGlyph->maxCharWidth;
}

Font::Type ResFontBase::GetType() const {
    return TYPE_RESOURCE;
}

GXTexFmt ResFontBase::GetTextureFormat() const {
    return static_cast<GXTexFmt>(mFontInfo->fontGlyph->sheetFormat);
}

int ResFontBase::GetLineFeed() const {
    return mFontInfo->lineFeed;
}

CharWidths ResFontBase::GetDefaultCharWidths() const {
    return mFontInfo->defaultWidth;
}

void ResFontBase::SetDefaultCharWidths(const CharWidths &widths) {
    mFontInfo->defaultWidth = widths;
}

bool ResFontBase::SetAlternateChar(u16 ch) {
    u32 index = FindGlyphIndex(ch);
    if (index != 0xFFFF) {
        mFontInfo->alterCharIndex = index;
        return true;
    }

    return false;
}

void ResFontBase::SetLineFeed(int lf) {
    mFontInfo->lineFeed = lf;
}

int ResFontBase::GetCharWidth(u16 ch) const {
    return GetCharWidths(ch).charWidth;
}

CharWidths ResFontBase::GetCharWidths(u16 ch) const {
    u16 index = GetGlyphIndex(ch);
    return GetCharWidthsFromIndex(index);
}

void ResFontBase::GetGlyph(Glyph *out, u16 ch) const {
    u16 index = GetGlyphIndex(ch);
    GetGlyphFromIndex(out, index);
}

bool ResFontBase::HasGlyph(u16 c) const {
    return FindGlyphIndex(c) != 0xFFFF;
}

FontEncoding ResFontBase::GetEncoding() const {
    return static_cast<FontEncoding>(mFontInfo->encoding);
}

u16 ResFontBase::GetGlyphIndex(u16 c) const {
    u16 index = FindGlyphIndex(c);
    return (index != 0xFFFF) ? index : mFontInfo->alterCharIndex;
}

u16 ResFontBase::FindGlyphIndex(u16 c) const {
    if (c == mLastCharCode) {
        return mLastGlyphIndex;
    }
    mLastCharCode = c;

    for (FontCodeMap *it = mFontInfo->fontMap; it != NULL; it = it->next) {
        if (it->firstChar <= c && c <= it->lastChar) {
            mLastGlyphIndex = FindGlyphIndex(it, c);
            return mLastGlyphIndex;
        }
    }

    mLastGlyphIndex = 0xFFFF;
    return mLastGlyphIndex;
}

u16 ResFontBase::FindGlyphIndex(const FontCodeMap *map, u16 c) const {
    struct CMapScanEntry {
        u16 code;  // at 0x0
        u16 index; // at 0x2
    };

    struct CMapInfoScan {
        u16 num;                 // at 0x0
        CMapScanEntry entries[]; // at 0x2
    };

    u16 index = 0xFFFF;

    switch (map->mappingMethod) {
        case FONT_MAPMETHOD_LINEAR: index = map->mapInfo[0] + (c - map->firstChar); break;
        case FONT_MAPMETHOD_ARRAY:  index = map->mapInfo[c - map->firstChar]; break;
        case FONT_MAPMETHOD_SCAN:
            const CMapInfoScan *info = reinterpret_cast<const CMapInfoScan *>(map->mapInfo);

            const CMapScanEntry *s = info->entries;
            const CMapScanEntry *e = &info->entries[info->num - 1];

            while (s <= e) {
                const CMapScanEntry *m = s + (e - s) / 2;

                if (m->code < c) {
                    s = m + 1;
                } else if (c < m->code) {
                    e = m - 1;
                } else {
                    return m->index;
                }
            }

            break;
    }

    return index;
}

const CharWidths &ResFontBase::GetCharWidthsFromIndex(u16 index) const {
    for (const FontWidth *it = mFontInfo->fontWidth; it != NULL; it = it->next) {
        if (it->firstChar <= index && index <= it->lastChar) {
            return GetCharWidthsFromIndex(it, index);
        }
    }

    return mFontInfo->defaultWidth;
}

const CharWidths &ResFontBase::GetCharWidthsFromIndex(const FontWidth *width, u16 index) const {
    return width->widthTable[index - width->firstChar];
}

void ResFontBase::GetGlyphFromIndex(Glyph *out, u16 index) const {
    const FontTextureGlyph *texGlyph = mFontInfo->fontGlyph;

    u32 cellsInASheet = texGlyph->sheetRow * texGlyph->sheetLine;

    u32 glyphCell = index % cellsInASheet;
    u32 glyphSheet = index / cellsInASheet;

    u32 unitX = glyphCell % texGlyph->sheetRow;
    u32 unitY = glyphCell / texGlyph->sheetRow;

    u32 pixelX = unitX * (texGlyph->cellWidth + 1);
    u32 pixelY = unitY * (texGlyph->cellHeight + 1);

    out->texture = texGlyph->sheetImage + (glyphSheet * texGlyph->sheetSize);

    out->widths = GetCharWidthsFromIndex(index);
    out->height = texGlyph->cellHeight;

    out->format = static_cast<GXTexFmt>(texGlyph->sheetFormat);

    out->texWidth = texGlyph->sheetWidth;
    out->texHeight = texGlyph->sheetHeight;

    out->cellX = pixelX + 1;
    out->cellY = pixelY + 1;
}

} // namespace detail
} // namespace ut
} // namespace nw4r
