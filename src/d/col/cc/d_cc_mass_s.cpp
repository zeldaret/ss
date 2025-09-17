#include "d/col/cc/d_cc_mass_s.h"

#include "common.h"
#include "m/m_vec.h"
#include "rvl/MTX/vec.h"

///////////////////////////////////////////////////////////////////////////////
// Obj
///////////////////////////////////////////////////////////////////////////////

dCcMassS_Obj::dCcMassS_Obj() {}

dCcMassS_Obj::~dCcMassS_Obj() {}

void dCcMassS_Obj::Set(cCcD_Obj *p_obj, u8 priority, dCcMassS_ObjCallback callback) {
    mpObj = p_obj;
    mPriority = priority;
    mpCallback = callback;
}

void dCcMassS_Obj::Clear() {
    mpObj = nullptr;
    mPriority = 5;
    mpCallback = nullptr;
    mDivideInfo.Set(0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
// Hit Info
///////////////////////////////////////////////////////////////////////////////

dCcMassS_HitInf::dCcMassS_HitInf() {}

dCcMassS_HitInf::~dCcMassS_HitInf() {}

void dCcMassS_HitInf::ClearPointer() {
    mpArea = nullptr;
    mpAtObj = nullptr;
    mpCoObj = nullptr;
    mCoHitLen = 0.0f;
}

///////////////////////////////////////////////////////////////////////////////
// Manager
///////////////////////////////////////////////////////////////////////////////

dCcMassS_Mng::dCcMassS_Mng() {
    Ct();
}

dCcMassS_Mng ::~dCcMassS_Mng() {}

void dCcMassS_Mng::Ct() {
    field_0x3AA = 0;
    mResultCam = 0;
    mCamTopPos.x = 0.0f;
    mCamTopPos.y = -1000000000.0f;
    mCamTopPos.z = 0.0f;

    mCamBottomPos.x = 0.0f;
    mCamBottomPos.y = -1000000000.0f;
    mCamBottomPos.z = 0.0f;

    Clear();
}

void dCcMassS_Mng::SetAttr(f32 radius, f32 height, u8 param_2, u8 param_3) {
    mCylAttr.SetR(radius);
    mCylAttr.SetH(height);
    field_0x3A8 = param_2;
    field_0x3A9 = param_3;
}

void dCcMassS_Mng::Prepare() {
    cM3dGAab aab;
    aab.ClearForMinMax();

    for (dCcMassS_Obj *pObj = mMassObjs; pObj < mMassObjs + mMassObjCount; ++pObj) {
        cCcD_ShapeAttr *objShape = pObj->GetObj()->GetShapeAttr();
        objShape->CalcAabBox();
        aab.SetMinMax(objShape->GetWorkAab());
    }

    for (dCcMassS_Obj *pObj = mMassAreas; pObj < mMassAreas + mMassAreaCount; ++pObj) {
        cCcD_ShapeAttr *objShape = pObj->GetObj()->GetShapeAttr();
        objShape->CalcAabBox();
        aab.SetMinMax(objShape->GetWorkAab());
    }

    if (field_0x3AA & 1) {
        mCpsAttr.CalcAabBox();
        aab.SetMinMax(mCpsAttr.GetWorkAab());
    }

    mDivideArea.SetArea(aab);
    for (dCcMassS_Obj *pObj = mMassObjs; pObj < mMassObjs + mMassObjCount; ++pObj) {
        cCcD_ShapeAttr *objShape = pObj->GetObj()->GetShapeAttr();
        cCcD_DivideInfo *divideInfo = &pObj->GetDivideInfo();
        mDivideArea.CalcDivideInfo(divideInfo, objShape->GetWorkAab());
    }

    for (dCcMassS_Obj *pObj = mMassAreas; pObj < mMassAreas + mMassAreaCount; ++pObj) {
        cCcD_ShapeAttr *objShape = pObj->GetObj()->GetShapeAttr();
        cCcD_DivideInfo *divideInfo = &pObj->GetDivideInfo();
        mDivideArea.CalcDivideInfo(divideInfo, objShape->GetWorkAab());
    }

    if (field_0x3AA & 1) {
        mDivideArea.CalcDivideInfo(&mDivideInfo, mCpsAttr.GetWorkAab());
    }

    mCamTopPos.x = 0.0f;
    mCamTopPos.y = -1000000000.0f;
    mCamTopPos.z = 0.0f;
    mCamTopDist = 1000000000.0f;

    mCamBottomPos.x = 0.0f;
    mCamBottomPos.y = -1000000000.0f;
    mCamBottomPos.z = 0.0f;
    mCamBottomDist = 1000000000.0f;
}

u32 dCcMassS_Mng::Chk(mVec3_c *p_xyz, dAcObjBase_c **p_actor, dCcMassS_HitInf *p_hitInf) {
    cCcD_DivideInfo divideInfo;
    u32 flagsMaybe = 0;
    *p_actor = NULL;

    mCylAttr.SetC(*p_xyz);
    mCylAttr.CalcAabBox();

    mDivideArea.CalcDivideInfoOverArea(&divideInfo, mCylAttr.GetWorkAab());
    p_hitInf->ClearPointer();

    if (field_0x3A8 & 8) {
        for (dCcMassS_Obj *massObj = mMassAreas; massObj < mMassAreas + mMassAreaCount; ++massObj) {
            if (massObj->GetDivideInfo().Chk(divideInfo)) {
                cCcD_Obj *obj = massObj->GetObj();
                cCcD_ShapeAttr *objShape = obj->GetShapeAttr();
                f32 f;
                if (obj->ChkCoSet() && mCylAttr.CrossCo(*objShape, &f)) {
                    flagsMaybe |= 4;
                    *p_actor = obj->GetStts()->GetAc();
                    if (p_hitInf != NULL) {
                        p_hitInf->SetAreaHitObj(obj);
                    }

                    if (massObj->GetCallback() != NULL) {
                        massObj->GetCallback()(obj->GetStts()->GetAc(), p_xyz, field_0x3A9);
                    }
                }
            }
        }
    }

    for (dCcMassS_Obj *massObj = mMassObjs; massObj < mMassObjs + mMassObjCount; ++massObj) {
        if (massObj->GetDivideInfo().Chk(divideInfo)) {
            cCcD_Obj *obj = massObj->GetObj();
            cCcD_ShapeAttr *objShape = obj->GetShapeAttr();
            mVec3_c unusedVec;
            if (obj->ChkAtSet() && !obj->ChkAtNoMass() && (field_0x3A8 & 1)) {
                *p_actor = obj->GetStts()->GetAc();
            }
            if (obj->ChkAtSet() && !obj->ChkAtNoMass() && mCylAttr.CrossAtTg(*objShape, &unusedVec) &&
                (field_0x3A8 & 1)) {
                flagsMaybe |= 1;
                *p_actor = obj->GetStts()->GetAc();
                if (p_hitInf != NULL) {
                    p_hitInf->SetAtHitObj(obj);
                }
            }

            f32 f;
            if (obj->ChkCoSet() && mCylAttr.CrossCo(*objShape, &f) && (field_0x3A8 & 2)) {
                flagsMaybe |= 2;
                *p_actor = obj->GetStts()->GetAc();

                if (field_0x3A8 & 0x10) {
                    mVec3_c vec;
                    PSVECSubtract((*p_actor)->getPosition(), *p_xyz, vec);
                    vec.y = 0;
                    f32 vecMag = PSVECMag(vec);
                    if (cM3d_IsZero(vecMag)) {
                        vec.x = 1;
                    } else {
                        PSVECScale(vec, vec, f / vecMag);
                    }
                    obj->GetStts()->PlusCcMove(vec.x, vec.y, vec.z);
                }

                if (p_hitInf != NULL) {
                    p_hitInf->SetCoHitObj(obj);
                    p_hitInf->SetCoHitLen(f);
                }
            }
        }
    }

    f32 f;
    if ((field_0x3AA & 1) && mDivideInfo.Chk(divideInfo) && mCylAttr.CrossCo(mCpsAttr, &f)) {
        mResultCam |= 1;
        mResultCam |= 1 << (field_0x3A9 + 1);

        if ((mResultCam & 2) || (mResultCam & 8)) {
            Vec tmpVec;
            f32 plusH = p_xyz->y + mCylAttr.GetH();
            tmpVec.x = p_xyz->x;
            tmpVec.y = plusH;
            tmpVec.z = p_xyz->z;

            if (mCamTopPos.y < (20.0f + plusH)) {
                f32 newCamTopDist = PSVECSquareDistance(&tmpVec, *mCpsAttr.GetStartP());
                if (mCamTopDist > newCamTopDist) {
                    mCamTopDist = newCamTopDist;
                    mCamTopPos = tmpVec;
                }
            }

            if (mCamBottomPos.y < (20.0f + plusH)) {
                f32 newCamBottomDist = PSVECSquareDistance(&tmpVec, *mCpsAttr.GetEndP());
                if (mCamBottomDist > newCamBottomDist) {
                    mCamBottomDist = newCamBottomDist;
                    mCamBottomPos = tmpVec;
                }
            }
        }
    }

    return flagsMaybe;
}

void dCcMassS_Mng::Clear() {
    mMassObjCount = 0;
    mMassAreaCount = 0;

    for (int i = 0; i < (s32)ARRAY_LENGTH(mMassObjs); ++i) {
        mMassObjs[i].Clear();
    }

    for (int i = 0; i < (s32)ARRAY_LENGTH(mMassAreas); ++i) {
        mMassAreas[i].Clear();
    }

    mCylAttr.SetR(0.0f);
    mCylAttr.SetH(0.0f);
    field_0x3A8 = 0;
    field_0x3A9 = 4;
}

void dCcMassS_Mng::SetObj(cCcD_Obj *p_obj, u8 priority) {
    if (mMassObjCount >= (s32)ARRAY_LENGTH(mMassObjs)) {
        for (int i = 0; i < (s32)ARRAY_LENGTH(mMassObjs); ++i) {
            int prevPrio = mMassObjs[i].GetPriority();
            if (prevPrio > priority || (prevPrio == priority)) {
                mMassObjs[i].Set(p_obj, priority, nullptr);
                break;
            }
        }
    } else {
        mMassObjs[mMassObjCount].Set(p_obj, priority, nullptr);
        mMassObjCount++;
    }
}

void dCcMassS_Mng::SetArea(cCcD_Obj *p_obj, u8 priority, dCcMassS_ObjCallback callback) {
    if (mMassAreaCount >= (s32)ARRAY_LENGTH(mMassAreas)) {
        for (int i = 0; i < (s32)ARRAY_LENGTH(mMassAreas); ++i) {
            if (mMassAreas[i].GetPriority() > priority) {
                mMassAreas[i].Set(p_obj, priority, callback);
                break;
            }
        }
    } else {
        mMassAreas[mMassAreaCount].Set(p_obj, priority, callback);
        mMassAreaCount++;
    }
}
