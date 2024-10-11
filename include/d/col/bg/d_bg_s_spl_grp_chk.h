#ifndef D_BG_S_SPL_GRP_CHK_H
#define D_BG_S_SPL_GRP_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "m/m_vec.h"

class dBgS_SplGrpChk : public cBgS_PolyInfo, public cBgS_Chk, public dBgS_Chk {
public:
    enum {
        /* 0x1 */ FIND = 1,
        /* 0x2 */ IN = 2,
        /* 0x4 */ MOVE_BG_ONLY = 4
    };

    void Set(mVec3_c &, f32);
    dBgS_SplGrpChk();
    void Init();

    virtual ~dBgS_SplGrpChk();

    void OffFind() {
        m_flags &= ~FIND;
    }
    void OffIn() {
        m_flags &= ~IN;
    }
    void OffMoveBGOnly() {
        m_flags &= ~MOVE_BG_ONLY;
    }
    void OnMoveBGOnly() {
        m_flags |= MOVE_BG_ONLY;
    }
    void OnFind() {
        m_flags |= FIND;
    }
    bool ChkMoveBGOnly() {
        return m_flags & MOVE_BG_ONLY;
    }
    void SetHeight(f32 height) {
        m_height = height;
    }
    f32 GetHeight() {
        return m_height;
    }
    mVec3_c &GetPosP() {
        return m_ground;
    }
    f32 GetRoof() const {
        return m_roof;
    }

private:
    mVec3_c m_ground;
    f32 m_roof;
    f32 m_height;
    u32 m_flags;
};

#endif
