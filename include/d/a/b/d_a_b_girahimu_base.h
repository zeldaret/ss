#ifndef D_A_B_GIRAHIMU_BASE_H
#define D_A_B_GIRAHIMU_BASE_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcGirahimuBase_c : public dAcEnBase_c {
public:
    dAcGirahimuBase_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcGirahimuBase_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcGirahimuBase_c);
};

#endif
