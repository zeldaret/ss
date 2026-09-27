#include "d/a/obj/d_a_obj_mushroom.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_wtr_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d.h"
#include "d/col/c/c_m3d_g_lin.h"
#include "d/col/c/c_m3d_g_sph.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_gfx.h"
#include "d/d_light_env.h"
#include "d/d_sc_game.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/gfx/eggCamera.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggLightManager.h"
#include "egg/gfx/eggLightTexture.h"
#include "egg/gfx/eggLightTextureMgr.h"
#include "egg/gfx/eggTexture.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m3d.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/g3d/g3d_dcc.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/g3d_state.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXGeometry.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/GX/GXVert.h"
#include "rvl/MTX/mtx.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"

class dAcOMushRoom_HIO_c {
public:
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    s32 field_0x0C;
    u16 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 mLightScaleS;
    f32 mLightScaleT;
    f32 mLightTransS;
    f32 mLightTransT;
    f32 field_0x3C;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    u16 field_0x4C;
    f32 field_0x50;
    f32 field_0x54;
    u16 field_0x58;
    u16 field_0x5A;
    u16 field_0x5C;
    u16 field_0x5E;
    u16 field_0x60;
    u16 field_0x62;
    u16 field_0x64;
    u16 field_0x66;

    static const dAcOMushRoom_HIO_c sInstance;
};

const dAcOMushRoom_HIO_c dAcOMushRoom_HIO_c::sInstance = {
    0.0f, 4500.0f, 0.0f, 10,   30, 200.0f, 100.0f, 100.0f, 100.0f, 200.0f, 100.0f, 1.25f, 0.5f, 0.5f, 0.5f,
    0.0f, 0.0f,    0.0f, 0.1f, 30, 6.5f,   300.0f, 180,    160,    255,    200,    12,    79,   56,   255,
};

static const mVec3_c sRadius[4] = {
    mVec3_c(105.0f, 90.0f, 210.0f),
    mVec3_c(105.0f, 90.0f, 205.0f),
    mVec3_c(105.0f, 135.0f, 312.0f),
    mVec3_c(105.0f, 135.0f, 312.0f),
};

SPECIAL_ACTOR_PROFILE(OBJ_MUSHROOM, dAcOMushRoom_c, fProfile::OBJ_MUSHROOM, 0x141, 0, 7);

static dCcD_SrcSph sSrcSph = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_NONE, 0, {0, 0, 0}, 0, 0, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */ {~AT_TYPE_COMMON0, 0x1000111, {0, 5, 0x407}, 0, CUT_DIR_NONE},
     /* mObjCo */ {0xE9}},
    /* mSphInf */
    {110.f}
};
static dCcD_SrcCyl sSrcCyl = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_NONE, 0, {0, 0, 0}, 0, 0, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */ {~AT_TYPE_COMMON0, 0x1000110, {0, 5, 0x407}, 0, CUT_DIR_NONE},
     /* mObjCo */ {0xE9}},
    /* mSphInf */
    {135.0f, 30.0f}
};

static mVec3_c sBboxMin[4] = {
    mVec3_c(-150.0, -30.0, -150.0),
    mVec3_c(-100.0, -30.0, -100.0),
    mVec3_c(-100.0, -30.0, -100.0),
    mVec3_c(-100.0, -30.0, -100.0),
};
static mVec3_c sBboxMax[4] = {
    mVec3_c(150.0, 250.0, 150.0),
    mVec3_c(100.0, 250.0, 100.0),
    mVec3_c(100.0, 350.0, 100.0),
    mVec3_c(100.0, 250.0, 100.0),
};

STATE_DEFINE(dAcOMushRoom_c, Wait);
STATE_DEFINE(dAcOMushRoom_c, Init);

dAcOMushRoom_c::camera_c dAcOMushRoom_c::sCameras[8];

dAcOMushRoom_c::camera_c::~camera_c() {}

static mMtx_c fn_328_830(s8 camIdx, s8 camSubIdx) {
    nw4r::g3d::Camera cam = m3d::getCamera(10);
    cam.SetOrtho(
        dGfx_c::getCurrentScreenTopF(), dGfx_c::getCurrentScreenBottomF(), dGfx_c::getCurrentScreenLeftF(),
        dGfx_c::getCurrentScreenRightF(), 0.01f, 1000.0f
    );

    dAcOMushRoom_c::camera_c &sCam = dAcOMushRoom_c::sCameras[camIdx];
    const dAcOMushRoom_HIO_c &sHIO = dAcOMushRoom_HIO_c::sInstance;

    f32 lightScaleS, lightScaleT, lightTransS, lightTransT;
    if (sCam.mAlpha[camSubIdx] == 0) {
        lightScaleS = lightScaleT = lightTransS = lightTransT = 0.0f;
    } else {
        if (sCam.field_0x1B3[camSubIdx] == 1) {
            lightScaleS = 3.0f;
        } else {
            lightScaleS = sHIO.mLightScaleS;
        }
        lightScaleT = sHIO.mLightScaleT;
        lightTransS = sHIO.mLightTransS;
        lightTransT = sHIO.mLightTransT;

        lightScaleT += 1.5f * (255.0f - sCam.mAlpha[camSubIdx]) / 255.0f;
    }

    cam.SetTexMtxParam(lightScaleS, -lightScaleT, lightTransS, lightTransT);
    sCam.mCam[camSubIdx].setG3DCamera(cam);
    mMtx_c projMtx, camMtx;
    cam.GetProjectionTexMtx(projMtx);
    cam.GetCameraMtx(camMtx);

    projMtx.concat(camMtx);

    mMtx_c ret = projMtx;
    return ret;
}

