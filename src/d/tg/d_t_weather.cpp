#include "d/t/d_t_weather.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_camera.h"
#include "d/d_light_env.h"
#include "d/d_sc_game.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggTexture.h"
#include "egg/math/eggMatrix.h"
#include "m/m3d/m_scnleaf.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXGeometry.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/minigame_mgr.h"

#include <cmath>

SPECIAL_ACTOR_PROFILE(WEATHER_TAG, dWeatherTag_c, fProfile::WEATHER_TAG, 0xC, 0x0, 0x104);

STATE_DEFINE(dWeatherTag_c, Wait);

bool dWeatherTag_c::spawnWeatherTag() {
    dAcObjBase_c::createActorUnkGroup3(fProfile::WEATHER_TAG, -1, 0, nullptr, nullptr, nullptr, -1);
    return sInstance != nullptr;
}

dWeatherTag_c *dWeatherTag_c::GetInstance() {
    return sInstance;
}

bool avoidRainInThunderheadLocation(bool unk) {
    // TODO
    return true;
}

bool isAirborne() {
    // TODO
    return true;
}

bool isInTentalusBossRoomAfterDefeat() {
    bool ret = false;
    if (dScGame_c::isCurrentStage("B301") && DungeonflagManager::sInstance->getCounterOrFlag(3, 8) &&
        !MinigameManager::GetInstance()->checkInBossRush()) {
        ret = true;
    }
    return ret;
}

bool dWeatherTag_c::createHeap() {
    // TODO
    u32 dummy1 = 0;
    if (!mRain.create(&mAllocator, &dummy1)) {
        return true;
    }
    mRain.setRes(dAcObjBase_c::getOarcFile("Common", "tex/WK2H_Raindrop00.bti"));

    u32 dummy2 = 0;
    if (!mMoya.create(&mAllocator, &dummy2)) {
        return true;
    }
    mMoya.setRes(dAcObjBase_c::getOarcFile("Common", "tex/WK2_moya.bti"));

    u32 dummy3 = 0;
    if (!mNearCloud.create(&mAllocator, &dummy3)) {
        return true;
    }
    mNearCloud.setRes(dAcObjBase_c::getOarcFile("Common", "tex/Cloud00.bti"));

    u32 dummy4 = 0;
    if (!mLightning.create(&mAllocator, &dummy4)) {
        return true;
    }

    u32 dummy5 = 0;
    if (!mSunFlareStars.create(&mAllocator, &dummy5)) {
        return true;
    }
    mSunFlareStars.setResLensFlare1(dAcObjBase_c::getOarcFile("Common", "tex/vr_lenzf_tex.bti"));
    mSunFlareStars.setResRing(dAcObjBase_c::getOarcFile("Common", "tex/txa_ring_a_32hafe.bti"));
    mSunFlareStars.setResLensFlare2(dAcObjBase_c::getOarcFile("Common", "tex/vr_lenzf_tex.bti"));
    mSunFlareStars.setResStar(dAcObjBase_c::getOarcFile("Common", "tex/star.bti"));

    u32 dummy6 = 0;
    if (!mMoon.create(&mAllocator, &dummy6)) {
        return true;
    }
    mMoon.doCreate();

    u32 dummy7 = 0;
    if (!mClouds.create(&mAllocator, &dummy7)) {
        return true;
    }
    mClouds.doCreate();

    return true;
}

