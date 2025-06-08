#ifndef D_A_OBJ_PUZZLE_ISLAND_H
#define D_A_OBJ_PUZZLE_ISLAND_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"

class dAcOpuzzleIsland_c : public dAcObjBase_c {
public:
    dAcOpuzzleIsland_c() {}
    virtual ~dAcOpuzzleIsland_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    static void rideCallback(dBgW *, dAcObjBase_c *, dAcObjBase_c *);

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ nw4r::g3d::ResFile mResFile;
    /* 0x350 */ dBgW mBgW;
};

#endif
