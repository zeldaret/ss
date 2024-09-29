#include <d/a/d_a_bombf.h>
#include <d/a/d_a_player.h>
#include <toBeSorted/actor_collision.h>
#include <toBeSorted/sceneflag_manager.h>

SPECIAL_ACTOR_PROFILE(TAG_WOOD_AREA, dAcBombf_c, fProfile::WOODAREA_TAG, 0x166, 0, 2);

STATE_DEFINE(dAcBombf_c, Wait);

bool dAcBombf_c::createHeap() {
    nw4r::g3d::ResFile resFile = getOarcResFile("FlowerBomb");
    nw4r::g3d::ResMdl resMdl = resFile.GetResMdl("LeafBomb");
    return mModel.create(resMdl, &heap_allocator, 0x120, 1, nullptr);
}

int dAcBombf_c::actorCreate() {
    mDespawnSceneFlag = (params >> 8) & 0xFF;
    field_0x3D4 = (params >> 16) & 0xF;
    if (SceneflagManager::sInstance->checkBoolFlag(roomid, mDespawnSceneFlag)) {
        return FAILED;
    }

    CREATE_ALLOCATOR(dAcBombf_c);

    field_0x3D2 = (params >> 4) & 0xF;
    field_0x3D0 = field_0x3D2 == 1;
    if (field_0x3D0) {
        mModel.setPriorityDraw(0x82, 0x7F);
    }

    mCCdStruct.setField0x38ToMagicValue();
    forwardAccel = -2.0f;
    forwardMaxSpeed = -80.0f;
    mStateMgr.changeState(StateID_Wait);
    setBoundingBox(mVec3_c(-80.0, -50.0f, -80.0f), mVec3_c(80.0, 60.0f, 80.0f));
    rotcopy1 = rotation;
    if (mDespawnSceneFlag < 0xFF) {
        actor_properties = (actor_properties & ~1) | 4;
    }

    return SUCCEEDED;
}

extern "C" bool fn_803421C0(const mVec3_c &, const mVec3_c &, dAcObjBase_c *);
extern "C" mVec3_c lbl_805D02D8;
extern "C" bool fn_80342300();

int dAcBombf_c::actorPostCreate() {
    mMtx_c mtx;
    mtx.ZXYrotS(rotation.x, rotation.y, rotation.z);
    mVec3_c v;
    PSMTXMultVecSR(mtx, mVec3_c::Ey, v);
    mVec3_c v1 = v * 10.0f;
    mVec3_c v3 = position + v1;
    mVec3_c v4 = position - v1;

    if (fn_803421C0(v3, v4, this)) {
        position = lbl_805D02D8;
        if (rotation.x == 0 && rotation.z == 0 && fn_80342300()) {
            // TODO
        }
    }

    return SUCCEEDED;
}

int dAcBombf_c::doDelete() {
    return SUCCEEDED;
}

extern "C" bool dBgS__ChkMoveBG(CollisionCheckContext *, UnkCollisionStruct4 *, bool);
extern "C" void dBgS__MoveBgTransPos(CollisionCheckContext *, UnkCollisionStruct4 *, bool param_1, mVec3_c *i_pos,
        mAng3_c *i_angle, mAng3_c *i_shapeAngle);

