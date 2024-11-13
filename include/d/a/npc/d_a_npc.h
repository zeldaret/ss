#ifndef D_A_NPC_H
#define D_A_NPC_H

#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "s/s_StateInterfaces.hpp"

// This is the NPC base. Most npcs actually use dAcOrdinaryNpc, but this just is a simpler one?

// Ghidra: ActorNpcBase
//   size: 0x6e4
// official name
class dAcNpc_c : public dAcObjBase_c {
public:
    dAcNpc_c();
    virtual ~dAcNpc_c();

    /* vt 0x080 */ virtual bool giveItem(void *unknown, ITEM_ID item_id);
    /* vt 0x084 */ virtual void getPosCopy3(mVec3_c &outResult) {
        outResult.copyFrom(poscopy3);
    }
    /* vt 0x088 */ virtual void acNpc_vt_0x88() {}
    /* vt 0x08C */ virtual void acNpc_vt_0x8C() {}
    /* vt 0x090 */ virtual int acNpc_vt_0x90() {
        return 1;
    }
    /* vt 0x094 */ virtual int acNpc_vt_0x94() {
        return 1;
    }
    /* vt 0x098 */ virtual int acNpc_vt_0x98() {
        return 1;
    }
    /* vt 0x09C */ virtual void acNpc_vt_0x9C();
    /* vt 0x0A0 */ virtual void acNpc_vt_0xA0();
    /* vt 0x0A4 */ virtual void acNpc_vt_0xA4();
    /* vt 0x0A8 */ virtual void acNpc_vt_0xA8();
    /* vt 0x0AC */ virtual void acNpc_vt_0xAC();
    /* vt 0x0B0 */ virtual void acNpc_vt_0xB0();
    /* vt 0x0B4 */ virtual void acNpc_vt_0xB4();
    /* vt 0x0B8 */ virtual void acNpc_vt_0xB8();
    /* vt 0x0BC */ virtual void acNpc_vt_0xBC();
    /* vt 0x0C0 */ virtual int eventFlowSwitch2() {
        return 0;
    }
    /* vt 0x0C4 */ virtual int eventFlowSwitch3() {
        return 0;
    }
    /* vt 0x0C8 */ virtual int eventFlowSwitch4() {
        return 0;
    }
    /* vt 0x0CC */ virtual void acNpc_vt_0xCC();
    /* vt 0x0D0 */ virtual int acNpc_vt_0xD0();
    /* vt 0x0D4 */ virtual void acNpc_vt_0xD4();
    /* vt 0x0D8 */ virtual int acNpc_vt_0xD8();
    /* vt 0x0DC */ virtual int acNpc_vt_0xDC() {
        return 0;
    }
    /* vt 0x0E0 */ virtual int acNpc_vt_0xE0() {
        return 0;
    }
    /* vt 0x0E4 */ virtual int acNpc_vt_0xE4() {
        return 1;
    }
    /* vt 0x0E8 */ virtual void acNpc_vt_0xE8();

    /* vt 0x074 */ virtual void *getObjectListEntry() override {
        return &mActorListEntry;
    }

    /* vt 0x0EC */ virtual int acNpc_vt_0xEC() {
        return 0;
    }

    /* vt 0x0F0 */ virtual bool isInState(const sStateIDIf_c &otherState) const {
        return *mStateMgr.getStateID() == otherState;
    }

    /* vt 0x0F4 */ virtual void setState(const sStateIDIf_c &otherState) {
        // Result is discarded, but I guess in demo builds this is used
        // to prevent actors from leaving the Demo state.
        (void)isInState(StateID_Demo);
        mStateMgr.changeState(otherState);
    }

    /* vt 0xF8 */ virtual void executeState() {
        mStateMgr.executeState();
    }

    STATE_VIRTUAL_FUNC_DECLARE(dAcNpc_c, Wait);
    STATE_VIRTUAL_FUNC_DECLARE(dAcNpc_c, Demo);

    /* vt 0x114 */ virtual int acNpc_vt_0x114() {
        return 0;
    }

