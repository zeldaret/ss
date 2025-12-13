#include "c/c_counter.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_mass_s.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_camera.h"
#include "d/d_heap.h"
#include "d/d_light_env.h"
#include "d/d_player_act.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/t/d_t_mass_obj.h"
#include "d/d_heap_alloc.h"
#include "egg/core/eggHeap.h"
#include "egg/math/eggQuat.h"
#include "f/f_base.h"
#include "m/m3d/m3d.h"
#include "m/m3d/m_scnleaf.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_heap.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_draw1mat1shp.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/g3d_state.h"
#include "nw4r/g3d/platform/g3d_gpu.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_geometry.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/special_item_drop_mgr.h"
#include "toBeSorted/time_area_mgr.h"
#include <cmath>


SPECIAL_ACTOR_PROFILE(MASS_OBJ_TAG, dTgMassObj_c, fProfile::MASS_OBJ_TAG, 0x28A, 0, 4);

GrassModel* dTgMassObj_c::sGrassModels[5] = {};
dTgMassObj_c* dTgMassObj_c::sInstance = nullptr;
mHeapAllocator_c* dTgMassObj_c::sAllocator = nullptr;

u8 dTgMassObj_c::getCurrentStageGrassSubtype() {
    u8 ret = 0;
    if (dStageMgr_c::GetInstance()->isSTIFAreaFaron()) {
        if (dScGame_c::isCurrentStage("D100") ||dScGame_c::isCurrentStage("B100")) {
            ret = 4;
        } else {
            ret = 3;
        }
    } else if(dStageMgr_c::GetInstance()->isSTIFAreaEldin()) {
        ret = 2;
    } else if(dStageMgr_c::GetInstance()->isSTIFAreaLanayru()) {
        ret = 1;
    }
    if (dScGame_c::isCurrentStage("D003")) {
        ret = 0;
    } else if (dScGame_c::isCurrentStage("D003_1") || dScGame_c::isCurrentStage("D003_2")) {
        ret = 2;
    } else if (dScGame_c::isCurrentStage("D003_3") || dScGame_c::isCurrentStage("D003_4")) {
        ret = 1;
    } else if (dScGame_c::isCurrentStage("D003_5") || dScGame_c::isCurrentStage("D003_6")) {
        ret = 3;
    }
    return ret;
}

int dTgMassObj_c::actorCreate() {
    return SUCCEEDED;
}

static const GrassModelInfo GRASS_A_TYPES[5] = {
    {
        "GrassA",
        "GrassA",
        "GrassACut",
        0x1770,
        0x64,
        0x2,
        0x7D0,
        0x8,
        0x80,
        0,
        0,
    },
    {
        "GrassA",
        "GrassA",
        "GrassACut",
        0x1770,
        0x64,
        0x2,
        0x3E8,
        0x8,
        0x80,
        0,
        0,
    },
    {
        "FlowerA00",
        "FlowerA00",
        "FlowerA00Cut",
        0x1770,
        0x64,
        0x2,
        0x12C,
        0x8,
        0x20,
        0x2,
        0,
    },
    {
        "FlowerB00",
        "FlowerB00",
        "FlowerB00Cut",
        0x1770,
        0x64,
        0x2,
        0x12C,
        0x8,
        0x20,
        0x2,
        0,
    },
    {
        "FlowerB01",
        "FlowerB01",
        "FlowerB01Cut",
        0x1770,
        0x64,
        0x2,
        0x12C,
        0x8,
        0x20,
        0x2,
        0,
    },
};

static const GrassModelNames GRASS_MODEL_NAMES[5] = {
    {
        "GrassA",
        "GrassA",
        "GrassACut",
    },
    {
        "GrassB",
        "GrassB",
        "GrassBCut",
    },
    {
        "GrassC",
        "GrassC",
        "GrassCCut",
    },
    {
        "GrassD",
        "GrassD",
        "GrassDCut",
    },
    {
        "GrassE",
        "GrassE",
        "GrassECut",
    },
};

static const f32 GRASS_VECS[4][3] = {
    {
        -0.5f,
        0.5f,
        -0.5f,
    },
    {
        0.5f,
        0.5f,
        -0.5f,
    },
    {
        0.5f,
        -0.5f,
        0.5f,
    },
    {
        -0.5f,
        -0.5f,
        0.5f,
    },
};

