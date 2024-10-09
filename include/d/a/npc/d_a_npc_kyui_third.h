#ifndef D_A_NPC_KYUI_THIRD_H
#define D_A_NPC_KYUI_THIRD_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcKyuiThird_c : public dAcNpc_c {
public:
    dAcNpcKyuiThird_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcKyuiThird_c() {}

    STATE_FUNC_DECLARE(dAcNpcKyuiThird_c, Wait2);
    STATE_FUNC_DECLARE(dAcNpcKyuiThird_c, Hang);
    STATE_FUNC_DECLARE(dAcNpcKyuiThird_c, HelpReq);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcKyuiThird_c);
};

#endif
