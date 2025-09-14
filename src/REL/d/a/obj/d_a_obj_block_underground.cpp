#include "d/a/obj/d_a_obj_block_underground.h"

#include "c/c_list.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "f/f_base.h"
#include "f/f_list_nd.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_BLOCK_UNDERGROUND, dAcOBlockUnderground, fProfile::OBJ_BLOCK_UNDERGROUND, 0x13F, 0, 3);

const s32 dAcOBlockUnderground::sSubtypeIdx[12] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 1, 1};

const char *const dAcOBlockUnderground::sOarcName = "MoleBlock";

const char *const dAcOBlockUnderground::sMdlNames[4] = {
    "MoleBlock",
    "MoleBlockBomb",
    "MoleBlockBreak",
    "MoleBlockBreak",
};

const char *const dAcOBlockUnderground::sDzbPlcNames[4] = {
    "MoleBlock_rock",
    "MoleBlock_rock",
    "MoleBlock_tuti",
    "MoleBlock_tuti",
};

const dCcD_SrcCyl dAcOBlockUnderground::sCylSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */ {AT_TYPE_BOMB, 0x11, {0, 8, 0x407}, 0x0, 0x0},
     /* mObjCo */ {0}},
    /* mCylInf */
    {50.f, 100.f}
};

u32 dAcOBlockUnderground::getCylTgType() {
    return 0x20020;
}

const char *dAcOBlockUnderground::getOarcName() {
    return sOarcName;
}

extern "C" cListMg_c *fn_8018B840(); // d_underground

dAcOBlockUnderground::dAcOBlockUnderground() : mRef(this), mUndergroundRef(nullptr) {
    fn_8018B840()->append(&mRef);
}

dAcOBlockUnderground::~dAcOBlockUnderground() {
    fn_8018B840()->remove(&mRef);
    delete mCyl;
}

bool dAcOBlockUnderground::createHeap() {
    const char *oarcName = getOarcName();
    void *data = getOarcResFile(oarcName);
    if (data == nullptr) {
        return false;
    }

    nw4r::g3d::ResFile resFile(data);
    if (!resFile.IsValid()) {
        return false;
    }

    const char *mdlName = getMdlName();
    nw4r::g3d::ResMdl mdl = resFile.GetResMdl(mdlName);
    if (!mdl.IsValid()) {
        return false;
    }

    if (!mMdl.create(mdl, &heap_allocator, 0x120, 1, nullptr)) {
        return false;
    }

    void *dzb = getOarcDZB(getOarcName(), sDzbPlcNames[getSubtypeIdx()]);
    if (dzb == nullptr) {
        return false;
    }

    void *plc = getOarcPLC(getOarcName(), sDzbPlcNames[getSubtypeIdx()]);
    if (plc == nullptr) {
        return false;
    }

    bool result = !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
    if (!result) {
        return false;
    }

    if (fn_459_A00()) {
        dCcD_Cyl *cyl = new dCcD_Cyl();
        mCyl = cyl;
        if (mCyl == nullptr) {
            return false;
        }
    }
    return true;
}

int dAcOBlockUnderground::actorCreate() {
    u8 sceneFlag = getSecondSceneFlag();
    if (sceneFlag < 0xff) {
        bool flag = SceneflagManager::sInstance->checkFlag(roomid, sceneFlag);
        if (flag) {
            return FAILED;
        }
    }

    bool success = false;
    s32 newSubtype = 0;
    u8 currentSubtype = getSubtype();
    const s32 *subtype = &sSubtypeIdx[0];
    while ((!success && newSubtype <= 3)) {
        if (currentSubtype == *subtype) {
            success = true;
        } else {
            subtype += 3;
            newSubtype++;
        }
    }
    if (success) {
        setSubtype(newSubtype);
    } else {
        return FAILED;
    }

    updateMatrix();
    CREATE_ALLOCATOR(dAcOBlockUnderground);
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.setPriorityDraw(0x82, 0x7f);
    if (fn_459_A00()) {
        mStts.SetRank(0xD);
        mCyl->Set(sCylSrc);
        mCyl->SetStts(mStts);
        if (fn_459_9A0()) {
            mCyl->mTg.mSrc.mType = getCylTgType();
        }
        mCyl->SetC(position);
    }
    mBgW.Move();
    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    boundingBox.Set(min, max);
    return SUCCEEDED;
}

