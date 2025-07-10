#include "d/snd/d_snd_source_obj.h"

#include "common.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/math/math_types.h"

void dSndSourceObj_c::postSetup() {
    if (isName("Fire")) {
        if (dSndStateMgr_c::isInStage("F008r")) {
            // Inside Goddess Statue
            setVolumeFade(0.7f, 0);
        }
        return;
    }

    if ((isName("TDoor") || isName("Wind") || isName("Flag")) &&
        dSndStateMgr_c::GetInstance()->getLayer() >= 13) {
        setVolumeFade(0.0f, 0);
    }
}

void dSndSourceObjLightShaft_c::setPosition(const nw4r::math::VEC3 &position) {
    if (dSndSourceMgr_c::getPlayerSource() != nullptr) {
        f32 y = dSndSourceMgr_c::getPlayerSource()->GetPosition().y;
        nw4r::math::VEC3 v = position;
        if (y > 0.0f) {
            v.y = y;
        } else {
            v.y = 0.0f;
        }
        SetPosition(v);
        mFlags = 0;
        holdSound(SE_LightLi_LV);
    }
}
