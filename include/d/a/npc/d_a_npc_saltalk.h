#ifndef D_A_NPC_SALTALK_H
#define D_A_NPC_SALTALK_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/npc/d_a_npc.h"
#include "d/a/npc/d_a_npc_volcano_f2_salbo.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/area_utils.h"

class dAcNpcSltk_c : public dAcNpc_c {
public:
    dAcNpcSltk_c() : mEvent(*this, nullptr), mpArea(nullptr) {}
    virtual ~dAcNpcSltk_c() {}

    virtual int create() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    virtual int acNpc_vt_0xDC() override;
    virtual int draw() override;

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcNpcSltk_c, dAcNpc_c, Wait);

private:
    int getParam0_0() const;
    int getParam0_1() const;

    int getParam1() const;
    int getParam2() const;
    int getParam3() const;
    int getParam4() const;

    int getSomething() const;
    bool isSomething0() const;
    bool isSomething1() const;

    bool checkSomething(mVec3_c pos) const;

    /* 0x6E4 */ dAcRef_c<dAcNpcSlb2_c> mRef;
    /* 0x6F0 */ int mAreaIndex;
    /* 0x6F4 */ AREA *mpArea;
    /* 0x6F8 */ ActorEventRelated mEvent;
    /* 0x748 */ int field_0x748;
    /* 0x74C */ int field_0x74C;
    /* 0x750 */ int field_0x750;
    /* 0x754 */ int field_0x754;
    /* 0x758 */ f32 field_0x758;
    /* 0x75C */ f32 field_0x75C;
    /* 0x760 */ mVec3_c mLinkPos;
    /* 0x76C */ u8 field_0x76C;
};

#endif
