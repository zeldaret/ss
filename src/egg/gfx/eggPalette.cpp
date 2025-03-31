#include "egg/gfx/eggPalette.h"

namespace EGG {

void Palette::storeTLUT(_GXTlut param_0, _GXTlutFmt param_1, JUTTransparency param_2, u16 param_3, void *param_4) {
    mTlutName = param_0;
    mFormat = param_1;
    mTransparency = param_2;
    mNumColors = param_3;
    mColorTable = (ResTLUT *)param_4;
    GXInitTlutObj(&mTlutObj, (void *)mColorTable, (GXTlutFmt)mFormat, mNumColors);
}

bool Palette::load() {
    bool check = mNumColors != 0;
    if (check) {
        GXLoadTlut(&mTlutObj, mTlutName);
    }

    return check;
}

} // namespace EGG
