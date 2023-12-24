#ifndef NW4R_UT_FONT_H
#define NW4R_UT_FONT_H
#include <nw4r/types_nw4r.h>
#include <nw4r/ut/ut_CharStrmReader.h>
#include <rvl/GX.h>

namespace nw4r {
namespace ut {

struct CharWidths {
    s8 leftSpacing; // at 0x0
    u8 glyphWidth;  // at 0x1
    s8 charWidth;   // at 0x2
};

struct Glyph {
    void *texture;     // at 0x0
    CharWidths widths; // at 0x4
    u8 height;         // at 0x7
    GXTexFmt format;   // at 0x8
    u16 texWidth;      // at 0xC
    u16 texHeight;     // at 0xE
    u16 cellX;         // at 0x10
    u16 cellY;         // at 0x12
};

enum FontEncoding { FONT_ENCODE_UTF8, FONT_ENCODE_UTF16, FONT_ENCODE_SJIS, FONT_ENCODE_CP1252 };

class Font {
public:
    enum Type { TYPE_NULL, TYPE_ROM, TYPE_RESOURCE, TYPE_PAIR };

public:
    Font() : mReadFunc(&CharStrmReader::ReadNextCharCP1252) {}
    virtual ~Font() {} // at 0x8

    virtual int GetWidth() const = 0;                                // at 0xC
    virtual int GetHeight() const = 0;                               // at 0x10
    virtual int GetAscent() const = 0;                               // at 0x14
    virtual int GetDescent() const = 0;                              // at 0x18
    virtual int GetBaselinePos() const = 0;                          // at 0x1C
    virtual int GetCellHeight() const = 0;                           // at 0x20
    virtual int GetCellWidth() const = 0;                            // at 0x24
    virtual int GetMaxCharWidth() const = 0;                         // at 0x28
    virtual Type GetType() const = 0;                                // at 0x2C
    virtual GXTexFmt GetTextureFormat() const = 0;                   // at 0x30
    virtual int GetLineFeed() const = 0;                             // at 0x34
    virtual CharWidths GetDefaultCharWidths() const = 0;             // at 0x38
    virtual void SetDefaultCharWidths(const CharWidths &widths) = 0; // at 0x3C
    virtual bool SetAlternateChar(u16 c) = 0;                        // at 0x40
    virtual void SetLineFeed(int lf) = 0;                            // at 0x44
    virtual int GetCharWidth(u16 c) const = 0;                       // at 0x48
    virtual CharWidths GetCharWidths(u16 c) const = 0;               // at 0x4C
    virtual void GetGlyph(Glyph *out, u16 c) const = 0;              // at 0x50
    virtual FontEncoding GetEncoding() const = 0;                    // at 0x54

    void InitReaderFunc(FontEncoding encode);

    CharStrmReader GetCharStrmReader() const {
        return CharStrmReader(mReadFunc);
    }

private:
    CharStrmReader::ReadFunc mReadFunc; // at 0x4
};

} // namespace ut
} // namespace nw4r

#endif
