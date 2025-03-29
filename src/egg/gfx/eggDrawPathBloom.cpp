#include "egg/gfx/eggDrawPathBloom.h"
#include "egg/gfx/eggDrawGX.h"

namespace EGG {

DrawPathBloom::DrawPathBloom(): field_0x01C(1), field_0x01D(8), field_0x37C(0) {
    Data nullDat = {0};
    field_0x380 = nullDat;

    mSimple.setField0x38(0);
    field_0x380.field_0x00 = 0.5f;
    field_0x380.field_0x04 = DrawGX::WHITE;
    field_0x380.field_0x08 = DrawGX::WHITE;
    field_0x380.field_0x0C = 0x21;

    field_0x380.field_0x10[0].field_0x00 = 1.0f;
    field_0x380.field_0x10[0].field_0x04 = 0.3f;
    field_0x380.field_0x10[0].field_0x08 = 0x15;

    field_0x380.field_0x10[1].field_0x00 = 2.0f;
    field_0x380.field_0x10[1].field_0x04 = 0.08f;
    field_0x380.field_0x10[1].field_0x08 = 0x15;

    field_0x380.field_0x10[2].field_0x00 = 4.0f;
    field_0x380.field_0x10[2].field_0x04 = 0.04f;
    field_0x380.field_0x10[2].field_0x08 = 0x15;
    
    field_0x380.field_0x51 = 2;
    field_0x380.field_0x50 = 1;

    field_0x380.field_0x70 = 0.0f;
    field_0x380.field_0x6C = 0.0f;

    field_0x380.field_0x52 = 0x15;
}

DrawPathBloom::~DrawPathBloom() {}

void DrawPathBloom::internalResetForDraw() {}

void DrawPathBloom::internalDraw(u16 idx) {}

void DrawPathBloom::SetBinaryInner(const Bin &bin) {}

void DrawPathBloom::GetBinaryInner(Bin *pBin) const {}

void DrawPathBloom::SetBinaryInner(const Bin &, const Bin &, f32) {}

} // namespace EGG