template <typename T>
static inline T extract_data(void *base, size_t offset) {
    return *reinterpret_cast<T *>(reinterpret_cast<u8 *>(base) + offset);
}
template <typename T>
static inline T *offset_ptr(void *base, size_t offset) {
    return reinterpret_cast<T *>(reinterpret_cast<u8 *>(base) + offset);
}

bool dAcOMushRoom_c::createHeap() {
    const char *resName = getResFileName();
    const char *mdlName = getResMdlName();

    bool result = mMdl.create(getOarcResFile(resName), mdlName, &mAllocator, 0x120);
    if (!result) {
        return false;
    }

    mMdl.setLODDistance(0, 0.0f);
    mMdl.setLODDistance(1, 4500.0f);

    void *dat = getOarcFile(resName, getDataBinName());

    u16 numIdx = extract_data<u16>(dat, 0x08);
    int posArrOffset = extract_data<u32>(dat, 0x0C);
    int nrmArrOffset = extract_data<u32>(dat, 0x10);
    int posIdxArrOffset = extract_data<u32>(dat, 0x14);
    int nrmIdxArrOffset = extract_data<u32>(dat, 0x18);

    mData_IdxMax = extract_data<u16>(dat, 4);
    mData_VecNum = numIdx / 3;
    mData_pPosArr = offset_ptr<mVec3_c>(dat, posArrOffset);
    mData_pNrmArr = offset_ptr<mVec3_c>(dat, nrmArrOffset);
    mData_pPosIdxArr = offset_ptr<u16>(dat, posIdxArrOffset);
    mData_pNrmIdxArr = offset_ptr<u16>(dat, nrmIdxArrOffset);
    EGG::ResTIMG *pImg = reinterpret_cast<EGG::ResTIMG *>(getOarcFile(resName, "tex/MushroomACut0.bti"));
    for (int i = 0; i < 3; ++i) {
        if (!mProc[i].create(
                pImg, 0x25, &mAllocator, mData_VecNum / 2 + 3, mData_pPosArr, mData_pNrmArr, mData_pPosIdxArr,
                mData_pNrmIdxArr
            )) {
            return false;
        }
    }
    mMdl.setPriorityDraw(0x1C, 9);

    return result;
}

int dAcOMushRoom_c::create() {
    if ((getFromParams(31, 0x1) == 1 && mActorSubtype == SPAWN_Param)) {
        if (cM::rndF(1.0f) <= 0.3f) {
            field_0x892 = true;
        }
    }

    CREATE_ALLOCATOR_SIZE(dAcOMushRoom_c, 0x2000);
    initCc();
    mAcceleration = 0.0f;
    mMaxSpeed = 0.0f;
    changeState(StateID_Init);
    mBoundingBox.Set(sBboxMin[getType()], sBboxMax[getType()]);
    field_0x7BC = 0.0f;

    mCameraIdx = -1;
    if (getType() == TYPE_MushroomD) {
        setLighting();
    }

    if (dScGame_c::isCurrentStage("F103") || dScGame_c::isCurrentStage("F103_1") ||
        dScGame_c::isCurrentStage("t_tkm24")) {
        if ((s32)getFromParams(4, 0xF) == 0xF) {
            field_0x891 = true;
        } else {
            field_0x891 = false;
        }
    } else if (dScGame_c::isCurrentStage("F100_1")) {
        if (mPosition.y < -300.0f) {
            field_0x891 = true;
        } else {
            field_0x891 = false;
        }
    } else {
        if ((s32)getFromParams(4, 0xF) == 0xF) {
            field_0x891 = false;
        } else {
            field_0x891 = true;
        }
    }

    if (isNotParamBit30()) {
        unsetActorProperty(AC_PROP_0x1);
    }

    return SUCCEEDED;
}

int dAcOMushRoom_c::doDelete() {
    if (mCameraIdx >= 0) {
        sCameras[mCameraIdx].field_0x1B6 = false;
        mCameraIdx = -1;
    }

    if (getType() == TYPE_MushroomD) {
        cutLighting();
    }

    return SUCCEEDED;
}

int dAcOMushRoom_c::actorExecute() {
    if (checkGlittering()) {
        startGlitterEmitter();
    }

    executeState();

    mWorldMtx.transS(mPosition);
    mWorldMtx.ZXYrotM(mRotation);
    mWorldMtx.YrotM(field_0x794.y);
    mWorldMtx.XrotM(field_0x794.x);
    mWorldMtx.YrotM(-field_0x794.y);
    mMdl.setScale(mScale.x + field_0x7BC, mScale.y + field_0x7BC, mScale.z + field_0x7BC);
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(false);

    setCc();
    fn_328_3970();
    if (getType() == TYPE_MushroomD) {
        fn_328_4EF0();
    }

    return SUCCEEDED;
}

int dAcOMushRoom_c::draw() {
    drawModelType1(mMdl.getMdl());
    mQuat_c rot(mVec3_c(0.0f, getRadiusY(), 0.0f), getRadiusX() * 2.0f);
    if (getType() == TYPE_MushroomD) {
        drawShadow(mShadow, nullptr, mWorldMtx, &rot, -1, 0xC8, 0xB4, 0xE6, 0xFF, 0.0f);
    } else {
        drawShadow(mShadow, nullptr, mWorldMtx, &rot, -1, -1, -1, -1, -1, 0.0f);
    }

    if (mCameraIdx >= 0) {
        updateCameras();
        for (int i = 0; i < 3; ++i) {
            if (mProcAlpha[i] != 0) {
                mMtx_c m = mWorldMtx;
                m.scaleM(mScale.x + field_0x7BC, mScale.y + field_0x7BC, mScale.z + field_0x7BC);
                mProc[i].setLocalMtx(m);
                mProc[i].entry();
            }
        }
    }

    return SUCCEEDED;
}

