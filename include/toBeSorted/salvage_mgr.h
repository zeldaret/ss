#ifndef D_SALVAGE_MGR_H
#define D_SALVAGE_MGR_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_salbage_obj.h"
#include "d/a/npc/d_a_npc_salbage_robot.h"
#include "m/m_mtx.h"

class dAcSalbageObj_c;
class dAcNpcSlb_c;

class dSalvageMgr_c {
public:
    // Directly accessing seems to be required
    // static dSalvageMgr_c *GetInstance() {
    //     return sInstance;
    // }

    // TODO a fun theory is that all those `const s32 &`s are just `const SalvageObj_e &`s.

    bool checkDeliveredStoryflag(const s32 &id) const;
    bool checkPickupStoryflag(const s32 &id) const;
    bool startedQuestForSalvageObj(const dAcSalbageObj_c *obj) const;
    bool hasDowsingForSalvageObj(const dAcSalbageObj_c *obj) const;
    mMtx_c getCarryRotMtx(const s32 &id) const;
    mMtx_c getCarryRotMtx2(const s32 &id) const {
        return getCarryRotMtx(id);
    }
    s32 getCurrentSalvageObjId() const;
    void deleteObjAndSlb();

    static dSalvageMgr_c *sInstance;
    static void create();
    static void remove();

    void init();
    void execute();

    void setPickupIndex(const s32 &id);
    void unsetPickupStoryflag(const s32 &id);
    void handlePickupIndex();
    void setupDeliverToActorLink();
    bool checkIsOnDeliveryStage() const;
    bool checkIsOnDeliveryStage2() const;
    u16 getDeliverToActorId(s32 id);
    dAcObjBase_c *findDeliverToActor();
    bool startedQuestForId(const s32 &id) const;
    bool hasDowsingForId(const s32 &id) const;

    void noop();

    bool triggerDeliveryEvent();

    u16 getTalkEntryPoint1ForPickup(dAcObjBase_c *obj);
    u16 getTalkEntryPoint2ForPickup(dAcObjBase_c *obj);
    u16 getTalkEntryPoint3ForPickup(dAcObjBase_c *obj);

    bool hasObjAndSlb() const {
        return mObjRef.isLinked() && mSlbRef.isLinked();
    }

    bool checkPickupMask(const s32 &id) const {
        return (mCurrentPickupMaskBit & (1 << id)) != 0;
    }

    /* 0x00 */ dAcRef_c<dAcSalbageObj_c> mObjRef;
    /* 0x0C */ dAcRef_c<dAcNpcSlb_c> mSlbRef;
    /* 0x18 */ dAcRef_c<dAcObjBase_c> mDeliverToRef;
    /* 0x24 */ s32 mCurrentPickupIdx;
    /* 0x28 */ s32 mCurrentPickupMaskBit;
    /* 0x2C */ s16 field_0x2C;

private:
};

#endif
