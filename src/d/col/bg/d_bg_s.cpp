#include "d/col/bg/d_bg_s.h"

#include "d/col/c/c_m3d.h"
#include "d/col/c/c_partition.h"

cBgS_ChkElm::cBgS_ChkElm() {
    Init();
}

cBgS_ChkElm::~cBgS_ChkElm() {}

void cBgS_ChkElm::Init() {
    mpBgW = nullptr;
    mObj.unlink();
}

void cBgS_ChkElm::Regist2(dBgW_Base *pdBgW, dAcObjBase_c *pObj) {
    mpBgW = pdBgW;
    if (pObj) {
        mObj.link(pObj);
    } else {
        mObj.unlink();
    }
}

void cBgS_ChkElm::Release() {
    Init();
}

cBgS::cBgS() : mSetCounter(0) {}

cBgS::~cBgS() {}

void cBgS::fn_80339de0(dBgW_Base *pBgW) {
    if (!cPartition::fn_80339070(pBgW->GetBnd())) {
        return pBgW->vt_0x3C();
    }
}