void dAcOMushRoom_c::initializeState_Wait() {
    field_0x7A4 = mVec3_c::Zero;
    field_0x7B0 = mPosition;
    field_0x7B0.y += 200.0f;
    field_0x7C0 = 500;
}
void dAcOMushRoom_c::executeState_Wait() {
    static mAng angle = 0xAAB;

    const dAcPy_c *pPlayer = dAcPy_c::GetLink();
    dCcD_Sph &cc = getMainCc();

    if (cc.ChkTgHit()) {
        if (cc.ChkTgAtHitType(AT_TYPE_BOMB)) {
            dAcObjBase_c *pObj = cc.GetTgActor();
            if (pObj != nullptr) {
                mVec3_c mod = mVec3_c::Ez * 10.0f;
                mod.rotY((mPosition - pObj->mPosition).atan2sX_Z());
                field_0x7A4.x += mod.x;
                field_0x7A4.z += mod.z;
                field_0x7C0 = 0;
                field_0x79C = 30;
                startSoundWithFloatParam(SE_Kinoko_SHAKE, mScale.x);
            }
        } else if (cc.ChkTgAtHitType(AT_TYPE_WHIP) || cc.ChkTgAtHitType(AT_TYPE_CLAWSHOT) ||
                   cc.ChkTgAtHitType(AT_TYPE_0x40) || cc.ChkTgAtHitType(AT_TYPE_ARROW)) {
            mAng hitAng = (mPosition - const_cast<const dCcD_Sph &>(cc).GetTgHitPos()).atan2sX_Z();
            mVec3_c mod = mVec3_c::Ez * (field_0x7A4.mag() + 1.5f);
            if (cc.ChkTgAtHitType(AT_TYPE_CLAWSHOT)) {
                mod *= 1.5f;
            }
            mod.rotY(hitAng);
            field_0x7A4.x = mod.x;
            field_0x7A4.z = mod.z;
            startSoundWithFloatParam(SE_Kinoko_SHAKE, mScale.x);
        } else if (cc.ChkTgAtHitType(AT_TYPE_BELLOWS)) {
            if (field_0x854 % 20 == 0) {
                mAng hitAng = (mPosition - const_cast<const dCcD_Sph &>(cc).GetTgHitPos()).atan2sX_Z();
                mVec3_c mod = mVec3_c::Ez * (field_0x7A4.mag() + 3.5f);
                mod.rotY(hitAng);
                field_0x7A4.x += mod.x;
                field_0x7A4.z += mod.z;
            }
            field_0x854++;
        } else if (cc.ChkTgAtHitType(AT_TYPE_SWORD) || cc.ChkTgAtHitType(AT_TYPE_0x800000)) {
            if (dAcPy_c::GetLink() != nullptr) {
                mAng hitAng = (mPosition - dAcPy_c::GetLink()->mPosition).atan2sX_Z();

                switch (cc.GetTgAtCutDir()) {
                    case CUT_DIR_LU:
                    case CUT_DIR_L:
                    case CUT_DIR_LD: hitAng = hitAng + -0x4000; break;
                    case CUT_DIR_RD:
                    case CUT_DIR_R:
                    case CUT_DIR_RU: hitAng = hitAng + 0x4000; break;
                }
                mVec3_c mod = mVec3_c::Ez * (field_0x7A4.mag() + 3.0f);
                mod.rotY(hitAng);
                field_0x7A4.x = mod.x;
                field_0x7A4.z = mod.z;
                field_0x7C0 = 0;
                field_0x79C = 40;
                startSoundWithFloatParam(SE_Kinoko_SHAKE, mScale.x);
                fn_328_39C0();
            }
        } else if (cc.ChkTgAtHitType(AT_TYPE_BUBBLE)) {
            if (pPlayer != nullptr) {
                mVec3_c mod = mVec3_c::Ez * 3.0f;
                mod.rotY(pPlayer->mRotation.y);
                field_0x7A4.x += mod.x;
                field_0x7A4.z += mod.z;
                field_0x7C0 = 0;
                field_0x79C = 30;
                startSoundWithFloatParam(SE_Kinoko_SHAKE, mScale.x);
            }
        }
    } else if (pPlayer != nullptr && pPlayer->checkFlags0x350(0x2000) || pPlayer->checkFlags0x350(0x40)) {
        if (field_0x7A0 != 0) {
            mVec3_c mod = mVec3_c::Ez * 10.0f;
            mod.rotY(pPlayer->mRotation.y);
            field_0x7A4.x += mod.x;
            field_0x7A4.z += mod.z;
            field_0x7C0 = 0;
            field_0x79C = 30;
            startSoundWithFloatParam(SE_Kinoko_SHAKE, mScale.x);
            mVec3_c bubbleSpawnPos(mPosition.x, mPosition.y + getRadiusZ() + 30.0f, mPosition.z);
            itemDroppingAndGivingRelated(&bubbleSpawnPos, 3);
            if (pPlayer->checkFlags0x350(0x40) && field_0x891) {
                dAcObjBase_c::create(fProfile::OBJ_BUBBLE, mRoomID, 4, &bubbleSpawnPos, nullptr, nullptr, 0xFFFFFFFF);
                dJEffManager_c::spawnEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_727_, bubbleSpawnPos, nullptr, nullptr, nullptr, nullptr, 0, 0
                );
            }
        } else {
            mVec3_c mod = mVec3_c::Ez * 3.0f;
            mod.rotY((mPosition - pPlayer->mPosition).atan2sX_Z());
            field_0x7A4.x += mod.x;
            field_0x7A4.z += mod.z;
        }
    }

    if (cc.ChkTgHit() && !cc.ChkTgAtHitType(AT_TYPE_BELLOWS) || !cc.ChkTgHit()) {
        field_0x854 = 0;
    }
    if (field_0x891 == 0) {
        if ((cc.ChkTgHit() && (cc.ChkTgAtHitType(AT_TYPE_BOMB) || cc.ChkTgAtHitType(AT_TYPE_SWORD) ||
                               cc.ChkTgAtHitType(AT_TYPE_0x800000))) ||
            (pPlayer != nullptr && pPlayer->checkFlags0x350(0x2000) && field_0x7A0 != 0)) {
            if (!checkUnderWater()) {
                u32 p1 = getSporeParams1();
                static mVec3_c sOffsets[] = {
                    mVec3_c(0.0f, 700.0f, 0.0f),
                    mVec3_c(0.0f, 80.0f, 0.0f),
                    mVec3_c(0.0f, 120.0f, 0.0f),
                    mVec3_c(0.0f, 120.0f, 0.0f),
                };
                s32 type = getType();
                mVec3_c pos = mPosition + sOffsets[type] * mScale.y;

                if (dAcObjBase_c::create(this, fProfile::OBJ_SPORE, p1, &pos, nullptr, nullptr, 0, 0x3F)) {
                    field_0x892 = false;
                }
                startCloudEmitter();
            }
        }
    }

    mVec3_c v0 = (mPosition - field_0x7B0) * 0.05f;
    v0.y = 0.0f;
    field_0x7A4 += v0;
    if (sLib::calcTimer(&field_0x79C) == 0) {
        field_0x7A4.x *= 0.93f;
        field_0x7A4.z *= 0.93f;
    }
    if (field_0x7A4.absXZ() > 10.0f) {
        field_0x7A4.normalizeRS();
        field_0x7A4 *= 10.0f;
    }
    field_0x7B0 += field_0x7A4;
    mVec3_c diff = field_0x7B0 - mPosition;
    field_0x794.x = cM::atan2s(diff.absXZ(), diff.y);
    field_0x794.y = diff.atan2sX_Z();
    if (field_0x794.x > angle) {
        field_0x794.x = angle;
    }
    if (field_0x794.x < -(angle.mVal)) {
        field_0x794.x = -angle;
    }

    // TODO: Figure out the const of the cc stuff
    // The loading of the values here is off
    static const s16 s0 = {0x2000};
    field_0x7A0 = fn_8002eff0(mScale.x * 150.0f, s0, (cCcD_Obj *)(&cc), mScale.x * 55.0f, mScale.x * -75.0f);
    if (field_0x7A0 == 0) {
        static const s16 s1 = {0x4000};
        field_0x7A0 = fn_328_4C30(mScale.x * 250.0f, s0, (cCcD_Obj *)(&cc), mScale.x * 200.0f, 0.0f);
    }

    if (field_0x7C0 < 1000) {
        field_0x7C0++;
    }

    f32 f = (30 - field_0x7C0) / 30.0f;
    if (f < 0.0f) {
        f = 0.0f;
    }
    field_0x7BC = 0.1f * mAng(field_0x7C0 * 0x1000).cos() * f;

    if (cM::isZero(field_0x7BC)) {
        field_0x7A1 = false;
    } else {
        field_0x7A1 = true;
    }

    if (field_0x7A4.absXZ() > 6.5f) {
        mVec3_c pos(mPosition.x, mPosition.y + getRadiusZ() + 30.0f, mPosition.z);
        itemDroppingAndGivingRelated(&pos, 3);
    }
}
void dAcOMushRoom_c::finalizeState_Wait() {}

