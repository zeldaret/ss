#include "d/col/c/c_bg_s_chk.h"

cBgS_Chk::cBgS_Chk()
    : mPolyPassChk(nullptr), mGrpPassChk(nullptr), mActorId(0), field_0x0C(0), field_0x0E(1), field_0x10(1) {}

cBgS_Chk::~cBgS_Chk() {}

void cBgS_Chk::SetActorID(u32 *pId) {
    u32 id = pId ? *pId : 0;
    mActorId = id;
}
