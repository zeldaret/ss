#include "common.h"
#include "nw4r/snd/snd_FxReverbStdDpl2.h"

extern "C" void fn_80366430(nw4r::snd::detail::FxReverbStdParam &param) {
    param.field_0x00 = 0.02f;
    param.field_0x04 = 3.0f;
    param.field_0x08 = 0.6f;
    param.field_0x0C = 0.4f;
    param.field_0x10 = 1.0f;
    param.field_0x20 = 0.0f;
    param.field_0x24 = 1.0f;
    param.field_0x14 = 7;
    param.field_0x1C = 4;
    param.field_0x18 = 0.02f;
}

extern "C" void fn_80366480(nw4r::snd::detail::FxReverbStdParam &param) {
    param.field_0x00 = 0.1f;
    param.field_0x04 = 3.0f;
    param.field_0x08 = 0.6f;
    param.field_0x0C = 0.4f;
    param.field_0x10 = 1.0f;
    param.field_0x20 = 0.0f;
    param.field_0x24 = 1.0f;
    param.field_0x14 = 5;
    param.field_0x1C = 5;
    param.field_0x18 = 0.1f;
}

extern "C" void fn_803664D0(nw4r::snd::detail::FxReverbStdParam &param) {
    param.field_0x00 = 0.1f;
    param.field_0x04 = 5.0f;
    param.field_0x08 = 0.6f;
    param.field_0x0C = 1.0f;
    param.field_0x10 = 1.0f;
    param.field_0x20 = 1.0f;
    param.field_0x24 = 1.0f;
    param.field_0x14 = 0;
    param.field_0x1C = 5;
    param.field_0x18 = 0.1f;
}

extern "C" void fn_80366520(nw4r::snd::detail::FxReverbStdParam &param) {
    param.field_0x00 = 0.05f;
    param.field_0x04 = 0.5f;
    param.field_0x08 = 0.5f;
    param.field_0x0C = 0.5f;
    param.field_0x10 = 1.0f;
    param.field_0x20 = 0.5f;
    param.field_0x24 = 1.0f;
    param.field_0x14 = 1;
    param.field_0x1C = 2;
    param.field_0x18 = 0.05f;
}

extern "C" void fn_80366560(nw4r::snd::detail::FxReverbStdParam &param) {
    param.field_0x00 = 0.02f;
    param.field_0x04 = 3.0f;
    param.field_0x08 = 0.6f;
    param.field_0x0C = 0.4f;
    param.field_0x10 = 1.0f;
    param.field_0x20 = 0.0f;
    param.field_0x24 = 1.0f;
    param.field_0x14 = 0;
    param.field_0x1C = 4;
    param.field_0x18 = 0.02f;
}

extern "C" void fn_803665B0(nw4r::snd::detail::FxReverbStdParam &param, int preset) {
    if (preset == 1) goto case1;
    if (preset == 2) goto case2;
    if (preset == 3) goto case3;
    if (preset == 4) goto case4;
    fn_80366430(param);
    return;
case1:
    fn_80366480(param);
    return;
case2:
    fn_803664D0(param);
    return;
case3:
    fn_80366520(param);
    return;
case4:
    fn_80366560(param);
}
