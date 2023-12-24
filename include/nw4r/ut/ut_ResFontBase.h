#ifndef NW4R_UT_RES_FONT_BASE_H
#define NW4R_UT_RES_FONT_BASE_H
#include <nw4r/types_nw4r.h>
#include <nw4r/ut/ut_Font.h>

namespace nw4r {
namespace ut {

enum FontMapMethod {
    FONT_MAPMETHOD_LINEAR,
    FONT_MAPMETHOD_ARRAY,
    FONT_MAPMETHOD_SCAN
};

struct FontTextureGlyph {
    u8 cellWidth;    // at 0x0
    u8 cellHeight;   // at 0x1
    s8 baselinePos;  // at 0x2
    u8 maxCharWidth; // at 0x3
    u32 sheetSize;   // at 0x4
    u16 sheetNum;    // at 0x8
    u16 sheetFormat; // at 0xA
    u16 sheetRow;    // at 0xC
    u16 sheetLine;   // at 0xE
    u16 sheetWidth;  // at 0x10
    u16 sheetHeight; // at 0x12
    u8* sheetImage;  // at 0x14
};

struct FontWidth {
    u16 firstChar;           // at 0x0
    u16 lastChar;            // at 0x2
    FontWidth* next;         // at 0x4
    CharWidths widthTable[]; // at 0x8
};

struct FontCodeMap {
    u16 firstChar;     // at 0x0
    u16 lastChar;      // at 0x2
    u16 mappingMethod; // at 0x4
    u16 reserved;      // at 0x6
    FontCodeMap* next; // at 0x8
    u16 mapInfo[];     // at 0xc
};

struct FontInformation {
    u8 type;                     // at 0x0
    s8 lineFeed;                 // at 0x1
    u16 alterCharIndex;          // at 0x2
    CharWidths defaultWidth;     // at 0x4
    u8 encoding;                 // at 0x7
    FontTextureGlyph* fontGlyph; // at 0x8
    FontWidth* fontWidth;        // at 0xC
    FontCodeMap* fontMap;        // at 0x10
    u8 height;                   // at 0x14
    u8 width;                    // at 0x15
    u8 ascent;                   // at 0x16
};

namespace detail {

class ResFontBase : public Font {
public:
    ResFontBase();
    virtual ~ResFontBase(); // at 0x8

    virtual int GetWidth() const;                                // at 0xC
    virtual int GetHeight() const;                               // at 0x10
    virtual int GetAscent() const;                               // at 0x14
    virtual int GetDescent() const;                              // at 0x18
    virtual int GetBaselinePos() const;                          // at 0x1C
    virtual int GetCellHeight() const;                           // at 0x20
    virtual int GetCellWidth() const;                            // at 0x24
    virtual int GetMaxCharWidth() const;                         // at 0x28
    virtual Type GetType() const;                                // at 0x2C
    virtual GXTexFmt GetTextureFormat() const;                   // at 0x30
    virtual int GetLineFeed() const;                             // at 0x34
    virtual CharWidths GetDefaultCharWidths() const;             // at 0x38
    virtual void SetDefaultCharWidths(const CharWidths& widths); // at 0x3C
    virtual bool SetAlternateChar(u16 c);                        // at 0x40
    virtual void SetLineFeed(int lf);                            // at 0x44
    virtual int GetCharWidth(u16 c) const;                       // at 0x48
    virtual CharWidths GetCharWidths(u16 c) const;               // at 0x4C
    virtual void GetGlyph(Glyph* out, u16 c) const;              // at 0x50
    virtual FontEncoding GetEncoding() const;                    // at 0x54

    bool IsManaging(const void* buffer) const { return mResource == buffer; }

    void SetResourceBuffer(void* buffer, FontInformation* info);

    u16 GetGlyphIndex(u16 c) const;

    u16 FindGlyphIndex(u16 c) const;
    u16 FindGlyphIndex(const FontCodeMap* map, u16 c) const;

    const CharWidths& GetCharWidthsFromIndex(u16 index) const;
    const CharWidths& GetCharWidthsFromIndex(const FontWidth* width,
                                             u16 index) const;

    void GetGlyphFromIndex(Glyph* out, u16 index) const;

private:
    void* mResource;            // at 0x10
    FontInformation* mFontInfo; // at 0x14
    u16 mLastCharCode;          // at 0x18
    u16 mLastGlyphIndex;        // at 0x18
};

} // namespace detail
} // namespace ut
} // namespace nw4r

#endif