void dAcOMushRoom_c::initializeState_Init() {}
void dAcOMushRoom_c::executeState_Init() {
    changeState(StateID_Wait);
}
void dAcOMushRoom_c::finalizeState_Init() {}

f32 dAcOMushRoom_c::getUnscaledRadiusX() {
    return sRadius[getType()].x;
}

f32 dAcOMushRoom_c::getUnscaledRadiusY() {
    return sRadius[getType()].y;
}

f32 dAcOMushRoom_c::getRadiusX() {
    return getUnscaledRadiusX() * mScale.x;
}

f32 dAcOMushRoom_c::getRadiusY() {
    return getUnscaledRadiusY() * mScale.x;
}

f32 dAcOMushRoom_c::getRadiusZ() {
    return sRadius[getType()].z * mScale.y;
}

const char *dAcOMushRoom_c::getResFileName() {
    if (mActorSubtype == SPAWN_Param) {
        switch ((s32)getFromParams(0, 0xF)) {
            case TYPE_MushroomA: return "MushroomA";
            case TYPE_MushroomB: return "MushroomB";
            case TYPE_MushroomC: return "MushroomC";
            case TYPE_MushroomD: return "MushroomD";
        }
        return "MushroomA";
    }
    switch (mActorSubtype) {
        case SPAWN_MushroomA: return "MushroomA";
        case SPAWN_MushroomB: return "MushroomB";
        case SPAWN_MushroomC: return "MushroomC";
        case SPAWN_MushroomD: return "MushroomD";
    }
    return nullptr;
}

const char *dAcOMushRoom_c::getResMdlName() {
    return getResFileName();
}