int dWeatherTag_c::create() {
    CREATE_ALLOCATOR(dWeatherTag_c);

    mMoya.setPriorityDraw(0, 0x8E);
    mMoya.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);

    mRain.setPriorityDraw(0, 0x8E);
    mRain.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);

    mNearCloud.setPriorityDraw(0, 0x8E);
    mNearCloud.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);

    mLightning.setPriorityDraw(0, 0x8E);
    mLightning.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);

    mSunFlareStars.setPriorityDraw(0x5, 0x8E);

    mMoon.setPriorityDraw(0x5, 0);
    mMoon.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_XLU, true);

    if (dScGame_c::isCurrentStage("F023")) {
        mClouds.setPriorityDraw(0, 0x3);
        mClouds.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);
    } else if (dScGame_c::isCurrentStage("F000") || dScGame_c::isCurrentStage("F020")) {
        mClouds.setPriorityDraw(0, 0x85);
        mClouds.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);
    } else {
        mClouds.setPriorityDraw(0x6, 0);
        mClouds.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_XLU, true);
    }

    mStateMgr.changeState(StateID_Wait);

    mNearCloud.setField_0xF5(0);
    mSunFlareStars.setField_0x1C0(0);
    field_0x4288 = mVec3_c(0.0f, 0.0f, 0.0f);
    field_0x4294 = mVec3_c(0.0f, 0.0f, 0.0f);

    if (dScGame_c::isCurrentStage("F300") || dScGame_c::isCurrentStage("F301") || dScGame_c::isCurrentStage("F301_4") ||
        ((dScGame_c::isCurrentStage("F000") || dScGame_c::isCurrentStage("F020") || dScGame_c::isCurrentStage("F023") ||
          dScGame_c::isCurrentStage("B400")) &&
         !dScGame_c::currentSpawnInfo.isNight())) {
        mNearCloud.setField_0xF5(1);
    }

    mClouds.setField_0x2DD9(0);
    if (dScGame_c::isCurrentStage("B100_1") || dScGame_c::isCurrentStage("B301") || dScGame_c::isCurrentStage("B400") ||
        dScGame_c::isCurrentStage("D000") || dScGame_c::isCurrentStage("D201_1") || dScGame_c::isCurrentStage("D301") ||
        dScGame_c::isCurrentStage("F000") || dScGame_c::isCurrentStage("F002r") || dScGame_c::isCurrentStage("F020") ||
        dScGame_c::isCurrentStage("F021") || dScGame_c::isCurrentStage("F023") || dScGame_c::isCurrentStage("F100") ||
        dScGame_c::isCurrentStage("F101") || dScGame_c::isCurrentStage("F102") || dScGame_c::isCurrentStage("F102_1") ||
        dScGame_c::isCurrentStage("F200") || dScGame_c::isCurrentStage("F201_3") ||
        dScGame_c::isCurrentStage("F201_4") || dScGame_c::isCurrentStage("F202") || dScGame_c::isCurrentStage("F300") ||
        dScGame_c::isCurrentStage("F300_1") || dScGame_c::isCurrentStage("F300_4") ||
        dScGame_c::isCurrentStage("F301") || dScGame_c::isCurrentStage("F301_1") ||
        dScGame_c::isCurrentStage("F301_3") || dScGame_c::isCurrentStage("F301_4") ||
        dScGame_c::isCurrentStage("F301_6") || dScGame_c::isCurrentStage("F302") || dScGame_c::isCurrentStage("F400") ||
        dScGame_c::isCurrentStage("F403") || dScGame_c::isCurrentStage("F406") || dScGame_c::isCurrentStage("S000") ||
        dScGame_c::isCurrentStage("S200") || dScGame_c::isCurrentStage("S300")) {
        mSunFlareStars.setField_0x1C0(1);
        mClouds.setField_0x2DD9(1);
    } else if (dScGame_c::isCurrentStage("F103")) {
        mSunFlareStars.setField_0x1C0(1);
    }

    if (dScGame_c::isCurrentStage("B301") || dScGame_c::isCurrentStage("F023") || dScGame_c::isCurrentStage("B400")) {
        mRain.setField_0x31(1);
    }

    return SUCCEEDED;
}

int dWeatherTag_c::doDelete() {
    // TODO
    return SUCCEEDED;
}

int dWeatherTag_c::actorExecuteInEvent() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dWeatherTag_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dWeatherTag_c::draw() {
    dLightEnv_c &env = dLightEnv_c::GetInstance();
    if (GetInstance() == nullptr) {
        return NOT_READY;
    }

    if (dAcPy_c::GetLinkM()->checkActionFlagsCont(0x400000)) {
        return SUCCEEDED;
    }

    if (!env.getField_0x5D59(9)) {
        mClouds.entry();
    }
    mMoya.entry();
    mRain.entry();
    mNearCloud.entry();
    mLightning.entry();
    if (!env.getField_0x5D59(9)) {
        mSunFlareStars.entry();
        mMoon.entry();
    }
    return SUCCEEDED;
}

