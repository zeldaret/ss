#include "d/t/d_t_spore.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_camera.h"
#include "d/d_light_env.h"
#include "d/d_sc_game.h"
#include "d/t/d_t_siren.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggDrawGX.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "rvl/GX/GXGeometry.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"

SPECIAL_ACTOR_PROFILE(SPORE_TAG, dSporeTag_c, fProfile::SPORE_TAG, 0xD, 0, 388);

STATE_DEFINE(dSporeTag_c, Wait);

bool dSporeTag_c::createHeap() {
    u32 size = 0;
    if (!mProc.create(&mAllocator, &size)) {
        return true;
    }
    mProc.mpTexture = static_cast<EGG::ResTIMG *>(dAcObjBase_c::getOarcFile("Common", "tex/WK2_Houshi.bti"));
    return true;
}

int dSporeTag_c::create() {
    CREATE_ALLOCATOR(dSporeTag_c);

    mProc.setPriorityDraw(0, 142);
    mProc.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);
    mStateMgr.changeState(StateID_Wait);
    mProc.field_0x39ED = true;

    return SUCCEEDED;
}

int dSporeTag_c::doDelete() {
    return SUCCEEDED;
}

int dSporeTag_c::actorExecuteInEvent() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dSporeTag_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dSporeTag_c::draw() {
    mProc.entry();
    return SUCCEEDED;
}

void dSporeProc_c::execute() {
    // NONMATCHING
    // I dont feel like it
    mVec3_c _playerPos = dAcPy_c::GetLink()->mPosition;
    dLightEnv_c &env = dLightEnv_c::GetInstance();
    dCamera_c *pCam = dScGame_c::getCamera();

    mVec3_c camPos;
    mVec3_c camTowards;

    camPos = pCam->getPositionMaybe();
    camTowards = pCam->getField_0x78() - camPos;

    if (pCam != nullptr && camTowards.normalizeRS()) {
        mVec3_c v;
        mVec3_c pos = camPos + camTowards * 1000.f;
        if (!field_0x39EC) {
            for (s32 i = 0; i < 400; ++i) {
                mSporesPos[i].x = pos.x + cM::rndFX(1100.f);
                mSporesPos[i].y = pos.y + cM::rndFX(1100.f);
                mSporesPos[i].z = pos.z + cM::rndFX(1100.f);

                field_0x1F58[i] = 0.f;
                field_0x1918[i] = 1.f;

                field_0x3218[i] = cM::rnd() * 65535.f;
                field_0x3538[i] = cM::rnd() * 65535.f;

                field_0x3858[i] = 0;

                field_0x2598[i] = cM::rndF(45.f) + 90.f;
                field_0x2BD8[i] = cM::rndF(360.f);

                field_0x12D8[i] = cM::rndF(0.9f) + 0.1f;

                field_0x3A02 += 200;
                field_0x3A04 += 50;
                field_0x3A06 += 400;
            }
            field_0x3A02 = 0;
            field_0x3A04 = 0;
            field_0x3A06 = 0;
            field_0x39EC = true;
        }

        f32 f0 = 1050.f;
        f32 f1 = 13.f;
        f32 end = -1e9f;
        f32 f3 = 900.f;
        f32 f4 = 100.f;
        f32 f5 = 3.f;

        for (s32 i = 0; i < 400; ++i) {
            mVec3_c playerPos = dAcPy_c::GetLink()->mPosition;
            mVec3_c point;
            env.sphere_to_cartesian(field_0x2598[i], field_0x2BD8[i], &point);

            switch (field_0x3858[i]) {
                case 0: {
                    field_0x3858[i] = 2;
                } break;
                case 1: {
                    if (playerPos.distance(mSporesPos[i]) < 150.f || dTgSiren_c::isOutOfTime()) {
                        field_0x3858[i]++;
                    } else {
                        mSporesPos[i].y += cM::rndF(30.f);
                    }
                } break;
                case 2: {
                    sLib::addCalc(&field_0x1918[i], 1.f, 0.5f, 0.1f, 0.001f);
                    f32 sin = field_0x3218[i].sin();
                    field_0x3218[i] += sin * 90.f + 10.f;

                    if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL) {
                        if (dTgSiren_c::isOutOfTime()) {
                            mSporesPos[i].y += field_0x12D8[i] * 16.f;
                            field_0x2598[i] += sin * (field_0x12D8[i] - 0.5f) * 48.f;
                            field_0x2BD8[i] += sin * (field_0x12D8[i] - 0.5f) * 48.f;
                            mSporesPos[i] += point * ((field_0x12D8[i] * 0.5f) * 32.f + 0.5f);
                        } else {
                            mSporesPos[i].y += field_0x12D8[i] * 1.5f;
                            field_0x2598[i] += sin * (field_0x12D8[i] - 0.5f) * 12.f;
                            field_0x2BD8[i] += sin * (field_0x12D8[i] - 0.5f) * 12.f;
                            mSporesPos[i] += point * (field_0x12D8[i] * 4.f);
                        }
                    } else {
                        mSporesPos[i].y += field_0x12D8[i] * -0.5f;
                        field_0x2598[i] += sin * (field_0x12D8[i] - 0.5f) * 14.f;
                        field_0x2BD8[i] += sin * (field_0x12D8[i] - 0.5f) * f1;
                        mSporesPos[i] += point * (field_0x12D8[i] * 5.f);
                    }

                    f32 camY = pCam->getField_0x2A8();
                    if (end != camY && mSporesPos[i].y < camY) {
                        mSporesPos[i].y = camY;
                    }
                } break;
            }

            v = mSporesPos[i];
            if (v.distance(pos) > 1050.f) {
                mSporesPos[i].x = pos.x + cM::rndFX(1100.f);
                mSporesPos[i].y = pos.y + cM::rndFX(1100.f);
                mSporesPos[i].z = pos.z + cM::rndFX(1100.f);
            } else if (v.distance(pos) > 1000.f) {
                mVec3_c out;
                env.get_vectle_calc(&v, &pos, &out);
                mSporesPos[i].x = pos.x + out.x * 1000.f;
                mSporesPos[i].y = pos.y + out.y * 1000.f;
                mSporesPos[i].z = pos.z + out.z * 1000.f;

                if (!dTgSiren_c::isOutOfTime()) {
                    field_0x3858[i] = 0;
                }
            }

            v = mSporesPos[i];
            f32 minDist = nw4r::ut::Min(v.distance(pos) / 1000.f, 1.f);

            f32 sin = (1.f - minDist) * field_0x3538[i].sin();
            field_0x3538[i] += f4 + f3 * field_0x12D8[i];
            field_0x1F58[i] = sin;
            if (sin < 0.f) {
                field_0x1F58[i] = 0.f;
            }

            field_0x1918[i] = f5 * v.distance(camPos) / 1000.f + 1.f;
        }
    }
}

