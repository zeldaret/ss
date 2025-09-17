#include "d/a/d_a_bird_base.h"

#include "c/c_math.h"
#include "common.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "m/m3d/m_fanm.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"

void dAcBirdBase_c::callback_c::timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) {
    // TODO
}

int dAcBirdBase_c::doDelete() {
    return SUCCEEDED;
}

int dAcBirdBase_c::draw() {
    drawModelType1(&mMdl.getModel());
    return SUCCEEDED;
}

void dAcBirdBase_c::updateMatrixBird() {
    mWorldMtx.transS(position);
    mWorldMtx.concat(field_0xF80);
    mWorldMtx.ZrotM(mRotation.z);
    mWorldMtx.scaleM(field_0xF50.x, field_0xF50.y, field_0xF50.z);
    mMdl.getModel().setLocalMtx(mWorldMtx);
    mMdl.getModel().calc(false);
}

bool dAcBirdBase_c::changeBirdAction(const char *animName, s32 action, m3d::playMode_e playMode, f32 blend) {
    void *data;
    if (action < BIRD_ACTION_HOVER) {
        data = getOarcResFile(getAnmArcNameBase());
    } else if (action < BIRD_ACTION_BASE_MAX) {
        data = getOarcResFile(getAnmArcNameLink());
    } else {
        data = getOarcResFile(getAnmArcNameSpecial());
    }
    mMdl.setAnmFile(data);
    bool ret = false;
    if (mBirdAction != action) {
        mMdl.setAnm(animName, playMode, blend);
        mBirdAction = action;
        ret = true;
    }
    return ret;
}

void dAcBirdBase_c::executeBirdAction() {
    if (mBirdAction == BIRD_ACTION_GLIDE) {
        if (--mBirdActionTimer <= 0) {
            changeBirdAction("Flap", BIRD_ACTION_FLAP, m3d::PLAY_MODE_4, 5.0f);
            mBirdActionTimer = cM::rndInt(100) + 100;
        } else if (field_0xF5C.y < -0.3f) {
            changeBirdAction("SwitchToDive", BIRD_ACTION_SWITCH_DIVE, m3d::PLAY_MODE_4, 5.0f);
        }
    } else if (mBirdAction == BIRD_ACTION_GLIDE_DIVE) {
        if (field_0xF5C.y >= -0.2f) {
            changeBirdAction("SwitchToDive", BIRD_ACTION_SWITCH_DIVE, m3d::PLAY_MODE_3, 5.0f);
        }
    } else if (mBirdAction == BIRD_ACTION_SWITCH_DIVE) {
        if (mMdl.getAnm().isStop()) {
            if (mMdl.getAnm().getPlayMode() == m3d::PLAY_MODE_3) {
                changeBirdAction("Glide", BIRD_ACTION_GLIDE, m3d::PLAY_MODE_4, 5.0f);
            } else {
                changeBirdAction("GlideDive", BIRD_ACTION_GLIDE_DIVE, m3d::PLAY_MODE_4, 5.0f);
            }
        } else if (field_0xF5C.y >= -0.15f) {
            changeBirdAction("SwitchToDive", BIRD_ACTION_SWITCH_DIVE, m3d::PLAY_MODE_3, 5.0f);
        }
    } else {
        if ((mMdl.getAnm().isStop() && mBirdAction == BIRD_ACTION_FLAP) ||
            (mMdl.getAnm().isStop() && mBirdAction == BIRD_ACTION_DAMAGE) ||
            (mMdl.getAnm().isStop() && mBirdAction == BIRD_ACTION_HOVER)) {
            changeBirdAction("Glide", BIRD_ACTION_GLIDE, m3d::PLAY_MODE_4, 5.0f);
            mBirdActionTimer = cM::rndInt(100) + 100;
        }
    }
}

bool dAcBirdBase_c::chkWallGndRoofHit() {
    mVec3_c tmp;
    tmp.set(position); // unused

    if (--mChkWallGndRoofTimer <= 0) {
        mChkWallGndRoofTimer = cM::rndInt(10) + 20;
        if (mObjAcch2.ChkWallHit(nullptr) || mObjAcch2.ChkGndHit() || mObjAcch2.ChkRoofHit()) {
            mChkWallGndRoofTimer = 60;
            handleWallGndRoofHit();
            return true;
        }
    }
    return false;
}

void dAcBirdBase_c::handleWallGndRoofHit() {
    dBgS_ObjLinChk linChk;
    mVec3_c start, end;
    start.set(position);
    end.set(position);

    mVec3_c v1;
    mMtx_c m1;
    mMtx_c m2;

    mAng rot1 = field_0xF5C.atan2sX_Z();
    if (mObjAcch2.ChkGndHit()) {
        end.set(position);
        v1.set(0.0f, 0.0f, 10000.0f);
        m1.YrotS(rot1);
        m1.XrotM(-0x4000);
        MTXMultVec(m1, v1, v1);
        end += v1;
        linChk.Set(&start, &end, nullptr);
        if (!dBgS::GetInstance()->LineCross(&linChk)) {
            field_0xF68.set(v1);
            field_0xF68.normalizeRS();
            return;
        }

        end.set(position);
        v1.set(0.0f, 0.0f, 10000.0f);
        m1.YrotS(rot1);
        m1.XrotM(0x4000);
        MTXMultVec(m1, v1, v1);
        end += v1;
        linChk.Set(&start, &end, nullptr);
        if (!dBgS::GetInstance()->LineCross(&linChk)) {
            field_0xF68.set(v1);
            field_0xF68.normalizeRS();
            return;
        }
    }

    v1.set(0.0f, 0.0f, 10000.0f);
    v1.rotY(rot1 + mAng(0x4000));
    end += v1;
    linChk.Set(&start, &end, nullptr);
    if (!dBgS::GetInstance()->LineCross(&linChk)) {
        field_0xF68.set(v1);
        field_0xF68.normalizeRS();
        return;
    }

    end.set(position);
    v1.set(0.0f, 0.0f, 10000.0f);
    v1.rotY(rot1 - mAng(0x4000));
    end += v1;
    linChk.Set(&start, &end, nullptr);
    if (!dBgS::GetInstance()->LineCross(&linChk)) {
        field_0xF68.set(v1);
        field_0xF68.normalizeRS();
        return;
    }

    end.set(position);
    v1.set(0.0f, 0.0f, 10000.0f);
    m2.YrotS(rot1);
    m2.XrotM(0xC000);
    MTXMultVec(m2, v1, v1);
    end += v1;
    linChk.Set(&start, &end, nullptr);
    if (!dBgS::GetInstance()->LineCross(&linChk)) {
        field_0xF68.set(v1);
        field_0xF68.normalizeRS();
        return;
    }

    end.set(position);
    v1.set(0.0f, 0.0f, 10000.0f);
    m2.YrotS(rot1);
    m2.XrotM(0x4000);
    MTXMultVec(m2, v1, v1);
    end += v1;
    linChk.Set(&start, &end, nullptr);
    if (!dBgS::GetInstance()->LineCross(&linChk)) {
        field_0xF68.set(v1);
        field_0xF68.normalizeRS();
        return;
    }
}