void dWeatherTag_c::rain_c::calc() {
    dLightEnv_c &env = dLightEnv_c::GetInstance();
    dCamera_c *cam = dScGame_c::getCamera();

    if (cam != nullptr && field_0x31) {
        field_0x2C = 400;
        env.setField_0x38E1(0);
        if (!isInTentalusBossRoomAfterDefeat()) {
            if (dScGame_c::isCurrentStage("F023")) {
                if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_LEVIAS_FIGHT_DEFEATED)) {
                    // No rain in Thunderhead after Levias is defeated
                    return;
                }
                if (env.getSpfSetting().mIdxStart == 0 && env.getSpfSetting().mIdxEnd == 1) {
                    field_0x2C = env.getSpfSetting().mRatio * 400.0f;
                }
                if (avoidRainInThunderheadLocation(0) == true) {
                    field_0x2C = -1;
                }
            } else if (dScGame_c::isCurrentStage("B400")) {
                if (DungeonflagManager::sInstance->getCounterOrFlag(3, 8)) {
                    // No rain in Demise Sanctuary after Demise is defeated
                    return;
                }

                if (env.getField_0x38E0() == 0) {
                    return;
                }
            }

            if (field_0x30 == 0) {
                if (!dScGame_c::isCurrentStage("B400")) {
                    field_0x28 = 1.0f;
                }
                field_0x30 = 1;
            }
            env.setField_0x38E1(1);
        }
    }
}

