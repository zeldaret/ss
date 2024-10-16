#ifndef NW4R_UT_CHAR_WRITER_H
#define NW4R_UT_CHAR_WRITER_H
#include "nw4r/math.h" // IWYU pragma: export
#include "nw4r/types_nw4r.h"
#include "nw4r/ut/ut_Color.h"
#include "rvl/GX.h" // IWYU pragma: export

namespace nw4r {
namespace ut {

class CharWriter {
public:
    enum GradationMode {
        GRADMODE_NONE,
        GRADMODE_H,
        GRADMODE_V
    };

    struct ColorMapping {
        Color min; // at 0x0
        Color max; // at 0x4
    };

    struct VertexColor {
        Color tl; // at 0x0
        Color tr; // at 0x4
        Color bl; // at 0x8
        Color br; // at 0xC
    };

    struct TextColor {
        Color start;            // at 0x0
        Color end;              // at 0x4
        GradationMode gradMode; // at 0x8
    };

    struct TextureFilter {
        GXTexFilter atSmall; // at 0x0
        GXTexFilter atLarge; // at 0x4

        bool operator!=(const TextureFilter &other) const {
            return atSmall != other.atSmall || atLarge != other.atLarge;
        }
    };

    struct LoadingTexture {
        GXTexMapID slot;      // at 0x0
        void *texture;        // at 0x4
        TextureFilter filter; // at 0x8

        bool operator!=(const LoadingTexture &other) const {
            return slot != other.slot || texture != other.texture || filter != other.filter;
        }

        void Reset() {
            slot = GX_TEXMAP_NULL;
            texture = NULL;
        }
    };

public:
    static void SetupVertexFormat();
    static void SetupGXDefault();
    static void SetupGXWithColorMapping(Color min, Color max);
    static void SetupGXForI();
    static void SetupGXForRGBA();

    CharWriter();
    ~CharWriter();

    void SetColorMapping(Color min, Color max) {
        mColorMapping.min = min;
        mColorMapping.max = max;
    }

    void ResetColorMapping() {
        SetColorMapping(Color(0x00000000), Color(0xFFFFFFFF));
    }

    void SetTextColor(Color start) {
        mTextColor.start = start;
        UpdateVertexColor();
    }

    void SetTextColor(Color start, Color end) {
        mTextColor.start = start;
        mTextColor.end = end;
        UpdateVertexColor();
    }

    void SetGradationMode(GradationMode mode) {
        mTextColor.gradMode = mode;
        UpdateVertexColor();
    }

    f32 GetScaleH() const {
        return mScale.x;
    }
    f32 GetScaleV() const {
        return mScale.y;
    }

    void SetScale(f32 x, f32 y) {
        mScale.x = x;
        mScale.y = y;
    }

    f32 GetCursorX() const {
        return mCursorPos.x;
    }
    void SetCursorX(f32 x) {
        mCursorPos.x = x;
    }

    f32 GetCursorY() const {
        return mCursorPos.y;
    }
    void SetCursorY(f32 y) {
        mCursorPos.y = y;
    }

    void SetCursor(f32 x, f32 y) {
        mCursorPos.x = x;
        mCursorPos.y = y;
    }
    void SetCursor(f32 x, f32 y, f32 z) {
        mCursorPos.x = x;
        mCursorPos.y = y;
        mCursorPos.z = z;
    }

    void MoveCursorX(f32 dx) {
        mCursorPos.x += dx;
    }
    void MoveCursorY(f32 dy) {
        mCursorPos.y += dy;
    }

    void EnableFixedWidth(bool enable) {
        mIsWidthFixed = enable;
    }
    bool IsWidthFixed() const {
        return mIsWidthFixed;
    }

    void SetFixedWidth(f32 width) {
        mFixedWidth = width;
    }
    f32 GetFixedWidth() const {
        return mFixedWidth;
    }

    void SetFont(const Font &font) {
        mFont = &font;
    }
    const Font *GetFont() const {
        return mFont;
    }

    void ResetTextureCache() {
        mLoadingTexture.Reset();
    }

    void SetupGX();
    void SetFontSize(f32 width, f32 height);
    void SetFontSize(f32 height);
    f32 GetFontWidth() const;
    f32 GetFontHeight() const;
    f32 GetFontAscent() const;
    f32 GetFontDescent() const;
    void EnableLinearFilter(bool atSmall, bool atLarge);
    f32 Print(u16 ch);
    void PrintGlyph(f32 x, f32 y, f32 z, const Glyph &glyph);
    void LoadTexture(const Glyph &glyph, GXTexMapID slot);
    void UpdateVertexColor();

private:
    ColorMapping mColorMapping; // at 0x0
    VertexColor mVertexColor;   // at 0x8
    TextColor mTextColor;       // at 0x18
    math::VEC2 mScale;          // at 0x24
    math::VEC3 mCursorPos;      // at 0x2C
    TextureFilter mFilter;      // at 0x38
    u8 padding[2];              // at 0x40
    u8 mAlpha;                  // at 0x42
    bool mIsWidthFixed;         // at 0x43
    f32 mFixedWidth;            // at 0x44
    const Font *mFont;          // at 0x48

    static LoadingTexture mLoadingTexture;
};

} // namespace ut
} // namespace nw4r

#endif
