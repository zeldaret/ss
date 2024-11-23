#include "d/a/obj/d_a_obj_base.h"
#include "m/m_mtx.h"

ActorCarryStruct::ActorCarryStruct()
    : actorLink(nullptr), carriedActor(nullptr), carryFlags(0), carryType(0), field_0x18(0),
      field_0x1C(0.0f, 0.0f, 0.0f), carryTransMtx(mMtx_c::Identity), field_0x58(mMtx_c::Identity), isCarried(0),
      field_0x8C(0.0f), field_0x90(0.0f), field_0x94(0.0f), field_0x98(0.0f) {}

ActorCarryStruct::~ActorCarryStruct() {
    fn_80051190(nullptr);
    fn_80050EB0(nullptr);
}
