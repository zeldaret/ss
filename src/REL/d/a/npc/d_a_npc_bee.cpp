#include "d/a/npc/d_a_npc_bee.h"
#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/e/d_a_e_bc.h"
#include "d/a/npc/d_a_npc_honeycomb.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_spore.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_camera.h"
#include "d/d_player_act.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_triangular.h"
#include "s/s_Math.h"
#include <cmath>

SPECIAL_ACTOR_PROFILE(NPC_BEE, dAcNpcBee_c, fProfile::NPC_BEE, 0x123, 0, 0);

bool dAcNpcBee_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("Bee"));
    static const char* sModelNames[4] = {
        "fly_A",
        "fly_B",
        "walk_A",
        "walk_B",
    };
    mSwarmBeeCount = 0x14;
    for (int i = 0; i < (int)ARRAY_LENGTH(sModelNames); i++) {
        // instruction reordering
        TRY_CREATE(mBeeModels[i].create(mSwarmBeeCount, mRes.GetResMdl(sModelNames[i]),  &mAllocator, true, nullptr));
    }
    return SUCCEEDED;
}

static dCcD_SrcSph sph;

int dAcNpcBee_c::create() {
    mSceneflag = getFromParams(0x10, 0xFF);
    // mRoom regalloc
    if (mSceneflag != 0xFF && SceneflagManager::sInstance && SceneflagManager::sInstance->checkFlag(mRoomID, mSceneflag)) {
        return FAILED;
    }
    CREATE_ALLOCATOR(dAcNpcBee_c);
    dAcNpcBeeSingleBee* bee = &mBees[0];
    for (int i = 0; i < mSwarmBeeCount; i++, bee++) {
        bee->mIndex = i;
        bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_CRAWL;
        bee->mPos = mPosition;
        bee->mRotY = cM::rndF(65536);
        mColliderList.addCc(bee->mCollider, sph);
        bee->mCollider.mTg.OffSPrm(0x100);
        bee->mCollider.mAt.OnSPrm(0x100);
        if (i < mSwarmBeeCount / 2) {
            bee->field_0x008 = 1;
        }
    }
    // probably inline here
    mStts.SetRank(3);
    mInteractionFlags |= 0x80;
    mColliderList.SetStts(mStts);
    field_0x037C = cM::rndF(65536);
    dAcObjBase_c* honeycomb = dAcObjBase_c::create(fProfile::NPC_HONEYCOMB, -1, mParams, &mPosition, nullptr, nullptr, -1);
    if (honeycomb == nullptr) {
        return FAILED;
    } else {
        mHiveRef.link(static_cast<dAcNpcHc_c*>(honeycomb));
        mAttackRef.link(dAcPy_c::GetLinkM());
    }
    return SUCCEEDED;
}