const char *dAcOMushRoom_c::getDataBinName() {
    switch (getType()) {
        case TYPE_MushroomA: return "dat/MushroomA.bin";
        case TYPE_MushroomB: return "dat/MushroomB.bin";
        case TYPE_MushroomC: return "dat/MushroomC.bin";
        case TYPE_MushroomD: return "dat/MushroomD.bin";
    }
    return nullptr;
}

void dAcOMushRoom_c::initCc() {
    mStts.SetRank(12);
    mSph0.Set(sSrcSph);
    mSph0.SetStts(mStts);
    mCyl.Set(sSrcCyl);
    mCyl.SetStts(mStts);
    mSph1.Set(sSrcSph);
    mSph1.SetStts(mStts);

    mSph0.SetR(getRadiusX());
    switch (getType()) {
        case TYPE_MushroomA: {
            mCyl.SetR(mScale.x * 135.0f);
            mCyl.SetH(mScale.x * 30.0f);
            mSph1.SetR(0.0f);
            mSph0.SetR(getRadiusX() + 20.0f);
        } break;
        case TYPE_MushroomB:
        case TYPE_MushroomD: {
            mCyl.SetR(0.0f);
            mCyl.SetH(0.0f);
            mSph1.SetR(0.0f);
        } break;
        case TYPE_MushroomC: {
            mCyl.SetR(0.0f);
            mCyl.SetH(0.0f);
            mSph1.SetR(mScale.x * 60.0f);
        } break;
    }
}

void dAcOMushRoom_c::setCc() {
    s32 type = getType();
    mVec3_c center = mVec3_c::Ey * getRadiusX();
    mWorldMtx.multVec(center, center);
    field_0x844 = center;
    mSph0.SetC(center);
    dCcS::GetInstance()->Set(&mSph0);
    if (type == TYPE_MushroomB) {
        mVec3_c cylC(center.x, center.y - mScale.x * 30.0f, center.z);
        mCyl.SetC(cylC);
        dCcS::GetInstance()->Set(&mCyl);
    } else if (type == TYPE_MushroomC) {
        mVec3_c sphC = mVec3_c::Ey * getRadiusX() * 1.8f;
        mWorldMtx.multVec(sphC, sphC);
        mSph1.SetC(sphC);
        dCcS::GetInstance()->Set(&mSph1);
    }
}

void dAcOMushRoom_c::startCloudEmitter() {
    static mVec3_c sOffsets[] = {
        mVec3_c(0.0f, 70.0f, 0.0f),  // TYPE_MushroomA
        mVec3_c(0.0f, 80.0f, 0.0f),  // TYPE_MushroomB
        mVec3_c(0.0f, 120.0f, 0.0f), // TYPE_MushroomC
        mVec3_c(0.0f, 120.0f, 0.0f), // TYPE_MushroomD
    };
    mVec3_c offset = sOffsets[getType()] * mScale.y;

    mMtx_c emitterLocation;
    emitterLocation.transS(mPosition);
    emitterLocation.ZXYrotM(mRotation);
    emitterLocation.multVec(offset, offset);
    emitterLocation.transS(offset);
    emitterLocation.ZYXrotM(mRotation);
    emitterLocation.YrotM(cM::rndRange(-0x7FFF, 0x7FFF));
    emitterLocation.scaleM(mScale);
    // 737 -> Blue Spores
    // 468 -> Yellow Dust
    // 467 -> Rainbow Dust
    if (field_0x892) {
        dEmitterBase_c *pEmmitter =
            dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_467_, emitterLocation, nullptr, nullptr, 0, 0);
        if (pEmmitter != nullptr) {
            s32 volumeSize = 60;
            if (getType() == TYPE_MushroomA) {
                volumeSize = 100;
            } else if (getType() == TYPE_MushroomB) {
                volumeSize = 90;
            } else if (getType() == TYPE_MushroomC) {
                volumeSize = 80;
            }
            pEmmitter->setVolumeSize(volumeSize);
        }
    } else if (getType() == TYPE_MushroomD) {
        dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_737_, emitterLocation, nullptr, nullptr, 0, 0);
    } else {
        dEmitterBase_c *pEmmitter =
            dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_468_, emitterLocation, nullptr, nullptr, 0, 0);
        if (pEmmitter != nullptr) {
            s32 volumeSize = 60;
            if (getType() == TYPE_MushroomA) {
                volumeSize = 100;
            } else if (getType() == TYPE_MushroomB) {
                volumeSize = 90;
            } else if (getType() == TYPE_MushroomC) {
                volumeSize = 80;
            }
            pEmmitter->setVolumeSize(volumeSize);
        }
    }
}

void dAcOMushRoom_c::startGlitterEmitter() {
    static mVec3_c sOffsets[] = {
        mVec3_c(0.0f, 100.0f, 0.0f),
        mVec3_c(0.0f, 110.0f, 0.0f),
        mVec3_c(0.0f, 180.0f, 0.0f),
        mVec3_c(0.0f, 180.0f, 0.0f),
    };

    mVec3_c offset = sOffsets[getType()] * mScale.y;

    mMtx_c emitterLocation;
    emitterLocation.transS(mPosition);
    emitterLocation.ZXYrotM(mRotation);
    emitterLocation.multVec(offset, offset);
    emitterLocation.transS(offset);
    emitterLocation.ZYXrotM(mRotation);
    emitterLocation.YrotM(cM::rndRange(-0x7FFF, 0x7FFF));
    emitterLocation.scaleM(mScale);
    if (mEmitter.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_466_, emitterLocation, nullptr, nullptr)) {
        s32 volumeSize;
        switch (getType()) {
            case TYPE_MushroomA:
            case TYPE_MushroomB: volumeSize = 120; break;
            case TYPE_MushroomC: volumeSize = 108; break;
            default:             volumeSize = 100; break;
        }

        mEmitter.setVolumeSize(volumeSize);
    }
}

