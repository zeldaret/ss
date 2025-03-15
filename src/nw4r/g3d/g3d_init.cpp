#include "nw4r/g3d.h" // IWYU pragma: export

#include "nw4r/ut.h" // IWYU pragma: export

#include "rvl/GX.h" // IWYU pragma: export
#include "rvl/OS.h" // IWYU pragma: export
#include "rvl/VI.h"

namespace {

NW4R_LIB_VERSION(G3D, "Dec 22 2009", "02:11:33", "0x4302_158");

} // namespace

namespace nw4r {
namespace g3d {

void G3dInit(bool enableLockedCache) {
    OSRegisterVersion(NW4R_G3D_Version_);

    if (enableLockedCache) {
        ut::LC::Enable();
    } else {
        ut::LC::Disable();
    }

    InitFastCast();

    GXRenderModeObj *pMode;
    switch ((u32)VIGetTvFormat()) {
        case VI_TV_FMT_NTSC: {
            pMode = &GXNtsc480IntDf;
            break;
        }

        case VI_TV_FMT_PAL: {
            pMode = &GXPal528IntDf;
            break;
        }

        case VI_TV_FMT_EURGB60: {
            pMode = &GXEurgb60Hz480IntDf;
            break;
        }

        case VI_TV_FMT_MPAL: {
            pMode = &GXMpal480IntDf;
            break;
        }

        default: {
            pMode = &GXNtsc480IntDf;
            break;
        }
    }

    G3DState::SetRenderModeObj(*pMode);
}

void G3dReset() {
    G3DState::Invalidate();
}

} // namespace g3d
} // namespace nw4r
