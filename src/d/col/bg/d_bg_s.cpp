#include "d/col/bg/d_bg_s.h"

#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_s_roof_chk.h"
#include "d/col/bg/d_bg_s_wtr_chk.h"
#include "d/col/c/c_bg_s_shdw_draw.h"
#include "d/col/c/c_m3d.h"
#include "d/col/c/c_partition.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/scgame.h"

const char *MAP_SOLID_MATERIAL_NAMES[31] = {
    0,
    "tex/m_tuti.bti",
    "tex/m_rock.bti",
    "tex/m_sand.bti",
    "tex/m_grass.bti",
    "tex/m_tree.bti",
    0,
    0,
    "tex/m_stone.bti",
    0,
    "tex/m_metal.bti",
    0,
    0,
    0,
    "tex/m_carpet.bti",
    0,
    "tex/m_wood.bti",
    0,
    "tex/m_naraku.bti",
    0,
    "tex/m_tree.bti",
    "tex/m_tree.bti",
    "tex/m_stone.bti",
    "tex/m_metal.bti",
    "tex/m_metal.bti",
    0,
    0,
    0,
    0,
    0,
    "tex/m_stone.bti",
};

const char *MAP_SCROLL_MATERIAL_NAMES[5] = {
    "tex/m_lava.bti", "tex/m_water.bti", "tex/m_numa.bti", "tex/m_quicksand.bti", "tex/m_death.bti",
};

const char *SKYKEEP_STAGE_NAMES[8] = {
    "D003_0", "D003_1", "D003_2", "D003_3", "D003_4", "D003_5", "D003_6", "D003_7",
};

char MAP_GRADATION_FILE[] = "dat/MapGradation.dat";
char MAP_SCROLL_TEX_FILE[] = "dat/mapScrollTex_00.dat";

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
    if (mpBgKCol == nullptr) {
        mpBgKCol = pKCol;
        mAcOBg.link(pAcBg);
        return true;
    }
    return false;
}

bool cBgS::ReleaseKCol(dBgWKCol *pKCol) {
    if (mpBgKCol && mpBgKCol == pKCol) {
        mpBgKCol = nullptr;
        return true;
    }
    return false;
}

// u32 cBgS::GetGrpInf(cBgS_PolyInfo const &) const {}

// s32 cBgS::GetGrpRoomId(cBgS_PolyInfo const &) const {}

void dBgS::Regist(dBgW_Base *pBg, int idx) {
    mChkElem[idx].Regist2(pBg, mAcOBg.get());
}

dBgS *dBgS::GetInstance() {
    return spInstance;
}

dBgS::dBgS() : mMapTexture(0), mInSkyKeep(false), mList_0x388C(), mMapGradation() {
    // Sad TList :(
    spInstance = this;
}

dBgS::~dBgS() {
    spInstance = nullptr;
}

void dBgS::Ct() {
    cBgS::Ct();
    for (int i = 0; i < BG_ID_MAX; ++i) {
        mColllisionTbl[i] = nullptr;
    }

    // TODO
    OarcManager::sInstance->getData("Common", MAP_GRADATION_FILE);

    mInSkyKeep = false;
    for (int i = 0; i < 8; ++i) {
        if (ScGame::isCurrentStage(SKYKEEP_STAGE_NAMES[i])) {
            mInSkyKeep = true;
            return;
        }
    }
}

void dBgS::Dt() {
    cBgS::Dt();
    for (int i = 0; i < BG_ID_MAX; ++i) {
        mColllisionTbl[i] = nullptr;
    }
    ClearMapSegments();
}

void dBgS::ClrMoveFlag() {
    const cBgS_ChkElm *end = &mChkElem[mSetCounter];
    for (cBgS_ChkElm *chkElm = mChkElem; chkElm < end; chkElm++) {
        if (chkElm->mpBgW) {
            if (chkElm->mObj.get()) {
                chkElm->mpBgW->CalcDiffShapeAngleY(chkElm->mObj.get()->rotation.y);
            }
            chkElm->mpBgW->OffMoveFlag();
        }
    }
}

bool dBgS::Regist(dBgW_Base *pBg, dAcObjBase_c *pObj) {
    if (!pBg) {
        return true;
    }
    if (pBg->ChkUsed()) {
        return false;
    }
    if (pObj && pBg->ChkMoveBg()) {
        pBg->SetOldShapeAngleY(pObj->rotation.y);
        pBg->SetRoomId(pObj->getRoomId());
    }
    return cBgS::Regist(pBg, pObj);
}

