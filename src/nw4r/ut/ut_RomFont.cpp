// Ported from https://github.com/kiwi515/ogws/blob/master/src/nw4r/ut/ut_RomFont.cpp --- Mostly unused

#include "nw4r/ut.h"

namespace nw4r {
namespace ut {

u16 RomFont::mFontEncode = 0xFFFF;

namespace {

bool IsCP1252Char(u16 c) {
    return c >= 0x20 && c <= 0xFF;
}

bool IsSJISHalfWidthChar(u16 c) {
    if (c > 0xFF) {
        return false;
    }

    return (c >= 0x20 && c <= 0x7E) || (c >= 0xA1 && c <= 0xDF);
}

bool IsSJISFullWidthChar(u16 c) {
    u8 hi = BitExtract<u16>(c, 8, 8);
    u8 lo = BitExtract<u16>(c, 0, 8);

    return hi >= 0x81 && hi <= 0x98 && lo >= 0x40 && lo <= 0xFC;
}

} // namespace

RomFont::RomFont() : mFontHeader(NULL), mAlternateChar('?') {
    mDefaultWidths.leftSpacing = 0;
    mDefaultWidths.glyphWidth = 0;
    mDefaultWidths.charWidth = 0;
}

RomFont::~RomFont() {}

bool RomFont::Load(void *buffer) {
    if (mFontHeader != NULL) {
        return false;
    }

    BOOL success = OSInitFont(static_cast<OSFontHeader *>(buffer));

    if (success) {
        mFontEncode = OSGetFontEncode();
        mFontHeader = static_cast<OSFontHeader *>(buffer);

        mDefaultWidths.leftSpacing = 0;
        mDefaultWidths.glyphWidth = GetCellWidth();
        mDefaultWidths.charWidth = GetMaxCharWidth();

        InitReaderFunc(GetEncoding());
    }

    return success;
}

u32 RomFont::GetRequireBufferSize() {
    switch (OSGetFontEncode()) {
        case OS_FONT_ENCODE_ANSI: return 0x00020120;
        case OS_FONT_ENCODE_SJIS: return 0x00120F00;
    }

    return 0;
}

int RomFont::GetWidth() const {
    return mFontHeader->width;
}

int RomFont::GetHeight() const {
    return GetAscent() + GetDescent();
}

int RomFont::GetAscent() const {
    return mFontHeader->ascent;
}

int RomFont::GetDescent() const {
    return mFontHeader->descent;
}

int RomFont::GetBaselinePos() const {
    return mFontHeader->ascent;
}

int RomFont::GetCellHeight() const {
    return mFontHeader->cellHeight;
}

int RomFont::GetCellWidth() const {
    return mFontHeader->cellWidth;
}

int RomFont::GetMaxCharWidth() const {
    return mFontHeader->width;
}

Font::Type RomFont::GetType() const {
    return TYPE_ROM;
}

GXTexFmt RomFont::GetTextureFormat() const {
    return GX_TF_I4;
}

int RomFont::GetLineFeed() const {
    return mFontHeader->leading;
}

CharWidths RomFont::GetDefaultCharWidths() const {
    return mDefaultWidths;
}

void RomFont::SetDefaultCharWidths(const CharWidths &widths) {
    mDefaultWidths = widths;
}

bool RomFont::SetAlternateChar(u16 c) {
    const u16 prev = mAlternateChar;
    mAlternateChar = 0xFFFF;

    u16 undef = HandleUndefinedChar(c);
    if (undef != 0xFFFF) {
        mAlternateChar = c;
        return true;
    } else {
        mAlternateChar = prev;
        return false;
    }
}

void RomFont::SetLineFeed(int lf) {
    mFontHeader->leading = lf;
}

int RomFont::GetCharWidth(u16 c) const {
    u32 width;
    char buffer[4];

    MakeCharPtr(buffer, c);
    OSGetFontWidth(buffer, &width);

    return width;
}

CharWidths RomFont::GetCharWidths(u16 c) const {
    int width = GetCharWidth(c);

    CharWidths widths;
    widths.leftSpacing = 0;
    widths.glyphWidth = width;
    widths.charWidth = width;

    return widths;
}

void RomFont::GetGlyph(Glyph *out, u16 c) const {
    void *texture;
    u32 x, y, width;
    char buffer[4];

    MakeCharPtr(buffer, c);
    OSGetFontTexture(buffer, &texture, &x, &y, &width);

    out->texture = texture;

    out->widths.leftSpacing = 0;
    out->widths.glyphWidth = width;
    out->widths.charWidth = width;

    out->height = mFontHeader->cellHeight;
    out->format = GX_TF_I4;

    out->texWidth = mFontHeader->sheetWidth;
    out->texHeight = mFontHeader->sheetHeight;

    out->cellX = x;
    out->cellY = y;
}

FontEncoding RomFont::GetEncoding() const {
    switch (mFontEncode) {
        case OS_FONT_ENCODE_ANSI: return FONT_ENCODE_CP1252;
        case OS_FONT_ENCODE_SJIS: return FONT_ENCODE_SJIS;
    }

    return FONT_ENCODE_CP1252;
}

void RomFont::MakeCharPtr(char *buffer, u16 c) const {
    c = HandleUndefinedChar(c);

    if (BitExtract<u16>(c, 8, 8) == 0) {
        buffer[0] = c & 0x00FF;
        buffer[1] = '\0';
    } else {
        buffer[0] = BitExtract<u16>(c, 8, 8);
        buffer[1] = c & 0x00FF;
        buffer[2] = '\0';
    }
}

u16 RomFont::HandleUndefinedChar(u16 c) const {
    bool valid;

    switch (mFontEncode) {
        case OS_FONT_ENCODE_ANSI: valid = IsCP1252Char(c); break;
        case OS_FONT_ENCODE_SJIS: valid = IsSJISHalfWidthChar(c) || IsSJISFullWidthChar(c); break;
    }

    return valid ? c : mAlternateChar;
}

} // namespace ut
} // namespace nw4r