void dAcNpcBee_c::actuallyUpdateSwarmBees() {
    if (mHiveRef.isLinked()) {
        setStartingPosition(mHiveRef.get()->field_0x928);
    }
    dAcPy_c *player = dAcPy_c::GetLink2();
    if ((field_0x037C & 0xF) == 0) {
        mAttackRef.link(player);
        bool bVar3 = true;
        fLiNdBa_c *current = dAcOSpore_c::getListHead();
        if (current != nullptr) {
            for (;current != nullptr;current = current->getNext()) {
                dAcOSpore_c* spore = static_cast<dAcOSpore_c*>(current->p_owner);
                if (spore != nullptr && (s32)(spore->mParams & 0xF) == 1) {
                    mVec3_c& sporePos = spore->mPosition;
                    if (spore->mPosition.squareDistanceToXZ(field_0xB830) < 1000000.f &&
                    std::fabsf(spore->mPosition.y - field_0xB830.y) < 500.f) {
                        if (!dBgS_ObjLinChk::LineCross(&field_0xB830, &sporePos, this)) {
                            mAttackRef.link(spore);
                            bVar3 = false;
                            field_0xB840 = spore;
                            break;
                        }
                    }
                }
            }
        }
        if (bVar3) {
            dAcEbc_c *boko = nullptr;
            while((boko = static_cast<dAcEbc_c*>(fManager_c::searchBaseByProfName(fProfile::E_BC, boko)))) {
                if (boko->getField_0x130D()) continue;
                f32 distToPlayer = field_0xB830.distance(dAcPy_c::GetLink()->mPositionCopy2);
                f32 distToBoko = field_0xB830.distance(boko->mPositionCopy2);
                if (distToBoko < distToPlayer) {
                    mAttackRef.link(boko);
                    break;
                }
            }
        }
    }
    mAttackActor = mAttackRef.get();
    if (mAttackActor == nullptr) {
        mAttackActor = player;
    }
    dAcNpcBeeSingleBee* bee = &mBees[0];
    u8 beesAlive = 0;
    int uVar13 = 0;
    static mVec3_c someVec(10000.f, -20000.f, 50000.f);
    dCamera_c* camera = dScGame_c::getCamera();
    bool bVar3 = true;
    int iVar12 = 0;
    int iVar17 = 0;
    field_0x0380 = mStartingPos.distance(mAttackActor->mPositionCopy2);
    mPosition = mStartingPos;
    for (int i = 0; i < mSwarmBeeCount; i++, bee++) {
        bool bVar9 = true;
        mMtx_c mtx;
        if (bee->mBeeState != dAcNpcBeeSingleBee::BEE_STATE_DEAD) {
            beesAlive++;
            bee->field_0x03C = bee->mPos;
            if (bee->field_0x002) {
                bee->field_0x002--;
            }
            if(bee->field_0x006) {
                bee->field_0x006--;
            }
            if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_CRAWL) {
                handleBeeCrawlingOnHive(bee);
                bee->field_0x006 = 5;
            } else {
                bee->mRotY++;
                if (bee->mBeeState <= dAcNpcBeeSingleBee::BEE_STATE_FLY_UP2) {
                    fn_14_1350(bee);
                    iVar12++;
                    if (bVar3) {
                        bVar3 = false;
                        mPosition = bee->mPos;
                    }
                } else if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_UNK4) {
                    fn_14_20F0(bee);
                } else if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_RISE) {
                    fn_14_1F40(bee);
                }
            }
            if (bee->field_0x008 == 0) {
                mVec3_c localA4 = bee->mPos - camera->getPosition();
                if (localA4.absXZ() < 3000.f) {
                    localA4.normalize();
                    mVec3_c localB0 = camera->getTarget() - camera->getPosition();
                    localB0.normalize();
                    if (localB0.dot(localA4) > 0.5f) {
                        if (bee->field_0x003) {
                            if (bee->mRotY & 1) {
                                uVar13 = 0;
                            } else {
                                uVar13 = 1;
                            }
                        } else {
                            s32 tmp = -(bee->mRotY >> 2 & 1);
                            tmp += 3;
                            uVar13 = tmp;
                        }
                        if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_CRAWL) {
                            mtx.transS(bee->mPos);
                            mtx.ZrotM(bee->field_0x060.x);
                            mtx.XrotM(bee->field_0x060.z);
                            mtx.YrotM(bee->field_0x060.y);
                            mtx.XrotM(0x8000);
                        } else {
                            mtx.transS(bee->mPos);
                            mtx.YrotM(bee->field_0x066.y);
                            mtx.XrotM(bee->field_0x066.x);
                            mtx.ZrotM(bee->field_0x066.z);
                        }
                        bVar9 = false;
                    }
                }
            }
            field_0xB830 = bee->mPos;
        }
        
        for (u32 uVar8 = 0, iVar18 = 4; iVar18 != 0; uVar8++, iVar18--) {
            if (!bVar9 && (uVar8 & 0xFF) == (uVar13 & 0xFF)) {
                mBeeModels[uVar8 & 0xFF].setDrawDisabled(i, false);
                mBeeModels[uVar8 & 0xFF].setTransform(i, mtx);
            } else {
                mBeeModels[uVar8 & 0xFF].setDrawDisabled(i, true);
            }
        }
        if (!bVar9 && bee->field_0x006 == 0) {
            bee->mCollider.SetC(bee->mPos);
        } else {
            bee->mCollider.SetC(someVec);
        }
    }
    if (iVar12 != 0) {
        holdSoundWithIntParam(SE_Bee_FLY_LV, iVar12);
    }
    mColliderList.registerColliders();
    if (!beesAlive) {
        deleteRequest();
    }
}

