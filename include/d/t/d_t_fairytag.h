#ifndef D_T_OBJ_FAIRY_H
#define D_T_OBJ_FAIRY_H

#include "d/a/obj/d_a_obj_fairy.h"
#include "d/t/d_tg.h"

class dTgFairy_c : public dTg_c {
public:
    dTgFairy_c() {}
    virtual ~dTgFairy_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    void createFairies();
    mVec3_c calcLocation(const f32 &);
    f32 calcRnd(const f32 &, const f32 &);

    dAcRef_c<dAcOFairy_c> *mpFairyRefs;
    s32 mCount;
};

#endif
