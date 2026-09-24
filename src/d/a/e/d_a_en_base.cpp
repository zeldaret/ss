#include "d/a/e/d_a_en_base.h"

#include "d/d_stage_mgr.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_camera.h"

bool dAcEnBase_c::isWithinCameraView(f32 w, f32 h) {
    nw4r::g3d::Camera cam = dStageMgr_c::GetInstance()->getCamera(0);

    f32 x, y, width, height, near, far;
    cam.GetViewport(&x, &y, &width, &height, &near, &far);

    mVec3_c src;
    cam.Project(src, mPosition);

    return ((width * w <= src.x) && (src.x < width * (1.0f - w))) &&
           ((height * h <= src.y) && src.y < height * (1.0f - h)) && ((near <= src.z) && (src.z < far));
}