int dTgMassObj_c::actorPostCreate() {
    s32 grassModelIndex = getGrassTypeFromParams();
    s32 retVar = SUCCEEDED;
    if (sInstance == nullptr) {
        sInstance = this;
        toGlobalRoom();
        sAllocator = new (dHeap::work1Heap.heap) mHeapAllocator_c();
        for (s32 i = 0; i < 5; i++) {
            sGrassModels[i] = new(dHeap::work1Heap.heap) GrassModel();
        }
        if (sAllocator->createFrmHeap(0xFFFFFFFF, dHeap::work1Heap.heap, "dTgMassObj_c::m_allocator", 0x20, mHeap::OPT_NONE) == 0) {
            return FAILED;
        }
        u8 grassSubtype = getCurrentStageGrassSubtype();
        for (s32 i = 0; i < 5; i++) {
            const GrassModelInfo* modelInfo;
            modelInfo = &GRASS_A_TYPES[i];
            sGrassModels[i]->setModelInfo(modelInfo, sAllocator);
            if (i == 0) {
                void* file = dAcObjBase_c::getOarcResFile(GRASS_MODEL_NAMES[grassSubtype].mArcName);
                nw4r::g3d::ResFile res(file);
                nw4r::g3d::ResMdl mdl = res.GetResMdl(GRASS_MODEL_NAMES[grassSubtype].mModelName);
                sGrassModels[i]->initResForModel(0, mdl.GetResMat(0), mdl.GetResShp(0));
                mdl = res.GetResMdl(GRASS_MODEL_NAMES[grassSubtype].mCutModelName);
                sGrassModels[i]->initResForModel(1, mdl.GetResMat(0), mdl.GetResShp(0));
            } else {
                void* file = dAcObjBase_c::getOarcResFile(modelInfo->mArcName);
                nw4r::g3d::ResFile res(file);
                nw4r::g3d::ResMdl mdl = res.GetResMdl(modelInfo->mModelName);
                sGrassModels[i]->initResForModel(0, mdl.GetResMat(0), mdl.GetResShp(0));
                mdl = res.GetResMdl(modelInfo->mCutModelName);
                sGrassModels[i]->initResForModel(1, mdl.GetResMat(0), mdl.GetResShp(0));
            }
        }
        sAllocator->adjustFrmHeap();
    } else {
        retVar = 2;
    }
    if (grassModelIndex == 0) {
        grassModelIndex = 0;
        mMassSubtype = getCurrentStageGrassSubtype();
        if (getFromParams(3, 1) != 0) {
            if (getFromParams(0x12, 1) != 0) {
                mMassSubtype = 1;
                grassModelIndex = 0;
            } else {
                mMassSubtype = 0;
                grassModelIndex = 1;
            }
        }
    } else if (grassModelIndex == 1) {
        mMassSubtype = 9;
        grassModelIndex = 4;
    } else if (grassModelIndex == 2) {
        mMassSubtype = 7;
        grassModelIndex = 2;
    } else if (grassModelIndex == 3) {
        if (s32(mParams >> 0x19) == 1) {
            mMassSubtype = 9;
            grassModelIndex = 4;
        } else {
            mMassSubtype = 8;
            grassModelIndex = 3;
        }
    }

    s32 fromParam4 = getFromParams(4, 0x7F);
    s32 fromParamB = getFromParams(0xB, 0x7F);
    s32 affectedByTimeshift = getFromParams(3, 1);
    s32 activeInPresent = getFromParams(0x12, 1);
    s32 uVar6 = fromParam4 * 10;
    f32 xzStep = uVar6;
    s32 xzDisplacement = xzStep * (fromParamB / 100.f);
    GrassModel** tmpGrassModel = &sGrassModels[grassModelIndex];

    mMtx_c mtx;
    mtx.transS(mPosition);
    mtx.YrotM(mRotation.y);
    mtx.scaleM(mScale);
    mMtx_c boundsMtx = mtx.copyInverse();
    mVec3_c minVec(FLOAT_MAX, FLOAT_MAX, FLOAT_MAX);
    mVec3_c maxVec(FLOAT_MIN, FLOAT_MIN, FLOAT_MIN);

    const f32 (*loop)[3] =  &GRASS_VECS[0];
    for (s32 i = 0; i < ARRAY_LENGTH(GRASS_VECS); i++) {
        mVec3_c mult = mtx.multVec2(mVec3_c(*(loop++)));
        if (mult.x < minVec.x) {
            minVec.x = mult.x;
        }
        if (mult.x > maxVec.x) {
            maxVec.x = mult.x;
        }
        if (mult.y < minVec.y) {
            minVec.y = mult.y;
        }
        if (mult.y > maxVec.y) {
            maxVec.y = mult.y;
        }
        if (mult.z < minVec.z) {
            minVec.z = mult.z;
        }
        if (mult.z > maxVec.z) {
            maxVec.z = mult.z;
        }
    }
    minVec.x += uVar6 - ((s32)minVec.x % uVar6);
    minVec.z += uVar6 - ((s32)minVec.z % uVar6);
    maxVec.x -= (s32)maxVec.x % uVar6;
    maxVec.z -= (s32)maxVec.z % uVar6;
    s32 specialItemDropId = getParams2UpperByte();
    fillUpperParams2Byte();
    mVec3_c groundCheckPos = mPosition;
    groundCheckPos.y += 200;
    u8 lightingCode = dBgS::GetInstance()->GetLightingCode(&groundCheckPos);
    s32 rnd = 0;
    // special item drop for farore's tear, unused
    if (specialItemDropId == 10) {
        if (getFromParams(0x15, 0xF) == 0) {
            s32 tmp = initializeCircle(*tmpGrassModel, &minVec, &maxVec, xzDisplacement, &boundsMtx, specialItemDropId, affectedByTimeshift, activeInPresent, -1, mMassSubtype, lightingCode, xzStep);
            rnd = cM::rndInt(tmp);
        } else {
            s32 tmp = initializeBox(*tmpGrassModel, &minVec, &maxVec, xzDisplacement, &boundsMtx, specialItemDropId, affectedByTimeshift, activeInPresent, -1, mMassSubtype, lightingCode, xzStep);
            rnd = cM::rndInt(tmp);
        }
    }
    if (getFromParams(0x15, 0xF) == 0) {
        initializeCircle(*tmpGrassModel, &minVec, &maxVec, xzDisplacement, &boundsMtx, specialItemDropId, affectedByTimeshift, activeInPresent, rnd, mMassSubtype, lightingCode, xzStep);
    } else {
        initializeBox(*tmpGrassModel, &minVec, &maxVec, xzDisplacement, &boundsMtx, specialItemDropId, affectedByTimeshift, activeInPresent, rnd, mMassSubtype, lightingCode, xzStep);
    }
    return retVar;
}

int dTgMassObj_c::doDelete() {
    if (sInstance == this) {
        GrassModel** modelP = sGrassModels;
        for (s32 i = 0; i < 5; i++, modelP++) {
            delete *modelP;
            *modelP = nullptr;
        }
        delete sAllocator;
        sAllocator = nullptr;
        sInstance = nullptr;
    }
    return SUCCEEDED;
}

int dTgMassObj_c::actorExecute() {
    dLightEnv_c& lightEnv = dLightEnv_c::GetInstance();
    if (sInstance == nullptr) {
        return NOT_READY;
    } else {
        dCcS::GetInstance()->GetMassMng().Prepare();
        GrassModel** modelP = sGrassModels;
        for (s32 i = 0; i < 5; i++) {
            if ((cCounter_c::GetGameFrame() & 1) == 0) {
                (*modelP)->calcCutCounter();
            }
            if (lightEnv.getfield_0x5D44() > 0.f) {
                (*modelP)->setPriorityDraw(0x1C, 0);
            } else {
                (*modelP)->setPriorityDraw(0x7F, 0);
            }
            (*modelP++)->update();
        }
        return SUCCEEDED;
    }
}

int dTgMassObj_c::draw() {
    if (sInstance == nullptr) {
        return NOT_READY;
    } else {
        GrassModel** modelP = sGrassModels;
        for (s32 i = 0; i < 5; i++) {
            (*modelP++)->entry();
        }
        return SUCCEEDED;
    }
}

void dTgMassObj_c::unloadRoom(u16 roomid) {
    GrassModel** modelP = sGrassModels;
    for (s32 i = 0; i < 5; i++) {
        (*modelP++)->unloadRoom(roomid);
    }
}

int dTgMassObj_c::initializeCircle(GrassModel *grassModel, mVec3_c *bbStart, mVec3_c *bbEnd,s32 xzDisplacement,mMtx_c *param_7,s32 specialItemDropIdParam,
                undefined4 affectedByTimeshift,u8 activeInPresent,int randInt,s32 massObjSubtype,
                u8 lightingCode, f32 fParam)
{
    f32 p4z = bbStart->z;
    s32 iVar3 = 0;
    while(p4z < bbEnd->z) {
        f32 p4x = bbStart->x;
        while (p4x < bbEnd->x) {
            mVec3_c position(
                p4x + cM::rndFX(xzDisplacement),
                bbEnd->y,
                p4z + cM::rndFX(xzDisplacement)
            );
            mVec3_c multiplied;
            param_7->multVec(position, multiplied);
            mVec3_c nul(0,0,0);
            multiplied.y = 0.f;
            f32 length = multiplied.distance(nul);
            if (length < 0.5f && dBgS_ObjGndChk::CheckPos(position) && 
                 dBgS_ObjGndChk::GetGroundHeight() >= (bbStart->y - 100.f)) {
                if (randInt >= 0) {
                    position.y = dBgS_ObjGndChk::GetGroundHeight();
                    s32 specialItemDropId = specialItemDropIdParam;
                    if (specialItemDropId == 10) {
                        if (randInt == iVar3) {
                            specialItemDropIdParam = 0xFF;
                        } else {
                            specialItemDropId = 0xFF;
                        }
                    }
                    if (!grassModel->spawnSingleGrass(0, getRoomId(), &position, 0, specialItemDropId, affectedByTimeshift, activeInPresent, massObjSubtype, lightingCode)) {
                        return 0;
                    }
                }
                iVar3++;
            }

            p4x += fParam;

        }
        p4z += fParam;
    }
    return iVar3;
}

