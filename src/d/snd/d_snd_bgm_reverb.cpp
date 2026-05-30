#include "common.h"
#include "nw4r/snd/snd_FxReverbStdDpl2.h"

extern "C" void fn_8037BD30(nw4r::snd::detail::FxReverbStdParam &param) {
    param.field_0x00 = 0.1f;
    param.field_0x04 = 2.0f;
    param.field_0x08 = 0.2f;
    param.field_0x0C = 0.7f;
    param.field_0x10 = 1.0f;
    param.field_0x14 = 4;
    param.field_0x18 = 0.15f;
    param.field_0x1C = 4;
    param.field_0x20 = 0.3f;
    param.field_0x24 = 1.0f;
}

extern "C" void fn_8037BD80(nw4r::snd::detail::FxReverbStdParam &param) {
    fn_8037BD30(param);
}
