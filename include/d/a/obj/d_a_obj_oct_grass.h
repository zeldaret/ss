#ifndef D_A_OBJ_OCT_GRASS_H
#define D_A_OBJ_OCT_GRASS_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOOctGrass_c : public dAcObjBase_c {
public:
    dAcOOctGrass_c() : mStateMgr(*this) {}
    virtual ~dAcOOctGrass_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    void spawnOcGrsL();

    STATE_FUNC_DECLARE(dAcOOctGrass_c, Wait);

    void executeState() {
        mStateMgr.executeState();
    }

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ STATE_MGR_DECLARE(dAcOOctGrass_c);

    static const char *GRASS_OARCS[4];
    static const char *GRASS_CUT_MODELS[4];
};

#endif