int dTgMassObj_c::initializeBox(GrassModel *grassModel,mVec3_c *minVec,
                mVec3_c *maxVec,int xzDisplacement,mMtx_c *boundsMtx,int specialItemDropIdParam,undefined4 affectedByTimeshift,
                u8 activeInPresent,int randInt,s32 massObjSubtype,u8 lightingCode, f32 fParam) {
    
    f32 p4z = minVec->z;
    s32 iVar3 = 0;
    while(p4z < maxVec->z) {
        f32 p4x = minVec->x;
        while (p4x < maxVec->x) {
            mVec3_c position(
                p4x + cM::rndFX(xzDisplacement),
                maxVec->y,
                p4z + cM::rndFX(xzDisplacement)
            );
            mVec3_c multiplied;
            boundsMtx->multVec(position, multiplied);
            if (-0.5f <= multiplied.x && multiplied.x <= 0.5f
                && -0.5f <= multiplied.z && multiplied.z <= 0.5f && dBgS_ObjGndChk::CheckPos(position)) {
                if (dBgS_ObjGndChk::GetGroundHeight() >= (minVec->y - 100.f)) {
                    if (randInt >= 0) {
                        position.y = dBgS_ObjGndChk::GetGroundHeight();
                        s32 specialItemDropId = specialItemDropIdParam;
                        if (specialItemDropId == 10) {
                            if (randInt == iVar3) {
                                specialItemDropIdParam = 0xFF;
                            } else {
                                specialItemDropId = 0xFF;
                            }
                        }
                        if (!grassModel->spawnSingleGrass(0, getRoomId(), &position, 0, specialItemDropId, affectedByTimeshift, activeInPresent, massObjSubtype, lightingCode)) {
                            return 0;
                        }
                    }
                }
                iVar3++;
            }

            p4x += fParam;

        }
        p4z += fParam;
    }
    return iVar3;
}


void GrassModel::remove() {
    if (mpModelData != nullptr) {
        delete[] mpModelData;
        mpModelData = nullptr;
    }
    if (mInstanceList != nullptr) {
        delete[] mInstanceList;
        mInstanceList = nullptr;
    }
    if (mStaticTransformationList != nullptr) {
        delete[] mStaticTransformationList;
        mStaticTransformationList = nullptr;
    }
    if (mDynamicTransformationList != nullptr) {
        delete[] mDynamicTransformationList;
        mDynamicTransformationList = nullptr;
    }
    m3d::scnLeaf_c::remove();
}

undefined4 GrassModel::setModelInfo(f32 radius,f32 param_2,int param_4,
            s32 roomCount,u16 instanceListLength,u16 staticTransformationListLength,int dynamicTransformationListLength,
            undefined1 param_9,s32 opaDrawPrio,u32 xluDrawPrio,mHeapAllocator_c *allocator) {
    if (!create(allocator, nullptr)) {
        return 0;
    }
    EGG::Heap* heap = allocator->mHeap;
    mpModelData = new (heap, 4) GrassModelData[param_4];
    if (!mpModelData) {
        remove();
        return 0;
    }
    GrassModelData* modelData = &mpModelData[0];
    for (s32 i = 0; i < param_4; i++) {
        if (!(modelData++)->tryCreateLinkedLists(roomCount, heap)) {
            remove();
            return 0;
        }
    }
    mInstanceList = new (heap, 4) dTgMassObjInstance[instanceListLength];
    if (!mInstanceList) {
        remove();
        return 0;
    }
    mStaticTransformationList = new (heap, 4) dTgMassObjTransform[staticTransformationListLength];
    if (!mStaticTransformationList) {
        remove();
        return 0;
    }
    mDynamicTransformationList = new (heap, 4) dTgMassObjTransform[dynamicTransformationListLength];
    if (!mDynamicTransformationList) {
        remove();
        return 0;
    }
    field_0x58 = param_4;
    mRoomCount = roomCount;
    mInstanceListLength = instanceListLength;
    mStaticTransformationListLength = staticTransformationListLength;
    mDynamicTransformationListLength = dynamicTransformationListLength;
    mRadius = radius;
    mRadiusSquared = radius * radius;
    field_0x54 = param_2;
    field_0x62 = param_9;
    {
        dTgMassObjInstance *itr1 = mInstanceList;
        s32 i = 0;
        if (getInstanceListLength() > 0) {
            for (; i < getInstanceListLength(); i++) {
                // regswap
                mFreeInstances.append(itr1++);
            }
        }
    }
    {
        dTgMassObjTransform *itr2 = mDynamicTransformationList;
        s32 i = 0;
        if (getDynamicTransformListLength() > 0) {
            for (; i < getDynamicTransformListLength(); i++) {
                // regswap
                mAvailableTransforms.append(itr2++);
            }
        }
    }
    if (opaDrawPrio >= 0) {
        setPriorityDraw(opaDrawPrio, 0);
        setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_XLU, 1);
    } else {
        setPriorityDraw(0, xluDrawPrio);
        setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, 1);
    }
    return 1;
}

void GrassModel::initResForModel(s32 room,nw4r::g3d::ResMat pResMat,nw4r::g3d::ResShp pResShp) {
    mpModelData[room].initRes(pResMat, pResShp);
}

undefined4 GrassModel::spawnSingleGrass(int modelSubtype,u16 roomid,mVec3_c *groundHeight,
            u16 yRotation,s32 specialItemDropId,int affectedByTimeshift,int activeInPresent,
            s32 massObjSubtype,u8 lightingCode) {
    const dLightEnv_c& lightEnv = dLightEnv_c::GetInstance();
    if (mFreeInstances.mCount == 0) {
        return 0;
    }
    // almost
    dTgMassObjInstance* first = &*mFreeInstances.GetBeginIter();
    mFreeInstances.remove(first);
    first->reset();
    first->mGroundHeight.set(*groundHeight);
    first->yRotation = yRotation;
    first->mSpecialItemDropId = specialItemDropId;
    first->mDynamicTransform = nullptr;
    s32 chosen = cM::rndInt(mStaticTransformationListLength);
    first->mInitPosTransform = &mStaticTransformationList[chosen];
    first->field_0x24 = cM::rndFX(0.2f) + 1.f;
    first->mMassObjSubtype = massObjSubtype;
    first->mLightingCode = lightingCode;
    first->mTevColor = lightEnv.GetCurrentSpf().mActorPalette.field_0x02C;
    if (first->mLightingCode == 0) {
        first->mTevColor.r = 0.7f * first->mTevColor.r;
        first->mTevColor.r &= 0xFF;
        first->mTevColor.g = 0.6f * first->mTevColor.g;
        first->mTevColor.g &= 0xFF;
        first->mTevColor.b = 0.7f * first->mTevColor.b;
        first->mTevColor.b &= 0xFF;
    }
    if (affectedByTimeshift) {
        first->mGrassFlags |= dTgMassObjInstance::TG_MASS_UNK2_TIMESHIFT_RELATED;
        if (activeInPresent) {
            first->mActiveInPresent = true;
        } else {
            first->mScale = 0.f;
        }
    }
    mpModelData[modelSubtype].addToRoom(roomid, first);
    return 1;
}

s32 GrassModel::addToRoom(u32 modelSubtype, s32 roomid, dTgMassObjInstance* pInstance) {
    mpModelData[modelSubtype].addToRoom(roomid, pInstance);
    if (modelSubtype == 1) {
        SpecialItemDropMgr::GetInstance()->giveSpecialDropItem(pInstance->mSpecialItemDropId, roomid, &pInstance->mGroundHeight, 0, 0, -1);
        pInstance->mSpecialItemDropId = 0xFF;
    }
    return 1;
}

