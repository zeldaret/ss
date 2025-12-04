#ifndef D_A_OBJ_WARP_H
#define D_A_OBJ_WARP_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/flag/storyflag_manager.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOWarp_c : public dAcObjBase_c {
public:
    dAcOWarp_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOWarp_c() {}

    STATE_FUNC_DECLARE(dAcOWarp_c, Wait);
    STATE_FUNC_DECLARE(dAcOWarp_c, SirenFinish);
    STATE_FUNC_DECLARE(dAcOWarp_c, GetItem);
    STATE_FUNC_DECLARE(dAcOWarp_c, GateWait);
    STATE_FUNC_DECLARE(dAcOWarp_c, GateOpen);
    STATE_FUNC_DECLARE(dAcOWarp_c, GateIn);
    STATE_FUNC_DECLARE(dAcOWarp_c, GateClear);

    virtual void fn_0x80();
    virtual void fn_0x84();
    virtual void fn_0x88();
    virtual s32 checkStateGateOpen();
    virtual s32 fn_0x90();
    virtual s32 checkThisHasSongItem();

    bool isFaronTrial() const {
        return mTrialIndexBitmask & 0x10;
    }

    bool isEldinTrial() const {
        return mTrialIndexBitmask & 0x40;
    }

    bool isLanayruTrial() const {
        return mTrialIndexBitmask & 0x20;
    }

    bool isHyliaTrial() const {
        return mTrialIndexBitmask & 0x80;
    }

    bool isFaronTrialAndCompleted() const {
        return isFaronTrial() && StoryflagManager::sInstance->getFlag(STORYFLAG_FARON_TRIAL_COMPLETE);
    }

    bool isEdlinTrialAndCompleted() const {
        return isEldinTrial() && StoryflagManager::sInstance->getFlag(STORYFLAG_ELDIN_TRIAL_COMPLETE);
    }

    bool isLanayruTrialAndCompleted() const {
        return isLanayruTrial() && StoryflagManager::sInstance->getFlag(STORYFLAG_LANAYRU_TRIAL_COMPLETE);
    }

    bool isHyliaTrialAndCompleted() const {
        return isHyliaTrial() && StoryflagManager::sInstance->getFlag(STORYFLAG_HYLIA_TRIAL_COMPLETE);
    }

    bool isTrialComplete() const {
        return isFaronTrialAndCompleted()
            || isEdlinTrialAndCompleted()
            || isLanayruTrialAndCompleted()
            || isHyliaTrialAndCompleted();
    }
private:
    /* 0x330 */ u8 field_0x330[0xC66-0x330];
    /* 0xC66 */ u8 mTrialIndexBitmask;
    /* 0x??? */ STATE_MGR_DECLARE(dAcOWarp_c);
};

#endif
