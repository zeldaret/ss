#include "d/d_gfx.h"

#include "common.h"
#include "egg/gfx/eggScreen.h"

s32 dGfx_c::g_CurrentScreenWidth_s32;
s32 dGfx_c::g_CurrentScreenHeight_s32;
s32 dGfx_c::g_CurrentScreenWidthLimit_s32;
s32 dGfx_c::g_CurrentScreenHeightLimit_s32;
s32 dGfx_c::g_CurrentScreenLeft_s32;
s32 dGfx_c::g_CurrentScreenRight_s32;
s32 dGfx_c::g_CurrentScreenTop_s32;
s32 dGfx_c::g_CurrentScreenBottom_s32;

s32 dGfx_c::g_CurrentEFBHeight_s32;
s32 dGfx_c::g_CurrentEFBHeightLimit_s32;
s32 dGfx_c::g_CurrentLetterBoxAmount_s32;

f32 dGfx_c::g_CurrentScreenWidth_f32;
f32 dGfx_c::g_CurrentScreenHeight_f32;
f32 dGfx_c::g_CurrentScreenWidthLimit_f32;
f32 dGfx_c::g_CurrentScreenHeightLimit_f32;
f32 dGfx_c::g_CurrentScreenLeft_f32;
f32 dGfx_c::g_CurrentScreenRight_f32;
f32 dGfx_c::g_CurrentScreenTop_f32;
f32 dGfx_c::g_CurrentScreenBottom_f32;

f32 dGfx_c::g_CurrentScreenAspect;

f32 dGfx_c::g_CurrentEFBHeight_f32;
f32 dGfx_c::g_CurrentEFBHeightLimit_f32;
f32 dGfx_c::g_CurrentEFBHeightDifference_f32;
f32 dGfx_c::g_CurrentHeightScaled_f32;
f32 dGfx_c::g_CurrentEFBHeightDifference2_f32;

s32 dGfx_c::g_Width4x3_s32;
s32 dGfx_c::g_Width4x3Limit_s32;
s32 dGfx_c::g_Width16x9_s32;
s32 dGfx_c::g_Width4x3Left_s32;
s32 dGfx_c::g_Width4x3Right_s32;

f32 dGfx_c::g_Width4x3_f32;
f32 dGfx_c::g_Width4x3Limit_f32;
f32 dGfx_c::g_Width16x9_f32;
f32 dGfx_c::g_Width4x3Left_f32;
f32 dGfx_c::g_Width4x3Right_f32;

f32 dGfx_c::g_16x9to4x3WidthScale;
f32 dGfx_c::g_CurrentScreenTo4x3WidthScale = 1.0f;

void dGfx_c::initGfxConstants() {
    // TODO - awful function
}

bool dGfx_c::isTvMode4To3() {
    return EGG::Screen::GetTVMode() == EGG::Screen::TV_MODE_4_3;
}

bool dGfx_c::isTvModeWidescreen() {
    return EGG::Screen::GetTVMode() != EGG::Screen::TV_MODE_4_3;
}
