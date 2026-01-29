#ifndef D_A_OBJ_CHEST_H
#define D_A_OBJ_CHEST_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/dowsing_target.h"

class dAcOChest_c : public dAcObjBase_c {
public:
    dAcOChest_c()
        : mStateMgr(*this), mEvent(*this, nullptr), mDowsingTarget(this, DowsingTarget::SLOT_NONE) {}
    virtual ~dAcOChest_c() {}

    static void changeStateOpenCallback(dAcOChest_c *);
    static void changeStateWaitCallback(dAcOChest_c *);
    static void changeStateFailCallback(dAcOChest_c *);
    static void changeStateWait2Callback(dAcOChest_c *);

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int draw() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOChest_c, Wait);
    STATE_FUNC_DECLARE(dAcOChest_c, OpenEvent);
    STATE_FUNC_DECLARE(dAcOChest_c, FailEvent);
    STATE_FUNC_DECLARE(dAcOChest_c, OrderOpenEventAfter);

    virtual void doInteraction(s32) override;

    void fn_326_C90();
    void changeStateOpen();
    void changeStateWait();
    void changeStateFail();
    void changeStateWait2();
    char *getName();
    char *getSubtypeName();
    char *getModelName();
    char *getOpenOrClose(u8);
    char *getDzbPlcName();
    s32 getSubtype();
    bool hasBeenOpened();
    void stateOpenUpdate2();
    void stateOpenEventEnter2();
    void fn_326_1440();
    bool hasInsideModel();
    void fn_326_1470();
    void fn_326_14A0();

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ d3d::AnmMdlWrapper mAnmMdl;
    /* 0x3A4 */ m3d::smdl_c mInsideMdl;
    /* 0x3C0 */ dBgW mBgW;
    /* 0x5D0 */ STATE_MGR_DECLARE(dAcOChest_c);
    /* 0x60C */ ActorEventRelated mEvent;
    /* 0x65C */ bool mGaveItem;
    /* 0x65D */ bool field_0x65D;
    /* 0x65E */ bool mHasInsideModel;
    /* 0x65F */ bool mIsLinksCloset;
    /* 0x660 */ DowsingTarget mDowsingTarget;

    static const f32 OPEN_WARDROBE_INTERACTION_FIELD_0x24;
    static const char *CHEST_OPEN;
    static const char *CHEST_OPEN_AFTER;
    static const char *CHEST_FAIL;
    static InteractionTargetDef OPEN_WARDROBE_INTERACTION;
    static char *INSIDE_MODEL_NAMES[4];
    static const u32 unused;
};

#endif
