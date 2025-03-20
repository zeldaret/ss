#include "egg/gfx/eggScreen.h"

#include "common.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggStateGX.h"
#include "rvl/SC/scapi.h"


// ported from OGWS, though this class has evolved a fair bit

using namespace nw4r;

namespace EGG {

Screen::TVMode Screen::sTVMode;
Screen::TVModeInfo Screen::sTVModeInfo[TV_MODE_MAX] = {
    {
     608, 456,
     nw4r::math::VEC2(1.0526316f, 1.0f),
     },
    {
     812, 456,
     nw4r::math::VEC2(0.7881773, 1.0f),
     },
    {
     640, 456,
     nw4r::math::VEC2(1.0f, 1.0f),
     }
};

Screen *Screen::spRoot = nullptr;

Screen::ChangeTVModeFunc Screen::sChangeTVModeFunc = nullptr;
void *Screen::spChangeTVModeFuncInfo = nullptr;

math::VEC2 Screen::sCanvasScale(1.0f, 1.0f);
math::VEC2 Screen::sCanvasOffset(0.0f, 0.0f);

void Screen::Initialize(const u16 *maxX, const u16 *maxY, Screen *userRoot) {

    sTVModeInfo[TV_MODE_4_3].width = maxX[0];
    sTVModeInfo[TV_MODE_4_3].height = maxY[0];
    
    sTVModeInfo[TV_MODE_4_3].ratios.x = (f32)StateGX::s_widthEfb / (f32)sTVModeInfo[TV_MODE_4_3].width;
    sTVModeInfo[TV_MODE_4_3].ratios.y = (f32)StateGX::s_heightEfb / (f32)sTVModeInfo[TV_MODE_4_3].height;
    
    sTVModeInfo[TV_MODE_16_9].width = maxX[1];
    sTVModeInfo[TV_MODE_16_9].height = maxY[1];

    sTVModeInfo[TV_MODE_16_9].ratios.x = (f32)StateGX::s_widthEfb / (f32)sTVModeInfo[TV_MODE_16_9].width;
    sTVModeInfo[TV_MODE_16_9].ratios.y = (f32)StateGX::s_heightEfb / (f32)sTVModeInfo[TV_MODE_16_9].height;
    
    sTVModeInfo[TV_MODE_UNK_3].width = StateGX::s_widthEfb;
    sTVModeInfo[TV_MODE_UNK_3].height = StateGX::s_heightEfb;
    
    sTVModeInfo[TV_MODE_UNK_3].ratios.x = 1.0f;
    sTVModeInfo[TV_MODE_UNK_3].ratios.y = 1.0f;

    static Screen defaultRoot;
    defaultRoot.SetProjectionType(PROJ_ORTHO);
    defaultRoot.SetCanvasMode(CANVASMODE_1);
    defaultRoot.SetNearZ(0.0f);
    defaultRoot.SetFarZ(1.0f);
    defaultRoot.SetSize(math::VEC2(sTVModeInfo[sTVMode].width, sTVModeInfo[sTVMode].height));

    userRoot = (userRoot == nullptr) ? &defaultRoot : userRoot;
    spRoot = userRoot;
    userRoot->mParent = nullptr;
    SetTVMode(sTVMode);
}

Screen::Screen()
    : Frustum(
          PROJ_PERSP, math::VEC2(sTVModeInfo[sTVMode].width, sTVModeInfo[sTVMode].height), 10.0f, 100000.0f,
          CANVASMODE_1
      ) {
    mParent = nullptr;
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    field_0x44.x = 0.0f;
    field_0x44.y = 0.0f;
    field_0x48.x = 0.0f;
    field_0x48.y = 0.0f;
    SetParent(nullptr);
}

Screen::Screen(f32 x, f32 y, f32 w, f32 h, const Screen *userRoot, CanvasMode canvasMode)
    : Frustum(PROJ_PERSP, math::VEC2(w, h), 10.0f, 100000.0f, canvasMode) {
    mParent = nullptr;
    mPosition.x = x;
    mPosition.y = y;
    field_0x44.x = 0.0f;
    field_0x44.y = 0.0f;
    field_0x48.x = 0.0f;
    field_0x48.y = 0.0f;
    SetParent(userRoot);
}

Screen::Screen(const Screen &other)
    : Frustum(other),
      mPosition(other.mPosition),
      mDataEfb(other.mDataEfb),
      field_0x44(other.field_0x44),
      field_0x48(other.field_0x48) {
    SetParent(other.mParent);
}

void Screen::SetProjectionGX() const {
    if (!(mFlags & 0x40)) {
        f32 sx, sy, ox, oy;
        GetGlobalScaleOffset(&sx, &sy, &ox, &oy);

        SetGlobalScaleOffset(sCanvasScale.x, sCanvasScale.y, sCanvasOffset.x, sCanvasOffset.y);
        Frustum::SetProjectionGX();

        SetGlobalScaleOffset(sx, sy, ox, oy);
    } else {
        Frustum::SetProjectionGX();
    }

    const DataEfb &efb = GetDataEfb();

    StateGX::GXSetViewport_(efb.vp.x1, efb.vp.y1, efb.vp.x2, efb.vp.y2, efb.vp.z1, efb.vp.z2);
    StateGX::GXSetScissor_(efb.sc_x, efb.sc_y, efb.sc_w, efb.sc_h);
    StateGX::GXSetScissorBoxOffset_(efb.sc_ox, efb.sc_oy);
}

void Screen::CopyToG3D(g3d::Camera cam) const {
    if (!(mFlags & 0x40)) {
        f32 sx, sy, ox, oy;
        GetGlobalScaleOffset(&sx, &sy, &ox, &oy);

        SetGlobalScaleOffset(sCanvasScale.x, sCanvasScale.y, sCanvasOffset.x, sCanvasOffset.y);
        Frustum::CopyToG3D(cam);

        SetGlobalScaleOffset(sx, sy, ox, oy);
    } else {
        Frustum::CopyToG3D(cam);
    }

    const DataEfb &efb = GetDataEfb();

    f32 x1, x2, y1, y2;
    f32 z1, z2;

    y2 = efb.vp.y2;
    x2 = efb.vp.x2;

    y1 = efb.vp.y1;
    x1 = efb.vp.x1;

    z2 = efb.vp.z2;
    z1 = efb.vp.z1;

    cam.SetViewport(x1, y1, x2, y2);
    cam.SetViewportZRange(z1, z2);
    cam.SetScissor(efb.sc_x, efb.sc_y, efb.sc_w, efb.sc_h);
    cam.SetScissorBoxOffset(efb.sc_ox, efb.sc_oy);
}

void Screen::CopyFromAnother(const Screen &other) {
    Frustum::CopyFromAnother(other);

    mPosition = other.mPosition;
    field_0x44 = other.field_0x44;
    field_0x48 = other.field_0x48;
    mDataEfb = other.mDataEfb;
    mParent = other.mParent;
}

void Screen::Reset(f32 x, f32 y, f32 w, f32 h, CanvasMode mode) {
    mPosition.x = x;
    mPosition.y = y;
    mSize.x = w;
    SetDirty(true);
    mSize.y = h;
    SetCanvasMode(mode);
}

void Screen::SetParent(const Screen *parent) {
    const Screen *newParent = parent != nullptr ? parent : spRoot;

    if (mParent != newParent) {
        SetDirty(true);
        mParent = newParent;
    }
}

void Screen::SetUnkFlag8() {
    if ((mFlags & 0x8) == 0) {
        SetDirty(true);
    }
    mFlags |= 0x8;
}

void Screen::GetPosSizeInEfb() const {
    // TODO
    const TVMode tvMode = (!(mFlags & 0x20)) ? sTVMode : TV_MODE_4_3;
    const TVModeInfo &tvRatio = sTVModeInfo[tvMode];

    f32 *px = &mDataEfb.vp.x1;
    f32 *py = &mDataEfb.vp.y1;
    GetGlobalPos(px, py);

    *px = sCanvasScale.x * *px;
    *py = sCanvasScale.y * *py;

    *px *= tvRatio.ratios.x;
    *py *= tvRatio.ratios.y;

    mDataEfb.sc_oy = 0;
    mDataEfb.sc_ox = 0;

    const f32 x = *px;
    if (x < 0.0f) {
        *px = 0.0f;

        const s32 lx = static_cast<s32>(-x);
        mDataEfb.sc_ox = lx - (lx & 0x1);
    }

    const f32 y = *py;
    if (y < 0.0f) {
        *py = 0.0f;

        const s32 ly = static_cast<s32>(-y);
        mDataEfb.sc_oy = ly - (ly & 0x1);
    }

    if (mFlags & 0x2) {
        *px -= static_cast<s32>(*px) & 0x1;
        *py -= static_cast<s32>(*py) & 0x1;
    }

    *px = static_cast<s32>(*px);
    *py = static_cast<s32>(*py);

    mDataEfb.vp.x2 = mSize.x;
    mDataEfb.vp.y2 = mSize.y;

    if (!(mFlags & 0x8)) {
        mDataEfb.vp.x2 = mSize.x * tvRatio.ratios.x;
        mDataEfb.vp.y2 = mSize.y * tvRatio.ratios.y;
    }

    if (mFlags & 0x4) {
        mDataEfb.vp.x2 -= static_cast<s32>(mDataEfb.vp.x2) & 0x3;
        mDataEfb.vp.y2 -= static_cast<s32>(mDataEfb.vp.y2) & 0x3;
    }

    mDataEfb.vp.x2 = (mDataEfb.vp.x2 >= 0.0f) ? mDataEfb.vp.x2 : 0.0f;
    mDataEfb.vp.y2 = (mDataEfb.vp.y2 >= 0.0f) ? mDataEfb.vp.y2 : 0.0f;

    mDataEfb.vp.x2 = static_cast<s32>(mDataEfb.vp.x2);
    mDataEfb.vp.y2 = static_cast<s32>(mDataEfb.vp.y2);
}

const Screen::DataEfb &Screen::GetDataEfb() const {
    if (IsChangeEfb()) {
        GetPosSizeInEfb();
        mDataEfb.vp.z1 = 0.0f;
        mDataEfb.vp.z2 = 1.0f;
        SetDirty(false);
    }

    return mDataEfb;
}

bool Screen::IsChangeEfb() const {
    if (mFlags & FLAG_DIRTY) {
        return true;
    }

    if (mParent != nullptr) {
        return mParent->IsChangeEfb();
    }

    return false;
}

void Screen::CalcMatrixForDrawQuad(math::MTX34 *mtx, f32 x, f32 y, f32 sx, f32 sy) const {
    PSMTXScale(*mtx, sx, sy, 1.0f);

    mtx->m[0][3] = x;
    mtx->m[1][3] = mCanvasMode == CANVASMODE_0 ? y - sy : y;
    mtx->m[2][3] = 0.0f;
}

void Screen::FillBufferGX(u32 flags, GXColor color, u32 r6) const {
    if (flags != 0) {
        math::MTX34 drawMtx;
        Screen clone(mPosition.x, mPosition.y, mSize.x, mSize.y, mParent, CANVASMODE_1);

        if ((mFlags & 0x8) != 0) {
            clone.SetUnkFlag8();
        }

        clone.SetNearZ(0.0f);
        clone.SetFarZ(1.0f);
        clone.SetFlag(0x40);
        clone.SetProjectionType(PROJ_ORTHO);

        clone.SetProjectionGX();
        clone.CalcMatrixForDrawQuad(&drawMtx, 0.0f, 0.0f, mSize.x, mSize.y);
        // Ugh
        DrawGX::ClearEfb(drawMtx, (flags & 1) != 0, (flags & 2) != 0, (flags & 4) != 0, color, true);
    }
}

void Screen::GetGlobalPos(f32 *ox, f32 *oy) const {
    f32 px, py;
    const Screen *parent = GetParent();

    if (parent != nullptr) {
        parent->GetGlobalPos(&px, &py);
        parent->ConvertToCanvasLU(mPosition.x, mPosition.y, ox, oy);

        *ox += px;
        *oy += py;
    } else {

        *ox = mPosition.x;
        *oy = mPosition.y;
    }
}

void Screen::SetTVMode(TVMode tvMode) {
    sTVMode = tvMode;
    if (spRoot != nullptr) {
        spRoot->SetSizeX(GetSizeXMax());
        spRoot->SetSizeY(GetSizeYMax());
    }

    if (sChangeTVModeFunc != nullptr) {
        sChangeTVModeFunc(spChangeTVModeFuncInfo);
    }
}

void Screen::SetTVModeDefault() {
    SetTVMode(SCGetAspectRatio() == SC_ASPECT_STD ? TV_MODE_4_3 : TV_MODE_16_9);
}

} // namespace EGG