void dAcOMushRoom_c::fn_328_3970() {
    if (mCameraIdx < 0) {
        return;
    }

    bool bVisible = false;
    for (int i = 0; i < 3; ++i) {
        if (field_0x83F[i] != 0) {
            field_0x83F[i]--;
            bVisible = true;
        } else {
            if (mProcAlpha[i] == 0xFF) {
                cM3dGSph sph;
                cM3dGLin lin;
                mVec3_c out0, out1;
                mVec3_c start, end;
                sph.Set(&field_0x844, getRadiusX());
                start = field_0x7C4[i];
                end = field_0x7E8[i];
                if (field_0x83C[i] == 1) {
                    start.y += 100.0f;
                    end.y += (mScale.x + field_0x7BC) * 100.0f;
                } else {
                    start.y += 200.0f;
                    end.y += (mScale.x + field_0x7BC) * 100.0f;
                }
                lin.Set(start, end);
                if (cM3d_Cross_LinSph_CrossPos(sph, lin, out0, out1) > 0) {
                    mVec3_c src;
                    src.set(out1);
                    getSoundSource()->startSoundAtPosition2(SE_Kinoko_CURE, src);
                }
            }
            sLib::chase(&mProcAlpha[i], 0, 12);
            if (mProcAlpha[i] != 0) {
                bVisible = true;
            }
        }
    }

    if (!bVisible) {
        sCameras[mCameraIdx].field_0x1B6 = 0;
        mCameraIdx = -1;
    }
}

bool dAcOMushRoom_c::fn_328_39C0() {
    if (mCameraIdx < 0) {
        mCameraIdx = findCameraIdx();
    }
    if (mCameraIdx < 0) {
        return false;
    }

    int idx = -1;
    int alpha = 0xFF;
    for (int i = 0; i < 3; ++i) {
        if (mProcAlpha[i] == 0) {
            idx = i;
            break;
        }
        if (alpha >= mProcAlpha[i]) {
            alpha = mProcAlpha[i];
            idx = i;
        }
    }
    if (idx < 0) {
        return false;
    }

    mProcAlpha[idx] = 0xFF;
    field_0x83F[idx] = 10;

    const dCcD_Sph &cc = getMainCc();
    field_0x7C4[idx] = dAcPy_c::GetLink()->mPosition;
    field_0x7E8[idx] = mPosition;

    mAng rot;
    s32 cutDir = cc.GetTgAtCutDir();
    if (cc.ChkTgAtHitType(AT_TYPE_0x800000)) {
        switch (cutDir) {
            case CUT_DIR_U:
            case CUT_DIR_D:
            case CUT_DIR_STAB: rot = 0x4000; break;

            case CUT_DIR_L:
            case CUT_DIR_R:    rot = 0; break;

            case CUT_DIR_LU:
            case CUT_DIR_RD:   rot = -0x2000; break;

            case CUT_DIR_LD:
            case CUT_DIR_RU:   rot = 0x2000; break;

            default:           rot = 0; break;
        }

        field_0x7E8[idx].x = cc.GetTgHitPos().x;
        field_0x7E8[idx].z = cc.GetTgHitPos().z;
    } else {
        rot = dAcPy_c::GetLink()->vt_0x258() + 0x4000;
    }

    if (cutDir == CUT_DIR_STAB) {
        field_0x83C[idx] = 1;
    } else {
        field_0x83C[idx] = 0;
    }
    sCameras[mCameraIdx].field_0x1B3[idx] = field_0x83C[idx];

    if (mRotation.x != 0 || mRotation.z != 0) {
        mMtx_c m;
        mVec3_c v;
        m.ZXYrotS(mRotation);
        m.multVecSR(mVec3_c::Ey, v);
        C_MTXLookAt(m, field_0x7C4[idx], mVec3_c::Ey, field_0x7E8[idx]);
        m.multVecSR(v, v);
        s16 a = (0x4000 - cM::atan2s(v.y, v.x));
        rot += a;
    }

    if (rot > 0x4000) {
        rot = -(0x7FFF - rot);
    }
    if (rot < -0x4000) {
        rot = rot + 0x7FFF;
    }
    mVec3_c v0 = field_0x7E8[idx] - field_0x7C4[idx];
    mVec3_c v1 = mVec3_c::Ez;
    v1.rotY(v0.atan2sX_Z());
    mMtx_c m;
    m.setAxisRotation(v1, -rot.radian_c());
    m.multVec(mVec3_c::Ey, field_0x80C[idx]);
    sCameras[mCameraIdx].mAlpha[idx] = 0xFF;

    proc_c &proc = mProc[idx];
    proc.clearIdx();
    mProc[idx].mCameraIdx = mCameraIdx;

    proc.mCameraSubIdx = idx;
    updateCameras();
    camera_c &cam = sCameras[mCameraIdx];

    mMtx_c view = static_cast<mMtx_c &>(cam.mCam[idx].getViewMatrix());
    mVec3_c pos = cam.mCam[idx].mPos;
    mVec3_c at = cam.mCam[idx].mAt;
    view.multVec(pos, pos);
    view.multVec(at, at);

    mMtx_c m1 = view;
    m1.concat(mWorldMtx);

    bool vals[200];
    for (int i = 0; i < mData_IdxMax; ++i) {
        mVec3_c posArr = mData_pPosArr[i];
        vals[i] = fn_328_4B20(m1, posArr, pos.z, at.z);
    }
    for (int i = 0; i < mData_VecNum; i++) {
        u16 a = mData_pPosIdxArr[3 * i + 0];
        u16 b = mData_pPosIdxArr[3 * i + 1];
        u16 c = mData_pPosIdxArr[3 * i + 2];
        if (vals[a] || vals[b] || vals[c]) {
            if (!mProc[idx].entryIdx(3 * i)) {
                break;
            }
        }
    }
    return true;
}

