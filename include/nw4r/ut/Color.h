#pragma once

#include "rvl/GX.h"
#include <common.h>


namespace nw4r {
namespace ut {

struct Color : public _GXColor {
    Color() {}
    /* 80007bc0 */ ~Color();
    Color(u32 color) {
        *(u32 *)this = color;
    }

    Color &operator=(u32 color) {
        *(u32 *)this = color;
        return *this;
    }
};

} // namespace ut
} // namespace nw4r
