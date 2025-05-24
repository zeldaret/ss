#ifndef D_JNT_COL_H
#define D_JNT_COL_H

#include "d/col/c/c_m3d_g_lin.h"
#include "m/m3d/m_mdl.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"

class dAcObjBase_c;

class dJntColData_c {
public:
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 mType;
    /* 0x2 */ s16 mJntNum;
    /* 0x4 */ f32 field_0x4;
    /* 0x8 */ Vec *field_0x8;

    mVec3_c getVec(int i) const {
        return field_0x8[i];
    }
};

class dJntCol_c {
public:
    dJntCol_c();
    void getAnmMtx(s32, mMtx_c *) const;
    int init(dAcObjBase_c *, dJntColData_c const *, m3d::mdl_c *, int);
    static void setNowLine(cM3dGLin *, mVec3_c const *, mAng3_c const *, mVec3_c *);
    int searchNearPos(cM3dGLin const *, mVec3_c const *, mVec3_c *, int) const;
    int getArrowOffsetPosAndAngle(mVec3_c const *, mAng3_c const *, mVec3_c *, mVec3_c *, bool) const;
    int getHitmarkPosAndAngle(mVec3_c const *, mAng3_c const *, mVec3_c *, mAng3_c *, int) const;
    void setArrowPosAndAngle(mVec3_c const *, mVec3_c const *, int, mVec3_c *, mAng3_c *);

    bool checkPassNum(int bit) const {
        return field_0x10 & (1 << bit);
    }
    void onPassNum(int num) {
        field_0x10 |= (1 << num);
    }
    void offPassNum(int num) {
        field_0x10 &= ~(1 << num);
    }

    s8 getType(int i) const {
        return mData[i].mType;
    }
    int getJntNum(int i) const {
        return mData[i].mJntNum;
    }
    u8 getfield_0x0(int i) const {
        return mData[i].field_0x0;
    }
    f32 getfield_0x4(int i) const {
        return mData[i].field_0x4;
    }
    mVec3_c getVec(int i) const {
        return mData->field_0x8[i];
    }

    /* 0x00 */ const dJntColData_c *mData;
    /* 0x04 */ m3d::mdl_c *mpModel;
    /* 0x08 */ dAcObjBase_c *mpActor;
    /* 0x0C */ int field_0xC;
    /* 0x10 */ int field_0x10;
};

#endif /* D_D_JNT_COL_H */
