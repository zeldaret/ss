#include "d/a/npc/d_a_npc.h"

#include "f/f_list_mg.h"
#include "nw4r/ut/ut_Color.h"
#include "s/s_StateID.hpp"
#include "sized_string.h"

fLiMgBa_c dAcNpc_c::NPC_ACTOR_LIST;

STATE_VIRTUAL_DEFINE(dAcNpc_c, Wait);
STATE_VIRTUAL_DEFINE(dAcNpc_c, Demo);

static const nw4r::ut::Color sColors0[] = {
    nw4r::ut::Color(0xEF, 0x30, 0x27, 0xFF),
    nw4r::ut::Color(0x00, 0x00, 0x00, 0xFF),
    nw4r::ut::Color(0xEF, 0x30, 0x27, 0x00),
};
static const nw4r::ut::Color sColors1[] = {
    nw4r::ut::Color(0xFF, 0xFF, 0xFF, 0x00),
    nw4r::ut::Color(0xFE, 0xF8, 0x9D, 0xFF),
    nw4r::ut::Color(0x88, 0x63, 0x36, 0x80),
};
static const nw4r::ut::Color sColors2[] = {
    nw4r::ut::Color(0x4F, 0x46, 0x33, 0xFF),
    nw4r::ut::Color(0xD8, 0xC5, 0x8D, 0xFF),
    nw4r::ut::Color(0x95, 0x69, 0x3B, 0xFF),
};
static const nw4r::ut::Color sColors3[] = {
    nw4r::ut::Color(0x50, 0x46, 0x32, 0x00),
    nw4r::ut::Color(0xD8, 0xC5, 0x8D, 0x00),
    nw4r::ut::Color(0x96, 0x69, 0x3C, 0x00),
};

SizedString<128> sNpcStr1;
SizedString<128> sNpcStr2;
SizedString<128> sNpcStr3;
SizedString<128> sNpcStr4;

dAcNpc_c::dAcNpc_c() : mStateMgr(*this, sStateID::null), mActorListEntry(this) {}

dAcNpc_c::~dAcNpc_c() {
    // TODO
}
