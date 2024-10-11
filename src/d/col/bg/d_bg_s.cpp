#include "d/col/bg/d_bg_s.h"

#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_s_roof_chk.h"
#include "d/col/bg/d_bg_s_wtr_chk.h"
#include "d/col/c/c_bg_s_shdw_draw.h"
#include "d/col/c/c_m3d.h"
#include "d/col/c/c_partition.h"

cBgS_ChkElm::cBgS_ChkElm() {
    Init();
}

cBgS_ChkElm::~cBgS_ChkElm() {}

void cBgS_ChkElm::Init() {
    mpBgW = nullptr;
    mObj.unlink();
}

void cBgS_ChkElm::Regist2(dBgW_Base *pdBgW, dAcObjBase_c *pObj) {
    mpBgW = pdBgW;
    if (pObj) {
        mObj.link(pObj);
    } else {
        mObj.unlink();
    }
}

void cBgS_ChkElm::Release() {
    Init();
}

cBgS::cBgS() : mSetCounter(0) {}

cBgS::~cBgS() {}

void cBgS::fn_80339de0(dBgW_Base *pBgW) {
    if (!cPartition::fn_80339070(pBgW->GetBnd())) {
        return pBgW->vt_0x3C();
    }
}

bool cBgS::Regist(dBgW_Base *pBgBase, dAcObjBase_c *pObj) {
    if (pBgBase->ChkMemoryError() || pBgBase->ChkNotReady()) {
        return true;
    }

    for (s32 i = 0; i < ARRAY_LENGTH(mChkElem); ++i) {
        // If not assigned already
        if (mChkElem[i].mpBgW == nullptr) {
            mChkElem[i].Regist2(pBgBase, pObj);
            pBgBase->Regist(i);
            // Set next slot available?
            if (mSetCounter < i + 1) {
                mSetCounter = i + 1;
            }
            fn_80339de0(pBgBase);
            return false;
        }
    }
    pBgBase->Release();

    return true;
}

bool cBgS::Release(dBgW_Base *pBgW) {
    if (pBgW == nullptr) {
        return true;
    }

    if (!pBgW->ChkUsed()) {
        return false;
    }

    u32 id = pBgW->GetId();
    if (id <= BG_ID_MAX - 1 && mChkElem[id].mpBgW != nullptr) {
        mChkElem[id].Release();
        pBgW->Release();
        s32 oldSet = mSetCounter;
        if (mSetCounter == (s32)id + 1) {
            mSetCounter = 0;
            for (int i = 0; i < oldSet; ++i) {
                if (mChkElem[i].mpBgW) {
                    mSetCounter = i + 1;
                }
            }
        }
    } else {
        return true;
    }

    return false;
}

void cBgS::Ct() {
    mSetCounter = 0;
    for (int i = 0; i < BG_ID_MAX; ++i) {
        mChkElem[i].Init();
    }
    cPartition::fn_80338fb0();
    mpBgKCol = nullptr;
    mAcOBg.unlink();
    dBgS_ObjGndChk::CtInstance();
    dBgS_ObjLinChk::CtInstance();
    dBgS_WtrLinChk::CtInstance();
    dBgS_ObjRoofChk::CtInstance();
    dBgS_WtrChk::CtInstance();
}

void cBgS::Dt() {
    for (int i = 0; i < BG_ID_MAX; ++i) {
        if (mChkElem[i].mpBgW != nullptr) {
            mChkElem[i].Release();
        }
        mChkElem[i].Init();
    }
    mSetCounter = 0;
}

struct cBgD_Grp_t_ {
    u32 strOffset;
    u8 data[0x30];
};

void cBgS::ConvDzb(void *pDzb) {
    // this is identical to cBgD_t except using u32's for the table offsets.
    // u32 is needed to match in ConvDzb ?
    struct cBgD_t_ {
        /* 0x00 */ int mVtxNum;
        /* 0x04 */ u32 mVtxTbl;
        /* 0x08 */ int mTriNum;
        /* 0x0C */ u32 mTriTbl;
        /* 0x10 */ int mBlkNum;
        /* 0x14 */ u32 mBlkTbl;
        /* 0x18 */ int mTreeNum;
        /* 0x1C */ u32 mTreeTbl;
        /* 0x20 */ int mGrpNum;
        /* 0x24 */ u32 mGrpTbl;
        /* 0x28 */ int mTiNum;
        /* 0x2C */ u32 mTiTbl;
        /* 0x30 */ u32 mFlags;
    };
    cBgD_t_ *pbgd = (cBgD_t_ *)pDzb;

    if ((pbgd->mFlags & 0x80000000) == 0) {
        pbgd->mFlags |= 0x80000000;
    } else {
        return;
    }

    if (pbgd->mVtxTbl != 0) {
        pbgd->mVtxTbl += (u32)pDzb;
    }

    pbgd->mTriTbl += (u32)pDzb;
    pbgd->mBlkTbl += (u32)pDzb;
    pbgd->mTreeTbl += (u32)pDzb;
    pbgd->mGrpTbl += (u32)pDzb;
    pbgd->mTiTbl += (u32)pDzb;

    for (int i = 0; i < pbgd->mGrpNum; ++i) {
        cBgD_Grp_t_ &grp = ((cBgD_Grp_t_ *)pbgd->mGrpTbl)[i];
        grp.strOffset = (u32)pbgd + (int)grp.strOffset;
    }
}

