#ifndef EGG_FRUSTUM_H
#define EGG_FRUSTUM_H

#include <common.h>
#include <nw4r/g3d/g3d_camera.h>

namespace EGG {

// TODO: Fill out more
class Frustum {
public:
    enum CanvasMode {};
    enum ProjectionType {};

    u8 TODO_0x00[0x38];
    // vt at 0x38
    virtual ~Frustum();
    virtual void SetProjectionGX() const;
    virtual void CopyToG3D(nw4r::g3d::Camera) const;
};

} // namespace EGG

#endif
