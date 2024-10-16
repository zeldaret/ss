
#include "d/col/bg/d_bg_w_base.h"

#include "d/col/bg/d_bg_s.h"

dBgW_Base::dBgW_Base() {
    ClrDBgWBase();
}

dBgW_Base::~dBgW_Base() {
    if (ChkUsed()) {
        dBgS::GetInstance()->Release(this);
    }
    if (GetRegistId() != BG_ID_MAX) {
        dBgS::GetInstance()->UnRegist(this);
    }
    ClrDBgWBase();
}

void dBgW_Base::ClrDBgWBase() {
    // Regswap?

    mRoomId = 0xFF;
    mPriority = 2;
    field_0x2E = 0xFF;
    // A set to 0 is required. I think 0x24 may be flags, so an unset then set?
    field_0x24 = 0;
    field_0x24 = 4;
    mRegistId = BG_ID_MAX;
    field_0x2F = 0xFF;
    mDiffShapeAngleY = 0;
    mOldShapeAngleY = 0;
    SetPushPullCallback(nullptr);
    ClearMap();
    field_0x20 = 0;
    field_0x22 = 0xFFFF;
}

bool dBgW_Base::ChkMemoryError() {
    return false;
}

void dBgW_Base::CallRideCallback(dAcObjBase_c *, dAcObjBase_c *) {
    // No Call
}

void dBgW_Base::CallArrowStickCallback(dAcObjBase_c *, dAcObjBase_c *, mVec3_c &) {
    // No Call
}

bool dBgW_Base::CallUnkCallback(dAcObjBase_c *, dAcObjBase_c *) {
    return false;
}

void dBgW_Base::CalcDiffShapeAngleY(s16 shapeAngleY) {
    mDiffShapeAngleY = shapeAngleY - mOldShapeAngleY;
    mOldShapeAngleY = shapeAngleY;
}

void dBgW_Base::SetOldShapeAngleY(s16 oldShapeAngleY) {
    mOldShapeAngleY = oldShapeAngleY;
}

bool dBgW_Base::InitMapStuff(mAllocator_c *pAllocator) {
    ClearMap();
    return UpdateDraw(pAllocator) != false;
}

bool dBgW_Base::fn_8034AD70() const {
    u8 val = mField_0x18.field_0x00;
    if (val == 0 || val == 2) {
        return true;
    }
    return false;
}

bool dBgW_Base::fn_8034ADA0() const {
    return mField_0x18.field_0x00 <= 1;
}

bool dBgW_Base::UpdateDraw(mAllocator_c * /* unused */) {
    ClearMap();
    return true;
}

void dBgW_Base::RegistBg(int id) {
    mRegistId = id;
}

void dBgW_Base::UnRegistBg() {
    mRegistId = BG_ID_MAX;
}

bool dBgW_Base::ChkReady() const {
    if (mRegistId == BG_ID_MAX || ChkNotReady()) {
        return false;
    }
    return true;
}

void dBgW_Base::Set0x2F(u8 val) {
    field_0x2F = val;
}