bool dBgS::RegistBg(dBgW_Base *pBg, dAcObjBase_c *pObj) {
    // Bg is null, idk why it allows it
    if (!pBg) {
        return true;
    }

    // Already Registered
    if (pBg->GetRegistId() != BG_ID_MAX) {
        return false;
    }

    // Update RoomID if moved
    if (pObj && pBg->ChkMoveBg()) {
        pBg->SetRoomId(pObj->getRoomId());
    }

    // Check Bg
    if (pBg->ChkMemoryError() || pBg->ChkNotReady()) {
        return true;
    }

    for (int i = 0; i < BG_ID_MAX; ++i) {
        if (mColllisionTbl[i] == nullptr) {
            mColllisionTbl[i] = pBg;
            pBg->RegistBg(i);
            if (mColllisionTblLen < i + 1) {
                mColllisionTblLen = i + 1;
            }
            return false;
        }
    }

    pBg->UnRegistBg();
    return true;
}

bool dBgS::UnRegist(dBgW_Base *pBg) {
    if (!pBg) {
        return true;
    }

    u32 id = pBg->GetRegistId();

    bool ret;
    if (id == BG_ID_MAX) {
        return false;
    }

    // Not Quite Right
    if (id <= (BG_ID_MAX - 1) && mColllisionTbl[id] != nullptr) {
        mColllisionTbl[id] = nullptr;
        pBg->UnRegistBg();

        int oldLen = mColllisionTblLen;
        if (oldLen == (int)id + 1) {
            mColllisionTblLen = 0;
            // Reset the count
            for (int i = 0; i < oldLen; i++) {
                if (mColllisionTbl[i] != nullptr) {
                    mColllisionTblLen = i + 1;
                }
            }
        }
        ret = false;
    } else {
        ret = true;
    }

    return ret;
}

bool dBgS::ChkMoveBG(cBgS_PolyInfo const &info, bool bChkLock) {
    dBgW_Base *pBg = GetInstance()->GetBgWBasePointer(info);
    if (pBg) {
        if (bChkLock && pBg->ChkLock()) {
            return false;
        }

        if (pBg->ChkMoveBg()) {
            return true;
        }
    }
    return false;
}

u32 dBgS::ChkShadowThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetShdwThrough(info.GetPolyIndex());
    }
    return 0;
}

int dBgS::GetSpecialCode(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetSpecialCode(info);
    }
    return 0;
}

s32 dBgS::GetWallCode(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetWallCode(info);
    }
    return 0;
}

int dBgS::GetPolyAtt0(cBgS_PolyInfo const &info) {
    if (!ChkPolySafe(info)) {
        return 0;
    }
    if (GetSpecialCode(info) == 7 /* LAVA */) {
        return 6;
    }
    return mChkElem[info.GetBgIndex()].mpBgW->GetPolyAtt0Material(info);
}

int dBgS::GetPolyAtt1(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolyAtt1(info);
    }
    return 0;
}

// TODO: Map to Enums And Cleanup??
int dBgS::GetMapCode(int att0, int att1, bool bAlt) {
    if (bAlt && ScGame::currentSpawnInfo.trial == 1) {
        return 0x1E;
    }
    if (att0 == 4 && att1 == 1) {
        return 0x13;
    }
    if (att0 == 5) {
        if (att1 == 1) {
            return 0x14;
        }
        if (att1 == 2) {
            return 0x15;
        }
    } else {
        if ((att0 == 8) && (att1 == 1)) {
            return 0x16;
        }
        if (att0 == 10) {
            if (att1 == 1) {
                return 0x17;
            }
            if (att1 == 2) {
                return 0x18;
            }
        }
    }

    if ((att0 == 0xc) && (att1 == 1)) {
        return 0x19;
    }

    if (att0 == 0xd) {
        if (att1 == 1) {
            return 0x1a;
        }
        if (att1 == 2) {
            return 0x1b;
        }
        if (att1 == 3) {
            return 0x1C;
        }
    } else {
        if (att0 != 0xf) {
            return att0;
        }
        if (att1 == 1) {
            return 0x1d;
        }
    }
}

