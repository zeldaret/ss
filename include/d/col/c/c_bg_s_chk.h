#ifndef C_BG_S_CHK_H
#define C_BG_S_CHK_H

#include "d/col/bg/d_bg_w_base.h"
#include "d/col/c/c_partition.h"
#include "f/f_base.h"
#include "rvl/MTX.h"

struct cBgD_Vtx_t : public Vec {};

class cBgS_GrpPassChk {
public:
    virtual ~cBgS_GrpPassChk() {}
};

class cBgS_PolyPassChk;

class cBgS_Chk {
public:
    /* 0x00 */ cBgS_PolyPassChk *mPolyPassChk;
    /* 0x04 */ cBgS_GrpPassChk *mGrpPassChk;
    /* 0x08 */ u32 mActorId;
    /* 0x0C */ u16 mField_0x0C;
    /* 0x0E */ u16 mField_0x0E;
    /* 0x10 */ u8 mField_0x10;
    /* 0x14 */ cPartition mPartition;
    /* 0x24 */ // __vtable__

public:
    cBgS_Chk();
    // vt at 0x24
    virtual ~cBgS_Chk();
    void SetActorID(u32 *);

    void SetPolyPassChk(cBgS_PolyPassChk *p_chk) {
        mPolyPassChk = p_chk;
    }
    void SetGrpPassChk(cBgS_GrpPassChk *p_chk) {
        mGrpPassChk = p_chk;
    }
    cBgS_PolyPassChk *GetPolyPassChk() const {
        return mPolyPassChk;
    }
    cBgS_GrpPassChk *GetGrpPassChk() const {
        return mGrpPassChk;
    }

    int ChkSameActorPid(const fBase_c *pActor) const {
        int id = pActor ? pActor->unique_ID : 0;
        if (id == 0 || mActorId == 0 || mField_0x10 == 0) {
            return false;
        }
        return mActorId == id ? 1 : 0;
    }

    bool ChkField_0xC(const dBgW_Base *pBg) const {
        return mField_0x0C & pBg->GetField_0x20();
    }

    bool ChkField_0xE(const dBgW_Base *pBg) const {
        return mField_0x0E & pBg->GetField_0x22();
    }
};

#endif
