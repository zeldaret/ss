#ifndef EGG_DRAWGX_H
#define EGG_DRAWGX_H

#include <common.h>
#include <egg/core/eggHeap.h>
#include <egg/math/eggMatrix.h>
#include <rvl/GX.h>

namespace EGG {

class DrawGX {
public:
    static void Initialize(Heap *);

    static GXTexMapID GetTexMapDefault();

    // TODO MORE

    /* static ?? s_DL */

    static Matrix34f s_cameraMtx;

    /* static ?? s_flag */

    static GXColor BLACK;
    static GXColor WHITE;
    static GXColor GREEN;
    static GXColor RED;
    static GXColor BLUE;
};

} // namespace EGG

#endif
