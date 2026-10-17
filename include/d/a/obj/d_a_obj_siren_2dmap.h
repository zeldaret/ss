#ifndef D_A_OBJ_SIREN_2DMAP_H
#define D_A_OBJ_SIREN_2DMAP_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"

class dAcOsiren2dmap_c : public dAcObjBase_c {
public:
    enum Subtype {
        SUBTYPE_SIREN_MAIKI_MAP = 0,
        SUBTYPE_F406_COL = 1,
    };

    dAcOsiren2dmap_c() {}
    virtual ~dAcOsiren2dmap_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ dBgW mBgW;
    /* 0x540 */ u8 mSubtype;
};

#endif