void dWeatherTag_c::rain_c::drawXlu() {
    u32 i1 = 10;
    dLightEnv_c &env = dLightEnv_c::GetInstance();
    dCamera_c *cam = dScGame_c::getCamera();
    bool b = false;

    mColor col(0xFF, 0xFF, 0xFF, 0x28);

    EGG::Matrix34f cameraMtx(
        EGG::DrawGX::s_cameraMtx._00, EGG::DrawGX::s_cameraMtx._01, EGG::DrawGX::s_cameraMtx._02,
        EGG::DrawGX::s_cameraMtx._03, EGG::DrawGX::s_cameraMtx._10, EGG::DrawGX::s_cameraMtx._11,
        EGG::DrawGX::s_cameraMtx._12, EGG::DrawGX::s_cameraMtx._13, EGG::DrawGX::s_cameraMtx._20,
        EGG::DrawGX::s_cameraMtx._21, EGG::DrawGX::s_cameraMtx._22, EGG::DrawGX::s_cameraMtx._23
    );

    mVec3_c camPos = cam->getPosition();
    mVec3_c camTgt = cam->getTarget();
    mVec3_c offset(0.0f, 220.0f, 0.0f);

    f32 camSpeed = mPrevCamPos.distance(camPos);
    mPrevCamPos = camPos;

    if (field_0x31 && (cam == nullptr || !cam->isUnderwater())) {
        mVec3_c camDir;
        env.get_vectle_calc(&camPos, &camTgt, &camDir);
        camDir.normalize();
        if (dScGame_c::isCurrentStage("B301")) {
            if (isInTentalusBossRoomAfterDefeat()) {
                return;
            }
            i1 = 8;
        } else if (dScGame_c::isCurrentStage("F023")) {
            if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_LEVIAS_FIGHT_DEFEATED)) {
                // No rain in Thunderhead after Levias is defeated
                return;
            }
            if (avoidRainInThunderheadLocation(0) == true) {
                b = true;
                i1 = 8;
            }
        } else if (dScGame_c::isCurrentStage("B400")) {
            if (DungeonflagManager::sInstance->getCounterOrFlag(3, 8)) {
                return;
            }
            if (env.getField_0x38E0() == 0) {
                return;
            }

            sLib::chase(&field_0x28, 1.0f, 0.0025f);
            i1 = 8;
        }

        f32 speed = camSpeed / 100.0f;
        if (speed > 1.0f) {
            speed = 1.0f;
        }

        f32 f2 = 1.0f - speed * 0.9f;
        u8 alpha = i1 * field_0x28;

        if (field_0x2C != 0) {
            GXSetCullMode(GX_CULL_NONE);
            GXSetClipMode(GX_CLIP_DISABLE);
            if (!b) {
                EGG::DrawGX::LoadTexture(mpTexture, GX_TEXMAP0);
                EGG::DrawGX::BeginDrawQuad(
                    EGG::DrawGX::COLORCHAN_1, EGG::DrawGX::ZMODE_0, EGG::DrawGX::BLEND_3, true, false
                );
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);
                GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
                GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
                f32 f = 0.0f;
                if (camDir.y > 0.0f) {
                    if (camDir.y < 0.5f) {
                        f = 1.0f - camDir.y / 0.5f;
                    } else {
                        f = 0.0f;
                    }
                } else {
                    f = 1.0f;
                }
                col.a = col.a * f;
                mVec3_c rayTgt(
                    camPos.x + camDir.x * 5000.0f, camPos.y + camDir.y * 4000.0f, camPos.z + camDir.z * 5000.0f
                );
                f32 scale = 80.0f;

                for (int i = 0; i < field_0x2C >> 1; i++) {
                    mVec3_c tmp;
                    tmp.x = cM::rndFX(3000.0f) + rayTgt.x;
                    tmp.y = cM::rndFX(3000.0f) + rayTgt.y;
                    tmp.z = cM::rndFX(3000.0f) + rayTgt.z;
                    mVec3_c tmp2;
                    MTXMultVec(*cameraMtx, tmp, tmp2);
                    mMtx_c mtx2;
                    mtx2.transS(tmp2);
                    mMtx_c mtx3;
                    MTXScale(mtx3, scale, scale, scale);
                    MTXConcat(mtx2, mtx3, mtx2);
                    EGG::DrawGX::DrawDL(EGG::DrawGX::DL_13, mtx2, col);
                }
            }
            GXSetClipMode(GX_CLIP_ENABLE);
            nw4r::math::MTX34 camMtx1 = EGG::DrawGX::s_cameraMtx;
            // what?
            nw4r::math::MTX34 camMtx2 = EGG::DrawGX::s_cameraMtx;
            MTXCopy(camMtx2, EGG::DrawGX::s_cameraMtx);
            if (field_0x2C > 0) {
                EGG::DrawGX::BeginDrawLine(EGG::DrawGX::COLORCHAN_1, EGG::DrawGX::ZMODE_0);
                for (int i = 0; i <= field_0x2C; i++) {
                    mVec3_c tmp;
                    tmp.x = camPos.x + camDir.x * 1000.0f + cM::rndFX(900.0);
                    tmp.y = camPos.y + camDir.y * 1000.0f + cM::rndFX(900.0);
                    tmp.z = camPos.z + camDir.z * 1000.0f + cM::rndFX(900.0);

                    mVec3_c tmp2;
                    tmp2.x = tmp.x + f2 * (cM::rndFX(10.0f) + offset.x);
                    tmp2.y = tmp.y + f2 * offset.y;
                    tmp2.z = tmp.z + f2 * (cM::rndFX(10.0f) + offset.z);

                    mVec3_c tmp3;
                    tmp3.set(tmp);
                    f32 camDist = tmp3.distance(camPos);
                    if (camDist < 100.0f) {
                        continue;
                    }
                    tmp3.set(tmp2);
                    camDist = tmp3.distance(camPos);
                    if (camDist < 100.0f) {
                        continue;
                    }
                    EGG::DrawGX::DrawLine(tmp, 2, mColor(0xB5, 0xD7, 0xFF, alpha), 6);
                }
            } else {
                EGG::DrawGX::BeginDrawLine(EGG::DrawGX::COLORCHAN_1, EGG::DrawGX::ZMODE_2);
                for (int i = 0; i <= 100; i++) {
                    // Why an nw4r vec here?
                    nw4r::math::VEC3 tmp;
                    tmp.x = cM::rndFX(150.0f) + -167642.0f;
                    tmp.y = cM::rndF(-1000.0f) + 18199.0f;
                    tmp.z = cM::rndFX(150.0f) + -51111.0f;
                    mVec3_c v = tmp + offset;
                    EGG::DrawGX::DrawLine(v, 2, mColor(0xB5, 0xD7, 0xFF, alpha), 6);
                }
            }
            MTXCopy(camMtx1, EGG::DrawGX::s_cameraMtx);
        }
    }
}

