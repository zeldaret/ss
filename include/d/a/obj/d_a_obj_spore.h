#ifndef D_A_O_SPORE_H
#define D_A_O_SPORE_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_list_mg.h"
class dAcOSpore_c : public dAcObjBase_c {
public:
    dAcOSpore_c() {}
    virtual ~dAcOSpore_c() {}

    static fLiNdBa_c *getListHead();
    s32 getSporeType() const {
        return mParams & 0xF;
    }
    bool isGlitteringSpore() const {
        return getSporeType() == 1;
    }

private:
    static fLiMgBa_c sSpores;
};

#endif
