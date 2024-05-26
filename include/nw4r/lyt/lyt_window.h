#ifndef NW4R_LYT_WINDOW_H
#define NW4R_LYT_WINDOW_H

#include <common.h>
#include <nw4r/lyt/lyt_common.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_types.h>

namespace nw4r {

namespace lyt {

struct Content {
    ut::Color vtxColors[4];          // at 0x00
    detail::TexCoordAry texCoordAry; // at 0x10
};

struct Frame {
    bool textureFlip;    // at 0x00
    Material *pMaterial; // at 0x04
};

struct WindowFramSize {
    f32 l; // at 0x00
    f32 r; // at 0x04
    f32 t; // at 0x08
    f32 b; // at 0x0C
};

class Window : public Pane {
public:
    Window(const res::Window *, const ResBlockSet &ResBlockSet);
    virtual ~Window();
    NW4R_UT_RTTI_DECL(Window);

private:
    res::InflationLRTB mContentInflation; // at 0x0D8
    Content mContent;                     // at 0x0E8
    Frame *mFrams;                        // at 0x100
    u8 mFrameNum;                         // 0x104
};

} // namespace lyt

} // namespace nw4r

#endif