void dWeatherTag_c::moya_c::calc() {
    dLightEnv_c &env = dLightEnv_c::GetInstance();
    f32 speed = -100.0f;
    u8 maxAlpha = 0xFF;
    if (field_0xB81 && (!env.fn_80024770(8) || env.fn_800247A0(8))) {
        mVec3_c playerPos_ = dAcPy_c::GetLinkM()->mPosition;
        // okay
        mVec3_c unused1(0.0f, -6.0f, 0.0f);
        mVec3_c unused2(0.0f, 220.0f, 0.0f);
        dCamera_c *cam = dScGame_c::getCamera();
        if (cam != nullptr) {
            mVec3_c camPos = cam->getPosition();
            mVec3_c camTgt = cam->getTarget();
            mVec3_c camDir = camTgt - camPos;
            if (camDir.normalizeRS()) {
                mVec3_c dir2 = camDir;
                f32 camSpeed = mPrevCamPos.distance(camPos);
                mPrevCamPos = camPos;
                f32 alpha = camSpeed / 100.0f;
                if (dAcPy_c::GetLinkM()->checkCurrentAction(19 /* FREE_FALL */)) {
                    alpha = camSpeed / 10.0f;
                }
                field_0xB82 = false;
                if (env.getMist().mMode != 0) {
                    u8 dec = env.getMist().mDecay;
                    maxAlpha = dec * 0.7f;
                    if (dec >= env.getMist().mAlpha) {
                        env.getMist().mDecay -= dec;
                    } else {
                        env.getMist().mDecay = 0;
                    }
                    speed = -env.getMist().mSpeed;
                    alpha = env.getMist().mDecay / 255.0f;
                    if (env.getMist().mMode != 2) {
                        env.sphere_to_cartesian(0.0f, env.getMist().mDirection, &dir2);
                    }
                    if (env.getMist().mMode == 3) {
                        dir2 = env.getMistPos();
                        env.getMist().mMode = 0;
                    }
                    field_0xB82 = true;
                }
                if (alpha > 1.0f) {
                    alpha = 1.0f;
                }
                sLib::addCalc(&mAlphaScale, alpha * 0.8f + 0.2f, 0.5f, 0.5f, 0.1f);
                mVec3_c rayTgt = camPos + camDir * 1800.0f;
                if (!mInitialSet) {
                    // Set all initial particle positions
                    for (int i = 0; i < NUM; i++) {
                        mParticleBasePos[i] = rayTgt;
                        mParticleOffset[i].x = cM::rndFX(1800.0f);
                        mParticleOffset[i].y = cM::rndFX(1800.0f);
                        mParticleOffset[i].z = cM::rndFX(1800.0f);
                        mMaxAlpha[i] = maxAlpha;
                        mParticleSpeed[i] = cM::rndF(0.4f) + 0.6f;
                    }
                    mInitialSet = true;
                }

                for (int i = 0; i < NUM; i++) {
                    mVec3_c t1;
                    mParticleOffset[i].x += speed * mParticleSpeed[i] * dir2.x;
                    mParticleOffset[i].y += speed * mParticleSpeed[i] * dir2.y;
                    mParticleOffset[i].z += speed * mParticleSpeed[i] * dir2.z;
                    t1 = mParticleBasePos[i] + mParticleOffset[i];
                    f32 dist = t1.distance(rayTgt);
                    // TODO
                    f32 excess = 0.2f;
                    f32 limit = 2000.0f;
                    if (dist > limit) {
                        if (dist > limit + excess * limit) {
                            // Reset particle position
                            mParticleBasePos[i] = rayTgt;
                            mParticleOffset[i].x = cM::rndFX(1800.0f);
                            mParticleOffset[i].y = cM::rndFX(1800.0f);
                            mParticleOffset[i].z = cM::rndFX(1800.0f);
                        } else {
                            mParticleBasePos[i] = rayTgt;
                            mVec3_c t2;
                            env.get_vectle_calc(&t1, &rayTgt, &t2);
                            t2.normalize();
                            t2.x *= cM::rndF(0.05f) + 1.0f;
                            t2.y *= cM::rndF(0.05f) + 1.0f;
                            t2.z *= cM::rndF(0.05f) + 1.0f;
                            // TODO - operand swap
                            mParticleOffset[i].x = t2.x * 2000.0f;
                            mParticleOffset[i].y = t2.y * 2000.0f;
                            mParticleOffset[i].z = t2.z * 2000.0f;
                        }
                    }

                    t1 = mParticleBasePos[i] + mParticleOffset[i];
                    f32 f5 = t1.distance(rayTgt) / 1800.0f;
                    if (f5 > 1.0f) {
                        f5 = 1.0f;
                    }

                    f32 tmp = (1.0f - f5 * f5);
                    mMaxAlpha[i] = maxAlpha * tmp;
                }
            }
        }
    }
}

