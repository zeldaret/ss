#include "d/col/bg/d_bg_plc.h"

void dBgPlc::setBase(void *pPlc) {
    mBase = (PLC *)pPlc;
}

u32 dBgPlc::getGrpCode(int idx) const {
    return getCode(idx)->code4;
}