bool dAcOMushRoom_c::proc_c::create(
    EGG::ResTIMG *pImg, u32 drawOpa, mAllocator_c *pAllocator, s32 idxNum, mVec3_c *pVtx, mVec3_c *pNrm, u16 *pVtxIdx,
    u16 *pNrmIdx
) {
    if (!m3d::proc_c::create(pAllocator, nullptr)) {
        return false;
    }

    mpIdxArr = new (pAllocator->getHeap()) u16[idxNum];
    if (mpIdxArr == nullptr) {
        remove();
        return false;
    }

    mpVtxArr = pVtx;
    mpNrmArr = pNrm;
    mpVtxIdxArr = pVtxIdx;
    mpNrmIdxArr = pNrmIdx;
    mIdxNum = 0;
    mIdxMax = idxNum;
    mpImg = pImg;
    setPriorityDraw(drawOpa, 0);
    setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_XLU, true);
    return true;
}

bool dAcOMushRoom_c::proc_c::entryIdx(u16 idx) {
    if (mIdxNum > mIdxMax) {
        return false;
    }

    mpIdxArr[mIdxNum] = idx;
    mIdxNum++;
    return true;
}

void dAcOMushRoom_c::proc_c::clearIdx() {
    mIdxNum = 0;
}

void dAcOMushRoom_c::proc_c::drawOpa() {
    EGG::LightManager *pLightMgr = m3d::getLightMgr(0);
    EGG::LightTexture *pLightTexture = pLightMgr->GetTextureMgr()->getTextureByName("LmBGMulti");
    pLightTexture->load(GX_TEXMAP1);
    EGG::DrawGX::LoadTexture(mpImg, GX_TEXMAP0);

    u32 pDiffColorMask;
    u32 pDiffAlphaMask;
    u32 pSpecColorMask;
    u32 pSpecAlphaMask;
    GXColor pColor;
    pLightMgr->LoadLightSet(1, &pDiffColorMask, &pDiffAlphaMask, &pSpecColorMask, &pSpecAlphaMask, &pColor);
    m3d::getFogMgr(0);
    nw4r::g3d::G3DState::LoadFog(0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, mTev0Alpha});

    m3d::resetMaterial();
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, (GXLightID)pDiffColorMask, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanAmbColor(GX_COLOR0, pColor);
    GXSetChanMatColor(GX_COLOR0, mColor(0xFFFFFFFF));
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0, GX_FALSE, GX_DUALMTX0);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_NRM, GX_TEXMTX1, GX_TRUE, GX_DUALMTX1);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetZCompLoc(GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetZMode(true, GX_LEQUAL, false);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_POS_XY, GX_F32, 0);

    GXSetArray(GX_VA_POS, mpVtxArr, sizeof(*mpVtxArr));
    GXSetArray(GX_VA_NRM, mpNrmArr, sizeof(*mpNrmArr));

    mMtx_c view;
    getViewMtx(view);
    GXLoadPosMtxImm(view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    view.inverseTransposeTo(view);
    GXLoadNrmMtxImm(view, GX_PNMTX0);

    mMtx_c tex;
    nw4r::g3d::Camera cam = m3d::getCurrentCamera();
    cam.GetEnvironmentTexMtx(tex);
    GXLoadTexMtxImm(view, GX_TEXMTX1, GX_MTX3x4);
    GXLoadTexMtxImm(tex, GX_DUALMTX1, GX_MTX3x4);
    mMtx_c m = fn_328_830(mCameraIdx, mCameraSubIdx);
    mMtx_c ident;
    MTXIdentity(ident);
    ident.concat(m);

    mMtx_c texMtx;

    nw4r::g3d::TexSrt srt;
    srt.Su = 2.0f;
    srt.Sv = 20.0f;
    srt.R = 0.0f;
    srt.Tu = 0.0f;
    srt.Tv = 0.0f;

    nw4r::g3d::CalcTexMtx(
        texMtx, true, srt,
        nw4r::g3d::TexSrt::Flag(
            nw4r::g3d::TexSrt::FLAG_ANM_EXISTS | nw4r::g3d::TexSrt::FLAG_ROT_ZERO | nw4r::g3d::TexSrt::FLAG_TRANS_ZERO
        ),
        nw4r::g3d::TexSrt::TEXMATRIXMODE_3DSMAX
    );
    f32 xz = texMtx.m[0][2];
    texMtx.m[0][2] = texMtx.m[0][3];
    texMtx.m[0][3] = xz;
    f32 yz = texMtx.m[1][2];
    texMtx.m[1][2] = texMtx.m[1][3];
    texMtx.m[1][3] = yz;
    texMtx.concat(ident);
    view = *reinterpret_cast<const mMtx_c *>(getLocalMtx());
    GXLoadTexMtxImm(view, GX_TEXMTX0, GX_MTX3x4);
    GXLoadTexMtxImm(texMtx, GX_DUALMTX0, GX_MTX3x4);
    draw();
}

