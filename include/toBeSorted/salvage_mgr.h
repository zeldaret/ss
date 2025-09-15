#ifndef D_SALVAGE_MGR_H
#define D_SALVAGE_MGR_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "m/m_mtx.h"

class dAcSalbageObj_c;
class dAcNpcSlb_c;

class dSalvageMgr_c {
public:
    // Directly accessing seems to be required
    // static dSalvageMgr_c *GetInstance() {
    //     return sInstance;
    // }

    bool checkDeliveredStoryflag(const s32& id) const;
    bool startedQuestForSalvageObj(const dAcSalbageObj_c *obj) const;
    bool hasDowsingForSalvageObj(const dAcSalbageObj_c *obj) const;
    mMtx_c getCarryRotMtx(const s32 &id) const;
    mMtx_c getCarryRotMtx2(const s32 &id) const {
        return getCarryRotMtx(id);
    }
    s32 getCurrentSalvageObjId() const;
    void deleteObjAndSlb();

    static dSalvageMgr_c *sInstance;

    bool hasObjAndSlb() const {
        return mObjRef.isLinked() && mSlbRef.isLinked();
    }

    /* 0x00 */ dAcRef_c<dAcSalbageObj_c> mObjRef;
    /* 0x0C */ dAcRef_c<dAcNpcSlb_c> mSlbRef;
private:
};

#endif
