#ifndef D_A_NPC_RIVAL_LOD_H
#define D_A_NPC_RIVAL_LOD_H

#include "d/a/npc/d_a_npc.h"
#include "d/d_shadow.h"
#include "m/m3d/m_anmmdl.h"
#include "s/s_State.hpp"
#include "s/s_StateInterfaces.hpp"

class dAcNpcRivalLOD_c : public dAcNpc_c {
public:
    dAcNpcRivalLOD_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcRivalLOD_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    // weird actor - uses its own state manager, still
    // overrides the virtual base state

    virtual void setState(const sStateIDIf_c &state) override {
        mStateMgr.changeState(state);
    }

    virtual void executeState() override {
        mStateMgr.executeState();
    }

    virtual bool isInState(const sStateIDIf_c &state) const override {
        return mStateMgr.isState(state);
    }

    STATE_FUNC_DECLARE(dAcNpcRivalLOD_c, Wait);

private:
    /* 0x6E4 */ m3d::mdlAnmChr mMdl;
    /* 0x74C */ dShadowCircle_c mShadow;
    /* 0x754 */ STATE_MGR_DECLARE(dAcNpcRivalLOD_c);
};

#endif
