#ifndef D_A_E_HB_LEAF_H
#define D_A_E_HB_LEAF_H

#include <c/c_math.h>
#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_anmmdl.h>
#include <m/m3d/m_anmtexpat.h>

extern "C" bool fn_800B9D80(f32, f32, f32 *, int, const mVec3_c &, u32);

class dAcEhb_leaf_c : public dAcObjBase_c {
public:
    dAcEhb_leaf_c() : someFloat(unkFloat0), field_0x3CC(0), field_0x3DE(0) {}
    virtual ~dAcEhb_leaf_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    void setAnm(const char *anm, f32 pos);

private:
    const static f32 unkFloat0;
    const static f32 unkFloat1;

    m3d::mdlAnmChr mModel; // 0x330
    m3d::anmTexPat_c mAnm; // 0x398

    f32 someFloat;        // 0x3C4
    UNKWORD field_0x3C8;  // 0x3C8
    u8 field_0x3CC;       // 0x3CC
    mVec3_c mStartingPos; // 0x3D0
    u16 someRot;          // 0x3DC
    u8 field_0x3DE;       // 0x3DE
    u8 mType;             // 0x3DF

    bool isNearZero() {
        return fabsf(someFloat) <= FLT_EPSILON;
    }

    bool isHidden() {
        return mType != 0 && isNearZero() ? true : false;
    }

    void doCheck() {
        fn_800B9D80(10.0f, 0.2f, &someFloat, roomid, mStartingPos, 0);
    }

    int getParam1() {
        return params & 7;
    }

    int getParam2() {
        return params >> 7 & 3;
    }
};

#endif
