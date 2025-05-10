#include "egg/gfx/eggGlobalDrawState.h"

#include "common.h"
#include "egg/gfx/eggStateEfb.h"
#include "egg/gfx/eggStateGX.h"
#include "rvl/MTX/mtx.h"

namespace EGG {

Screen const *GlobalDrawState::spScreen;
u16 GlobalDrawState::sCameraId;
u32 GlobalDrawState::sDrawFlag;

// unknown
nw4r::math::MTX34 GlobalDrawState::sMtx;

GlobalDrawState sState;

void GlobalDrawState::beginDrawView(u16 cameraId, const nw4r::math::MTX34 &mtx, const Screen &screen) {
    spScreen = &screen;
    sCameraId = cameraId;
    PSMTXCopy(mtx, sMtx);
    StateEfb::Clean();
}

void GlobalDrawState::setDrawSettingGX(bool b1, bool b2) {
    StateGX::GXSetColorUpdate_((sDrawFlag & 1) != 0);
    StateGX::GXSetAlphaUpdate_((sDrawFlag & 2) != 0);
    StateGX::GXSetDither_((sDrawFlag & 4) != 0);
    StateGX::GXSetDstAlpha_(false, 0);
    if (b2 && spScreen != nullptr) {
        spScreen->SetProjectionGX();
    }
}


} // namespace EGG
