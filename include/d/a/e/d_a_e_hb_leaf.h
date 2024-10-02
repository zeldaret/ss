#ifndef D_A_E_HB_LEAF_H
#define D_A_E_HB_LEAF_H

#include <c/c_math.h>
#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_anmmdl.h>
#include <m/m3d/m_anmtexpat.h>
#include <toBeSorted/time_area_mgr.h>

class dAcEhb_leaf_c : public dAcObjBase_c {
public:
    dAcEhb_leaf_c() : mTimeAreaStruct(unkFloat0), field_0x3DE(0) {}
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

    TimeAreaStruct mTimeAreaStruct; // 0x3C4
    mVec3_c mStartingPos; // 0x3D0
    u16 someRot;          // 0x3DC
    u8 field_0x3DE;       // 0x3DE
    u8 mType;             // 0x3DF

    bool isHidden() {
        return mType != 0 && mTimeAreaStruct.isNearZero() ? true : false;
    }

    void doCheck() {
        mTimeAreaStruct.check(roomid, mStartingPos, 0, 10.0f, 0.2f);
    }

    int getParam1() {
        return params & 7;
    }

    int getParam2() {
        return params >> 7 & 3;
    }
};

#endif
