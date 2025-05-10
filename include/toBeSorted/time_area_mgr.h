#ifndef D_TIME_AREA_MGR_H
#define D_TIME_AREA_MGR_H

#include "m/m3d/m_proc.h"
#include "m/m_allocator.h"
#include "m/m_vec.h"

class dTimeAreaMgrProcA_c : public m3d::proc_c {
public:
    dTimeAreaMgrProcA_c() {}

    bool create(mHeapAllocator_c *alloc);

    void drawOpa() override;
};

class dTimeAreaMgrProcB_c : public m3d::proc_c {
public:
    dTimeAreaMgrProcB_c() {}

    bool create(mHeapAllocator_c *alloc);

    void drawOpa() override;

private:
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ u8 field_0x1A;
    /* 0x1B */ u8 field_0x1B;
};

struct TimeAreaStruct {
    TimeAreaStruct() : field_0x00(0.0f), field_0x08(0) {}
    TimeAreaStruct(f32 arg) : field_0x00(arg), field_0x08(0) {}
    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ UNKWORD field_0x04;
    /* 0x08 */ u8 field_0x08;

    bool isNearZero() {
        // is cM3d_IsZero
        return fabsf(field_0x00) <= FLT_EPSILON;
    }

    f32 getDistMaybe() {
        return field_0x00;
    }

    bool check(int, const mVec3_c &, u32, f32, f32);
};

class dTimeAreaMgr_c {
public:
    dTimeAreaMgr_c();
    virtual ~dTimeAreaMgr_c();

    bool init();
    void draw();

    f32 checkPositionIsInPastState(s32 roomId, const mVec3_c &position, float *, float);
    bool fn_800B9B60(s32 roomId, const mVec3_c &position);

    bool isInLanayruMiningFacility() const {
        return mIsInLanayruMiningFacility;
    }

    bool isField0x78() const {
        return field_0x78;
    }

    void setField0x78(bool val) {
        field_0x78 = val;
    }

    void setField0x7A(bool val) {
        field_0x7A = val;
    }

    static dTimeAreaMgr_c *GetInstance() {
        return sInstance;
    }

private:
    static dTimeAreaMgr_c *sInstance;

    /* 0x04 */ u8 field_0x04[0x0C - 0x04];
    /* 0x0C */ mHeapAllocator_c mAllocator;
    /* 0x28 */ dTimeAreaMgrProcA_c mProcA;
    /* 0x40 */ dTimeAreaMgrProcB_c mProcB;
    /* 0x5C */ u8 field_0x5C[0x64 - 0x5C];
    /* 0x64 */ s32 field_0x64;
    /* 0x68 */ u8 field_0x68[0x78 - 0x68];
    /* 0x78 */ bool field_0x78;
    /* 0x79 */ u8 field_0x79[0x7A - 0x79];
    /* 0x7A */ bool field_0x7A;
    /* 0x7B */ bool mIsInLanayruMiningFacility;
};

#endif