void GrassModel::addToFreeInstances(dTgMassObjInstance *param_2) {
    mFreeInstances.append(param_2);
}

void GrassModel::unloadRoom(u16 roomid) {
    GrassModelData* modelData = mpModelData;
    for (s32 i = 0; i < field_0x58; i++) {
        (modelData++)->unloadRoom(this, roomid);
    }
}

inline void SetMassMngAttr(f32 radius, f32 height, u8 param_2, u8 param_3) {
    dCcS::GetInstance()->GetMassMng().SetAttr(radius, height, param_2, param_3);
}

void GrassModel::update() {
    dTgMassObjTransform* obj = mStaticTransformationList;
    for (s32 i = 0; i < mStaticTransformationListLength; obj++, i++ ) {
        obj->update();
    }
    SetMassMngAttr(mRadius, mRadius * 2, 0xB, field_0x62);
    GrassModelData* modelData = mpModelData;
    for (s32 i = 0; i < field_0x58; i++) {
        (modelData++)->update(this);
    }
}

void GrassModel::draw() {
    nw4r::math::MTX34 cameraMtx;
    m3d::getCurrentCamera().GetCameraMtx(&cameraMtx);
    GrassModelData* data = mpModelData;
    for (s32 i = 0; i < field_0x58; data++, i++) {
        data->draw(mRadius, field_0x54, &cameraMtx);
    }
}

dTgMassObjTransform* GrassModel::aquireTransform() {
    if (mAvailableTransforms.mCount == 0) {
        return nullptr;
    }
    // not exactly, this produces an additional instruction
    dTgMassObjTransform * first = &*(mAvailableTransforms.GetBeginIter());
    mAvailableTransforms.remove(first);
    mAquiredTransforms.append(first);
    return first;
}

// regalloc, should be equivalent
void GrassModel::releaseTransform(dTgMassObjTransform* param2) {
    mAquiredTransforms.remove(param2);
    mAvailableTransforms.append(param2);
}

static s32 lbl_80573AF0 = 0x1000;

dTgMassObjTransform::dTgMassObjTransform() {
    field_0x00 = 0;
    field_0x04 = 0;
    mRotXSpeed = 0;
    mRotY = cM::rndInt(0x10000);
    mRotX = lbl_80573AF0;
    mQuat.setUnit();
    s32 rnd = cM::rndInt(100);
    for (s32 i = 0; i < rnd; i++) {
        update();
    }
}

#define CLAMP2(low, high, x) ((x) < (low) ? (low) : ((x) > (high) ? (high) : (x)))

// matches besides data
void dTgMassObjTransform::update() {
    mRotXSpeed -= (s16)(mRotX * 0.005f);
    mRotXSpeed = CLAMP2(-0x4B, 0x4B, mRotXSpeed);
    mRotX += mRotXSpeed;
    mVec3_c tmp1(0, 1, 0);
    mVec3_c tmp2(0, 1, 0);
    tmp2.rotX(mRotX);
    tmp2.rotY(mRotY);
    EGG::Quatf quat;
    quat.makeVectorRotation(tmp1, tmp2);
    mQuat.slerpTo(quat, 0.5f, mQuat);
    mQuat.normalise();
    mQuat.makeWPositive();
    mMtx.fromQuat(mQuat);
}

void dTgMassObjInstance::releaseDynamicTransform(GrassModel *param_2) {
    if (mDynamicTransform != nullptr) {
        param_2->releaseTransform(mDynamicTransform);
        mDynamicTransform = nullptr;
    }
}

void dTgMassObjInstance::reset() {
    mGrassFlags = 0;
    mTevColor.a = 0;
    mTevColor.b = 0;
    mTevColor.g = 0;
    mTevColor.r = 0;
    mActiveInPresent = false;
    mScale = 1;
}

// matches besides data
void dTgMassObjInstance::getDrawMatrix(mMtx_c *pOut) {
    if (mDynamicTransform != nullptr) {
        *pOut = mDynamicTransform->getMtx();
    } else if (mInitPosTransform != nullptr) {
        *pOut = mInitPosTransform->getMtx();
    } else {
        pOut->YrotS(yRotation);
    }
    pOut->setBase(3, mGroundHeight);
    pOut->scaleM(mScale, mScale, mScale);
    s32 sure = ((s16)mGroundHeight.x >> 1) & 7;
    pOut->scaleM(
        sure * 0.015f + 1.f,
        sure * 0.015f + 1.f,
        sure * 0.015f + 1.f
    );
}

// matches besides data
s32 dTgMassObjInstance::checkForHit(GrassModel *param_2,
            GrassModelData *param_3,u16 roomid) {
    dCcMassS_HitInf massHitInf;
    dAcObjBase_c* actor;
    u32 chk = dCcS::GetInstance()->GetMassMng().Chk(&mGroundHeight, &actor, &massHitInf);
    s32 iVar5 = checkForHit(chk, massHitInf, actor, param_2, param_3, roomid);
    if (iVar5 == 0
        && FUN_80278c70(chk, massHitInf, actor, param_2) == 0
        && handleLinkSpinAttack(param_2) == 0) {
            if (mDynamicTransform != nullptr) {
                EGG::Quatf& dynQuat = mDynamicTransform->mQuat;
                f32 fVar2 = (mInitPosTransform->mQuat.dot(dynQuat));
                fVar2 *= 0.1f;
                if (fVar2 < 0.1f) {
                    fVar2 = 0.1f;
                }
                dynQuat.slerpTo(mInitPosTransform->mQuat, fVar2, dynQuat);
                mDynamicTransform->mQuat.normalise();
                mDynamicTransform->mQuat.makeWPositive();
                mDynamicTransform->mMtx.fromQuat(mDynamicTransform->mQuat);
                if (mInitPosTransform->mQuat.dot(mDynamicTransform->mQuat) >= 1.f) {
                    param_2->releaseTransform(mDynamicTransform);
                    mDynamicTransform = nullptr;
                }
            }
            return 0;
        } else {
            return 1;
        }
}

