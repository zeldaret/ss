#ifndef NW4R_UT_COLOR_H
#define NW4R_UT_COLOR_H

#include "nw4r/types_nw4r.h"
#include "rvl/GX.h" // IWYU pragma: export

namespace nw4r {
namespace ut {

class Color : public GXColor {
public:
    Color() {
        *this = 0xFFFFFFFF;
    }
    Color(u32 color) {
        *this = color;
    }
    Color(int red, int green, int blue, int alpha) {
        Set(red, green, blue, alpha);
    }
    Color(const GXColor &clr) {
        ToU32ref() = *(u32 *)&clr;
    }
    ~Color() {}

    void Set(int red, int green, int blue, int alpha) {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    Color &operator=(u32 color) {
        ToU32ref() = color;
        return *this;
    }

    Color &operator=(const GXColor &c) {
        ToU32ref() = *(u32 *)&c; // TODO -> This Seems Maybe Wrong
        return *this;
    }

    Color operator|(u32 color) {
        return Color(ToU32() | color);
    }
    Color operator&(u32 color) {
        return Color(ToU32() & color);
    }

    u32 &ToU32ref() {
        return *reinterpret_cast<u32 *>(this);
    }
    const u32 &ToU32ref() const {
        return *reinterpret_cast<const u32 *>(this);
    }

    u32 ToU32() const {
        return ToU32ref();
    }

    operator u32() const {
        return ToU32ref();
    }
    // clang-format off
    static const u32 RED   = 0xFF0000FF;
    static const u32 GREEN = 0x00FF00FF;
    static const u32 BLUE  = 0x0000FFFF;

    static const u32 CYAN    = 0x00FFFFFF;
    static const u32 MAGENTA = 0xFF00FFFF;
    static const u32 YELLOW  = 0xFFFF00FF;

    static const u32 BLACK = 0x000000FF;
    static const u32 GRAY  = 0x808080FF;
    static const u32 WHITE = 0xFFFFFFFF;
    // clang-format on
} ALIGN_DECL(4);

} // namespace ut
} // namespace nw4r

#endif
