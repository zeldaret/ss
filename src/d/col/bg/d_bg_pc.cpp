#include "d/col/bg/d_bg_pc.h"

bool dBgPc::getWtr() const {
    return (m_code.code3 & 0x1F) == 0xC;
}
