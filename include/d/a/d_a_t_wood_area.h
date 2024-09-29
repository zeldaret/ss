#ifndef D_A_T_WOOD_AREA_H
#define D_A_T_WOOD_AREA_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcTWoodArea_c : public dAcObjBase_c {
public:
    dAcTWoodArea_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcTWoodArea_c() {}
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcTWoodArea_c, Init);
    STATE_FUNC_DECLARE(dAcTWoodArea_c, Wait);

    void attachCloseObjects(ProfileName profID);
    void dropItems();
    bool attachObject(dAcObjBase_c *obj);

    f32 getAttachRadius() {
        return scale.x * scaleX;
    }
    f32 getAttachRadiusSquare() {
        return getAttachRadius() * getAttachRadius();
    }
    f32 getAttachHeight() {
        return scale.y * scaleY;
    }

    static const f32 scaleX;
    static const f32 scaleY;

private:
    STATE_MGR_DECLARE(dAcTWoodArea_c);

    dAcRef_c<dAcObjBase_c> mRefs[8];
};

#endif
