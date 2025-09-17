#include "d/d_jnt_col.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "egg/math/eggMath.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/MTX/mtxvec.h"

dJntCol_c::dJntCol_c() : mData(nullptr), mpModel(nullptr), mpActor(nullptr), field_0xC(0), field_0x10(0) {}

int dJntCol_c::init(dAcObjBase_c *i_actorP, dJntColData_c const *i_jntColP, m3d::mdl_c *i_modelP, int param_3) {
    mData = i_jntColP;
    mpModel = i_modelP;
    field_0xC = param_3;
    field_0x10 = 0;

    if (i_actorP) {
        i_actorP->setJntCol(this);
        mpActor = i_actorP;
    }

    return 1;
}

void dJntCol_c::getAnmMtx(s32 jntId, mMtx_c *pOutMtx) const {
    if (mpModel != nullptr) {
        mpModel->getNodeWorldMtx(jntId, *pOutMtx);
    } else {
        *pOutMtx = mpActor->mWorldMtx;
    }
}

void dJntCol_c::setNowLine(cM3dGLin *i_line, mVec3_c const *param_1, mAng3_c const *param_2, mVec3_c *param_3) {
    mMtx_c mtx;
    if (param_2 != nullptr) {
        mtx.ZXYrotS(*param_2);
        mtx.multVec(mVec3_c::Ez, *param_3);
    } else {
        *param_3 = mVec3_c::Ez;
    }

    mVec3_c start_pos = *param_1 - *param_3 * 500.0f;
    mVec3_c end_pos = *param_1 + *param_3 * 500.0f;
    i_line->Set(start_pos, end_pos);
}