int dAcOBlockUnderground::actorPostCreate() {
    if (mBgW.ChkUsed()) {
        mBgW.Lock();
        return SUCCEEDED;
    }
    registBg();
    return NOT_READY;
}

int dAcOBlockUnderground::actorExecute() {
    if (fn_459_A00()) {
        if (mCyl->ChkTgHit()) {
            if (mUndergroundRef.p_owner != nullptr) {
                mUndergroundRef.get()->fn_458_9D0(mField_0x57C, mField_0x57E);
            }

            if (getSubtype() == 1) {
                spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_461_);
                dSndSmallEffectMgr_c::GetInstance()->playSoundAtPosition(0xaa6, position);
            } else {
                spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_462_);
                dSndSmallEffectMgr_c::GetInstance()->playSoundAtPosition(0xaa7, position);
            }

            u8 firstSceneFlag = getFirstSceneFlag();
            if (firstSceneFlag < 0xFF) {
                SceneflagManager::sInstance->setFlag(roomid, firstSceneFlag);
            }

            u8 secondSceneFlag = getSecondSceneFlag();
            if (secondSceneFlag < 0xFF) {
                SceneflagManager::sInstance->setFlag(roomid, secondSceneFlag);
            }

            deleteRequest();
        } else if (dAcPy_c::GetLink()->checkActionFlagsCont(dAcPy_c::FLG0_SWING_ROPE)) {
            dCcS::GetInstance()->Set(mCyl);
        }
    }
    return SUCCEEDED;
}

int dAcOBlockUnderground::draw() {
    if (dAcPy_c::GetLink()->checkActionFlagsCont(dAcPy_c::FLG0_SWING_ROPE)) {
        drawModelType1(&mMdl);
    }
    return SUCCEEDED;
}

void dAcOBlockUnderground::setUndergroundRef(dAcOUnderground_c *underground) {
    mUndergroundRef.link(underground);
}

bool dAcOBlockUnderground::undergroundRefEquals(dAcOUnderground_c *underground) {
    return underground == mUndergroundRef.p_owner;
}

void dAcOBlockUnderground::setBlockUndergroundPosition(mVec3_c *pos) {
    setPostion(*pos);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    if (fn_459_A00()) {
        mCyl->SetC(*pos);
    }
    mBgW.Move();
}

void dAcOBlockUnderground::fn_459_840(u16 field_0x57c, u16 field_0x57e) {
    mField_0x57C = field_0x57c;
    mField_0x57E = field_0x57e;
}

u16 dAcOBlockUnderground::getField_0x57C() {
    return mField_0x57C;
}

u16 dAcOBlockUnderground::getField_0x57E() {
    return mField_0x57E;
}

void dAcOBlockUnderground::registBg() {
    dBgS::GetInstance()->Regist(&mBgW, this);
}

void dAcOBlockUnderground::releaseBg() {
    dBgS::GetInstance()->Release(&mBgW);
}

void dAcOBlockUnderground::setSubtype(u32 subtype) {
    mSubtype = subtype;
}

u8 dAcOBlockUnderground::getSubtype() {
    return getFromParams(0, 0xff);
}

u8 dAcOBlockUnderground::getFirstSceneFlag() {
    return getFromParams(8, 0xff);
}

u8 dAcOBlockUnderground::getSecondSceneFlag() {
    return getFromParams(16, 0xff);
}

u32 dAcOBlockUnderground::fn_459_930() {
    return sSubtypeIdx[mSubtype * 3 + 1];
}

u32 dAcOBlockUnderground::getSubtypeIdx() {
    return sSubtypeIdx[mSubtype * 3 + 2];
}

bool dAcOBlockUnderground::fn_459_970() {
    return fn_459_930() == 0;
}

bool dAcOBlockUnderground::fn_459_9A0() {
    return fn_459_930() == 2;
}

const char *dAcOBlockUnderground::getMdlName() {
    return sMdlNames[getSubtypeIdx()];
}

bool dAcOBlockUnderground::fn_459_A00() {
    return !fn_459_970();
}

void dAcOBlockUnderground::spawnEffect(u16 effectResourceId) const {
    mVec3_c pos(position.x, position.y + 50.f, position.z);
    dJEffManager_c::spawnEffect(effectResourceId, pos, nullptr, nullptr, nullptr, nullptr, 0, 0);
}
