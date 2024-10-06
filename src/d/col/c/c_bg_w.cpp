#include "d/col/c/c_bg_w.h"

cBgW_BgId::cBgW_BgId() {
    Ct();
}

cBgW_BgId::~cBgW_BgId() {}

void cBgW_BgId::Regist(int id) {
    m_id = id;
}

void cBgW_BgId::Release() {
    Ct();
}

bool cBgW_BgId::ChkUsed() const {
    // What??
    u16 id = GetId();
    return id <= BG_ID_MAX - 1;
}
