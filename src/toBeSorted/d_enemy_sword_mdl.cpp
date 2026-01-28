#include "toBeSorted/d_enemy_sword_mdl.h"

#include "common.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_sc_game.h"
#include "m/m3d/m_shadow.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

dCcD_SrcCps sCcSrc = {
    {
     {AT_TYPE_DAMAGE, 0x8D, {1, 0, 0}, 1, 0, 0, 0, 0, 0},
     {0, 0, {0, 0, 0x407}, 0, 0},
     {0},
     },
    {30.0f},
};

// Yeah these two functions do the same thing. Could've made one call the other...

bool dEnemySwordMdl_c::create(
    mAllocator_c *alloc, void *pData, const char *mdlName, u32 bufferOption, const mVec3_c &v1, const mVec3_c &v2,
    cCcD_Stts &stts, EGG::ResTIMG *pImg, int nView, u32 *pSize
) {
    nw4r::g3d::ResMdl mdl = nw4r::g3d::ResFile(pData).GetResMdl(mdlName);
    if (!mMdl.create(mdl, alloc, bufferOption, nView, pSize)) {
        return false;
    }

    if (!mProc.create(100, alloc, pImg, 0.015f)) {
        mMdl.remove();
        return false;
    }

    field_0x074.set(v1.x, v1.y, v1.z);
    field_0x080.set(v2.x, v2.y, v2.z);

    for (int i = 0; i < 3; i++) {
        mCcList.addCc(mCcs[i], sCcSrc);
    }
    mCcList.SetStts(stts);
    mCcList.ClrAt();
    return true;
}

bool dEnemySwordMdl_c::create(
    mAllocator_c *alloc, nw4r::g3d::ResFile resFile, const char *mdlName, u32 bufferOption, const mVec3_c &v1,
    const mVec3_c &v2, cCcD_Stts &stts, EGG::ResTIMG *pImg, int nView, u32 *pSize
) {
    nw4r::g3d::ResMdl mdl = resFile.GetResMdl(mdlName);
    if (!mMdl.create(mdl, alloc, bufferOption, nView, pSize)) {
        return false;
    }

    if (!mProc.create(100, alloc, pImg, 0.015f)) {
        mMdl.remove();
        return false;
    }

    field_0x074.set(v1.x, v1.y, v1.z);
    field_0x080.set(v2.x, v2.y, v2.z);

    for (int i = 0; i < 3; i++) {
        mCcList.addCc(mCcs[i], sCcSrc);
    }
    mCcList.SetStts(stts);
    mCcList.ClrAt();
    return true;
}

void dEnemySwordMdl_c::enableAttack() {
    mCcs[0].OnAtSet();
    mIsActive = true;
    mFirstFramePassed = false;
}

// TODO - ...

void dEnemySwordMdl_c::calc(const mMtx_c &mtx, const mVec3_c &v1, bool mass) {
    mMdl.setLocalMtx(mtx);
    mMdl.calc(false);
    MTXMultVec(mtx, field_0x074, field_0x08C);
    MTXMultVec(mtx, field_0x080, field_0x098);

    if (mIsActive) {
        mVec3_c v = field_0x098 - field_0x08C;
        v.normalize();

        mVec3_c hi = field_0x098 + v * field_0x0A8;

        mCcs[1].Set(mCcs[0].mEnd, hi);
        mCcs[2].Set(mCcs[0].mStart, hi);
        mCcs[0].Set(field_0x08C - v * field_0x0A4, hi);

        mCcs[0].SetAtVec(v1);
        mCcs[1].SetAtVec(v1);
        mCcs[2].SetAtVec(v1);

        if (mFirstFramePassed && !mCcs[1].ChkAtSet()) {
            mCcs[1].OnAtSet();
            mCcs[2].OnAtSet();
        } else {
            mFirstFramePassed = true;
        }

        mCcList.registerColliders();
        if (mass) {
            dCcS::GetInstance()->GetMassMng().SetObj(&mCcs[0], 2);
        }
    } else {
        mFirstFramePassed = false;
        if (mCcs[0].ChkAtSet()) {
            mCcList.ClrAt();
        }
    }
    mProc.calc(field_0x098, field_0x08C);
}

bool dEnemySwordMdl_c::entry(dAcObjBase_c *obj, dShadowCircle_c *shadow, mQuat_c *quat) {
    if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL) {
        obj->fn_8002ECD0(&mMdl, 7);
    } else {
        obj->drawModelType1(&mMdl);
    }
    mProc.entry();
    if (shadow != nullptr && quat != nullptr) {
        m3d::mShadow_c::GetInstance()->addMdlToCircle(shadow, mMdl, *quat);
    }
    return true;
}