void dAcOMushRoom_c::proc_c::draw() {
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, mIdxNum * 3);
    for (int i = 0; i < mIdxNum; ++i) {
        u16 *pVtxIdx = mpVtxIdxArr + mpIdxArr[i];
        u16 *pNrmIdx = mpNrmIdxArr + mpIdxArr[i];

        u16 vx = *pVtxIdx++;
        u16 vy = *pVtxIdx++;
        u16 vz = *pVtxIdx++;
        u16 nx = *pNrmIdx++;
        u16 ny = *pNrmIdx++;
        u16 nz = *pNrmIdx++;

        GXPosition1x16(vx);
        GXNormal1x16(nx);
        GXPosition1x16(vy);
        GXNormal1x16(ny);
        GXPosition1x16(vz);
        GXNormal1x16(nz);
    }
    GXEnd();
}

void dAcOMushRoom_c::updateCameras() {
    mMtx_c m0 = mWorldMtx;
    mMtx_c m1;
    m1.ZXYrotS(mRotation);
    m1.inverse();
    m0.concat(m1);
    for (int i = 0; i < 3; ++i) {
        camera_c *mushCam = sCameras + mCameraIdx;
        mProc[i].mTev0Alpha = mProcAlpha[i];
        mushCam->mAlpha[i] = mProcAlpha[i];
        if (mProcAlpha[i] == 0) {
            mushCam->mCam[i].mPos = mVec3_c::Ez;
            mushCam->mCam[i].mAt = mVec3_c::Zero;
            mushCam->mCam[i].mUp = mVec3_c::Ey;
            mushCam->mCam[i].EGG::LookAtCamera::doUpdateMatrix();
        } else {
            mVec3_c pos = field_0x7C4[i];
            mVec3_c at = field_0x7E8[i];
            mVec3_c up = field_0x80C[i];

            pos -= mPosition;
            at -= mPosition;
            if (field_0x83C[i] == 1) {
                pos.y += 100.0f;
                at.y += (mScale.x + field_0x7BC) * 100.0f;
            } else {
                pos.y += 200.0f;
                at.y += (mScale.x + field_0x7BC) * 100.0f;
            }
            m0.multVecSR(pos);
            m0.multVecSR(up);
            m0.multVecSR(at);
            pos += mPosition;
            at += mPosition;

            mushCam->mCam[i].mPos = pos;
            mushCam->mCam[i].mAt = at;
            mushCam->mCam[i].mUp = up;
            mushCam->mCam[i].EGG::LookAtCamera::doUpdateMatrix();
        }
    }
}
bool dAcOMushRoom_c::fn_328_4B20(const mMtx_c &m, const mVec3_c &vIn, f32 zHi, f32 zLo) {
    f32 tempx = 200.0f;
    f32 x = tempx * 0.5f;

    f32 tempy = 100.0f;
    f32 y = tempy * 0.5f;

    mVec3_c v;
    m.multVec(vIn, v);
    if (v.x > x) {
        return false;
    }
    if (v.x < -x) {
        return false;
    }
    if (v.y > y) {
        return false;
    }
    if (v.y < -y) {
        return false;
    }
    if (v.z > zHi) {
        return false;
    }
    if (v.z < zLo) {
        return false;
    }
    return true;
}

// NONMATCHING - I dont feel like dealing with this one lol
bool dAcOMushRoom_c::fn_328_4C30(const f32 &radius, const s16 &angle, cCcD_Obj *ccObj, const f32 &yHi, const f32 &yLo) {
    if (ccObj != nullptr && ccObj->ChkTgHit() && ccObj->ChkTgAtHitType(AT_TYPE_BUBBLE)) {
        dAcPy_c *pPlayer = dAcPy_c::GetLinkM();
        f32 yDiff = pPlayer->mPosition.y - mPosition.y;
        f32 rad = radius + 40.f;

        if ((yLo < yDiff && yDiff < yHi) && mPosition.squareDistanceToXZ(pPlayer->mPosition) < rad * rad &&
            labs(mAng::fromVec(mPosition - pPlayer->mPosition) - pPlayer->mAngle.y) < angle) {
            pPlayer->onFlags_0x360(1);
            return true;
        }
    }

    return false;
}
void dAcOMushRoom_c::setLighting() {
    mLightInfluence.mPos = mPositionCopy3;
    mLightInfluence.mClr.Set(0xC, 0x4F, 0x38, 0xFF);
    mLightInfluence.mScale = 0.0f;
    mLightInfluence.field_0x18 = 0;
    dLightEnv_c::GetPInstance()->plight_set(&mLightInfluence);

    mShadowInfluence.mPos = mPositionCopy3;
    mShadowInfluence.mPos.y += 250.0f;
    mShadowInfluence.mRadius = 400.0f;
    dLightEnv_c::GetPInstance()->shadow_set(&mShadowInfluence);
}

void dAcOMushRoom_c::cutLighting() {
    dLightEnv_c::GetPInstance()->plight_cut(&mLightInfluence);
    dLightEnv_c::GetPInstance()->shadow_cut(&mShadowInfluence);
}

void dAcOMushRoom_c::fn_328_4EF0() {
    if (field_0x888 > 0) {
        field_0x888--;
        mLightInfluence.mScale = 0.0f;
    } else {
        if (field_0x890 != 0) {
            if (field_0x88C > 0) {
                field_0x88C--;
            }
            if (field_0x88C == 0) {
                field_0x890--;
                field_0x888 = 2 + cM::rndInt(10);
            }
        }
        mLightInfluence.mScale = mScale.x * 300.0f;
        mLightInfluence.mPos = mPosition;
    }
}

bool dAcOMushRoom_c::checkUnderWater() {
    if (!dBgS_WtrChk::CheckPos(&mPosition, true, 200.0f, 0.0f) ||
        mPosition.y + mScale.y * 100.0f > dBgS_WtrChk::GetWaterHeight()) {
        return false;
    }
    return true;
}
