#ifndef D_BG_S_WTR_CHK_H
#define D_BG_S_WTR_CHK_H

#include "d/col/bg/d_bg_s_spl_grp_chk.h"

class dBgS_WtrChk : public dBgS_SplGrpChk {
public:
    dBgS_WtrChk();

    virtual ~dBgS_WtrChk();

    dBgS_WtrChk &GetInstance();
    static void ClearInstance();

    static dBgS_WtrChk sInstance;
};

#endif
