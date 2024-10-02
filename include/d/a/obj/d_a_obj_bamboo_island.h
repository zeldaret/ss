#ifndef D_A_OBJ_BAMBOO_ISLAND_H
#define D_A_OBJ_BAMBOO_ISLAND_H

#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_smdl.h>
#include <toBeSorted/actor_collision.h>

class dAcObambooIsland_c : public dAcObjBase_c {
public:
    dAcObambooIsland_c() {}
    virtual ~dAcObambooIsland_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

private:
    static const f32 unusedFloat1;
    static const f32 unusedFloat2;

    static void interactCallback(void *unknown, dAcBase_c *actor, dAcObjBase_c *interactor);

    m3d::smdl_c mModels[2];
    nw4r::g3d::ResFile mBrres;
    ActorCollision mCollision;
};

#endif