void dWeatherTag_c::moya_c::drawXlu() {
    dLightEnv_c &env = dLightEnv_c::GetInstance();
    dCamera_c *cam = dScGame_c::getCamera();
    if (field_0xB81 && (!env.fn_80024770(8) || env.fn_800247A0(8)) && mInitialSet &&
        (cam == nullptr || !cam->isUnderwater())) {
        EGG::DrawGX::LoadTexture(mpTexture, GX_TEXMAP0);
        EGG::DrawGX::BeginDrawQuad(EGG::DrawGX::COLORCHAN_1, EGG::DrawGX::ZMODE_0, EGG::DrawGX::BLEND_0, true, false);
        mColor tevCol0 = ActorLighting::getLightTev1Color();
        EGG::Matrix34f cameraMtx(
            EGG::DrawGX::s_cameraMtx._00, EGG::DrawGX::s_cameraMtx._01, EGG::DrawGX::s_cameraMtx._02,
            EGG::DrawGX::s_cameraMtx._03, EGG::DrawGX::s_cameraMtx._10, EGG::DrawGX::s_cameraMtx._11,
            EGG::DrawGX::s_cameraMtx._12, EGG::DrawGX::s_cameraMtx._13, EGG::DrawGX::s_cameraMtx._20,
            EGG::DrawGX::s_cameraMtx._21, EGG::DrawGX::s_cameraMtx._22, EGG::DrawGX::s_cameraMtx._23
        );
        mVec3_c v1_ = cam->getPosition(); // unused
        mVec3_c posInCamSpace;
        mVec3_c screenCenter(0.0f, 0.0f, 0.0f);
        mColor tevCol1 = ActorLighting::getLightTev1Color();
        tevCol1.r *= 0.8f;
        tevCol1.g *= 0.8f;
        tevCol1.b *= 0.8f;

        GXSetCullMode(GX_CULL_NONE);
        GXSetClipMode(GX_CLIP_DISABLE);
        mColor c3;
        GXSetChanAmbColor(GX_COLOR0, mColor(0xFF, 0xFF, 0xFF, 0xFF));
        GXSetTevColor(GX_TEVREG1, tevCol1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

        for (int i = 0; i < NUM; i++) {
            f32 distAlphaFactor = 1.0f;

            mVec3_c screenPos;
            mVec3_c worldPos(mParticleBasePos[i] + mParticleOffset[i]);
            MTXMultVec(*cameraMtx, worldPos, posInCamSpace);
            mMtx_c mtx2;
            mtx2.transS(posInCamSpace);
            // TODO FPR regswap
            int i1 = -(posInCamSpace.z + 50.0f) / 2.0f;
            f32 scale = -posInCamSpace.z / 2000.0f;
            if (scale > 1.0f) {
                scale = 1.0f;
            }
            scale = scale * 3.0f * 300.0f + 300.0f;
            if (i1 > 0) {
                if (i1 < mMaxAlpha[i]) {
                    tevCol0.a = i1;
                } else {
                    tevCol0.a = mMaxAlpha[i];
                }
                if (!field_0xB82) {
                    d3d::worldToScreen(screenPos, worldPos);
                    if (isnan(screenPos.x) || isnan(screenPos.y) || isnan(screenPos.z) ||
                        ((int)screenPos.x == 0 && (int)screenPos.y == 0) || screenPos.z <= 0.0f) {
                        continue;
                    }
                    screenCenter.z = screenPos.z;
                    f32 screenDist = screenCenter.distance(screenPos);
                    if (screenDist < 350.0f) {
                        if (screenDist < 150.0f) {
                            distAlphaFactor = 0.0f;
                        } else {
                            distAlphaFactor = (screenDist - 150.0f) / 200.0f;
                        }
                    }
                }
                tevCol0.a = tevCol0.a * mAlphaScale * distAlphaFactor;
                if (tevCol0.a != 0) {
                    mMtx_c m3;
                    m3.scaleS(scale, scale, 1.0f);
                    MTXConcat(mtx2, m3, mtx2);
                    GXSetTevColor(GX_TEVREG0, tevCol0);
                    EGG::DrawGX::DrawDL(EGG::DrawGX::DL_13, mtx2, EGG::DrawGX::WHITE);
                }
            }
        }
        GXSetClipMode(GX_CLIP_ENABLE);
    }
}

void dWeatherTag_c::near_cloud_c::calc() {
    // TODO
}

void dWeatherTag_c::near_cloud_c::drawXlu() {
    // TODO
}

void dWeatherTag_c::lightning_c::calc() {
    // TODO
}

void dWeatherTag_c::lightning_c::drawXlu() {
    // TODO
}

void dWeatherTag_c::sunflare_stars_c::remove() {
    m3d::scnLeaf_c::remove();
}

void dWeatherTag_c::sunflare_stars_c::calc() {
    // TODO
}

void dWeatherTag_c::sunflare_stars_c::drawOpa() {
    // TODO
}

void dWeatherTag_c::sunflare_stars_c::drawXlu() {
    // TODO
}

void dWeatherTag_c::moon_c::doCreate() {
    field_0x18 = (EGG::ResTIMG *)dAcObjBase_c::getOarcFile("Common", "tex/moon.bti");
    field_0x1C = (EGG::ResTIMG *)dAcObjBase_c::getOarcFile("Common", "tex/vr_lenzf_tex.bti");
    field_0x20 = (EGG::ResTIMG *)dAcObjBase_c::getOarcFile("Common", "tex/moon_alpha.bti");
}

void dWeatherTag_c::moon_c::remove() {
    m3d::scnLeaf_c::remove();
}

void dWeatherTag_c::moon_c::calc() {
    // TODO
}

void dWeatherTag_c::moon_c::drawOpa() {
    // TODO
}

void dWeatherTag_c::clouds_c::doCreate() {
    field_0x18 = (EGG::ResTIMG *)dAcObjBase_c::getOarcFile("Common", "tex/Cloud00.bti");
    field_0x1C = (EGG::ResTIMG *)dAcObjBase_c::getOarcFile("Common", "tex/Cloud01.bti");
    field_0x20 = (EGG::ResTIMG *)dAcObjBase_c::getOarcFile("Common", "tex/Cloud02.bti");
}

void dWeatherTag_c::clouds_c::remove() {
    m3d::scnLeaf_c::remove();
}

void dWeatherTag_c::clouds_c::init() {
    // TODO
}

void dWeatherTag_c::clouds_c::calc() {
    // TODO
}

void dWeatherTag_c::clouds_c::drawOpa() {
    // TODO
}

void dWeatherTag_c::clouds_c::drawXlu() {
    // TODO
}

void dWeatherTag_c::initializeState_Wait() {}

void dWeatherTag_c::executeState_Wait() {
    dLightEnv_c &env = dLightEnv_c::GetInstance();
    if (GetInstance() == nullptr) {
        return;
    }

    if (dAcPy_c::GetLinkM()->checkActionFlagsCont(0x400000)) {
        return;
    }

    mMoya.calc();
    mRain.calc();
    mNearCloud.calc();
    mLightning.calc();

    if (!env.getField_0x5D59(9)) {
        mSunFlareStars.calc();
        mMoon.calc();
        mClouds.calc();
    } else {
        dLightEnv_c::GetPInstance()->set0x35B0_i(0.0f);
    }

    if ((dScGame_c::isCurrentStage("F000") || dScGame_c::isCurrentStage("F020") || dScGame_c::isCurrentStage("F023")) && isAirborne()) {
        mMoya.setField_0xB81(1);
    } else {
        mMoya.setField_0xB81(0);
    }
}

void dWeatherTag_c::finalizeState_Wait() {}