void cBgS::fn_8033a1e0() {
    cBgS_ChkElm *endElm = &mChkElem[mSetCounter];
    for (cBgS_ChkElm *chk_elm = &mChkElem[0]; chk_elm < endElm; ++chk_elm) {
        if (chk_elm->ChkUsed()) {
            chk_elm->mpBgW->vt_0x3C();
        }
    }
}

bool cBgS::LineCross(cBgS_LinChk *pLine) {
    pLine->ClearPi();
    pLine->ClrHit();
    cM3dGAab aabb;
    aabb.SetBounds(pLine->mLin.mStart, pLine->mLin.mEnd);
    pLine->mPartition.fn_803391f0(&aabb);

    cBgS_ChkElm *chkElm = mChkElem;
    int i = 0;
    do {
        if (chkElm->CheckAll(pLine) && chkElm->mpBgW->LineCheck(pLine)) {
            const dAcObjBase_c *pObj = chkElm->mObj.get();
            pLine->SetActorInfo(i, chkElm->mpBgW, pObj ? pObj->unique_ID : 0);
            pLine->SetHit();
        }

        i++;
        chkElm++;
    } while (i < mSetCounter);

    if (pLine->ChkHit()) {
        pLine->vt_0xC(&pLine->mLin.mEnd);
        return true;
    } else {
        return false;
    }
}

f32 cBgS::GroundCross(cBgS_GndChk *pGnd) {
    pGnd->SetNowY(-1.0e9);
    pGnd->ClearPi();
    pGnd->mPartition.Calc(&pGnd->mPos);

    pGnd->SetField_0x68(pGnd->mPos);

    cBgS_ChkElm *chkElm = mChkElem;
    int i = 0;
    do {
        if (chkElm->CheckAll(pGnd) && chkElm->mpBgW->GroundCross(pGnd)) {
            const dAcObjBase_c *pObj = chkElm->mObj.get();
            pGnd->SetActorInfo(i, chkElm->mpBgW, pObj ? pObj->unique_ID : 0);
        }
        i++;
        chkElm++;
    } while (i < mSetCounter);

    return pGnd->GetNowY();
}

void cBgS::ShdwDraw(cBgS_ShdwDraw *pShdw) {
    pShdw->mPartition.fn_803391f0(pShdw->GetBndP());

    const cBgS_ChkElm *chkElm = mChkElem;
    const cBgS_ChkElm *end = &mChkElem[mSetCounter];
    do {
        if (chkElm->CheckAll(pShdw)) {
            chkElm->mpBgW->ShdwDraw(pShdw);
        }
        chkElm++;
    } while (chkElm < end);
}

const dAcObjBase_c *cBgS::GetActorPointer(int idx) const {
    return mChkElem[idx].mObj.get();
}

const dAcObjBase_c *cBgS::GetActorPointer(const cBgS_PolyInfo &info) const {
    if (!ChkPolySafe(info)) {
        return nullptr;
    }
    return GetActorPointer(info.GetBgIndex());
}

dBgW_Base *cBgS::GetBgWBasePointer(cBgS_PolyInfo const &info) const {
    if (!ChkPolySafe(info)) {
        return nullptr;
    }
    return mChkElem[info.GetBgIndex()].mpBgW;
}

bool cBgS::ChkPolySafe(cBgS_PolyInfo const &info) const {
    if (!info.ChkSetInfo()) {
        return false;
    }

    u32 bgIndex = info.GetBgIndex();
    if (bgIndex > BG_ID_MAX - 1) {
        return false;
    }

    if (mChkElem[bgIndex].mpBgW == nullptr) {
        return false;
    }
    const dAcObjBase_c *pObj = mChkElem[bgIndex].mObj.get();
    return info.ChkSafe(mChkElem[bgIndex].mpBgW, pObj ? pObj->unique_ID : 0);
}

bool cBgS::GetTriPla(cBgS_PolyInfo const &info, cM3dGPla *pPlane) const {
    if (!ChkPolySafe(info)) {
        return nullptr;
    }
    mChkElem[info.GetBgIndex()].mpBgW->GetTriPla(info, pPlane);
    return true;
}

bool cBgS::GetTriPnt(cBgS_PolyInfo const &info, mVec3_c *pVA, mVec3_c *pVB, mVec3_c *pVC) const {
    if (!ChkPolySafe(info)) {
        return nullptr;
    }
    return mChkElem[info.GetBgIndex()].mpBgW->GetTriPnt(info, pVA, pVB, pVC);
}

bool cBgS::RegistKCol(dBgWKCol *pKCol, dAcObg_c *pAcBg) {
    if (pKCol == nullptr) {
        return false;
    }
    mpBgKCol = pKCol;
    mAcOBg.link(pAcBg);
    return true;
}

u32 cBgS::GetGrpInf(cBgS_PolyInfo const &) const {}

s32 cBgS::GetGrpRoomId(cBgS_PolyInfo const &) const {}