// matches besides data
undefined4 dTgMassObjInstance::checkForHit(u32 param_2,dCcMassS_HitInf& param_3,dAcObjBase_c* param_4,
            GrassModel *param_5,GrassModelData* param_6,undefined4 roomid) {
    dAcPy_c * link = dAcPy_c::GetLinkM();
    if (link == nullptr) {
        return 0;
    }
    if ((param_2 & 1) == 0 || param_4 == nullptr) {
        return 0;
    }
    f32 impactDistanceFactor;
    f32 impactFactor;
    f32 maybeMaxImpactDistance = 0.f;
    f32 distance;
    s32 isNotCut;
    f32 fVar20;
    f32 fVar5;
    f32 fVar4;
    f32 fVar2;
    f32 fVar21;
    mVec3_c localB4(0.f,1.f,0.f);
    cCcD_Obj * hitObj = param_3.GetAtHitObj();
    if (hitObj != nullptr) {
        mVec3_c& p4Pos = param_4->getPosition();
        mVec3_c hitPosition = param_4->mPosition;
        s32 needsLightingRelated = 0;
        impactFactor = 0.f;
        if (hitObj->ChkAtType(AT_TYPE_BELLOWS)) {
            hitPosition = link->mPosition;
            needsLightingRelated = 1;
            maybeMaxImpactDistance = 1000.f;
            impactFactor = cM::rndF(0.5f) + 0.5f;
            isNotCut = 1;
        } else if (hitObj->ChkAtType(AT_TYPE_SLINGSHOT)) {
            maybeMaxImpactDistance = 120.f;
            impactFactor = cM::rndF(0.06f) + 0.6f;
            isNotCut = 1;
        } else if (hitObj->ChkAtType(AT_TYPE_0x200000) || hitObj->ChkAtType(AT_TYPE_BUGNET)) {
            maybeMaxImpactDistance = 1000.f;
            impactFactor = cM::rndF(0.2f) + 0.6f;
            isNotCut = 1;
        } else if (hitObj->ChkAtType(AT_TYPE_0x40)) {
            mVec3_c localCC = mGroundHeight;
            isNotCut = 1;
            if (std::fabsf(localCC.y - param_4->mPosition.y) < 60.f) {
                localCC.y = param_4->mPosition.y;
                f32 dist = localCC.distance(p4Pos);
                if (dist < 160.f) {
                    isNotCut = 0;
                }
            }
            if (isNotCut == 1) {
                maybeMaxImpactDistance = 600.f;
                impactFactor = cM::rndF(0.3f) + 0.3f;
            }
            needsLightingRelated = 1;

        } else if (hitObj->ChkAtType(AT_TYPE_BEETLE)) {
            mVec3_c localD8 = mGroundHeight;
            isNotCut = 1;
            if (std::fabsf(localD8.y - param_4->mPosition.y) < 80.f) {
                localD8.y = param_4->mPosition.y;
                f32 dist = localD8.distance(param_4->mPosition);
                if (dist < 160.f) {
                    isNotCut = 0;
                }
            }
            if (isNotCut == 1) {
                maybeMaxImpactDistance = 800.f;
                impactFactor = cM::rndF(0.2f) + 0.5f;
            }

        } else if (hitObj->ChkAtType(AT_TYPE_WHIP)) {
            mVec3_c localE4 = mGroundHeight;
            hitPosition = *link->anotherThingWithWhip();
            if (localE4.distance(hitPosition) < 120.f) {
                isNotCut = 0;
            } else {
                maybeMaxImpactDistance = 300.f;
                impactFactor = cM::rndF(0.05f) + 0.5f;
                isNotCut = 1;
            }
        } else if (hitObj->ChkAtType(AT_TYPE_0x800000)) {
            isNotCut = 0;
        } else if (hitObj->ChkAtType(AT_TYPE_ARROW)) {
            mVec3_c localE4 = mGroundHeight;
            if (localE4.distance(param_4->mPosition) < 80.f) {
                isNotCut = 0;
            } else {
                maybeMaxImpactDistance = 200.f;
                impactFactor = cM::rndF(0.04f) + 0.4f;
                isNotCut = 1;
            }
        } else if (hitObj->ChkAtType(AT_TYPE_BOMB)) {
            mVec3_c localE4 = mGroundHeight;
            if (localE4.distance(param_4->mPosition) < 300.f) {
                isNotCut = 0;
            } else {
                maybeMaxImpactDistance = 5000.f;
                impactFactor = cM::rndF(0.09f) + 0.9f;
                isNotCut = 1;
            }
        } else if (hitObj->ChkAtType(AT_TYPE_SWORD)) {
            s32 currentSword = daPlayerActBase_c::getCurrentSwordTypeInline();
            fVar20 = 120.f;
            fVar21 = 260.f;
            fVar5 = 430.f;
            fVar4 = 305.f;
            fVar2 = 1000.f;
            if (currentSword == 0) {
                fVar5 = 200.f;
                fVar21 = 150.f;
                fVar2 = 180.f;
                fVar4 = 200.f;
            } else if (currentSword == 1) {
                fVar21 = 170.f;
                fVar5 = 400.f;
                fVar4 = 250.f;
                fVar2 = 200.f;
            } else if (currentSword == 2 || currentSword == 3) {
                fVar20 = 130.f;
                fVar21 = 200.f;
                fVar5 = 420.f;
                fVar4 = 290.f;
                fVar2 = 250.f;
            }
            hitPosition = link->mPosition;
            distance = mGroundHeight.distance(hitPosition);
            if (link->getSpecificAttackDirection() == 1
                || link->getSpecificAttackDirection() == 0x10
                || link->getSpecificAttackDirection() == 0x100) {
                    if (distance < fVar20) {
                        isNotCut = 0;
                    } else {
                        maybeMaxImpactDistance = 300.f;
                        impactFactor = cM::rndF(0.05f) + 0.5f;
                        isNotCut = 1;
                        if (distance >= fVar20 + 60.f) {
                            return 1;
                        }
                    }
                } else {
                    if (mMassObjSubtype == 8 || mMassObjSubtype == 9) {
                        fVar21 = fVar20 * 0.8f;
                        hitPosition = dAcPy_c::GetLink()->getSwordPos();

                        distance = mGroundHeight.distance(hitPosition);
                        if (link->getSpecificAttackDirection() != 2 && link->getSpecificAttackDirection() != 0x80) {
                            return 1;
                        }
                        if (distance >= fVar21) {
                            return 1;
                        }
                    }
                    if (link->isAttackingSpin()) {
                        if (link->checkSwordAndMoreStates(daPlayerActBase_c::SKYWARD_STRIKE_ACTIVE)) {
                            fVar21 = fVar5;
                        } else {
                            fVar21 = fVar4;
                        }
                    }
                    if (distance < fVar21) {
                        isNotCut = 0;
                    } else if (distance < fVar2) {
                        maybeMaxImpactDistance = 5000.f;
                        impactFactor = cM::rndF(0.5f) + 0.5f;
                        isNotCut = 1;
                    } else {
                        return 1;
                    }
                }
        } else {
            isNotCut = 0;
        }
        if (isNotCut == 1) {
            // grass is not cut, just move
            f32 fVar20 = cM::rndFX(0.025f) + 0.05f;
            if (!hitObj->ChkAtType(AT_TYPE_WHIP) && !hitObj->ChkAtType(AT_TYPE_BELLOWS)) {
                if ((!link->isAttackingSpin())) {
                    hitPosition = param_4->mPosition;
                }
            }
            f32 tempDistance = mGroundHeight.distance(hitPosition);
            if (tempDistance > maybeMaxImpactDistance) {
                tempDistance = maybeMaxImpactDistance;
            }
            // check this for regalloc
            impactDistanceFactor = (1.f - (tempDistance / maybeMaxImpactDistance) * (tempDistance / maybeMaxImpactDistance));
            if (mDynamicTransform == nullptr) {
                mDynamicTransform = param_5->aquireTransform();
                if (mDynamicTransform == nullptr) {
                    return 0;
                }
                mDynamicTransform->mQuat.set(mInitPosTransform->mQuat);
                mDynamicTransform->mMtx.fromQuat(mInitPosTransform->mQuat);
            }
            if (param_4 == nullptr) {
                return 0;
            }
            mVec3_c local108 = mGroundHeight - hitPosition;
            local108.rotY(0x4000);
            local108.normalize();
            mVec3_c local114 = hitObj->mAt.mVec;
            mVec3_c local120 = local114;
            if (needsLightingRelated) {
                fVar20 = cM::rndF(0.5f) + 0.15f;
                hitPosition.y += 50.f;
                dLightEnv_c::GetPInstance()->get_vectle_calc(&hitPosition, &mGroundHeight, &local120);
                if (!local120.normalizeRS()) {
                    local120.x = 1.f;
                    local120.z = 0.f;
                }
                if (!local120.normalizeRS()) {
                    local120 = localB4;
                }
            } else if (local114.isZero2()) {
                local120 = mGroundHeight - hitPosition;
                if (!local120.normalizeRS()) {
                    local120.x = 1.f;
                    local120.z = 0.f;
                }
                local120.y = 0.5f;
                if (!local120.normalizeRS()) {
                    local120 = localB4;
                }
                fVar20 = cM::rndF(0.5f) + 0.5f;
            } else {
                local114.y = 0.f;
                local114.normalize();
                f32 fVar2 = local108.inprodXZ(local114);
                if (fVar2 != 0.f) {
                    local120.y = 2.f / fVar2;
                }
                if (local108.inprodXZ(local114) >= 0.f) {
                    local120.rotY(-0x4000);
                } else {
                    local120.rotY(0x4000);
                }
                if (!local120.normalizeRS()) {
                    local120 = localB4;
                }
            }
            mQuat_c local130;
            local130.makeVectorRotation(localB4, local120);
            local130.slerpTo(mInitPosTransform->mQuat, 1.f - impactFactor * impactDistanceFactor, local130);
            local130.normalise();
            local130.makeWPositive();
            mQuat_c local140;
            mDynamicTransform->mMtx.toQuat(local140);
            local140.slerpTo(local130, fVar20, local140);
            local140.normalise();
            local140.makeWPositive();
            mDynamicTransform->mQuat.set(local130);
            mDynamicTransform->mMtx.fromQuat(local140);
        } else if (hitObj->ChkAtType(AT_TYPE_WIND)) {
            if (mDynamicTransform == nullptr) {
                mDynamicTransform = param_5->aquireTransform();
                if (mDynamicTransform == nullptr) {
                    return 0;
                }
                mDynamicTransform->mQuat.set(mInitPosTransform->mQuat);
                mDynamicTransform->mMtx.fromQuat(mInitPosTransform->mQuat);
            }
            if (param_4 == nullptr) {
                return 0;
            }
            mVec3_c local150 = mGroundHeight - param_4->mPosition;
            mQuat_c local160;
            local150.y = 0.f;
            local150.x *= (cM::rndFX(0.5f) + 0.5f);
            local150.z *= (cM::rndFX(0.5f) + 0.5f);
            if (!local150.normalizeRS()) {
                local150 = localB4;
            }
            local160.makeVectorRotation(localB4, local150);
            // float regalloc
            f32 fVar2 = (local160.dot(mDynamicTransform->mQuat));
            fVar2 *= 0.1f;
            if (fVar2 < 0.1f) {
                fVar2 = 0.1f;
            }
            local160.slerpTo2(cM::rndF(0.4f) + 0.3f, mInitPosTransform->mQuat, local160);
            local160.normalise();
            local160.makeWPositive();
            mDynamicTransform->mQuat.slerpTo(local160, fVar2, mDynamicTransform->mQuat);
            mDynamicTransform->mQuat.normalise();
            if (mDynamicTransform->mQuat.w < 0) {
                mDynamicTransform->mQuat.multScalar(-1);
            }
            mDynamicTransform->mMtx.fromQuat(mDynamicTransform->mQuat);
        } else {
            // grass is cut
            param_6->removeFromRoom(roomid, this);
            param_5->addToRoom(1, roomid, this);
            mGrassFlags |= TG_MASS_UNK2_IS_CUT;
            if (mDynamicTransform != nullptr) {
                // this is wrong...
                yRotation = *(u16*)mDynamicTransform->mRotY.ref();
                param_5->releaseTransform(mDynamicTransform);
                mDynamicTransform = nullptr;
            }
            yRotation = *(u16*)mInitPosTransform->mRotY.ref();
            mInitPosTransform = nullptr;
            if (param_5->mCutCounter < 0x19) {
                param_5->mCutCounter++;
                mVec3_c local16C(mGroundHeight.x, mGroundHeight.y, mGroundHeight.z);
                mColor local17C = 0xFFFFFFFF;
                f32 camField2A8 = dScGame_c::getCamera()->getField_0x2A8();
                local17C.r = mTevColor.r;
                local17C.g = mTevColor.g;
                local17C.b = mTevColor.b;
                local17C.a = mTevColor.a;
                if (local16C.y + 100.f < camField2A8 && mMassObjSubtype <= 5) {
                    if (dScGame_c::isCurrentStage("D100")) {
                        mMassObjSubtype = 6;
                    } else {
                        mMassObjSubtype = 5;
                    }
                }
                dEmitterBase_c * ret;
                switch (mMassObjSubtype) {
                    case 0:
                        dJEffManager_c::createMassObjEffect(PARTICLE_RESOURCE_ID_MAPPING_120_, local16C, nullptr, &local17C);
                        break;
                    case 1:
                        dJEffManager_c::createMassObjEffect(PARTICLE_RESOURCE_ID_MAPPING_121_, local16C, nullptr, &local17C);
                        break;
                    case 2:
                        dJEffManager_c::createMassObjEffect(PARTICLE_RESOURCE_ID_MAPPING_122_, local16C, nullptr, &local17C);
                        break;
                    case 3:
                        dJEffManager_c::createMassObjEffect(PARTICLE_RESOURCE_ID_MAPPING_123_, local16C, nullptr, &local17C);
                        break;
                    case 4:
                        dJEffManager_c::createMassObjEffect(PARTICLE_RESOURCE_ID_MAPPING_124_, local16C, nullptr, &local17C);
                        break;
                    case 5:
                        dJEffManager_c::createMassObjEffect(PARTICLE_RESOURCE_ID_MAPPING_687_, local16C, nullptr, &local17C);
                        break;
                    case 6:
                        dJEffManager_c::createMassObjEffect(PARTICLE_RESOURCE_ID_MAPPING_800_, local16C, nullptr, &local17C);
                        break;
                    case 7:
                        ret = dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_838_, local16C, nullptr, nullptr, &local17C, nullptr, 0, 0);
                        if (ret != nullptr) {
                            ret->bindShpEmitter(0x24, true);
                        }
                        ret = dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_839_, local16C, nullptr, nullptr, &local17C, nullptr, 0, 0);
                        if (ret != nullptr) {
                            ret->bindShpEmitter(0x25, true);
                        }
                        break;
                    case 8:
                        ret = dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_840_, local16C, nullptr, nullptr, &local17C, nullptr, 0, 0);
                        if (ret != nullptr) {
                            ret->bindShpEmitter(0x26, true);
                        }
                        break;
                    case 9:
                        ret = dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_841_, local16C, nullptr, nullptr, &local17C, nullptr, 0, 0);
                        if (ret != nullptr) {
                            ret->bindShpEmitter(0x27, true);
                        }
                        break;
                }
                dSndSmallEffectMgr_c::GetInstance()->playSoundAtPosition(SE_O_GRASS_CUT, mGroundHeight);
            }
        }
    }
    return 1;
}

