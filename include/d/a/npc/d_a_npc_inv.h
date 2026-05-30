#ifndef D_A_NPC_INV_H
#define D_A_NPC_INV_H

#include "d/a/npc/d_a_npc.h"

class dAcNpcInv_c : public dAcNpc_c {
public:
    dAcNpcInv_c() {}
    virtual ~dAcNpcInv_c() {}

    s32 getActualFlowNpcId() const;

private:
};

#endif
