#ifndef D_S_BOOT_H
#define D_S_BOOT_H

#include <d/d_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dScBoot_c : public dBase_c {
public:
    dScBoot_c();
    virtual ~dScBoot_c();

    STATE_FUNC_DECLARE(dScBoot_c, Init);
    STATE_FUNC_DECLARE(dScBoot_c, Strap);
    STATE_FUNC_DECLARE(dScBoot_c, Connect);
    STATE_FUNC_DECLARE(dScBoot_c, Save);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dScBoot_c);
};

#endif
