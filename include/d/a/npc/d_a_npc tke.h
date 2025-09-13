#ifndef D_A_NPC_TKE_H
#define D_A_NPC_TKE_H

#include "d/a/npc/d_a_npc.h"

/**
 * Actor - NPC - Talk Event (?)
 * This actor has no model, so it appears to be something like a "virtual" NPC for
 * purposes of event / flow stuff?
 */
class daNpcTke_c : public dAcNpc_c {
public:
    void setFinishFlags();
};

#endif
