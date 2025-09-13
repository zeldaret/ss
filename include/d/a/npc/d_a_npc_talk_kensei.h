#ifndef D_A_NPC_TALK_KENSEI_H
#define D_A_NPC_TALK_KENSEI_H

#include "d/a/npc/d_a_ordinary_npc.h"

class dAcSalbageObj_c;

class dAcNpcTalkKensei_c : public dAcOrdinaryNpc_c {
public:
    static dAcNpcTalkKensei_c *GetInstance();
    void triggerSalvageQuestion(dAcSalbageObj_c *);

private:
};

#endif
