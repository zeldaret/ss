#include "d/col/bg/d_bg_s_wtr_chk.h"

#include "d/col/bg/d_bg_s.h"

dBgS_WtrChk dBgS_WtrChk::sInstance;
f32 dBgS_WtrChk::sWaterHeight;

dBgS_WtrChk::dBgS_WtrChk() {
    OnWaterGrp();
}

dBgS_WtrChk::~dBgS_WtrChk() {}

void dBgS_WtrChk::ClearInstance() {
    sInstance.ClearPi();
}

bool dBgS_WtrChk::CheckPos(const mVec3_c *pPos, bool b, f32 f1, f32 f2) {
    mVec3_c water_pos;
    water_pos.set(pPos->x, pPos->y + f2, pPos->z);
    sInstance.Set(water_pos, pPos->y + f1);

    if (dBgS::GetInstance()->WaterChk(&sInstance)) {
        if (b && dBgS::GetInstance()->GetSpecialCode(sInstance) != 0xC /* POLY_GROUND_WATER */) {
            // Other Liquid
        } else {
            sWaterHeight = sInstance.mGroundH;
            return true;
        }
    }
    return false;
}

int dBgS_WtrChk::GetMaterial() {
    return dBgS::GetInstance()->GetPolyAtt0(sInstance);
}