void dAcNpcBee_c::handleBeeCrawlingOnHive(dAcNpcBeeSingleBee* bee) {
    f32 fVar7 = 0.0f;
    dAcNpcHc_c* hive = mHiveRef.get();
    if (hive == nullptr || hive->mHealth == 0) {
        bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_DEAD;
    } else {
        if (bee->field_0x008) {
            if (hive->mHealth == 1) {
                bee->field_0x008 = 0;
                bee->field_0x024.x = cM::rndFX(300.f);
                bee->field_0x024.z = cM::rndFX(300.f);
            } else {
                bee->mPos = mStartingPos;
                return;
            }
        }
        if (dAcPy_c::GetLink()->getRidingActorType() != daPlayerActBase_c::RIDING_LOFTWING && (mAttackActor->getPosition().y - getPosition().y) < 0.f && bee->field_0x005 != 4) {
            f32 compSpeed = mAttackActor->getSpeed() * 20.f + 350.f;
            if (hive->field_0x915 >= 5 || hive->mHealth == 1 || (hive->field_0x916 == 0 && field_0x0380 < compSpeed)) {
                bee->field_0x005 = 4;
                bee->field_0x002 = cM::rndF(15.f);
            } else if (bee->field_0x005 != 2 && hive->field_0x915 != 0) {
                bee->field_0x002 = cM::rndF(70.f) + 30.f;
                bee->field_0x005 = 2;
            }
        }
        mVec3_c local_74;
        mVec3_c local_80 = bee->field_0x054 - bee->field_0x024;
        bee->field_0x003 = 0;
        local_74 = local_80;
        if (bee->field_0x005 == 0) {
            bee->field_0x010 = 0.f;
            if (bee->field_0x002 == 0) {
                if (cM::rnd() < 0.02f) {
                    bee->field_0x002 = cM::rndF(70.f) + 30.f;
                    bee->field_0x005 = 2;
                } else {
                    bee->field_0x002 = cM::rndF(30.f) + 5.f;
                    bee->field_0x054.x = cM::rndFX(300.f);
                    bee->field_0x054.z = cM::rndFX(300.f);
                    bee->field_0x005 = 1;
                }
            }
        } else if (bee->field_0x005 == 1) {
            bee->mRotY++;
            sLib::addCalcAngle(bee->field_0x060.y.ref(), local_74.atan2sX_Z(), 2, 0x400);
            if (bee->field_0x002 == 0 || local_74.absXZ() < 10.f) {
                bee->field_0x005 = 0;
                bee->field_0x002 = cM::rndF(15.f) + 2.f;
            }
            fVar7 = 2.f;
        } else if (bee->field_0x005 == 2) {
            bee->field_0x003 = 1;
            sLib::addCalcScaledDiff(&bee->field_0x010, 50.f, 0.1f, 5.f);
            sLib::addCalcAngle(bee->field_0x060.y.ref(), local_74.atan2sX_Z(), 2, 0x800);
            if ((bee->field_0x002 & 0x7) == 0) {
                bee->field_0x054.x = cM::rndFX(300.f);
                bee->field_0x054.z = cM::rndFX(300.f);
            }
            if (bee->field_0x002 == 0) {
                bee->field_0x005 = 3;
            }
            fVar7 = 5.f;
        } else if (bee->field_0x005 == 3) {
            bee->field_0x003 = 1;
            sLib::addCalcScaled(&bee->field_0x010, 1.f, 5.f);
            if (bee->field_0x010 <= 0.1f) {
                bee->field_0x005 = 0;
            }
        } else if (bee->field_0x005 == 4 && bee->field_0x002 == 0) {
            bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_RISE;
            bee->field_0x005 = 0;
        }
        sLib::addCalcScaledDiff(&bee->field_0x014, fVar7, 0.5f, 0.5f);
        mMtx_c mtx;
        mtx.YrotS(bee->field_0x060.y);
        local_74.set(0.f, 0.f, bee->field_0x014);
        mtx.multVec(local_74, bee->field_0x048);
        bee->field_0x024 += bee->field_0x048;
        s16 xRot = -bee->field_0x024.x * 32768.f / 200.f;
        s16 zRot = bee->field_0x024.z * 32768.f / 200.f;
        local_74.y = bee->field_0x010;
        local_74.x = 0.f;
        local_74.z = 0.f;
        local_74.y = -68.f - local_74.y;
        mtx.scaleS(1.f, 1.15f, 1.f);
        mtx.YrotM(hive->mRotation.y);
        mtx.ZrotM(xRot);
        mtx.XrotM(zRot);
        mtx.multVec(local_74, bee->mPos);
        bee->mPos += mStartingPos;
        if (bee->field_0x003 == 0) {
            sLib::addCalcAngle(bee->field_0x060.x.ref(), xRot, 2, 0x800);
            sLib::addCalcAngle(bee->field_0x060.z.ref(), zRot, 2, 0x800);
        } else {
            bee->mRotY++;
            sLib::addCalcAngle(bee->field_0x060.x.ref(), 0, 1, 0x1000);
            sLib::addCalcAngle(bee->field_0x060.z.ref(), -0x8000, 1, 0x1000);
        }
    }
}

