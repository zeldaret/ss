#ifndef D_GFX_H
#define D_GFX_H

#include "common.h"

// Made up name
class dGfx_c {
public:
    typedef void (*DrawCallback)(void);

    void setDrawCallback(DrawCallback cb) {
        mDrawCallback = cb;
    }

    void setField0x09(u8 val) {
        field_0x09 = val;
    }

    static dGfx_c *GetInstance() {
        return sInstance;
    }

public:
    // clang-format off
    static s32 getCurrentScreenWidth() { return g_CurrentScreenWidth_s32; }
    static s32 getCurrentScreenHeight() { return g_CurrentScreenHeight_s32; }
    static s32 getCurrentScreenWidthLimit() { return g_CurrentScreenWidthLimit_s32; }
    static s32 getCurrentScreenHeightLimit() { return g_CurrentScreenHeightLimit_s32; }
    static s32 getCurrentScreenLeft() { return g_CurrentScreenLeft_s32; }
    static s32 getCurrentScreenRight() { return g_CurrentScreenRight_s32; }
    static s32 getCurrentScreenTop() { return g_CurrentScreenTop_s32; }
    static s32 getCurrentScreenBottom() { return g_CurrentScreenBottom_s32; }

    static s32 getEFBHeight() { return g_CurrentEFBHeight_s32; }
    static s32 getEFBHeightLimit() { return g_CurrentEFBHeightLimit_s32; }
    static s32 getLetterboxAmount() { return g_CurrentLetterBoxAmount_s32; }
    
    static f32 getCurrentScreenWidthF() { return g_CurrentScreenWidth_f32; }
    static f32 getCurrentScreenHeightF() { return g_CurrentScreenHeight_f32; }
    static f32 getCurrentScreenWidthLimitF() { return g_CurrentScreenWidthLimit_f32; }
    static f32 getCurrentScreenHeightLimitF() { return g_CurrentScreenHeightLimit_f32; }
    static f32 getCurrentScreenLeftF() { return g_CurrentScreenLeft_f32; }
    static f32 getCurrentScreenRightF() { return g_CurrentScreenRight_f32; }
    static f32 getCurrentScreenTopF() { return g_CurrentScreenTop_f32; }
    static f32 getCurrentScreenBottomF() { return g_CurrentScreenBottom_f32; }
    
    static f32 getScreenAspectF() { return g_CurrentScreenAspect; }

    static f32 getEFBHeightF() { return g_CurrentEFBHeight_f32; }
    static f32 getEFBHeightLimitF() { return g_CurrentEFBHeightLimit_f32; }
    static f32 getLetterboxAmountF() { return g_CurrentEFBHeightDifference_f32; }
    static f32 getHeightScaledF() { return g_CurrentHeightScaled_f32; }
    static f32 getEFBHeightDifferenceF() { return g_CurrentEFBHeightDifference2_f32; }

    static s32 getWidth4x3() { return g_Width4x3_s32; }
    static s32 getWidth4x3Limit() { return g_Width4x3Limit_s32; }
    static s32 getWidth16x9() { return g_Width16x9_s32; }
    static s32 getWidth4x3Left() { return g_Width4x3Left_s32; }
    static s32 getWidth4x3Right() { return g_Width4x3Right_s32; }

    static f32 getWidth4x3F() { return g_Width4x3_f32; }
    static f32 getWidth4x3LimitF() { return g_Width4x3Limit_f32; }
    static f32 getWidth16x9F() { return g_Width16x9_f32; }
    static f32 getWidth4x3LeftF() { return g_Width4x3Left_f32; }
    static f32 getWidth4x3RightF() { return g_Width4x3Right_f32; }

    static f32 get16x9to4x3WidthScaleF() { return g_16x9to4x3WidthScale; }
    // clang-format on

public:
    static dGfx_c *sInstance;

    static s32 g_CurrentScreenWidth_s32;
    static s32 g_CurrentScreenHeight_s32;
    static s32 g_CurrentScreenWidthLimit_s32;
    static s32 g_CurrentScreenHeightLimit_s32;
    static s32 g_CurrentScreenLeft_s32;
    static s32 g_CurrentScreenRight_s32;
    static s32 g_CurrentScreenTop_s32;
    static s32 g_CurrentScreenBottom_s32;

    static s32 g_CurrentEFBHeight_s32;
    static s32 g_CurrentEFBHeightLimit_s32;
    static s32 g_CurrentLetterBoxAmount_s32;

    static f32 g_CurrentScreenWidth_f32;
    static f32 g_CurrentScreenHeight_f32;
    static f32 g_CurrentScreenWidthLimit_f32;
    static f32 g_CurrentScreenHeightLimit_f32;
    static f32 g_CurrentScreenLeft_f32;
    static f32 g_CurrentScreenRight_f32;
    static f32 g_CurrentScreenTop_f32;
    static f32 g_CurrentScreenBottom_f32;

    static f32 g_CurrentScreenAspect;

    static f32 g_CurrentEFBHeight_f32;
    static f32 g_CurrentEFBHeightLimit_f32;
    static f32 g_CurrentEFBHeightDifference_f32;
    static f32 g_CurrentHeightScaled_f32;
    static f32 g_CurrentEFBHeightDifference2_f32;

    static s32 g_Width4x3_s32;
    static s32 g_Width4x3Limit_s32;
    static s32 g_Width16x9_s32;
    static s32 g_Width4x3Left_s32;
    static s32 g_Width4x3Right_s32;

    static f32 g_Width4x3_f32;
    static f32 g_Width4x3Limit_f32;
    static f32 g_Width16x9_f32;
    static f32 g_Width4x3Left_f32;
    static f32 g_Width4x3Right_f32;

    static f32 g_16x9to4x3WidthScale;

    /* 0x00 */ u8 _0x00[0x04 - 0x00];
    /* 0x04 */ DrawCallback mDrawCallback;
    /* 0x08 */ u8 field_0x08;
    /* 0x09 */ u8 field_0x09;
};

#endif