void dSporeProc_c::drawXlu() {
    using namespace EGG;

    DrawGX::LoadTexture(mpTexture, GX_TEXMAP0);
    mColor color0(0x78, 0xB4, 0xFF, 1);    // #78B4FF
    mColor color1(0x2D, 0x50, 0x41, 0xFF); // #2D5041FF

    Matrix34f matrix(
        EGG::DrawGX::s_cameraMtx._00, EGG::DrawGX::s_cameraMtx._01, EGG::DrawGX::s_cameraMtx._02,
        EGG::DrawGX::s_cameraMtx._03, EGG::DrawGX::s_cameraMtx._10, EGG::DrawGX::s_cameraMtx._11,
        EGG::DrawGX::s_cameraMtx._12, EGG::DrawGX::s_cameraMtx._13, EGG::DrawGX::s_cameraMtx._20,
        EGG::DrawGX::s_cameraMtx._21, EGG::DrawGX::s_cameraMtx._22, EGG::DrawGX::s_cameraMtx._23
    );

    dCamera_c *pCam = dScGame_c::getCamera();
    mVec3_c pos = pCam->getPositionMaybe();

    if (dTgSiren_c::isOutOfTime()) {
        color0.r = 0xFF;
        color0.g = 0xDB;
        color0.b = 0xA5;
    }

    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_DISABLE);

    f32 scaleMult = 12.f;

    DrawGX::BeginDrawQuad(DrawGX::COLORCHAN_1, DrawGX::ZMODE_2, DrawGX::BLEND_3, true, false);
    GXSetTevColor(GX_TEVREG1, color1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);

    GXColor c = EGG::DrawGX::WHITE;
    s16 zRot;
    s32 i;
    mVec3_c *sporePos = mSporesPos;
    for (i = 0, zRot = 0; i < 400; i++, zRot += 10) {
        mVec3_c b;
        mVec3_c a = mSporesPos[i];
        MTXMultVec(*matrix, a, b);

        mMtx_c m;
        m.transS(b);

        color0.a = field_0x1F58[i] * 255.f;
        if (color0.a != 0) {
            m.ZrotM(field_0x3A02 + zRot);

            mMtx_c scale;
            f32 scaleX = field_0x1918[i] * scaleMult * field_0x1F58[i];
            scale.scaleS(scaleX, scaleX, 1.f);

            m += scale;
            GXSetTevColor(GX_TEVREG0, color0);
            DrawGX::DrawDL(EGG::DrawGX::DL_13, m, c);
        }
    }

    GXSetClipMode(GX_CLIP_ENABLE);
}

void dSporeTag_c::initializeState_Wait() {}
void dSporeTag_c::executeState_Wait() {
    mProc.execute();
}
void dSporeTag_c::finalizeState_Wait() {}
