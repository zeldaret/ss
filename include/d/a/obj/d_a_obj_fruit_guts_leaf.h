#ifndef D_A_OBJ_FRUIT_GUTS_LEAF_H
#define D_A_OBJ_FRUIT_GUTS_LEAF_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_smdl.h"

class dAcOFruitGutsLeaf_c : public dAcObjBase_c {
public:
    dAcOFruitGutsLeaf_c() {}
    virtual ~dAcOFruitGutsLeaf_c() {}

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:

    s32 getType();
    bool hideInPast();
    s32 getPastBehavior();

    /* 0x330 */ m3d::smdl_c mMdl;
};

#endif