int dJntCol_c::searchNearPos(cM3dGLin const *param_1, mVec3_c const *param_2, mVec3_c *param_3, int param_4) const {
    mVec3_c const *pcVar9 = param_2;
    const dJntColData_c *pcVar12 = mData;
    mMtx_c mtx;
    cM3dGSph sphere;
    cM3dGCyl cylinder;
    cM3dGLin line;
    cM3dGPla plane;
    mVec3_c local_19c, local_1a8;
    mVec3_c local_1b4, local_1c0;
    mVec3_c local_1cc, local_1d8;
    mVec3_c local_1e4, local_1f0;
    f32 mag;

    bool bVar5 = false;
    int rv = -1;

    f32 max = EGG::Math<f32>::maxNumber();
    f32 min = -10000000.0f;
    for (int i = 0; i < field_0xC; i++, pcVar12++) {
        if (checkPassNum(i) == 0 && (param_4 & (1 << pcVar12->mType))) {
            getAnmMtx(pcVar12->mJntNum, &mtx);
            mtx.multVec(pcVar12->getVec(0), local_1cc);
            f32 radius = nw4r::math::FSqrt(mtx(0) * mtx(0) + mtx(4) * mtx(4) + mtx(8) * mtx(8)) * pcVar12->field_0x4;
            if (pcVar12->field_0x0 == 0) {
                sphere.Set(&local_1cc, radius);
                int iVar6 = cM3d_Cross_LinSph_CrossPos(sphere, *param_1, &local_1b4, &local_1c0);
                if (iVar6 != 0) {
                    if (iVar6 == 1) {
                        local_19c = local_1b4;
                    } else {
                        f32 dVar15 = local_1c0.squareDistance(param_1->GetStart());
                        f32 dVar16 = local_1b4.squareDistance(param_1->GetStart());
                        if (dVar16 < dVar15) {
                            local_19c = local_1b4;
                        } else {
                            local_19c = local_1c0;
                        }
                    }
                } else {
                    local_1a8 = *param_2 - local_1cc;
                    local_1a8.normalize();
                    local_19c = local_1cc + local_1a8 * radius;
                }
            } else if (pcVar12->field_0x0 == 1) {
                mtx.multVecSR(pcVar12->getVec(1), local_1a8);
                local_1d8 = local_1cc + local_1a8;
                int bVar13;
                if (!cM::isZero(local_1a8.absXZ())) {
                    mtx.transS(local_1cc);
                    mtx.XrotM(cM::atan2s(-local_1a8.absXZ(), local_1a8.y));
                    mtx.YrotM(cM::atan2s(-local_1a8.x, local_1a8.z));
                    mtx.transM(-local_1cc.x, -local_1cc.y, -local_1cc.z);
                    mtx.multVec(local_1d8, local_1c0);
                    mtx.multVec(param_1->GetStart(), local_1e4);
                    mtx.multVec(param_1->GetEnd(), local_1f0);
                    line.Set(local_1e4, local_1f0);
                    bVar13 = true;
                } else {
                    if (local_1d8.y < local_1cc.y) {
                        mVec3_c tmp = local_1cc;
                        local_1cc = local_1d8;
                        local_1d8 = tmp;
                    }
                    local_1c0 = local_1d8;
                    line.Set(param_1->GetStart(), param_1->GetEnd());
                    bVar13 = false;
                }

                cylinder.SetC(local_1cc, radius, nw4r::ut::Max<f32>(local_1c0.y - local_1cc.y, 0.f));
                if (cM3d_Cross_CylLin(cylinder, line, &local_1b4, &local_1c0)) {
                    if (bVar13) {
                        mtx.inverse();
                        mtx.multVec(local_1b4, local_19c);
                    } else {
                        local_19c = local_1b4;
                    }
                } else {
                    line.Set(local_1cc, local_1d8);
                    f32 dVar15 = cM3d_lineVsPosSuisenCross(line, *param_2, local_1b4);
                    local_1a8 = *param_2 - local_1b4;
                    mag = local_1a8.getSquareMag();
                    if (mag < 1e-04f) {
                        local_1b4 = line.GetStart() - line.GetEnd();
                        local_1c0 = param_1->GetStart() - param_1->GetEnd();
                        f32 inprod = local_1b4.dot(local_1c0);
                        if (inprod > 0.0f) {
                            local_19c = line.GetEnd();
                        } else if (inprod < 0.0f) {
                            local_19c = line.GetStart();
                        } else if (dVar15 < 0.5f) {
                            local_19c = line.GetStart();
                        } else {
                            local_19c = line.GetEnd();
                        }
                    } else {
                        if (dVar15 >= 0.0f && dVar15 <= 1.0f) {
                            local_19c = local_1b4 + (local_1a8 / nw4r::math::FSqrt(mag)) * radius;
                        } else {
                            local_1b4 = line.GetStart() - line.GetEnd();
                            local_1c0 = param_1->GetStart() - param_1->GetEnd();
                            f32 dVar14 = local_1b4.dot(local_1c0);
                            if (dVar14 > 0.0f) {
                                local_1b4 = line.GetEnd();
                            } else {
                                if (dVar14 < 0.0f) {
                                    local_1b4 = line.GetStart();
                                } else {
                                    if (dVar15 < 0.5f) {
                                        local_1b4 = line.GetStart();
                                    } else {
                                        local_1b4 = line.GetEnd();
                                    }
                                }
                            }
                            local_19c = local_1b4 + (local_1a8 / nw4r::math::FSqrt(mag)) * radius;
                        }
                    }
                }
            } else {
                mtx.multVecSR(pcVar12->getVec(1), local_1a8);
                plane.SetupNP0(local_1a8, local_1cc);
                if (plane.CrossInfLin(*param_2, param_1->GetEnd(), local_19c)) {
                    if (!(local_1cc.distance(local_19c) < radius)) {
                        local_1a8 = local_19c - local_1cc;
                        local_1a8.normalize();
                        local_19c = local_1cc + local_1a8 * radius;
                    }
                } else {
                    f32 pla = cM3d_SignedLenPlaAndPos(&plane, param_2);
                    local_1a8.normalize();
                    local_19c = *param_2 - local_1a8 * pla;
                }
            }
            f32 cross = cM3d_lineVsPosSuisenCross(*param_1, local_19c, &local_1b4);
            f32 dVar16 = local_19c.squareDistance(local_1b4);
            if (dVar16 < 1e-04f) {
                if (!bVar5 || min < cross) {
                    min = cross;
                    *param_3 = local_19c;
                    rv = i;
                    bVar5 = TRUE;
                }
            } else {
                if (!bVar5 && (max >= dVar16)) {
                    max = dVar16;
                    *param_3 = local_19c;
                    rv = i;
                }
            }
        };
    }
    return rv;
}