int dBgS::GetLightingCode(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetLightingCode(info);
    }
    return 0xF;
}

int dBgS::GetGroundCode(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetGroundCode(info);
    }
    return 0;
}

int dBgS::GetCode1_0x02000000(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetCode1_0x02000000(info.GetPolyIndex());
    }
    return 0;
}

s32 dBgS::GetRoomCamId(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetRoomCamId(info);
    }
    return 0xFF;
}

s32 dBgS::GetRoomId(cBgS_PolyInfo const &info) {
    if (!ChkPolySafe(info)) {
        return -1;
    }
    int roomId = mChkElem[info.GetBgIndex()].mpBgW->GetRoomId();
    if (roomId == 0xFF) {
        roomId = -1;
    }
    return roomId;
}

bool dBgS::GetPolyObjectThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolyObjThrough(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyCameraThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolyCamThrough(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyShadowThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetShdwThrough(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyLinkThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolyLinkThrough(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyArrowThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolyArrowThrough(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyBombThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolyBombThrough(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyBeetleThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolyBeetleThrough(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyClawshotThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolyClawshotThrough(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyThrough_Code1_0x04000000(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetCode1_0x04000000(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyThrough_Code1_0x08000000(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetCode1_0x08000000(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolyWhipThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolyWhipThrough(info.GetPolyIndex());
    }
    return false;
}

bool dBgS::GetPolySlingshotThrough(cBgS_PolyInfo const &info) {
    if (ChkPolySafe(info)) {
        return mChkElem[info.GetBgIndex()].mpBgW->GetPolySlingshotThrough(info.GetPolyIndex());
    }
    return false;
}

void dBgS::WallCorrect(dBgS_Acch *, bool) {}

f32 dBgS::RoofChk(dBgS_RoofChk *) {}

bool dBgS::SplGrpChk(dBgS_SplGrpChk *) {}

bool dBgS::SphChk(dBgS_SphChk *, void *) {}

void dBgS::MoveBgCrrPos(
    cBgS_PolyInfo const &, bool param_1, mVec3_c *i_pos, mAng3_c *i_angle, mAng3_c *i_shapeAngle, bool param_5,
    bool param_6
) {}

void dBgS::MoveBgTransPos(
    cBgS_PolyInfo const &i_poly, bool param_1, mVec3_c *i_pos, mAng3_c *i_angle, mAng3_c *i_shapeAngle
) {}

void dBgS_MoveBGProc_Typical(
    dBgW *param_0, void *param_1, cBgS_PolyInfo const &param_2, bool param_3, mVec3_c *param_4, mAng3_c *param_5,
    mAng3_c *param_6
) {}

void dBgS_MoveBGProc_RotY(
    dBgW *param_0, void *param_1, cBgS_PolyInfo const &param_2, bool param_3, mVec3_c *param_4, mAng3_c *param_5,
    mAng3_c *param_6
) {}

void dBgS_MoveBGProc_TypicalRotY(
    dBgW *param_0, void *param_1, cBgS_PolyInfo const &param_2, bool param_3, mVec3_c *param_4, mAng3_c *param_5,
    mAng3_c *param_6
) {}

void dBgS::RideCallBack(cBgS_PolyInfo const &, dAcBase_c *) {}

void dBgS::ArrowStickCallBack(cBgS_PolyInfo const &, dAcBase_c *, mVec3_c &) {}

void UnkCallback(cBgS_PolyInfo const &, dAcBase_c *) {}

dAcBase_c *dBgS::PushPullCallBack(cBgS_PolyInfo const &, dAcBase_c *, s16, dBgW_Base::PushPullLabel) {}

bool dBgS_CheckBWallPoly(cBgS_PolyInfo const &) {}

bool dBgS_CheckBGroundPoly(cBgS_PolyInfo const &) {}

bool dBgS_CheckBRoofPoly(cBgS_PolyInfo const &) {}

f32 dBgS_GetNY(cBgS_PolyInfo const &poly) {}

mVec3_c dBgS_GetN(cBgS_PolyInfo const &) {}

void dBgS::UpdateScrollTex() {}

void dBgS::SetupMapGX(mMtx_c *) {}

void dBgS::SetupMapMaterial(int matIdx, bool, s32 roomId) {}

UNKTYPE *dBgS::GetMapAccessor() {}

void dBgS::DrawMap(u8 roomId, mMtx_c *, bool bColor, int) {}

void dBgS::SetupScrollGX() {}

void dBgS::SetupScrollMaterial(int matIdx, s32, bool) {
    const char *_ = "dat/mapScrollTex_00.dat";
}

void dBgS::DrawMapScroll(u8 roomId, mMtx_c *, bool bColor, int) {}

void dBgS::DrawSkyKeepMap(mMtx_c *, int) {}

bool dBgS::ConfigureMapTexture(EGG::Heap *) {}

MapLineSegment::MapLineSegment() {}

MapLineSegment::~MapLineSegment() {}

void MapLineSegment::fn_8033e9a0() {}

void MapLineSegment::fn_8033e8b0() {}

void MapLineSegment::fn_8033e9c0() {}

void MapLineSegment::Append() {}

void MapLineSegment::Remove() {}

void MapLineSegment::Draw(int, mMtx_c *, bool, int) {}

void dBgS::InitMapParts() {
    for (int i = 0; i < 31; ++i) {
        if (MAP_SOLID_MATERIAL_NAMES[i]) {
            spSolidMatTex[i] = OarcManager::sInstance->getData("Common", MAP_SOLID_MATERIAL_NAMES[i]);
        } else {
            spSolidMatTex[i] = nullptr;
        }
    }
}

void dBgS::AppendMapSegment(MapLineSegment *) {}

void dBgS::RemoveMapSegment(MapLineSegment *) {}

void dBgS::DrawMapSegments(int p1, mMtx_c *p2, bool p3, int p4) {
    MapLineList::Iterator end, it;
    end = mList_0x388C.GetEndIter();
    it = mList_0x388C.GetBeginIter();
    while (it != end) {
        it->Draw(p1, p2, p3, p4);
        ++it;
    };
}

void dBgS::ClearMapSegments() {
    // Idk tbh
    for (MapLineList::Iterator prevIt, it = mList_0x388C.GetBeginIter(); it != mList_0x388C.GetEndIter(); it = prevIt) {
        prevIt = ++it;
        mList_0x388C.remove(&*prevIt);
    };
}

void dBgS::SetLightingCode(dAcObjBase_c *pObj, const cBgS_PolyInfo &info) {
    const dAcObjBase_c *actor = GetInstance()->GetActorPointer(info);
    if (actor && actor->mLightingInfo.mLightingCode != 0xF) {
        pObj->mLightingInfo.mLightingCode = actor->mLightingInfo.mLightingCode;
    } else {
        pObj->mLightingInfo.mLightingCode = GetLightingCode(info);
    }
}

f32 dBgS::SetLightingCode(dAcObjBase_c *pObj, f32 height) {
    dBgS_ObjGndChk objGndChk;
    mVec3_c pos = pObj->GetPostion();
    pos.y += height;
    objGndChk.SetPos(&pos);
    f32 gndCross = GroundCross(&objGndChk);
    if (gndCross != -1.0e9f) {
        SetLightingCode(pObj, objGndChk);
    }
    return gndCross;
}

int dBgS::GetLightingCode(const mVec3_c *pPos) {
    dBgS_ObjGndChk objGndChk;
    objGndChk.SetPos(pPos);
    if (GroundCross(&objGndChk) != -1.0e9f) {
        return GetLightingCode(objGndChk);
    } else {
        return 0xF;
    }
}

bool dBgS::GetPolyPreventObjOnly(const cBgS_PolyInfo &info) {
    if (!GetPolyObjectThrough(info) && GetPolyLinkThrough(info) && GetPolyCameraThrough(info) &&
        GetPolyShadowThrough(info) && GetPolyArrowThrough(info) && GetPolyBombThrough(info) &&
        GetPolyBeetleThrough(info) && GetPolyClawshotThrough(info) && GetPolyThrough_Code1_0x08000000(info) &&
        GetPolyWhipThrough(info) && GetPolyThrough_Code1_0x04000000(info)) {
        return true;
    }
    return false;
}

bool dBgS::GetMapGradationColor(GXColor *pColor) {
    if (!mMapGradation.mHasGradation) {
        return false;
    }
    pColor->r = mMapGradation.mColorR;
    pColor->g = mMapGradation.mColorG;
    pColor->b = mMapGradation.mColorB;
    pColor->a = mMapGradation.mColorA;
    return true;
}