int dAcBombf_c::actorExecute() {
    if (field_0x3D3 != 0) {
        mMtx_c *mtx = &worldMatrix;
        mtx->mat.getTranslation(position);
        dAcBomb_c *bomb = mBombRef.get();
        if (bomb != nullptr) {
            bomb->setTransformFromFlower(*mtx);
        }
        mModel.setLocalMtx(worldMatrix);
        poscopy2 = position;
        poscopy3 = position;
        field_0x3D3 = 0;
    } else {
        if (dBgS__ChkMoveBG(CollisionCheckContext::get(), &field_0x398, true)) {
            dBgS__MoveBgTransPos(CollisionCheckContext::get(), &field_0x398, true, &position, &rotcopy1, &rotation);
            updateMatrix();
            dAcBomb_c *bomb = mBombRef.get();
            if (bomb != nullptr) {
                bomb->setTransformFromFlower(worldMatrix);
            }
            mModel.setLocalMtx(worldMatrix);
            poscopy2 = position;
            poscopy3 = position;
        }
    }

    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcBombf_c::draw() {
    if (field_0x3D0 != 0 && (dAcPy_c::LINK->mActionFlagsCont & 0x400000) == 0) {
        return SUCCEEDED;
    }
    drawModelType1(&mModel);
    return SUCCEEDED;
}

void dAcBombf_c::regrowBomb() {
    // These params are hell
    s8 viewclip_idx = (actor_properties & 1) != 0 ? viewclip_index : -1;
    u32 actorParams1;
    actorParams1 = 1;
    if (field_0x3D0) {
        actorParams1 = 2;
    }
    dAcObjBase_c *ac = dAcObjBase_c::create("Bomb", roomid, actorParams1, &position, nullptr, nullptr, 0xFFFFFFFF,
            0xFFFF, viewclip_idx);
    mBombRef.link(static_cast<dAcBomb_c *>(ac));
    dAcBomb_c *bomb = mBombRef.get();
    if (bomb != nullptr) {
        field_0x394 = 0x3C;
        bomb->setTransformFromFlower(worldMatrix);
        if ((actor_properties & 1) != 0) {
            bomb->actor_properties |= 1;
        }
        if (field_0x3D4 == 0) {
            bomb->field_0xA44 *= 1.5f;
        }
    }
}

void dAcBombf_c::initializeState_Wait() {}

extern "C" void fn_800297B0(u16, mVec3_c *, mVec3_c *, u32, u32, u32, u32, u32);
extern "C" u16 lbl_8057A750;

void dAcBombf_c::executeState_Wait() {
    // Stack problems
    if (SceneflagManager::sInstance->checkBoolFlag(roomid, mDespawnSceneFlag)) {
        deleteRequest();
        dAcBomb_c *bomb = mBombRef.get();
        if (bomb != nullptr) {
            bomb->deleteRequest();
            bomb->mEffects1.remove(true);
            bomb->mEffects2.remove(true);
        }
        return;
    }

    f32 scaleFactor = scale.x;
    if (field_0x3D2 == 0 || field_0x3D2 == 2) {
        mVec3_c m;
        PSMTXMultVecSR(worldMatrix, mVec3_c::Ey, m);
        mVec3_c m3 = position + (m * 30.0f);

        if (mTimeAreaStruct.check(roomid, m3, 0, 30.0f, 1.0f) && field_0x3D4 != 1) {
            if (mTimeAreaStruct.field_0x04 == 1) {
                playSound(0xC0A);
            } else {
                playSound(0xC0B);
            }
            fn_800297B0(lbl_8057A750, &position, nullptr, 0, 0, 0, 0, 0);
        }

        scaleFactor *= mTimeAreaStruct.field_0x00;
        if (field_0x3D2 == 2) {
            scaleFactor = 1.0f - scaleFactor;
        }
        scaleFactor = nw4r::ut::Max(scaleFactor, 0.0001f);
        mModel.setScale(scaleFactor, scaleFactor, scaleFactor);
    }
    dAcBomb_c *bomb = mBombRef.get();
    if (bomb != nullptr) {
        if (!bomb->mStateMgr.isState(dAcBomb_c::StateID_FlowerWait) && field_0x3D0 == 0) {
            mBombRef.unlink();
            field_0x394 = 0x3C;
        }
    } else {
        if (field_0x394 != 0) {
            field_0x394--;
        } else if (field_0x3D0 == 0 || dAcPy_c::LINK->getSquareDistanceTo(position) > 22500.0f) {
            regrowBomb();
        }
    }

    bomb = mBombRef.get();
    if (bomb != nullptr) {
        bomb->field_0xA50 = scaleFactor;
    }
}

void dAcBombf_c::finalizeState_Wait() {}