int dJntCol_c::getArrowOffsetPosAndAngle(
    mVec3_c const *param_1, mAng3_c const *param_2, mVec3_c *param_3, mVec3_c *param_4, bool param_5
) const {
    cM3dGLin acStack_34;
    mVec3_c cStack_40;
    setNowLine(&acStack_34, param_1, param_2, &cStack_40);
    mVec3_c cStack_4c;
    int iVar1 = searchNearPos(&acStack_34, param_1, &cStack_4c, param_5 ? 12 : 3);
    if (iVar1 < 0) {
        return -3;
    }
    const dJntColData_c *jntColData = &mData[iVar1];
    if (jntColData->mType == 0) {
        return -3;
    }
    if (jntColData->mType == 2) {
        return -2;
    }
    mMtx_c mtx;
    getAnmMtx(jntColData->mJntNum, &mtx);
    mtx.inverse();
    MTXMultVecSR(mtx, cStack_40, *param_4);
    MTXMultVec(mtx, cStack_4c, *param_3);
    return jntColData->mJntNum;
}

int dJntCol_c::getHitmarkPosAndAngle(
    mVec3_c const *param_1, mAng3_c const *param_2, mVec3_c *param_3, mAng3_c *param_4, int param_5
) const {
    cM3dGLin acStack_54;
    mVec3_c cStack_60;
    setNowLine(&acStack_54, param_1, param_2, &cStack_60);
    int iVar3 = searchNearPos(&acStack_54, param_1, param_3, param_5);
    if (iVar3 < 0) {
        return -1;
    }
    const dJntColData_c *jntColData = &mData[iVar3];
    mVec3_c cStack_6c;
    mVec3_c local_78;
    mMtx_c mtx;
    getAnmMtx(jntColData->mJntNum, &mtx);
    MTXMultVec(mtx, jntColData->getVec(0), cStack_6c);

    if (jntColData->field_0x0 == 0) {
        local_78 = *param_3 - cStack_6c;
    } else if (jntColData->field_0x0 == 1) {
        mVec3_c cStack_84;
        mVec3_c cStack_90;
        MTXMultVecSR(mtx, jntColData->getVec(1), cStack_84);
        cStack_84 += cStack_6c;
        cM3d_lineVsPosSuisenCross(cStack_6c, cStack_84, *param_3, &cStack_90);
        local_78 = *param_3 - cStack_90;

        if (local_78.getSquareMag() < 1e-04f) {
            if (param_3->squareDistance(cStack_6c) < param_3->squareDistance(cStack_84)) {
                local_78 = cStack_6c - cStack_84;
            } else {
                local_78 = cStack_84 - cStack_6c;
            }
        }
    } else {
        MTXMultVecSR(mtx, jntColData->getVec(1), local_78);
    }
    param_4->x = cM::atan2s(local_78.y, local_78.absXZ());
    param_4->y = cM::atan2s(-local_78.x, -local_78.z);
    param_4->z = 0;
    return iVar3;
}

void dJntCol_c::setArrowPosAndAngle(
    mVec3_c const *param_0, mVec3_c const *i_srcPos, int i_anmMtxIdx, mVec3_c *param_3, mAng3_c *i_arrowPosP
) {
    mMtx_c mtx;
    mVec3_c dstPos;
    getAnmMtx(i_anmMtxIdx, &mtx);
    MTXMultVecSR(mtx, *i_srcPos, dstPos);

    i_arrowPosP->x = dstPos.atan2sY_XZ();
    i_arrowPosP->y = dstPos.atan2sX_Z();
    MTXMultVec(mtx, *param_0, *param_3);
}
