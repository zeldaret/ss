#include <egg/gfx/eggScreen.h>
#include <toBeSorted/lyt/d2d.h>

namespace d2d {

Multi_c::Multi_c() : Base_c(0x80), mLayout(), mDrawInfo(), mpResAcc(nullptr), field_0x88(0) {
    mDrawInfo.SetLocationAdjustScale(nw4r::math::VEC2((float)EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_1) /
                    (float)EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_2),
            1.0f));
    mDrawInfo.SetLocationAdjust(true);
}

LytBase_c::LytBase_c(): mpMsbtInfo(nullptr) {}

extern "C" void fn_80324170(void *);
LytBase_c::~LytBase_c() {
    if (mpMsbtInfo != nullptr) {
        fn_80324170(mpMsbtInfo);
    }
}

void LytBase_c::draw() {
    mLayout.Draw(mDrawInfo);
}

} // namespace d2d