// non matching
undefined4 dTgMassObjInstance::FUN_80278c70(u32 param_2,dCcMassS_HitInf &param_3, dAcBase_c *param_4,
            GrassModel *param_5) {
    mVec3_c local54 = mGroundHeight;
    f32 fVar12 = 1000.f;
    s32 iVar11 = 0;
    f32 fVar1 = 0.55f;
    if ((param_2 & 2) == 0) {
        return 0;
    }
    if (param_4 == nullptr) {
        param_4 = dAcPy_c::GetLinkM();
    }
    if (param_3.GetCoHitObj()->ChkCo_0x10() && dAcPy_c::GetLink() != nullptr) {
        mVec3_c local60 = mGroundHeight;
        mVec3_c local6C;
        if (dAcPy_c::GetLink()->isUsingBugnet()) {
            local6C = dAcPy_c::GetLink()->getBugNetPos();
        } else {
            local6C = dAcPy_c::GetLink()->vt_0x278();
            local6C += (dAcPy_c::GetLink()->getSwordPos() - dAcPy_c::GetLink()->vt_0x278()) * 0.5f;
        }
        local60.y += 60.f;
        local54 = local60 - local6C;
        fVar12 = local60.distance(local6C);
        if (fVar12 > 40.f) {
            iVar11 = 1;
        } else {
            fVar12 = 1 - fVar12 / 250.f;
        }
    } else {
        iVar11 = param_3.GetCoHitObj()->ChkCo_0x2() ? 1 : 2;
    }
    if (iVar11 != 0) {
        // TODO here
        f32 fVar12_2 = 100.f;
        if (iVar11 == 2) {
            fVar12_2 = 200.f;
            fVar1 = 0.8f;
        }
        local54 = mGroundHeight - param_4->getPosition();
        if (param_5->mRadiusSquared < local54.squareMagXZ()) {
            return 0;
        }
        f32 distance = mGroundHeight.distance(param_4->getPosition());
        f32 fVar13;
        if (distance > fVar12_2) {
            fVar13 = fVar12_2;
        } else {
            fVar13 = distance;
            if (iVar11 == 1) {
                param_4->getSoundSource()->holdSound(SE_L_GRASS_RUSTLE_LV);
            }
        }
        fVar12 = 1 - fVar13 / fVar12_2;
    }
    mVec3_c local78(0.f, 1.f, 0.f);
    if (mDynamicTransform == nullptr) {
        mDynamicTransform = param_5->aquireTransform();
        if (mDynamicTransform == nullptr) {
            return 0;
        }
        mDynamicTransform->mQuat.set(mInitPosTransform->mQuat);
        mDynamicTransform->mMtx.fromQuat(mInitPosTransform->mQuat);
    }
    if (local54.isZero2()) {
        local54.set(1.f, 0.5f, 0.f);
    }
    mQuat_c local88;
    mVec3_c local98 = local54;
    if (!local98.normalizeRS()) {
        local98.x = 1.f;
        local98.z = 0.f;
    }
    local98.y = 0.5f;
    if (!local98.normalizeRS()) {
        local98 = local78;
    }
    local88.makeVectorRotation(local78, local98);
    local88.slerpTo(mInitPosTransform->mQuat, 1 - fVar1 * fVar12, local88);
    local88.normalise();
    local88.makeWPositive();
    mQuat_c localA8;
    mDynamicTransform->mMtx.toQuat(localA8);
    localA8.slerpTo(local88, cM::rndF(0.2f) + 0.2f, localA8);
    localA8.normalise();
    localA8.makeWPositive();
    mDynamicTransform->setQuat(local88);
    mDynamicTransform->setMtxFromQuat(localA8);
    return 1;
}

