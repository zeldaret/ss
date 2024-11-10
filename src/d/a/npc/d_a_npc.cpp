#include "d/a/npc/d_a_npc.h"
#include "s/s_StateID.hpp"

fLiMgBa_c NPC_ACTOR_LIST;

STATE_VIRTUAL_DEFINE(dAcNpc_c, Wait);
STATE_VIRTUAL_DEFINE(dAcNpc_c, Demo);

extern nw4r::ut::Color sColors[] = {
    nw4r::ut::Color(0xEF, 0x30, 0x27, 0xFF),
    nw4r::ut::Color(0, 0, 0, 0xFF),
    nw4r::ut::Color(0xEF, 0x30, 0x27, 0),
    nw4r::ut::Color(0xFF, 0xFF, 0xFF, 0),
    nw4r::ut::Color(0xFE, 0xF8, 0x9D, 0xFF),
    nw4r::ut::Color(0x88, 0x63, 0x36, 0x80),
    nw4r::ut::Color(0x4F, 0x46, 0x33, 0xFF),
    nw4r::ut::Color(0xD8, 0xC5, 0x8D, 0xFF),
    nw4r::ut::Color(0x95, 0x69, 0x3B, 0xFF),
    nw4r::ut::Color(0x50, 0x46, 0x32, 0),
    nw4r::ut::Color(0xD8, 0xC5, 0x8D, 0),
    nw4r::ut::Color(0x96, 0x69, 0x3C, 0),
    nw4r::ut::Color(0, 0, 0, 0),
};


dAcNpc_c::dAcNpc_c() : mStateMgr(*this, sStateID::null) {}
