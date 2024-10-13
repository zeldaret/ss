#ifndef D_BG_S_CHK_H
#define D_BG_S_CHK_H

// Include order matters here
// clang-format off
#include "d/col/bg/d_bg_s_poly_pass_chk.h"
#include "d/col/bg/d_bg_s_grp_pass_chk.h"
// clang-format on

class dBgS_Chk : public dBgS_PolyPassChk, public dBgS_GrpPassChk {
public:
    dBgS_Chk();
    cBgS_GrpPassChk *GetGrpPassChkInfo();
    cBgS_PolyPassChk *GetPolyPassChkInfo();

    virtual ~dBgS_Chk();
};

#endif
