#ifndef D_T_REACTION_H
#define D_T_REACTION_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>
#include <toBeSorted/cc/d_cc_shape_colliders.h>
#include <toBeSorted/dowsing_target.h>

class dTgReaction_c : public dAcObjBase_c {
public:
    dTgReaction_c() : mStateMgr(*this, sStateID::null), mDowsingTarget(this, DowsingTarget::SLOT_NONE) {}
    virtual ~dTgReaction_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dTgReaction_c, Wait);

private:
    static dCcD_SrcCyl sCcSrc;
    static const f32 sRadius;
    static const f32 sHeight;

    u32 getParam0x00() {
        return params & 0xFF;
    }

    int getParam0x08() {
        return (params >> 8) & 0xFF;
    }

    s32 getParam0x10() {
        return (params >> 0x10) & 0xF;
    }

    bool getParam0x14() {
        return (params >> 0x14) & 0x1;
    }

    int getParam0x1C() {
        return params >> 0x1C;
    }

    void onDelete();
    void checkForBonkItem();
    void checkForBubble();
    void checkForSlingBellowsItem();

    bool fn_578_DB0(const mVec3_c &pos, u32 arg);
    bool spawnHearts(s32 params, const mVec3_c &pos, s32 arg, mAng angle);

    STATE_MGR_DECLARE(dTgReaction_c);
    dCcD_Cyl mCollision;
    DowsingTarget mDowsingTarget;

    bool mHasDowsingTarget;
    bool field_0x4DD;
    bool field_0x4DE;
    u32 field_0x4E0;
    f32 field_0x4E4;
};

#endif
