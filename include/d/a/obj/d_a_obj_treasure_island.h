#ifndef D_A_OBJ_TREASURE_ISLAND_H
#define D_A_OBJ_TREASURE_ISLAND_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmtexsrt.h"
#include "toBeSorted/unk_with_water.h"

class dAcOtreasureIsland_c : public dAcObjBase_c {
public:
    dAcOtreasureIsland_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual ~dAcOtreasureIsland_c() override;
    virtual int actorExecute() override;

private:
    /* 0x330 */ UnkWithWater waterStuff;
    /* 0x348 */ m3d::smdl_c mModel;
    /* 0x364 */ m3d::smdl_c mWater00;
    /* 0x380 */ m3d::smdl_c mWater01;
    /* 0x39C */ m3d::anmTexSrt_c mWaterSrt00;
    /* 0x3C8 */ m3d::anmTexSrt_c mWaterSrt01;
    /* 0x3F4 */ nw4r::g3d::ResFile mResFile;
    /* 0x3F8 */ dBgW mCollision;
    /* 0x608 */ u8 subtype;
};

#endif