// matches besides data
undefined4 dTgMassObjInstance::handleLinkSpinAttack(GrassModel *param_2) {
    const dAcPy_c* link = dAcPy_c::GetLink();
    if (link == nullptr) {
        return 0;
    }
    if (!link->isAttackingSpin()) {
        return 0;
    } else {
        mVec3_c local3C;
        dAcPy_c::GetLink()->getPostionDifferenceOut(mGroundHeight, local3C);
        f32 dist = mGroundHeight.distance(dAcPy_c::GetLinkM()->getPosition());
        f32 comparison = 350.f;
        if (link->isAttackingDown()) {
            comparison = 150.f;
        } else if (link->checkSwordAndMoreStates(daPlayerActBase_c::SKYWARD_STRIKE_ACTIVE)) {
            comparison = 450.f;
        }
        if (dist > comparison) {
            return 0;
        }
        mVec3_c local48(0.f, 1.f, 0.f);
        if (mDynamicTransform == nullptr) {
            mDynamicTransform = param_2->aquireTransform();
            if (mDynamicTransform == nullptr) {
                return 0;
            }
            mDynamicTransform->mQuat.set(mInitPosTransform->mQuat);
            mDynamicTransform->mMtx.fromQuat(mInitPosTransform->mQuat);
        }
        if (local3C.isZero2()) {
            local3C.set(1.f, 0.5f, 0.f);
        }
        mQuat_c local58;
        mVec3_c local68 = local3C;
        if (!local68.normalizeRS()) {
            local68.x = 1.f;
            local68.z = 0.f;
        }
        local68.y = 0.5f;
        if (!local68.normalizeRS()) {
            local68 = local48;
        }
        local58.makeVectorRotation(local48, local68);
        local58.slerpTo2(cM::rndF(0.5f), mInitPosTransform->mQuat, local58);
        local58.normalise();
        local58.makeWPositive();
        mQuat_c local78;
        mDynamicTransform->mMtx.toQuat(local78);
        local78.slerpTo(local58, cM::rndF(0.3f) + 0.5f, local78);
        local78.normalise();
        local78.makeWPositive();
        mDynamicTransform->mQuat.set(local58);
        mDynamicTransform->setMtxFromQuat(local78);
        return 1;
    }
}

extern "C" bool fn_801BB700(EGG::Quatf*,f32);

// matches besides data
bool dTgMassObjInstance::isHidden(f32 param2, f32 param3) {
    bool uVar1 = mSpecialItemDropId != 10;
    if (uVar1) {
        mVec3_c tmp2(mGroundHeight.x, mGroundHeight.y + param2, mGroundHeight.z);
        EGG::Quatf tmp(param2 + 700.f, tmp2);
        uVar1 = fn_801BB700(&tmp, param3);
    }
    if (uVar1) {
        mGrassFlags |= dTgMassObjInstance::TG_MASS_UNK2_IS_HIDDEN;
        return true;
    } else {
        mGrassFlags &= ~dTgMassObjInstance::TG_MASS_UNK2_IS_HIDDEN;
        return false;
    }
}

// matches besides data
s32 dTgMassObjInstance::handleTimeshiftZone() {
    s32 uVar1 = 1;
    f32 fVar2 = dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(-1, mGroundHeight, nullptr, 8.f);
    if (mActiveInPresent == false) {
        sLib::addCalc(&mScale, fVar2, 0.5f, 0.2f, 0.01f);
        if (mScale <= 0) {
            uVar1 = 0;
        }
    } else if (mActiveInPresent == true) {
        sLib::addCalc(&mScale, 1.f - fVar2, 0.5f, 0.2f, 0.01f);
        if (mScale <= 0) {
            uVar1 = 0;
        }
    }
    return uVar1;
}

