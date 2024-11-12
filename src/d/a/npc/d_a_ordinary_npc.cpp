#include "d/a/npc/d_a_ordinary_npc.h"
#include "d/a/npc/d_a_npc.h"
#include "s/s_StateID.hpp"

STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Wait);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Pain);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Surprised);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Withstand);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Walk);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, LookAwaySt);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, LookAway);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Turn);
STATE_DEFINE(dAcOrdinaryNpc_c, PreWalkTurn);