void dAcNpcBee_c::fn_14_1350(dAcNpcBeeSingleBee* bee) {

}

void dAcNpcBee_c::fn_14_1F40(dAcNpcBeeSingleBee* bee) {
    bee->field_0x003 = 1;
    switch (bee->field_0x005) {
        case 0: {
            bee->field_0x054 = mStartingPos + (bee->mPos - mStartingPos) * (cM::rndF(0.5f) + 1.5f);
            bee->field_0x002 = 0x14;
            bee->field_0x005 = 1;
            // fallthrough
        }
        case 1: {
            cLib::addCalcPos2(&bee->mPos, bee->field_0x054, 0.2f, 15.0f);
            sLib::addCalcAngle(bee->field_0x060.y.ref(), cLib::targetAngleY(bee->mPos, mAttackActor->mPositionCopy2), 2, 0x800);
            sLib::addCalcAngle(bee->field_0x060.x.ref(), 0, 2, 0x800);
            sLib::addCalcAngle(bee->field_0x060.z.ref(), 0, 2, 0x800);
            if (bee->field_0x002 == 0) {
                bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_CHASE;
                bee->field_0x005 = 0;
                bee->field_0x00E = cM::rndF(155.f) + 300.f; 
            }
        }
    }   
}

void dAcNpcBee_c::fn_14_20F0(dAcNpcBeeSingleBee* bee) {
    bee->mPos += bee->field_0x048;
    bee->mPos.x += nw4r::math::SinIdx(bee->mRotY * 0x900) * 10.f;
    bee->mPos.y += nw4r::math::SinIdx(bee->mRotY * 0xa00) * 10.f;
    bee->mPos.z += nw4r::math::SinIdx(bee->mRotY * 0xb00) * 10.f;
    if ((bee->mIndex & 1) != 0) {
        bee->field_0x066.y += 0xD00;
    } else {
        bee->field_0x066.y -= 0xD00;
    }
    bee->field_0x066.x += 0xA00;
    if (bee->field_0x002 == 0) {
        bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_CHASE;
        bee->field_0x005 = 0;
        bee->field_0x00E = cM::rndF(155.f) + 300.f;
    }
    fn_14_2690(bee);
}

int dAcNpcBee_c::actorExecute() {
    field_0x037C++;
    updateSwarmBeeColliders();
    actuallyUpdateSwarmBees();
    if (field_0x0379) {
        field_0x0379--;
        if (field_0x0379 == 7) {
            dRumble_c::start(dRumble_c::sRumblePreset1, 1);
        } else if (field_0x0379 == 5) {
            dRumble_c::stop(-1);
        }
    }
    return SUCCEEDED;
}

void dAcNpcBee_c::updateSwarmBeeColliders() {

}

int dAcNpcBee_c::draw() {
    dShpProc1_c* model = &mBeeModels[0];
    for (int i = 0; i < 4; i++, model++) {
        model->entry();
    }
    return 1;
}

u32 fn_14_2690(dAcNpcBeeSingleBee* bee) {
    dBgS_ObjGndChk gndChk;
    dBgS_ObjLinChk linChk;
    u32 ret = 0;
    mVec3_c local_148_2 = bee->mPos - bee->field_0x03C;
    mVec3_c local_148 = local_148_2;
    local_148.y = 0.f;
    local_148.normalize();
    mVec3_c scaled = local_148 * 50.f;
    mVec3_c another = bee->mPos + scaled;
    linChk.Set(&bee->mPos, &another, nullptr);
    if (dBgS::GetInstance()->LineCross(&linChk)) {
        ret = 2;
        bee->mPos.x = linChk.GetLinEnd().x - scaled.x;
        bee->mPos.z = linChk.GetLinEnd().z - scaled.z;
    }
    mVec3_c local_149 = bee->mPos;
    local_149.z += 100.f;
    gndChk.SetPos(&local_149);
    if (bee->mPos.y <= dBgS::GetInstance()->GroundCross(&gndChk) + 15.f) {
        // imagine using temp variables...
        bee->mPos.y = dBgS::GetInstance()->GroundCross(&gndChk) + 15.f;
        ret |= 1;
    }
    return ret;
}