bool GrassModelData::tryCreateLinkedLists(s32 entrycount, EGG::Heap* heap) {
    mLinkedLists = new (heap, 4) dTgMassObjInstanceList[entrycount];
    if (mLinkedLists == nullptr) {
        destroyLinkedLists();
        return false;
    } else {
        mLinkedListsCount = entrycount;
        return true;
    }
}

void GrassModelData::destroyLinkedLists() {
    if (mLinkedLists != nullptr) {
        delete [] mLinkedLists;
        mLinkedLists = nullptr;
    }
}

void GrassModelData::unloadRoom(GrassModel *param_2,int roomid) {
    dTgMassObjInstanceList& gll = mLinkedLists[roomid];
    dTgMassObjInstanceList::Iterator it = gll.GetBeginIter();
    while (it != gll.GetEndIter()) {
        dTgMassObjInstance* lst = (&*(it));
        ++it;
        lst->releaseDynamicTransform(param_2);
        gll.remove(lst);
        param_2->addToFreeInstances(lst);
    }
}

void GrassModelData::initRes(nw4r::g3d::ResMat pResMat, nw4r::g3d::ResShp pResShp) {
    mResMat = pResMat;
    mResShp = pResShp;
    nw4r::g3d::ResMatChan chan = mResMat.GetResMatChan();
    chan.GXSetChanAmbColor(GX_COLOR0, mColor(0xFF, 0xFF, 0xFF, 0xFF));
}

void GrassModelData::addToRoom(s32 room, dTgMassObjInstance* p3) {
    mLinkedLists[room].append(p3);
}

// regalloc
void GrassModelData::removeFromRoom(s32 room, dTgMassObjInstance* p3) {
    mLinkedLists[room].remove(p3);
}

// matches besides data
void GrassModelData::update(GrassModel* param2) {
    mVec3_c groundHeight(999999.f, 999999.f, 999999.f);
    dLightEnv_c& lightEnv = dLightEnv_c::GetInstance();
    bool bVar1 = false;
    s32 count = 0;
    f32 minDist = lightEnv.getfield_0x38C0();
    dTgMassObjInstanceList* grassList = mLinkedLists;
    for (s32 roomid = 0; roomid < mLinkedListsCount; roomid++, grassList++ ) {
        if (grassList->mCount == 0 || dStage_c::GetInstance()->getRoom(roomid)->checkFlag(4)) {
            continue;
        }
        dTgMassObjInstanceList::Iterator it = grassList->GetBeginIter();
        while (it != grassList->GetEndIter()) {
            dTgMassObjInstance& lst = *it;
            ++it;
            if ((lst.mGrassFlags & dTgMassObjInstance::TG_MASS_UNK2_TIMESHIFT_RELATED) && !lst.handleTimeshiftZone()) {
                lst.mGrassFlags |= dTgMassObjInstance::TG_MASS_UNK2_IS_HIDDEN;
            } else {
                if ((lst.mGrassFlags & dTgMassObjInstance::TG_MASS_UNK2_IS_HIDDEN) == 0 && (lst.mGrassFlags & dTgMassObjInstance::TG_MASS_UNK2_IS_CUT) == 0) {
                    lst.checkForHit(param2, this, roomid);
                }
                if (lightEnv.getfield_0x38B0() == 1) {
                    if (lightEnv.getfield_0x38B4().distance(lst.mGroundHeight) < minDist) {
                        minDist = lightEnv.getfield_0x38B4().distance(lst.mGroundHeight);
                        groundHeight = lst.mGroundHeight;
                    }
                    if (lightEnv.getfield_0x38B4().distance(lst.mGroundHeight) < lightEnv.getfield_0x38C0()) {
                        bVar1 = true;
                        count++;
                    }
                }
            }
        }
    }
    if (lightEnv.getfield_0x38B0() == 1) {
        lightEnv.setfield_0x38B0(0);
        if (bVar1) {
            lightEnv.getfield_0x38C8() = groundHeight;
        }
        lightEnv.setfield_0x38C4(count);
    }
}

extern void LoadMaterial(nw4r::g3d::ResMat mat, u32 ctrl, nw4r::g3d::Draw1Mat1ShpSwap *pSwap, nw4r::g3d::G3DState::IndMtxOp *pIndMtxOp, bool bIgnoreMaterial);

// matches besides data
void GrassModelData::draw(f32 param_1,f32 param_2,
                 nw4r::math::MTX34 *pMtx) {
    mVec3_c cameraPosition = dScGame_c::getCamera()->getPositionMaybe();
    bool isInFaronWoods = dScGame_c::isCurrentStage("F100");
    nw4r::g3d::ResMatMisc miscData(mResMat.GetResMatMisc());
    miscData.SetLightSetIdx(1);
    miscData.SetFogIdx(0);
    LoadMaterial(mResMat, 0, nullptr, nullptr, false);
    nw4r::g3d::G3DState::LoadResShpPrePrimitive(mResShp);
    const static u32 fifoMtx[8] = {
        0x3C,
        0x3C,
        0x3C,
        0x3C,
        0x3C,
        0x3C,
        0x3C,
        0x3C,
    };
    nw4r::g3d::fifo::GDSetCurrentMtx(fifoMtx);
    GXSetCurrentMtx(0);
    dTgMassObjInstanceList* grassList = mLinkedLists;
    for (s32 roomid = 0; roomid < mLinkedListsCount; roomid++, grassList++ ) {
        if (grassList->mCount == 0 || dStage_c::GetInstance()->getRoom(roomid)->checkFlag(4)) {
            continue;
        }
        dTgMassObjInstanceList::Iterator it = grassList->GetBeginIter();
        for (;it != grassList->GetEndIter();++it) {
            dTgMassObjInstance* lst = (&*it);
            if (!lst->isHidden(param_1, param_2)) {
                mMtx_c local160;
                lst->getDrawMatrix(&local160);
                f32 cameraDist = lst->mGroundHeight.distance(cameraPosition);
                f32 fVar6 = param_2 * 0.6f;
                if (cameraDist > fVar6) {
                    f32 fVar7 = (cameraDist - fVar6) / (param_2 - fVar6);
                    if (fVar7 > 1.f) {
                        fVar7 = 1.f;
                    }
                    if (fVar7 < 0.f) {
                        fVar7 = 0.f;
                        continue;
                    } else {
                        f32 scale = 1. - fVar7; // double
                        local160.scaleM(scale, scale, scale);
                    }
                }
                if (isInFaronWoods
                    && lst->mGroundHeight.x <= -5200.f
                    && lst->mGroundHeight.x >= -5500.f
                    && lst->mGroundHeight.z <= -5900.f
                    && lst->mGroundHeight.z >= -6100.f) {
                    local160.scaleM(1.f, 1.5f, 1.f);
                }
                f32 fVar7 = ((s32)(lst->mGroundHeight.x + lst->mGroundHeight.z) & 0x1F) / 31.f;
                f32 scale = fVar7 * 0.1f + 1.f;
                local160.scaleM(scale, scale, scale);
                local160.YrotM(fVar7 * 65535.f);
                MTXConcat(*pMtx, local160, local160);
                GXLoadPosMtxImm(local160, 0);
                GXLoadNrmMtxImm(local160, 0);
                GXSetTevColorS10(GX_TEVREG1, lst->mTevColor);
                mResShp.CallPrimitiveDisplayList(false);
            }
        }
    }
}
