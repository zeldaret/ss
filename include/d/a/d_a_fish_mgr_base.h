#ifndef D_A_FISH_MGR_BASE_H
#define D_A_FISH_MGR_BASE_H

#include "d/t/d_tg.h"
#include "d/a/d_a_fish_base.h"
#include "s/s_State.hpp"

// Not sure if dTg_c is the right parent class but there is one parent before dAcBase_c without extra data
class dAcFishMgrBase_c : public dTg_c {
public:
    dAcFishMgrBase_c() : mStateMgr(*this) {}
    virtual ~dAcFishMgrBase_c() {}

    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int doDelete() override;
    virtual int draw() override;

    dAcFishBase_c *getFish() {
        return mRef.get();
    }

    f32 getField_0x144() const {
        return field_0x144;
    }

    f32 getField_0x148() const {
        return field_0x148;
    }

    f32 getField_0x14C() const {
        return field_0x14C;
    }

    bool getField_0x151() const {
        return field_0x151;
    }

protected:
    STATE_VIRTUAL_FUNC_DECLARE(dAcFishMgrBase_c, Wait);
    virtual bool vt_0x80();

private:
    /* 0x0FC */ STATE_MGR_DECLARE(dAcFishMgrBase_c);
    /* 0x138 */ dAcRef_c<dAcFishBase_c> mRef;
    /* 0x144 */ f32 field_0x144;
    /* 0x148 */ f32 field_0x148;
    /* 0x14C */ f32 field_0x14C;
    /* 0x150 */ bool field_0x150;
    /* 0x151 */ bool field_0x151;
};

#endif
