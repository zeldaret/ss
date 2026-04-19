#ifndef D_T_BEETLE_H
#define D_T_BEETLE_H

#include "d/t/d_tg.h"
#include "m/m_mtx.h"
#include "d/a/obj/d_a_obj_boomerang.h"
#include "toBeSorted/area_math.h"

//This is a tag actor for controlling where the beetle can fly
//BtlTgC is leaves (beetle plays leaf animation as it passes through)

class dTgBeetle_c : public dTg_c {
public:
    dTgBeetle_c() {}
    virtual ~dTgBeetle_c() {}
    virtual int create() override;
    u32 fn_302_210();
    virtual int actorExecute();
    u32 isWithinSphere(const mVec3_c& param) const;
    u32 isWithinCylinder(const mVec3_c& param) const;
    void updateBeetle(dAcBoomerang_c& boomerang);
    u32 fn_302_3A0();

    enum Variant_e {
        BtlTg,
        BtlTgA,//is this even used?
        BtlTgB,//is this even used?
        BtlTgC,//spherical leaf object        
    };

    enum ZoneType_e {
        LeafZone = 1,
        BorderZone = 2,
        UnknownZone = 3,
    };

    enum ZoneShape_e {
        RectangularPrismShape,
        CylinderShape,
        SphereShape,
    };

private:
    /* 0xFC */ mMtx_c matrix1;
    /* 0x12C*/ u8 mZoneType;//what does this object do
    /* 0x12D*/ u8 mZoneShape;//the shape created inside the object that it checks
    /* 0x12E*/ u8 mUnknown;//unused
    /* 0x12F*/ u8 mBeetleFlag;
    /* 0x130*/ u8 mPastOnly;//object only exists in past state
};

#endif
