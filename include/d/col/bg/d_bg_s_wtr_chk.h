#ifndef D_BG_S_WTR_CHK_H
#define D_BG_S_WTR_CHK_H

#include "d/col/bg/d_bg_s_spl_grp_chk.h"

class dBgS_WtrChk : public dBgS_SplGrpChk {
public:
    dBgS_WtrChk();

    virtual ~dBgS_WtrChk();

    static dBgS_WtrChk &GetInstance() {
        return sInstance;
    }
    static f32 GetWaterHeight() {
        return sWaterHeight;
    }

    static void ClearInstance();
    static bool CheckPos(const mVec3_c *, bool, f32, f32);
    static int GetMaterial();

private:
    static dBgS_WtrChk sInstance;
    static f32 sWaterHeight;
};

#endif
