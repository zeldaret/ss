#ifndef D_A_OBJ_ISLAND_LOD_H
#define D_A_OBJ_ISLAND_LOD_H

#include "d/a/obj/d_a_obj_base.h"

class dAcOislandLOD_c : public dAcObjBase_c {
public:
    dAcOislandLOD_c() {}
    virtual ~dAcOislandLOD_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    static const char* const resFilenames[5];
    static const char* const resMdlNames[5];
    
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ u8 mMdlLodIdx;
};

#endif