    /* vt 0x118 */ virtual int acNpc_vt_0x118() {
        return 0;
    }
    /* vt 0x11C */ virtual void acNpc_vt_0x11C();
    /* vt 0x120 */ virtual void acNpc_vt_0x120() {}
    /* vt 0x124 */ virtual void acNpc_vt_0x124() {}
    /* vt 0x128 */ virtual int acNpc_vt_0x128() {
        return 0;
    }
    /* vt 0x12C */ virtual int acNpc_vt_0x12C() {
        return 0;
    }
    /* vt 0x130 */ virtual void acNpc_vt_0x130();
    /* vt 0x134 */ virtual void acNpc_vt_0x134();
    /* vt 0x138 */ virtual void acNpc_vt_0x138();
    /* vt 0x13C */ virtual void acNpc_vt_0x13C();
    /* vt 0x140 */ virtual void acNpc_vt_0x140();
    /* vt 0x144 */ virtual void acNpc_vt_0x144();
    /* vt 0x148 */ virtual void acNpc_vt_0x148();
    /* vt 0x14C */ virtual int acNpc_vt_0x14C() {
        return 0;
    }
    /* vt 0x150 */ virtual int acNpc_vt_0x150() {
        return 1;
    }
    /* vt 0x154 */ virtual void acNpc_vt_0x154() {}
    /* vt 0x158 */ virtual void acNpc_vt_0x158() {}
    /* vt 0x15C */ virtual void acNpc_vt_0x15C() {}
    /* vt 0x160 */ virtual void acNpc_vt_0x160() {}
    /* vt 0x164 */ virtual void acNpc_vt_0x164();
    /* vt 0x168 */ virtual int acNpc_vt_0x168() {
        return 0;
    }
    /* vt 0x16C */ virtual void acNpc_vt_0x16C();
    /* vt 0x170 */ virtual void acNpc_vt_0x170();
    /* vt 0x174 */ virtual void acNpc_vt_0x174();
    /* vt 0x178 */ virtual void acNpc_vt_0x178();
    /* vt 0x17C */ virtual void acNpc_vt_0x17C();
    /* vt 0x180 */ virtual void acNpc_vt_0x180();
    /* vt 0x184 */ virtual void acNpc_vt_0x184();
    /* vt 0x188 */ virtual void acNpc_vt_0x188();
    /* vt 0x18C */ virtual void acNpc_vt_0x18C();
    /* vt 0x190 */ virtual void acNpc_vt_0x190();
    /* vt 0x194 */ virtual void acNpc_vt_0x194();
    /* vt 0x198 */ virtual void acNpc_vt_0x198();
    /* vt 0x19C */ virtual void acNpc_vt_0x19C();
    /* vt 0x1A0 */ virtual void acNpc_vt_0x1A0();
    /* vt 0x1A4 */ virtual void acNpc_vt_0x1A4();
    /* vt 0x1A8 */ virtual void acNpc_vt_0x1A8();
    /* vt 0x1AC */ virtual void acNpc_vt_0x1AC();
    /* vt 0x1B0 */ virtual void acNpc_vt_0x1B0();
    /* vt 0x1B4 */ virtual void acNpc_vt_0x1B4();
    /* vt 0x1B8 */ virtual void acNpc_vt_0x1B8();
    /* vt 0x1BC */ virtual void acNpc_vt_0x1BC();
    /* vt 0x1C0 */ virtual void acNpc_vt_0x1C0();
    /* vt 0x1C4 */ virtual void acNpc_vt_0x1C4();
    /* vt 0x1C8 */ virtual void acNpc_vt_0x1C8();
    /* vt 0x1CC */ virtual void acNpc_vt_0x1CC();
    /* vt 0x1D0 */ virtual void acNpc_vt_0x1D0();
    /* vt 0x1D4 */ virtual void acNpc_vt_0x1D4();
    /* vt 0x1D8 */ virtual void acNpc_vt_0x1D8();
    /* vt 0x1DC */ virtual void acNpc_vt_0x1DC();
    /* vt 0x1E0 */ virtual void acNpc_vt_0x1E0();
    /* vt 0x1E4 */ virtual void acNpc_vt_0x1E4();
    /* vt 0x1E8 */ virtual int acNpc_vt_0x1E8() {
        return 1;
    }
    /* vt 0x1EC */ virtual int acNpc_vt_0x1EC() {
        return 1;
    }
    /* vt 0x1F0 */ virtual void *acNpc_vt_0x1F0() {
        return field_0x364;
    }
    /* vt 0x1F4 */ virtual int acNpc_vt_0x1F4() {
        return 1;
    }
    /* vt 0x1F8 */ virtual bool acNpc_vt_0x1F8() {
        return !acNpc_vt_0x1F4();
    }
    /* vt 0x1FC */ virtual bool acNpc_vt_0x1FC() {
        return field_0x684 == 1;
    }
    /* vt 0x200 */ virtual bool acNpc_vt_0x200() {
        return !acNpc_vt_0x1FC();
    }
    /* vt 0x204 */ virtual int acNpc_vt_0x204() {
        return 0;
    }

protected:
    void npcExecute();
    static mVec3_c getLinkPos();

private:
    /* 0x330 */ u8 field_0x330[0x358 - 0x330];
    /* 0x358 */ fLiNdBa_c mActorListEntry;
    /* 0x364 */ u8 field_0x364[0x4C0 - 0x364];
    /* 0x4C0 */ STATE_MGR_DECLARE(dAcNpc_c);
    /* 0x4FC */ u8 field_0x4FC[0x684 - 0x4FC];
    /* 0x684 */ u8 field_0x684;
    /* 0x685 */ u8 field_0x685[0x6E4 - 0x685];
};

#endif
