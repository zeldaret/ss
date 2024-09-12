#ifndef EGG_SCREEN_H
#define EGG_SCREEN_H

#include <common.h>
#include <egg/gfx/eggFrustum.h>

namespace EGG {

// TODO: Fill out more
class Screen : public Frustum {
public:
    Screen();
    Screen(f32, f32, f32, f32, Screen *, CanvasMode);

    virtual ~Screen();
    virtual void SetProjectionGX() const override;
    virtual void CopyToG3D(nw4r::g3d::Camera) const override;

    static void Initialize(const u16 *, const u16 *, Screen *);
    static void SetTVModeDefault();

    u8 TODO_0x3C[0x88 - 0x3C];
};

} // namespace EGG

#endif
