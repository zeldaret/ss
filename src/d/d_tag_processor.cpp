#include "d/d_tag_processor.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/d_font_manager.h"
#include "d/d_message.h"
#include "d/d_textunk.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d_lyt_util_items.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_color.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/ut/ut_CharWriter.h"
#include "nw4r/ut/ut_Color.h"
#include "nw4r/ut/ut_Font.h"
#include "nw4r/ut/ut_Rect.h"
#include "nw4r/ut/ut_TagProcessorBase.h"
#include "nw4r/ut/ut_TextWriterBase.h"
#include "sized_string.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/music_mgrs.h"

#include <libc.h>

#define READ_U8(src, off) (*((const u8 *)((const u8 *)src + off)))
#define READ_S8(src, off) (*((const s8 *)((const u8 *)src + off)))
#define READ_U16(src, off) (*((const u16 *)((const u8 *)src + off)))
#define READ_S16(src, off) (*((const s16 *)((const u8 *)src + off)))
#define READ_U32(src, off) (*((const u32 *)((const u8 *)src + off)))

// picture_00.brfnt
enum PictureFontIdx {
    FONT_BTN_A = 0,
    FONT_BTN_B = 1,
    FONT_BTN_MINUS = 2,
    FONT_BTN_PLUS = 3,
    FONT_BTN_1 = 4,
    FONT_BTN_2 = 5,
    FONT_BTN_C = 6,
    FONT_BTN_Z = 7,
    FONT_BTN_NUN_STK = 8,
    FONT_BTN_NUN_STK_UP = 9,
    FONT_BTN_NUN_STK_DOWN = 10,
    FONT_BTN_NUN_STK_LEFT = 11,
    FONT_BTN_NUN_STK_RIGHT = 12,
    FONT_BTN_NUN_STK_WIGGLE_VERTICAL = 13,
    FONT_BTN_NUN_STK_WIGGLE_HORIZONTAL = 14,
    FONT_BTN_CROSS_FULL = 15,
    FONT_BTN_CROSS_UP = 16,
    FONT_BTN_CROSS_DOWN = 17,
    FONT_BTN_CORSS_LEFT = 18,
    FONT_BTN_CROSS_RIGHT = 19,
    FONT_ARROW_UP = 20,
    FONT_ARROW_DOWN = 21,
    FONT_ARROW_LEFT = 22,
    FONT_ARROW_RIGHT = 23,
    FONT_CURSOR = 24,
    FONT_X = 25,
    FONT_HOLE = 26,
};

nw4r::ut::Color FontColors1[] = {
    nw4r::ut::Color(0xff, 0x4b, 0x32, 0xff), // #ff4b32ff
    nw4r::ut::Color(0xff, 0x4b, 0x32, 0xff), // #ff4b32ff
    nw4r::ut::Color(0xfa, 0xaf, 0x05, 0xff), // #faaf05ff
    nw4r::ut::Color(0xdc, 0xf5, 0xfa, 0xff), // #dcf5faff
    nw4r::ut::Color(0x91, 0xff, 0x6e, 0xff), // #91ff6eff
    nw4r::ut::Color(0xff, 0xbe, 0x1e, 0xff), // #ffbe1eff
    nw4r::ut::Color(0xeb, 0xc8, 0xff, 0xff), // #ebc8ffff
    nw4r::ut::Color(0x14, 0xd7, 0x46, 0xff), // #14d746ff
    nw4r::ut::Color(0x19, 0xa5, 0xeb, 0xff), // #19a5ebff
    nw4r::ut::Color(0xf0, 0x3c, 0x1e, 0xff), // #f03c1eff
    nw4r::ut::Color(0xc8, 0xc8, 0xc8, 0xff), // #c8c8c8ff
    nw4r::ut::Color(0xfa, 0xaf, 0x05, 0xff), // #faaf05ff
    nw4r::ut::Color(0x32, 0x23, 0x46, 0xff), // #322346ff
    nw4r::ut::Color(),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff), // #ede2d0ff
    nw4r::ut::Color(0xf6, 0xf6, 0xde, 0xff), // #f6f6deff
    nw4r::ut::Color(0xe6, 0xe6, 0xa0, 0xff), // #e6e6a0ff
    nw4r::ut::Color(0xe8, 0xad, 0x55, 0xff), // #e8ad55ff
    nw4r::ut::Color(0xff, 0xfa, 0xe6, 0xff), // #fffae6ff
    nw4r::ut::Color(0xf5, 0xf2, 0xeb, 0xff), // #f5f2ebff
    nw4r::ut::Color(0x64, 0x5f, 0x69, 0xff), // #645f69ff
    nw4r::ut::Color(0x5e, 0x51, 0x46, 0xff), // #5e5146ff
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff), // #ede2d0ff
    nw4r::ut::Color(),
    nw4r::ut::Color(0xfa, 0xf6, 0xe7, 0xff), // #faf6e7ff
    nw4r::ut::Color(0xff, 0xff, 0xff, 0xff), // #ffffffff
    nw4r::ut::Color(0x4c, 0x32, 0x0f, 0xff), // #4c320fff
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff), // #ede2d0ff
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff), // #ede2d0ff
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff), // #ede2d0ff
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff), // #ede2d0ff
    nw4r::ut::Color(),
    nw4r::ut::Color(0xf6, 0xf2, 0xea, 0xff), // #f6f2eaff
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff), // #ede2d0ff
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff), // #ede2d0ff
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xff, 0xff, 0xff, 0xff), // #ffffffff
    nw4r::ut::Color(),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff), // #ede2d0ff
    nw4r::ut::Color(0x20, 0x20, 0x20, 0xff), // #202020ff
    nw4r::ut::Color(0x74, 0x6d, 0x5c, 0xff), // #746d5cff
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
};

nw4r::ut::Color FontColors2[] = {
    nw4r::ut::Color(0xff, 0x4b, 0x32, 0xff), // #ff4b32ff
    nw4r::ut::Color(0xcd, 0x4b, 0x32, 0xff), // #cd4b32ff
    nw4r::ut::Color(0xff, 0xe6, 0x8c, 0xff), // #ffe68cff
    nw4r::ut::Color(0x7d, 0xc3, 0xff, 0xff), // #7dc3ffff
    nw4r::ut::Color(0x64, 0xc8, 0x41, 0xff), // #64c841ff
    nw4r::ut::Color(0xff, 0xbe, 0x1e, 0xff), // #ffbe1eff
    nw4r::ut::Color(0x91, 0x5f, 0xff, 0xff), // #915fffff
    nw4r::ut::Color(0xaf, 0xe6, 0x2d, 0xff), // #afe62dff
    nw4r::ut::Color(0xb4, 0xe6, 0xff, 0xff), // #b4e6ffff
    nw4r::ut::Color(0xff, 0x91, 0x78, 0xff), // #ff9178ff
    nw4r::ut::Color(0xe6, 0xe6, 0xc8, 0xff), // #e6e6c8ff
    nw4r::ut::Color(0xff, 0xe6, 0x8c, 0xff), // #ffe68cff
    nw4r::ut::Color(0x50, 0x4b, 0x5f, 0xff), // #504b5fff
    nw4r::ut::Color(),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff), // #caaf81ff
    nw4r::ut::Color(0xe7, 0xd7, 0xed, 0xff), // #e7d7edff
    nw4r::ut::Color(0xa5, 0x96, 0x6e, 0xff), // #a5966eff
    nw4r::ut::Color(0xb5, 0x52, 0x2e, 0xff), // #b5522eff
    nw4r::ut::Color(0xff, 0xfa, 0xe6, 0xff), // #fffae6ff
    nw4r::ut::Color(0xe1, 0xc8, 0x9b, 0xff), // #e1c89bff
    nw4r::ut::Color(0x55, 0x50, 0x5a, 0xff), // #55505aff
    nw4r::ut::Color(0x35, 0x2d, 0x27, 0xff), // #352d27ff
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff), // #caaf81ff
    nw4r::ut::Color(),
    nw4r::ut::Color(0xe3, 0xcc, 0x8f, 0xff), // #e3cc8fff
    nw4r::ut::Color(0xff, 0xff, 0xff, 0xff), // #ffffffff
    nw4r::ut::Color(0x3a, 0x25, 0x09, 0xff), // #3a2509ff
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff), // #caaf81ff
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff), // #caaf81ff
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff), // #caaf81ff
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff), // #caaf81ff
    nw4r::ut::Color(),
    nw4r::ut::Color(0xe1, 0xc7, 0x9b, 0xff), // #e1c79bff
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff), // #caaf81ff
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff), // #caaf81ff
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xff, 0xff, 0xff, 0xff), // #ffffffff
    nw4r::ut::Color(),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff), // #caaf81ff
    nw4r::ut::Color(0x20, 0x20, 0x20, 0xff), // #202020ff
    nw4r::ut::Color(0x4b, 0x47, 0x3f, 0xff), // #4b473fff
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
};

static const wchar_t replacementSeq0xDF[] = {0x53, 0x53};

struct ReplacementSequence {
    /* 0x00 */ wchar_t origChar;
    /* 0x02 */ u8 replacementSequenceLen;
    /* 0x04 */ const wchar_t *replacementSequence;
};

static const ReplacementSequence sReplacementSequences[] = {
    {0xDF, 2, replacementSeq0xDF},
    {   0, 0,            nullptr}
};

static const wchar_t sUpperCaseChars[] = {
    L'A',   L'B',   L'C',   L'D',   L'E',   L'F',   L'G',   L'H',   L'I',   L'J',   L'K',   L'L',   L'M',   L'N',
    L'O',   L'P',   L'Q',   L'R',   L'S',   L'T',   L'U',   L'V',   L'W',   L'X',   L'Y',   L'Z',   0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC,
    0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x0000, 0x00D8, 0x00D9, 0x00DA,
    0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x0178, 0x0000, 0x0100, 0x0000, 0x0102, 0x0000, 0x0104, 0x0000, 0x0106, 0x0000,
    0x0108, 0x0000, 0x010A, 0x0000, 0x010C, 0x0000, 0x010E, 0x0000, 0x0110, 0x0000, 0x0112, 0x0000, 0x0114, 0x0000,
    0x0116, 0x0000, 0x0118, 0x0000, 0x011A, 0x0000, 0x011C, 0x0000, 0x011E, 0x0000, 0x0120, 0x0000, 0x0122, 0x0000,
    0x0124, 0x0000, 0x0126, 0x0000, 0x0128, 0x0000, 0x012A, 0x0000, 0x012C, 0x0000, 0x012E, 0x0000, 0x0049, 0x0000,
    0x0132, 0x0000, 0x0134, 0x0000, 0x0136, 0x0000, 0x0000, 0x0139, 0x0000, 0x013B, 0x0000, 0x013D, 0x0000, 0x013F,
    0x0000, 0x0141, 0x0000, 0x0143, 0x0000, 0x0145, 0x0000, 0x0147, 0x0000, 0x0000, 0x014A, 0x0000, 0x014C, 0x0000,
    0x014E, 0x0000, 0x0150, 0x0000, 0x0152, 0x0000, 0x0154, 0x0000, 0x0156, 0x0000, 0x0158, 0x0000, 0x015A, 0x0000,
    0x015C, 0x0000, 0x015E, 0x0000, 0x0160, 0x0000, 0x0162, 0x0000, 0x0164, 0x0000, 0x0166, 0x0000, 0x0168, 0x0000,
    0x016A, 0x0000, 0x016C, 0x0000, 0x016E, 0x0000, 0x0170, 0x0000, 0x0172, 0x0000, 0x0174, 0x0000, 0x0176, 0x0000,
    0x0000, 0x0179, 0x0000, 0x017B, 0x0000, 0x017D, 0x0053, 0x0000,
};

s32 toUpperCase(wchar_t orig, wchar_t *out, u32 maxLen) {
    if (maxLen < 3) {
        return -1;
    }

    if (orig >= L'a' && orig <= 383) {
        const ReplacementSequence *seq = sReplacementSequences;
        s32 len = seq->replacementSequenceLen;
        while (true) {
            if (seq->replacementSequenceLen == 0) {
                break;
            }
            if (seq->origChar == orig) {
                s32 len = seq->replacementSequenceLen;
                const wchar_t *r = seq->replacementSequence;
                s32 i = seq->replacementSequenceLen;
                do {
                    *out = *r;
                    out++;
                    r++;
                } while (--i);
                *out = L'\0';
                return len;
            }
            seq++;
        }
        u32 ret = 1;
        wchar_t replacement = sUpperCaseChars[orig - L'a'];
        if (replacement == L'\0') {
            replacement = orig;
            ret = 0;
        }
        out[0] = replacement;
        out[1] = L'\0';
        return ret;
    }
    out[0] = orig;
    out[1] = L'\0';
    return 0;
}

const u16 sMsgWindowFlags[2050] = {
    0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800,
};

dTagProcessor_c::dTagProcessor_c() {
    field_0x82C = -1;
    field_0x828 = -1;
    field_0x90E = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 256; j++) {
            field_0x008[i][j] = 0;
        }
        field_0x808[i] = 0;
        field_0x90F[i] = 0;
    }

    field_0x904 = 0.0f;
    field_0x908 = 0.0f;
    resetSomething();
    field_0x830 = -1;
    mPauseFramesLeft = 0;
    field_0x838 = 0;
    field_0x83C = 0;
    field_0x840 = 0;
    field_0x844 = 0;
    field_0x848 = 0;
    field_0x84C = 0;
    field_0x850 = 0;
    field_0x87C = 0;
    field_0x854 = 0;
    field_0x858 = 0;
    field_0x85C = 0;
    field_0x860 = 0;
    field_0x880 = 0;
    field_0x864 = 0;
    field_0x868 = 0;
    field_0x884 = 0;
    field_0x86C = 0;
    field_0x870 = 0;
    field_0x894 = 0;
    field_0x874 = 0;
    field_0x878 = 0xffffffff;
    field_0x898 = 0;
    field_0x888 = 0;
    field_0x88C = 0;
    field_0x824 = 0;
    field_0x890 = 0;
    field_0x8FC = -1;
    field_0x900 = -1;
    resetLineData();
    field_0x81C = 0;
    field_0x820 = 0;
    setNumericArg0(0);
    mButtonPressAnimTimer = 0;
    mButtonWiggleAnimTimer = 0;
    mFirstNumericArg = 0;

    mNumericArgsCopy[0] = 0;
    mNumericArgsCopy[1] = 0;
    mNumericArgsCopy[2] = 0;
    mNumericArgsCopy[3] = 0;
    mNumericArgsCopy[4] = 0;
    mNumericArgsCopy[5] = 0;
    mNumericArgsCopy[6] = 0;
    mNumericArgsCopy[7] = 0;
    mNumericArgsCopy[8] = 0;
    mNumericArgsCopy[9] = 0;
    mNumericArgsCopy[10] = 0;

    field_0xEE5 = 0;
    field_0xEE6 = 0;
    field_0xEE7 = 0;
    field_0xEE8 = 0;
    field_0xEE9 = 0;
    field_0xEEA = 0;
    field_0xEEB = 0;
    field_0xEEC = 0;
    field_0xEED = 0;
    field_0xEEE = 0;
    field_0xEEF = 0;
    field_0xEE0 = 0;
    field_0xEE1 = 0;
    field_0xEE2 = 0;
    field_0xEE3 = 0;
    field_0xEE4 = 0;
    mpTextBox = nullptr;
    mMsgWindowSubtype = dLytMsgWindow_c::MSG_WINDOW_36;
    field_0x90D = 4;
    field_0xEF0 = 0;
    field_0xEF1 = 0;
}

dTagProcessor_c::~dTagProcessor_c() {}

struct StackThing {
    wchar_t us[4];
};

void dTagProcessor_c::format(
    dTextBox_c *textBox, const wchar_t *src, wchar_t *dest, u32 destLen, u32 *pOutLen, void *unk, ...
) {
    va_list l;
    va_start(l, str);
    formatV(textBox, src, dest, destLen, pOutLen, unk, l);
    va_end(l);
}

void dTagProcessor_c::formatV(
    dTextBox_c *textBox, const wchar_t *src, wchar_t *dest, u32 destLen, u32 *pOutLen, void *unk, va_list list
) {
    s32 state3 = 0;
    s32 state4 = 0;

    s32 state1 = -1;
    s32 state2 = -1;

    f32 currScale, backupScale;
    backupScale = currScale = fn_800B8040(0, mMsgWindowSubtype);

    if (textBox != nullptr) {
        currScale *= textBox->getMyScale();
        resetLineData();
        textBox->set0x1F8(0);
    }

    s32 local_b4 = 0;

    StackThing x = {0x000E, 0x0F0F, 0x0F0F, 0x0002};
    wchar_t *writePtr = dest;
    if (textBox != nullptr) {
        writePtr += 5;
        dest[0] = x.us[0];
        dest[1] = x.us[1];
        dest[2] = x.us[2];
        dest[3] = x.us[3];
        dest[4] = mLineData.mNumLines;
    }

    StackThing yTmp;
    StackThing y = {0x000E, 0x0F0F, 0x0F0E, 0x0001};

    do {
        wchar_t c = *src;
        if (c == nullptr) {
            if (textBox != nullptr) {
                mLineData.mNumLines++;
            }
            *writePtr = '\0';
            goto end;
        }

        if (c == 0xE) {
            u8 cmdLen = 0;
            s32 cmd = 0;
            wchar_t *endPtr = nullptr;
            getTextCommand(c, src + 1, &cmdLen, &cmd, &endPtr);
            bool bVar3 = false;
            switch (cmd) {
                case 0x0F0F0F0F:
                    if (state4 != 0 && field_0x90E != 0) {
                        const wchar_t *t = src;
                        u32 len = (cmdLen / 2) + 1;
                        for (int i = 0; i < len; i++) {
                            getTmpBuffer()[local_b4] = *(t++);
                            onWriteTmpBuffer();
                            local_b4++;
                        }
                    } else {
                        const wchar_t *t = src;
                        u32 len = (cmdLen / 2) + 1;
                        for (int i = 0; i < len; i++) {
                            *(writePtr++) = *(t++);
                        }
                    }
                    break;
                case 0x10000: {
                    u8 a = READ_U8(endPtr, 0);
                    u8 b = READ_U8(endPtr, 1);
                    switch (a) {
                        case 0: state2 = 0; break;
                        case 1: state1 = 0; break;
                    }
                    field_0x90F[0] = b;
                    yTmp = y;
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 256; j++) {
                            if (j < 4) {
                                field_0x008[i][j] = yTmp.us[j];
                            } else {
                                field_0x008[i][j] = 0;
                            }
                        }
                        field_0x808[i] = 4;
                    }
                    state3 = 1;
                    bVar3 = true;
                } break;
                case 0x10001: {
                    u8 a = READ_U8(endPtr, 0);
                    u8 b = READ_U8(endPtr, 1);
                    switch (a) {
                        case 0: state2 = 1; break;
                        case 1: state1 = 1; break;
                    }
                    field_0x90F[1] = b;
                    state3 = 2;
                    bVar3 = true;
                } break;
                case 0x10002: {
                    u8 a = READ_U8(endPtr, 0);
                    u8 b = READ_U8(endPtr, 1);
                    switch (a) {
                        case 0: state2 = 2; break;
                        case 1: state1 = 2; break;
                    }
                    field_0x90F[2] = b;
                    state3 = 3;
                    bVar3 = true;
                } break;
                case 0x10003: {
                    u8 a = READ_U8(endPtr, 0);
                    u8 b = READ_U8(endPtr, 1);
                    switch (a) {
                        case 0: state2 = 3; break;
                        case 1: state1 = 3; break;
                    }
                    field_0x90F[3] = b;
                    state3 = 4;
                    bVar3 = true;
                } break;
                case 0x10008:
                    if (textBox != nullptr) {
                        currScale = fn_800B8040(READ_U8(endPtr, 0), mMsgWindowSubtype);
                        currScale *= textBox->getMyScale();
                    }
                    writePtr = writeTextNormal(src, writePtr, &local_b4, cmdLen, state4);
                    break;
                case 0x30000: {
                    if (textBox != nullptr) {
                        backupScale = currScale;
                        currScale *= UnkTextThing::getField0x768();
                        currScale *= textBox->getMyScale();
                    }
                    writePtr = writeTextNormal(src, writePtr, &local_b4, cmdLen, state4);
                } break;
                case 0x10010: fn_800B5520(endPtr); break;
                case 0x20004:
                    if (textBox != nullptr) {
                        saveIconWidth(textBox, endPtr, currScale);
                    }
                    writePtr = writeTextNormal(src, writePtr, &local_b4, cmdLen, state4);
                    break;
                case 0x20000: writePtr = writeHeroname(writePtr, &local_b4, state4); break;
                case 0x20001: writePtr = writeItem(writePtr, endPtr, &local_b4, state4); break;
                case 0x20002: writePtr = writeStringArg(writePtr, endPtr, &local_b4, state4); break;
                case 0x20003: writePtr = writeNumericArg(writePtr, endPtr, &local_b4, state4); break;

                case 0x30004: writePtr = writeSingularOrPluralWord(writePtr, endPtr, &local_b4, state4); break;
                case 0x30001:
                    field_0xEF1 = 1;
                    writePtr = writeTextNormal(src, writePtr, &local_b4, cmdLen, state4);
                    break;

                default: writePtr = writeTextNormal(src, writePtr, &local_b4, cmdLen, state4); break;
            }

            if (bVar3) {
                field_0x82C = state1;
                state4 = 1;
                field_0x828 = state2;
                field_0x90E = state3;
                local_b4 = field_0x808[state3 - 1];
            }
            src += (cmdLen / 2) + 1;
        } else if (c == 0xF) {
            s32 tmp = 0;
            process0xFCommand(c, src + 1, &tmp);
            if (tmp == 0x30000) {
                currScale = backupScale;
            }
            writePtr[0] = src[0];
            writePtr[1] = src[1];
            writePtr[2] = src[2];
            writePtr += 3;
            src += 3;
        } else if (state4 != 0 && field_0x90E != 0) {
            // Note: Return ignored here
            writeSingleCharacter(c, &getTmpBuffer()[local_b4], &local_b4);
            src++;
            onWriteTmpBuffer();
        } else {
            if (textBox != nullptr) {
                if (c == L'\n') {
                    *writePtr = c;
                    src++;
                    writePtr++;
                    mLineData.mNumLines++;
                    s32 i10 = getMaxNumLines(mMsgWindowSubtype);
                    if (mLineData.mNumLines % i10 == 0) {
                        currScale = fn_800B8040(0, mMsgWindowSubtype);
                        currScale *= textBox->getMyScale();
                    }
                    if (textBox != nullptr) {
                        wchar_t *buf = (wchar_t *)&x;
                        writePtr[0] = buf[0];
                        writePtr[1] = buf[1];
                        writePtr[2] = buf[2];
                        writePtr[3] = buf[3];
                        writePtr[4] = mLineData.mNumLines;
                        writePtr += 5;
                    }
                } else {
                    const nw4r::ut::Font *fnt = textBox->GetFont();
                    mLineData.mLineWidths[mLineData.mNumLines] +=
                        currScale * fnt->GetCharWidth(*src) + textBox->GetCharSpace();
                    writePtr = writeSingleCharacter(*src, writePtr, nullptr);
                    src++;
                }

            } else {
                writePtr = writeSingleCharacter(c, writePtr, nullptr);
                src++;
            }
        }

    } while (destLen > writePtr - dest);

    dest[destLen - 1] = 0;
end:
    if (pOutLen != nullptr) {
        *pOutLen = writePtr - dest;
    }
}

void dTagProcessor_c::computeCharacterPlacement(
    dTextBox_c *textBox, const wchar_t *src_, wchar_t *dest, s32 unkArg, u16 *pOutLen, dLytMsgWindowCharData *charData
) {
    const wchar_t *src = src_;
    wchar_t *writePtr = dest;
    bool b1 = false;
    s32 lineNum = 0;
    bool b2 = false;
    s32 charDataIdx;

    f32 currScale = fn_800B8040(0, mMsgWindowSubtype);
    f32 backupScale = currScale;

    charDataIdx = 0;

    f32 posX = 0.0f;
    f32 posY = 0.0f;

    // Float order
    1.0f;

    if (textBox != nullptr) {
        currScale *= textBox->getMyScale();
    }

beginning:
    bool r22 = !b1;
    while (!b1) {
        wchar_t c = *src;
        if (c == '\0') {
            *writePtr = '\0';
            break;
        } else if (c == 0xE) {
            u8 cmdLen = 0;
            s32 cmd = 0;
            wchar_t *endPtr = nullptr;
            getTextCommand(c, src + 1, &cmdLen, &cmd, &endPtr);
            switch (cmd) {
                case 0x0F0F0F0F: {
                    if (lineNum / getMaxNumLines(mMsgWindowSubtype) == unkArg) {
                        nw4r::lyt::Size fontSize = getTextBox()->GetFontSize();
                        posX = getMarginForCenteredLine(lineNum);
                        if ((mMsgWindowSubtype < dLytMsgWindow_c::MSG_WINDOW_WOOD ||
                             mMsgWindowSubtype >= dLytMsgWindow_c::MSG_WINDOW_LINK + 1) &&
                            mMsgWindowSubtype != dLytMsgWindow_c::MSG_WINDOW_DEMO) {
                            posX = 0.0f;
                        }

                        f32 tmp3;
                        f32 tmp = fn_800B8040(0, mMsgWindowSubtype);

                        if (textBox != nullptr) {
                            tmp *= textBox->getMyScale();
                        }

                        if (lineNum % getMaxNumLines(mMsgWindowSubtype) == 0) {
                            currScale = fn_800B8040(0, mMsgWindowSubtype);
                            if (textBox != nullptr) {
                                currScale *= textBox->getMyScale();
                            }
                            if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_DEMO) {
                                tmp3 = -2.0f;
                            } else {
                                tmp3 = 3.0f;
                            }
                            posY = (fn_800B85C0(lineNum) - (fontSize.height * (currScale / tmp) / 2.0f)) - tmp3;
                        } else {
                            posY = (posY - (fontSize.height * (currScale / tmp) + getTextBox()->GetLineSpace()));
                        }
                        if (!b2) {
                            b2 = true;
                        }
                    } else if (b2) {
                        b2 = false;
                        b1 = true;
                    }
                    lineNum++;
                } break;
                case 0x10000:
                case 0x10001:
                case 0x10002:
                case 0x10003: b1 = true; break;
                case 0x10008: {
                    if (b2) {
                        f32 newScale = fn_800B8040(READ_U8(endPtr, 0), mMsgWindowSubtype);
                        f32 baseScale = fn_800B8040(0, mMsgWindowSubtype);
                        if (textBox != nullptr) {
                            newScale *= textBox->getMyScale();
                            baseScale *= textBox->getMyScale();
                        }
                        if (currScale != newScale) {
                            nw4r::lyt::Size fontSize = mpTextBox->GetFontSize();
                            posY -= (fontSize.height * ((currScale - newScale) / baseScale) / 2.0f) *
                                    UnkTextThing::getField0x788();
                            currScale = newScale;
                        }
                    }
                } break;
                case 0x30000: {
                    if (b2) {
                        backupScale = currScale;
                        f32 tmp = backupScale * UnkTextThing::getField0x768();
                        if (textBox != nullptr) {
                            tmp *= textBox->getMyScale();
                        }
                        if (currScale != tmp) {
                            nw4r::lyt::Size _size = mpTextBox->GetFontSize();
                            currScale = tmp;
                        }
                    }
                } break;
                case 0x20004:
                    if (b2) {
                        *writePtr = -1;
                        const nw4r::ut::Font *fnt = dFontMng_c::getFont(dFontMng_c::PICTURE);
                        s8 c = getAnimatedIconReplacement(READ_U8(endPtr, 0));
                        f32 tmp = UnkTextThing::getField0x764();
                        f32 wid = tmp / fnt->GetWidth() * currScale;
                        f32 charSpace = textBox->GetCharSpace();
                        f32 charWidth = wid * fnt->GetCharWidth(c);
                        posX += charWidth + charSpace;
                        writePtr++;
                    }
                    break;
                case 0x10012:
                    if (b2) {
                        somethingWithScrapperAndMusic(endPtr);
                    }
                    break;
            }
            src += cmdLen / 2 + 1;
            // For some reason the compiler believes that this branch affects something about
            // charData/charDataIdx and branches to a re-calculation of the strength-reduced
            // pointer, which means the compiler keepts charDataIdx actually around while still
            // incrementing the strength-reduced charData pointer by 0x14. Very weird!
            // This charDataIdx++, charDataIdx-- causes this behavior, not sure what the real match would be.
            charDataIdx++;
            charDataIdx--;
            goto beginning;
        } else if (c == 0xF) {
            s32 cmd = 0;
            process0xFCommand(c, &src[1], &cmd);
            if (cmd == 0x30000) {
                currScale = backupScale;
            }
            src += 3;
        } else {
            if (b2) {
                *writePtr = c;
                const nw4r::ut::Font *fnt = textBox->GetFont();
                f32 tmp = currScale * fnt->GetCharWidth(READ_U16(src, 0)) * 0.5f;
                posX += tmp;
                if (charData != nullptr) {
                    wchar_t s = *writePtr;
                    if (s != 10 && s != 0x20 && s != 0x3000) {
                        charData[charDataIdx].posX = posX;
                        charData[charDataIdx].posY = posY;
                        charData[charDataIdx].field_0x08 = currScale;
                        charData[charDataIdx].character = *writePtr;
                        charDataIdx++;
                    }
                }
                writePtr++;
                posX += (tmp + textBox->GetCharSpace());
            }
            src++;
        }
    }

end:
    if (pOutLen != nullptr) {
        *pOutLen = writePtr - dest;
    }
}

nw4r::ut::Operation dTagProcessor_c::Process(u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) {
    return ProcessTags(nullptr, ch, ctx);
}

nw4r::ut::Operation dTagProcessor_c::CalcRect(nw4r::ut::Rect *rect, u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) {
    return ProcessTags(rect, ch, ctx);
}

nw4r::ut::Operation dTagProcessor_c::ProcessTags(nw4r::ut::Rect *rect, u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) {
    if (ch != 0x0E) {
        if (ch != 0x0F) {
            if (rect != nullptr) {
                return nw4r::ut::TagProcessorBase<wchar_t>::CalcRect(rect, ch, ctx);
            } else {
                return nw4r::ut::TagProcessorBase<wchar_t>::Process(ch, ctx);
            }
        } else {
            s32 cmd = 0;
            process0xFCommand(ch, ctx->str, &cmd);
            if (cmd == 0x30000) {
                changeScale(rect, ctx, true);
            }
            ctx->str += 2;
            return nw4r::ut::OPERATION_DEFAULT;
        }
    }

    u8 cmdLen = 0;
    s32 cmd = 0;
    wchar_t *endPtr = nullptr;
    getTextCommand(ch, ctx->str, &cmdLen, &cmd, &endPtr);
    switch (cmd) {
        case 0x10000:
            ctx->str += cmdLen / 2;
            ctx->str += field_0x808[0];
            return nw4r::ut::OPERATION_DEFAULT;
        case 0x10001:
            ctx->str += cmdLen / 2;
            ctx->str += field_0x808[1];
            return nw4r::ut::OPERATION_DEFAULT;
        case 0x10002:
            ctx->str += cmdLen / 2;
            ctx->str += field_0x808[2];
            return nw4r::ut::OPERATION_DEFAULT;
        case 0x10003:
            ctx->str += cmdLen / 2;
            ctx->str += field_0x808[3];
            return nw4r::ut::OPERATION_DEFAULT;
        case 0x0F0F0F0F: fn_800B4FF0(rect, ctx, cmdLen, endPtr); break;
        case 0x0F0F0F0E: field_0xEE2 = 1; break;
        case 0x3:
            if (field_0xEE1 == 0) {
                setColor(rect, ctx, cmdLen, endPtr);
            }
            break;
        case 0x2: setScale(rect, ctx, cmdLen, endPtr); break;
        case 0x10004:
            // Pause
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEED == 0 && mNumericArgsCopy[8] == field_0x838) {
                    setFramesLeftOnPause(rect, ctx, cmdLen, endPtr);
                    field_0xEED = 1;
                    field_0x838++;
                }
                mNumericArgsCopy[8]++;
            }
            break;
        case 0x10005:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEE == 0 && mNumericArgsCopy[9] == field_0x83C) {
                    fn_800B5500(cmdLen, endPtr);
                    field_0xEEE = 1;
                    field_0x83C++;
                }
                mNumericArgsCopy[9]++;
            }
            break;
        case 0x10007:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEE5 == 0 && mNumericArgsCopy[0] == field_0x87C) {
                    fn_800B60E0(cmdLen, endPtr);
                    field_0xEE5 = 1;
                    field_0x87C++;
                }
                mNumericArgsCopy[0]++;
            }
            break;
        case 0x10008: fn_800B61D0(rect, ctx, cmdLen, endPtr); break;
        case 0x30000: changeScale(rect, ctx, false); break;
        case 0x10009:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEE6 == 0 && mNumericArgsCopy[1] == field_0x880) {
                    fn_800B6110(cmdLen, endPtr);
                    field_0xEE6 = 1;
                    field_0x880++;
                }
                mNumericArgsCopy[1]++;
            }
            break;
        case 0x1000A:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEE7 == 0 && mNumericArgsCopy[2] == field_0x884) {
                    fn_800B6140(cmdLen, endPtr);
                    field_0xEE7 = 1;
                    field_0x884++;
                }
                mNumericArgsCopy[2]++;
            }
            break;
        case 0x1000B:
            // Sound
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEE8 == 0 && mNumericArgsCopy[3] == field_0x888) {
                    playSound(cmdLen, endPtr);
                    field_0xEE8 = 1;
                    field_0x888++;
                }
                mNumericArgsCopy[3]++;
            }
            break;
        case 0x1000C:
            // "entrypoint"
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEA == 0 && mNumericArgsCopy[5] == field_0x890) {
                    fn_800B6170(cmdLen, endPtr);
                    field_0xEEA = 1;
                    field_0x890++;
                }
                mNumericArgsCopy[5]++;
            }
            break;
        case 0x1000D:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEB == 0 && mNumericArgsCopy[6] == field_0x894) {
                    fn_800B6190(cmdLen, endPtr);
                    field_0xEEB = 1;
                    field_0x894++;
                }
                mNumericArgsCopy[6]++;
            }
            break;
        case 0x1000E:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEC == 0 && mNumericArgsCopy[7] == field_0x898) {
                    fn_800B61B0(cmdLen, endPtr);
                    field_0xEEC = 1;
                    field_0x898++;
                }
                mNumericArgsCopy[7]++;
            }
            break;
        case 0x20004:
            if (field_0xEE3 != 0) {
                if (rect == nullptr) {
                    drawIcon(rect, ctx, cmdLen, endPtr);
                } else {
                    calcRectIcon(rect, ctx, endPtr);
                }
            }
            break;
        case 0x10011:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEF == 0 && mNumericArgsCopy[10] == field_0x840) {
                    fn_800B5540(endPtr);
                    field_0xEEF = 1;
                    field_0x840++;
                }
                mNumericArgsCopy[10]++;
            }
            break;
    }
    ctx->str += cmdLen / 2;
    return nw4r::ut::OPERATION_DEFAULT;
}

void dTagProcessor_c::fn_800B4FF0(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr) {
    if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_22 || mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_DEMO) {
        return;
    }

    if (field_0xEE0 != 0) {
        int arg = ptr[0];
        nw4r::lyt::Size textBoxSize = getTextBox()->GetSize();
        nw4r::lyt::Size fontSize = getTextBox()->GetFontSize();
        if (arg % getMaxNumLines(mMsgWindowSubtype) == 0 && mMsgWindowSubtype != dLytMsgWindow_c::MSG_WINDOW_31 &&
            mMsgWindowSubtype != dLytMsgWindow_c::MSG_WINDOW_8) {
            int v = 0;
            int u = 0;
            // similar code in fn_800B85C0
            for (int i = arg; i < arg + getMaxNumLines(mMsgWindowSubtype) && i < 0x32; i++) {
                f32 f6 = getLineWidth(i);
                if (f6 > 0.0f) {
                    v++;
                    if (u != 0) {
                        v += u;
                        u = 0;
                    }
                } else {
                    u++;
                }
            }

            if (u < getMaxNumLines(mMsgWindowSubtype)) {
                // Begin unused code - the results here are unused and disappear
                // in the assembly, so we don't know which fields are accessed in
                // the tmp vars, but control flow from the inlines remains
                f32 w1 = fontSize.width * 0.5f;
                f32 h1 = fontSize.height * 0.5f;
                if (w1 < UnkTextThing::getField0x758() || h1 < UnkTextThing::getField0x758()) {
                    if (w1 < h1) {
                        f32 tmp = UnkTextThing::getField0x758();
                    } else {
                        f32 tmp2 = UnkTextThing::getField0x758();
                    }
                }
                f32 tmp3 = UnkTextThing::getField0x758();
                // End unused code

                f32 f7 = (fontSize.height + getTextBox()->GetLineSpace()) / 2.0f;
                f32 f8 = f7 * (getMaxNumLines(mMsgWindowSubtype) - v);
                field_0x814 = ctx->writer->GetCursorY();
                field_0x818 = ctx->y;
                field_0xEE4 = 1;
                if (f8 > 0.0f) {
                    ctx->writer->MoveCursorY(f8);
                    field_0x810 = f8;
                }
            }
        }

        f32 lineWidth = getLineWidth(arg);
        f32 margin = (textBoxSize.width - lineWidth) / 2.0f;
        if ((mMsgWindowSubtype < dLytMsgWindow_c::MSG_WINDOW_WOOD ||
             mMsgWindowSubtype > dLytMsgWindow_c::MSG_WINDOW_LINK) &&
            mMsgWindowSubtype != dLytMsgWindow_c::MSG_WINDOW_DEMO) {
            margin = 0.0f;
        }
        if (margin > 0.0f) {
            ctx->writer->MoveCursorX(margin);
        }
    }
}

void dTagProcessor_c::setColor(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *buf) {
    u16 cmd = buf[0];
    if (cmd == 0xFFFF) {
        restoreColor(ctx, mMsgWindowSubtype);
        return;
    }
    nw4r::ut::Color c1 = FontColors1[cmd & 0xFFFF];
    nw4r::ut::Color c2 = FontColors2[cmd & 0xFFFF];
    if (cmd == 0) {
        if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_SWORD_FI) {
            // #FF6E54, #FF6E54
            c1.r = 0xFF;
            c1.g = 0x6E;
            c1.b = 0x64;
            c2.r = 0xFF;
            c2.g = 0x6E;
            c2.b = 0x64;
        } else if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_STONE) {
            // #E64B32, #E64B32
            c1.r = 0xE6;
            c1.g = 0x4B;
            c1.b = 0x32;
            c2.r = 0xE6;
            c2.g = 0x4B;
            c2.b = 0x32;
        }
    } else if (cmd == 1) {
        if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_SWORD_FI) {
            // #F5645A, #C8645A
            c1.r = 0xF5;
            c1.g = 0x64;
            c1.b = 0x5A;
            c2.r = 0xC8;
            c2.g = 0x64;
            c2.b = 0x5A;
        } else if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_STONE) {
            // #B45050, #8C4040
            c1.r = 0xB4;
            c1.g = 0x50;
            c1.b = 0x50;
            c2.r = 0x8C;
            c2.g = 0x40;
            c2.b = 0x40;
        }
    } else if (cmd == 3 && mMsgWindowSubtype >= dLytMsgWindow_c::MSG_WINDOW_SWORD_FI &&
               mMsgWindowSubtype < dLytMsgWindow_c::MSG_WINDOW_SWORD_FI + 3) {
        // #50E6FA, #C8E6F5
        c1.r = 0x50;
        c1.g = 0xE6;
        c1.b = 0xFA;
        c2.r = 0xC8;
        c2.g = 0xE6;
        c2.b = 0xF5;
    }

    u8 u5 = 0xFF;
    if (getTextBox() != nullptr) {
        u5 = getTextBox()->GetGlobalAlpha();
    }

    c2.a = u5;
    c1.a = u5;
    ctx->writer->SetGradationMode(nw4r::ut::CharWriter::GRADMODE_V);
    ctx->writer->SetTextColor(c1, c2);
}

void dTagProcessor_c::setScale(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *buf) {
    u16 scale = READ_U16(buf, 0);
    if (scale == 0) {
        scale = 50;
    }
    f32 x = scale / 100.0f;
    f32 y = scale / 100.0f;
    ctx->writer->SetScale(x, y);
}

void dTagProcessor_c::setFramesLeftOnPause(
    nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr
) {
    mPauseFramesLeft = READ_U16(ptr, 0);
}

void dTagProcessor_c::fn_800B5500(u8 cmdLen, wchar_t *ptr) {
    u16 val = READ_U16(ptr, 0);
    if (field_0x830 == -1) {
        field_0x830 = val;
    }
}

void dTagProcessor_c::fn_800B5520(wchar_t *src) {
    field_0x81C = READ_U32(src, 0);
    field_0x820 = READ_U32(src, 4);
}

void dTagProcessor_c::fn_800B5540(wchar_t *src) {
    dLytMsgWindow_c::getInstance()->onFlag0x820(sMsgWindowFlags[READ_U8(src, 0)]);
}

wchar_t *dTagProcessor_c::writeHeroname(wchar_t *dest, s32 *outArg, s32 arg) {
    if (FileManager::GetInstance()->getHeroname()[0] != '\0') {
        for (int i = 0; FileManager::GetInstance()->getHeroname()[i] != '\0'; i++) {
            if (arg != 0 && field_0x90E != 0) {
                wchar_t *heroName = FileManager::GetInstance()->getHeroname();
                writeSingleCharacter(heroName[i], &getTmpBuffer()[*outArg], outArg);
                onWriteTmpBuffer();
            } else {
                wchar_t *heroName = FileManager::GetInstance()->getHeroname();
                dest = writeSingleCharacter(heroName[i], dest, nullptr);
            }
        }
    }
    return dest;
}

wchar_t *dTagProcessor_c::writeItem(wchar_t *dest, wchar_t *src, s32 *outArg, s32 arg) {
    int itemIndex = READ_U16(src, 0);
    wchar_t c;

    int i = 0;
    SizedString<16> mName;
    mName.sprintf("NAME_ITEM_%03d", itemIndex);
    MsbtInfo *info = dMessage_c::getMsbtInfoForIndex(/* ItemGet */ 3);
    const wchar_t *text = LMS_GetTextByLabel(info, mName);

    while ((c = text[i]) != 0) {
        if (c == 0xE) {
            int len = ((text[i + 3] / 2) & 0x7F) + 4;
            if (arg != 0 && field_0x90E != 0) {
                for (int j = 0; j < len; j++) {
                    writeSingleCharacter(text[i], &getTmpBuffer()[*outArg], outArg);
                    onWriteTmpBuffer();
                    i++;
                }
            } else {
                for (int j = 0; j < len; j++) {
                    dest = writeSingleCharacter(text[i], dest, nullptr);
                    i++;
                }
            }
        } else {
            if (arg != 0 && field_0x90E != 0) {
                writeSingleCharacter(c, &getTmpBuffer()[*outArg], outArg);
                onWriteTmpBuffer();
            } else {
                dest = writeSingleCharacter(c, dest, nullptr);
            }
            i++;
        }
    }

    return dest;
}

wchar_t *dTagProcessor_c::writeStringArg(wchar_t *dest, wchar_t *src, s32 *outArg, s32 arg) {
    s32 argIdx = READ_U32(src, 0);
    wchar_t c;
    int i = 0;
    while ((c = mStringArgs[argIdx][i]) != 0) {
        if (c == 0xE) {
            int len = ((mStringArgs[argIdx][i + 3] / 2) & 0x7F) + 4;
            if (arg != 0 && field_0x90E != 0) {
                for (int j = 0; j < len; j++) {
                    writeSingleCharacter(mStringArgs[argIdx][i], &getTmpBuffer()[*outArg], outArg);
                    onWriteTmpBuffer();
                    i++;
                }
            } else {
                for (int j = 0; j < len; j++) {
                    dest = writeSingleCharacter(mStringArgs[argIdx][i], dest, nullptr);
                    i++;
                }
            }
        } else {
            if (arg != 0 && field_0x90E != 0) {
                writeSingleCharacter(c, &getTmpBuffer()[*outArg], outArg);
                onWriteTmpBuffer();
            } else {
                dest = writeSingleCharacter(c, dest, nullptr);
            }
            i++;
        }

        if (i >= 0x40) {
            break;
        }
    }

    return dest;
}

wchar_t *dTagProcessor_c::writeNumericArg(wchar_t *dest, wchar_t *src, s32 *outArg, s32 arg) {
    int numZeroDigits = READ_U8(src, 4);
    bool writeZeroDigits = false;
    s32 argIdx = READ_U32(src, 0);
    s32 numberArg = mNumericArgs[argIdx];
    mFirstNumericArg = numberArg;

    // This could potentially be an unrolled loop

    s32 digit = numberArg / 10000;
    s32 number = numberArg % 10000;

    if (!writeZeroDigits && numZeroDigits == 5) {
        writeZeroDigits = true;
    }
    if (digit > 0 || writeZeroDigits) {
        if (arg != 0 && field_0x90E != 0) {
            getTmpBuffer()[*outArg] = '0' + digit;
            onWriteTmpBuffer();
            (*outArg)++;
        } else {
            *(dest++) = '0' + digit;
        }
        writeZeroDigits = true;
    }

    digit = number / 1000;
    number = number % 1000;
    if (!writeZeroDigits && numZeroDigits == 4) {
        writeZeroDigits = true;
    }
    if (digit > 0 || writeZeroDigits) {
        if (arg != 0 && field_0x90E != 0) {
            getTmpBuffer()[*outArg] = '0' + digit;
            onWriteTmpBuffer();
            (*outArg)++;
        } else {
            *(dest++) = '0' + digit;
        }
        writeZeroDigits = true;
    }

    digit = number / 100;
    number = number % 100;
    if (!writeZeroDigits && numZeroDigits == 3) {
        writeZeroDigits = true;
    }
    if (digit > 0 || writeZeroDigits) {
        if (arg != 0 && field_0x90E != 0) {
            getTmpBuffer()[*outArg] = '0' + digit;
            onWriteTmpBuffer();
            (*outArg)++;
        } else {
            *(dest++) = '0' + digit;
        }
        writeZeroDigits = true;
    }

    digit = number / 10;
    number = number % 10;
    if (!writeZeroDigits && numZeroDigits == 2) {
        writeZeroDigits = true;
    }
    if (digit > 0 || writeZeroDigits) {
        if (arg != 0 && field_0x90E != 0) {
            getTmpBuffer()[*outArg] = '0' + digit;
            onWriteTmpBuffer();
            (*outArg)++;
        } else {
            *(dest++) = '0' + digit;
        }
        writeZeroDigits = true;
    }

    digit = number;
    if (arg != 0 && field_0x90E != 0) {
        getTmpBuffer()[*outArg] = '0' + digit;
        onWriteTmpBuffer();
        (*outArg)++;
    } else {
        *(dest++) = '0' + digit;
    }
    return dest;
}

wchar_t *dTagProcessor_c::writeSingularOrPluralWord(wchar_t *dest, wchar_t *src, s32 *outArg, s32 arg) {
    int itemIndex = READ_U8(src, 0);
    wchar_t c;

    int i = 0;
    SizedString<32> mName;
    s32 amount = 2;
    if (shouldUseSingular(mFirstNumericArg)) {
        amount = 1;
    }
    mName.sprintf("lang:word:%03d:%02d", itemIndex, amount);
    MsbtInfo *info = dMessage_c::getMsbtInfoForIndex(/* word */ 18);
    const wchar_t *text = LMS_GetTextByLabel(info, mName);

    while ((c = text[i]) != 0) {
        if (c == 0xE) {
            int len = ((text[i + 3] / 2) & 0x7F) + 4;
            if (arg != 0 && field_0x90E != 0) {
                for (int j = 0; j < len; j++) {
                    writeSingleCharacter(text[i], &getTmpBuffer()[*outArg], outArg);
                    onWriteTmpBuffer();
                    i++;
                }
            } else {
                for (int j = 0; j < len; j++) {
                    dest = writeSingleCharacter(text[i], dest, nullptr);
                    i++;
                }
            }
        } else {
            if (arg != 0 && field_0x90E != 0) {
                writeSingleCharacter(c, &getTmpBuffer()[*outArg], outArg);
                onWriteTmpBuffer();
            } else {
                dest = writeSingleCharacter(c, dest, nullptr);
            }
            i++;
        }
    }

    return dest;
}

wchar_t *dTagProcessor_c::writeSingleCharacter(wchar_t character, wchar_t *dst, s32 *pOutLen) {
    if (field_0xEF1) {
        field_0xEF1 = 0;
        wchar_t replacementSequence[3];
        toUpperCase(character, replacementSequence, 3);
        wchar_t *ret = dst;
        for (s32 i = 0; i < 3; i++) {
            if (replacementSequence[i] == L'\0') {
                return dst;
            }
            *dst = replacementSequence[i];
            dst++;
            if (pOutLen != nullptr) {
                (*pOutLen)++;
            }
        }
        return dst;
    } else {
        *dst = character;
        if (pOutLen != nullptr) {
            (*pOutLen)++;
        }
        return dst + 1;
    }
}

void dTagProcessor_c::fn_800B60E0(u8 cmdLen, wchar_t *ptr) {
    field_0x844 = READ_U8(ptr, 0);
    field_0x848 = READ_U8(ptr, 1);
    field_0x84C = READ_U8(ptr, 2);
    field_0x850 = READ_U8(ptr, 3);
}
void dTagProcessor_c::fn_800B6110(u8 cmdLen, wchar_t *ptr) {
    field_0x854 = READ_U8(ptr, 0);
    field_0x858 = READ_U8(ptr, 1);
    field_0x85C = READ_U8(ptr, 2);
    field_0x860 = READ_U8(ptr, 3);
}
void dTagProcessor_c::fn_800B6140(u8 cmdLen, wchar_t *ptr) {
    field_0x864 = READ_U16(ptr, 0);
    field_0x868 = READ_U16(ptr, 2);
}
void dTagProcessor_c::playSound(u8 cmdLen, wchar_t *ptr) {
    dFlow_c::playSound(READ_U32(ptr, 0));
}
void dTagProcessor_c::fn_800B6170(u8 cmdLen, wchar_t *ptr) {
    field_0x8FC = READ_U16(ptr, 0);
    field_0x900 = READ_U16(ptr, 2);
}
void dTagProcessor_c::fn_800B6190(u8 cmdLen, wchar_t *ptr) {
    field_0x86C = READ_S8(ptr, 0);
    field_0x870 = READ_S8(ptr, 1);
}
void dTagProcessor_c::fn_800B61B0(u8 cmdLen, wchar_t *ptr) {
    field_0x874 = READ_S8(ptr, 0);
    field_0x878 = READ_S16(ptr, 1);
}

void dTagProcessor_c::fn_800B61D0(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr) {
    f32 scale = fn_800B8040(READ_S8(ptr, 0), mMsgWindowSubtype);
    if (getTextBox() != nullptr) {
        scale *= getTextBox()->getMyScale();
    }
    ctx->writer->SetScale(scale, scale);
}

void dTagProcessor_c::changeScale(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, bool direct) {
    f32 scale;
    f32 posY;

    if (direct) {
        scale = field_0x904;
        posY = field_0x908;
    } else {
        field_0x904 = ctx->writer->GetScaleV();
        field_0x908 = ctx->writer->GetCursorY();
        scale = field_0x904 * UnkTextThing::getField0x768();
        posY = field_0x908 - ctx->writer->GetFontAscent() * UnkTextThing::getField0x76C();
    }

    ctx->writer->SetCursorY(posY);
    ctx->writer->SetScale(scale, scale);
}

void dTagProcessor_c::saveIconWidth(dTextBox_c *textBox, wchar_t *cmd, f32 arg) {
    nw4r::ut::Font *f = dFontMng_c::getFont(dFontMng_c::PICTURE);
    s8 c3 = getAnimatedIconReplacement(READ_U8(cmd, 0));
    arg = UnkTextThing::getField0x764() / f->GetWidth() * arg;

    mLineData.mLineWidths[mLineData.mNumLines] += arg * f->GetCharWidth(c3) + textBox->GetCharSpace();
}

void dTagProcessor_c::drawIcon(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr) {
    s8 c = getAnimatedWiggleIconReplacement(READ_U8(ptr, 0));
    u8 alpha = 0xFF;
    if (getTextBox() != nullptr) {
        alpha = mpTextBox->GetGlobalAlpha();
    }
    s32 x1 = alpha / 255.0f * getAlphaForAnimatedIcon(READ_U8(ptr, 0));
    s32 x2 = alpha / 255.0f * (255 - getAlphaForAnimatedIcon(READ_U8(ptr, 0)));

    nw4r::ut::TextWriterBase<wchar_t> w = *ctx->writer;
    drawPicture(&w, ctx, (u32)c, x1);
    if (READ_U8(ptr, 0) >= FONT_BTN_NUN_STK_UP && READ_U8(ptr, 0) <= FONT_BTN_NUN_STK_WIGGLE_HORIZONTAL) {
        nw4r::ut::TextWriterBase<wchar_t> w2 = *ctx->writer;
        s8 c = symbolToFontIdx(FONT_BTN_NUN_STK);
        drawPicture(&w2, ctx, c, x2);
    }
    ctx->writer->SetCursorX(w.GetCursorX());
    ctx->writer->SetupGX();
    if (mpTextBox != nullptr) {
        mpTextBox->setupGX();
    }
}

void dTagProcessor_c::drawPicture(
    nw4r::ut::CharWriter *w, nw4r::ut::PrintContext<wchar_t> *ctx, u16 character, u8 alpha
) {
    nw4r::ut::Font *font = dFontMng_c::getFont(dFontMng_c::PICTURE);

    f32 f1 = UnkTextThing::getField0x764() * w->GetScaleV() / UnkTextThing::getFn800B1FC0();
    f32 f3 = UnkTextThing::getFn800B1FC0();
    f32 f4 = UnkTextThing::getField0x764();
    f32 f5 = UnkTextThing::getFn800B1FC0();
    f32 scaleY2 = w->GetScaleV();

    s32 fontHeight = w->GetFont()->GetHeight();
    f32 f6 = (scaleY2 * (fontHeight * f3 - f4)) / f5 / 2.0f;

    w->SetFont(*font);
    // #000000, #FFFFFF
    w->ResetColorMapping();

    s32 x = 0;
    w->SetFontSize((s32)(f1 + 1.0f + x));
    if (field_0xEE4) {
        w->SetCursorY(f6 + field_0x818 - (field_0x814 - ctx->writer->GetCursorY()));
    } else {
        w->SetCursorY(ctx->y + f6);
    }
    w->SetGradationMode(nw4r::ut::CharWriter::GRADMODE_V);
    if (character == /* FONT_X */ 0x38) {
        // #FFFFFF, #FFFFFF
        mColor c1(0xFF, 0xFF, 0xFF, alpha);
        mColor c2(0xFF, 0xFF, 0xFF, alpha);
        w->SetTextColor(c1, c2);
        // #FFAB7F, #DD4809
        mColor c3(0xFF, 0xAB, 0x7F, 0);
        mColor c4(0xDD, 0x48, 0x09, 0xFF);
        w->SetColorMapping(c3, c4);
    } else if (character == /* FONT_HOLE */ 0x39) {
        // #FFFFFF, #FFFFFF
        mColor c1(0xFF, 0xFF, 0xFF, alpha);
        mColor c2(0xFF, 0xFF, 0xFF, alpha);
        w->SetTextColor(c1, c2);
        // #24D49F, #FFF8A4
        mColor c3(0x24, 0xD4, 0x9F, 0);
        mColor c4(0xFF, 0xF8, 0xA4, 0xFF);
        w->SetColorMapping(c3, c4);
    } else if (character >= /* FONT_ARROW_UP */ 0x33 && character <= /* FONT_ARROW_RIGHT */ 0x36) {
        if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_WOOD) {
            // #F2DABC, #F2DABC
            mColor c1(0xF2, 0xDA, 0xBC, alpha);
            mColor c2(0xF2, 0xDA, 0xBC, alpha);
            w->SetTextColor(c1, c2);
        } else if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_STONE) {
            // #6E6973, #6E6973
            mColor c1(0x6E, 0x69, 0x73, alpha);
            mColor c2(0x6E, 0x69, 0x73, alpha);
            w->SetTextColor(c1, c2);
        } else {
            // #FFFCEE, #F3EFE1
            mColor c1(0xFF, 0xFC, 0xEE, alpha);
            mColor c2(0xF3, 0xEF, 0xE1, alpha);
            w->SetTextColor(c1, c2);
        }
    } else {
        // #FFFCEE, #F3EFE1
        mColor c1(0xFF, 0xFC, 0xEE, alpha);
        mColor c2(0xF3, 0xEF, 0xE1, alpha);
        w->SetTextColor(c1, c2);
    }
    if (field_0xEE1 != 0) {
        // #505050
        mColor c1(0x50, 0x50, 0x50, 0);
        mColor c2(0x50, 0x50, 0x50, 0x50);
        SetupGXCommon();
        SetupGXWithColorMapping(c1, c2);
        SetupVertexFormat();
    } else {
        w->SetupGX();
    }
    w->Print(character);
}

void dTagProcessor_c::calcRectIcon(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, wchar_t *ptr) {
    nw4r::ut::Font *f = dFontMng_c::getFont(dFontMng_c::PICTURE);
    s8 c3 = getAnimatedIconReplacement(READ_U8(ptr, 0));
    nw4r::ut::TextWriterBase<wchar_t> copy = *ctx->writer;
    f32 charWidth = f->GetCharWidth(c3);
    configureWriterForIcon(&copy, ctx, c3, 0xFF);
    ctx->writer->MoveCursorX(charWidth * copy.GetScaleH());

    f32 cursorX = ctx->writer->GetCursorX();
    rect->left = rect->left > cursorX ? cursorX : rect->left;
    rect->right = rect->right < cursorX ? cursorX : rect->right;
}

void dTagProcessor_c::configureWriterForIcon(
    nw4r::ut::TextWriterBase<wchar_t> *w, nw4r::ut::PrintContext<wchar_t> *ctx, u16 arg, s32 alpha
) {
    nw4r::ut::Font *font = dFontMng_c::getFont(dFontMng_c::PICTURE);

    f32 f1 = UnkTextThing::getField0x764() * ctx->writer->GetScaleV() / UnkTextThing::getFn800B1FC0();
    w->SetFont(*font);
    w->SetFontSize(f1);
    w->SetGradationMode(nw4r::ut::CharWriter::GRADMODE_V);
    if (arg == /* FONT_X */ 0x38) {
        // #F26E36, #F26E36
        mColor c1(0xF2, 0x6E, 0x36, alpha);
        mColor c2(0xF2, 0x6E, 0x36, alpha);
        w->SetTextColor(c1, c2);
    } else if (arg >= /* FONT_ARROW_UP */ 0x33 && arg < /* FONT_ARROW_DOWN */ 0x37) {
        if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_WOOD) {
            // #F2DABC, #F2DABC
            mColor c1(0xF2, 0xDA, 0xBC, alpha);
            mColor c2(0xF2, 0xDA, 0xBC, alpha);
            w->SetTextColor(c1, c2);
        } else if (mMsgWindowSubtype == dLytMsgWindow_c::MSG_WINDOW_STONE) {
            // #6E6973, #6E6973
            mColor c1(0x6E, 0x69, 0x73, alpha);
            mColor c2(0x6E, 0x69, 0x73, alpha);
            w->SetTextColor(c1, c2);
        } else {
            // #FFFCEE, #F3EFE1
            mColor c1(0xFF, 0xFC, 0xEE, alpha);
            mColor c2(0xF3, 0xEF, 0xE1, alpha);
            w->SetTextColor(c1, c2);
        }
    } else {
        // #FFFCEE, #F3EFE1
        mColor c1(0xFF, 0xFC, 0xEE, alpha);
        mColor c2(0xF3, 0xEF, 0xE1, alpha);
        w->SetTextColor(c1, c2);
    }
}

void dTagProcessor_c::somethingWithScrapperAndMusic(wchar_t *src) {
    field_0x824 = READ_U32(src, 0);
    if (field_0x824 == 100) {
        dAcBase_c *ac = nullptr;
        dAcBase_c *parent = nullptr;
        while ((parent = ac = static_cast<dAcBase_c *>(fManager_c::searchBaseByGroupType(fBase_c::ACTOR, parent))) !=
               nullptr) {
            if ((ac->profile_name >= fProfile::NPC_SLFB && ac->profile_name <= fProfile::NPC_SLFL) ||
                (ac->profile_name >= fProfile::NPC_SLB && ac->profile_name <= fProfile::FLY_SLB) ||
                ac->profile_name == fProfile::NPC_SLB2 || ac->profile_name == fProfile::NPC_DRBC) {
                fn_8035E790(BGM_MGR, dLytMsgWindow_c::getInstance()->getField_0x784(), ac);
                return;
            }
        }
    } else if (field_0x824 == 101) {
        fn_8035E790(BGM_MGR, dLytMsgWindow_c::getInstance()->getField_0x784(), nullptr);
    }
}

void dTagProcessor_c::restoreColor(nw4r::ut::PrintContext<wchar_t> *ctx, u8 windowType) {
    if (field_0xEE2 != 0) {
        windowType = 1;
    }
    int colorIndex;
    switch (windowType) {
        case 2:  colorIndex = 15; break;
        case 3:  colorIndex = 16; break;
        case 4:  colorIndex = 17; break;
        case 6:  colorIndex = 19; break;
        case 7:  colorIndex = 20; break;
        case 8:  colorIndex = 21; break;
        case 9:  colorIndex = 22; break;
        case 11: colorIndex = 24; break;
        case 12: colorIndex = 25; break;
        case 13: colorIndex = 26; break;
        case 17: colorIndex = 30; break;
        case 18: colorIndex = 31; break;
        case 19: colorIndex = 32; break;
        case 30: colorIndex = 43; break;
        case 20: colorIndex = 33; break;
        case 22: colorIndex = 35; break;
        case 23: colorIndex = 36; break;
        case 24: colorIndex = 37; break;
        case 27: colorIndex = 40; break;
        case 29: colorIndex = 42; break;
        case 31: colorIndex = 44; break;
        case 5:  colorIndex = 18; break;
        default: colorIndex = 14; break;
    }

    nw4r::ut::Color c1 = FontColors1[colorIndex & 0xFFFF];
    nw4r::ut::Color c2 = FontColors2[colorIndex & 0xFFFF];

    if (windowType == 7) {
        c2.a = 0xC8;
    } else if (windowType == 12) {
        c2.a = 0xFF;
    } else if (windowType == 13) {
        c2.a = 0xC8;
    }

    u8 u5 = 0xFF;
    if (getTextBox() != nullptr) {
        u5 = getTextBox()->GetGlobalAlpha();
    }

    c1.a = c1.a * u5 / 255.0f;
    c2.a = c2.a * u5 / 255.0f;
    ctx->writer->SetGradationMode(nw4r::ut::CharWriter::GRADMODE_V);
    ctx->writer->SetTextColor(c1, c2);
}

wchar_t *dTagProcessor_c::writeTextNormal(const wchar_t *src, wchar_t *dest, s32 *pArg, u8 cmdLen, s32 arg) {
    if (arg != 0 && field_0x90E != 0) {
        for (u32 i = 0; i < (cmdLen / 2 + 1); i++) {
            getTmpBuffer()[*pArg] = *(src++);
            onWriteTmpBuffer();
            (*pArg)++;
        }
    } else {
        memcpy(dest, src, cmdLen + 2);
        dest += (cmdLen / 2 + 1);
    }

    return dest;
}

u8 dTagProcessor_c::symbolToFontIdx(s32 s) {
    static const s8 index[] = " !\"#$%&\'()*+,)+-/0123456789";
    return index[s];
}

u8 dTagProcessor_c::getAnimatedIconReplacement(u8 arg) {
    if (arg >= FONT_BTN_NUN_STK_UP && arg <= FONT_BTN_NUN_STK_RIGHT) {
        if (mButtonPressAnimTimer >= UnkTextThing::getAnimBtnTotalDuration()) {
            // Show base instead
            return symbolToFontIdx(FONT_BTN_NUN_STK);
        }
    } else if (arg >= FONT_BTN_CROSS_UP && arg <= FONT_BTN_CROSS_RIGHT) {
        if (mButtonPressAnimTimer >= UnkTextThing::getAnimBtnTotalDuration()) {
            // Show base instead
            // Empty Cross Btn
            return '.';
        }
    } else if (arg == FONT_BTN_NUN_STK_WIGGLE_VERTICAL) {
        if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration()) {
            return symbolToFontIdx(FONT_BTN_NUN_STK_UP);
        }

        if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration() * 2) {
            return symbolToFontIdx(FONT_BTN_NUN_STK);
        }

        if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration() * 3) {
            return symbolToFontIdx(FONT_BTN_NUN_STK_DOWN);
        }
        return symbolToFontIdx(FONT_BTN_NUN_STK);
    } else if (arg == FONT_BTN_NUN_STK_WIGGLE_HORIZONTAL) {
        if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration()) {
            return symbolToFontIdx(FONT_BTN_NUN_STK_LEFT);
        }

        if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration() * 2) {
            return symbolToFontIdx(FONT_BTN_NUN_STK);
        }

        if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration() * 3) {
            return symbolToFontIdx(FONT_BTN_NUN_STK_RIGHT);
        }
        return symbolToFontIdx(FONT_BTN_NUN_STK);
    }

    return symbolToFontIdx(arg);
}

u8 dTagProcessor_c::getAnimatedWiggleIconReplacement(u8 arg) {
    if (arg == FONT_BTN_NUN_STK_WIGGLE_VERTICAL) {
        if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration() * 2) {
            return symbolToFontIdx(FONT_BTN_NUN_STK_UP);
        } else {
            return symbolToFontIdx(FONT_BTN_NUN_STK_DOWN);
        }
    } else if (arg == FONT_BTN_NUN_STK_WIGGLE_HORIZONTAL) {
        if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration() * 2) {
            return symbolToFontIdx(FONT_BTN_NUN_STK_LEFT);
        } else {
            return symbolToFontIdx(FONT_BTN_NUN_STK_RIGHT);
        }
    }
    return symbolToFontIdx(arg);
}

u8 dTagProcessor_c::getAlphaForAnimatedIcon(u8 arg) {
    f32 ret = 255.0f;
    f32 duration = UnkTextThing::getAnimBtnTransitionDuration();
    if (arg >= FONT_BTN_NUN_STK_UP && arg <= FONT_BTN_NUN_STK_RIGHT) {
        if (mButtonPressAnimTimer < UnkTextThing::getAnimBtnTransitionDuration()) {
            // Ramp-up, become visible
            ret = (mButtonPressAnimTimer / duration) * ret;
        } else if (mButtonPressAnimTimer < UnkTextThing::getAnimBtnTotalDuration()) {
            // Fully visible
            ret = 255.0f;
        } else if (mButtonPressAnimTimer <
                   UnkTextThing::getAnimBtnTotalDuration() + UnkTextThing::getAnimBtnTransitionDuration()) {
            // Ramp-down, become invisible
            ret = (UnkTextThing::getAnimBtnTotalDuration() + UnkTextThing::getAnimBtnTransitionDuration() -
                   mButtonPressAnimTimer) /
                  duration * ret;
        } else {
            // Invisible
            ret = 0.0f;
        }
    } else if (arg >= FONT_BTN_NUN_STK_WIGGLE_VERTICAL && arg <= FONT_BTN_NUN_STK_WIGGLE_HORIZONTAL) {
        if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTransitionDuration()) {
            // Ramp-up direction variant, become visible
            ret = (mButtonWiggleAnimTimer / duration) * ret;
        } else if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration()) {
            // Direction fully visible
            ret = 255.0f;
        } else if (mButtonWiggleAnimTimer <
                   UnkTextThing::getAnimBtnTotalDuration() + UnkTextThing::getAnimBtnTransitionDuration()) {
            // Ramp-down direction variant, become invisible
            ret = (UnkTextThing::getAnimBtnTotalDuration() + UnkTextThing::getAnimBtnTransitionDuration() -
                   mButtonWiggleAnimTimer) /
                  duration * ret;
        } else if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration() * 2) {
            // Invisible
            ret = 0.0f;
        } else if (mButtonWiggleAnimTimer <
                   UnkTextThing::getAnimBtnTransitionDuration() + UnkTextThing::getAnimBtnTotalDuration() * 2) {
            // Ramp-up base variant, become visible
            ret = (mButtonWiggleAnimTimer - 2 * UnkTextThing::getAnimBtnTotalDuration()) / duration * ret;
        } else if (mButtonWiggleAnimTimer < UnkTextThing::getAnimBtnTotalDuration() * 3) {
            // Base variant fully visible
            ret = 255.0f;
        } else if (mButtonWiggleAnimTimer <
                   UnkTextThing::getAnimBtnTransitionDuration() + UnkTextThing::getAnimBtnTotalDuration() * 3) {
            // Ramp-down base variant, become invisible
            ret = ((UnkTextThing::getAnimBtnTransitionDuration() + 3 * UnkTextThing::getAnimBtnTotalDuration()) -
                   mButtonWiggleAnimTimer) /
                  duration * ret;
        } else {
            // Invisible
            ret = 0.0f;
        }
    }

    return ret;
}

void dTagProcessor_c::setStringArg(const wchar_t *arg, s32 index) {
    for (int i = 0; i < 0x40; i++) {
        mStringArgs[index][i] = arg[i];
    }
}

f32 dTagProcessor_c::fn_800B8040(s8 factor, u32 windowType) {
    // Fun little recursion here
    f32 f1 = UnkTextThing::getFn800B1F70();
    if (windowType == dLytMsgWindow_c::MSG_WINDOW_WOOD) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.93f);
    } else if (windowType == dLytMsgWindow_c::MSG_WINDOW_STONE) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.9f);
    } else if (windowType == dLytMsgWindow_c::MSG_WINDOW_LINK) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.68f);
    } else if (windowType == dLytMsgWindow_c::MSG_WINDOW_DEMO) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.9f);
    } else if (windowType == dLytMsgWindow_c::MSG_WINDOW_34) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.86f);
    } else {
        f32 x;
        switch (factor) {
            case -2: x = 0.55f; break;
            case -1: x = 0.68f; break;
            case -0: x = 0.8f; break;
            case 1:  x = 0.95f; break;
            case 2:  x = 1.1f; break;
        }
        x *= UnkTextThing::getFn800B1F10();
        return x * f1;
    }
}

// Copied exactly from nw4r::ut::CharWriter
void dTagProcessor_c::SetupGXCommon() {
    static const nw4r::ut::Color fog = 0;

    GXSetFog(GX_FOG_NONE, fog, 0.0f, 0.0f, 0.0f, 0.0f);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, FALSE, GX_DUALMTX_IDENT);
    GXSetNumIndStages(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
}

// Copied from nw4r::ut::CharWriter with one modification
void dTagProcessor_c::SetupGXWithColorMapping(nw4r::ut::Color min, nw4r::ut::Color max) {
    GXSetNumTevStages(2);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);

    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

    GXSetTevColor(GX_TEVREG0, min);
    GXSetTevColor(GX_TEVREG1, max);

    // GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
}

// Copied exactly from nw4r::ut::CharWriter
void dTagProcessor_c::SetupVertexFormat() {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 15);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

f32 dTagProcessor_c::getMarginForCenteredLine(s32 lineIdx) {
    nw4r::lyt::Size size = mpTextBox->GetSize();
    f32 margin = (size.width - getLineWidth(lineIdx)) / 2.0f;
    if (margin < 0.0f) {
        margin = 0.0f;
    }
    return margin;
}

f32 dTagProcessor_c::fn_800B85C0(s32 lineIdx) {
    nw4r::lyt::Size size = mpTextBox->GetSize();
    nw4r::lyt::Size fontSize = mpTextBox->GetFontSize();

    s32 v = 0;
    s32 u = 0;

    s32 numLines = getMaxNumLines(mMsgWindowSubtype);
    s32 rem = lineIdx % numLines;
    if (rem != 0) {
        lineIdx -= rem;
    }
    // similar code in fn_800B4FF0
    for (s32 i = lineIdx; i < getMaxNumLines(mMsgWindowSubtype) + lineIdx && i < 0x32; i++) {
        if (getLineWidth(i) > 0.0f) {
            v++;
            if (u != 0) {
                v += u;
                u = 0;
            }
        } else {
            u++;
        }
    }

    f32 ret = 0.0f;
    if (u < getMaxNumLines(mMsgWindowSubtype)) {
        // Begin unused code - the results here are unused and disappear
        // in the assembly, so we don't know which fields are accessed in
        // the tmp vars, but control flow from the inlines remains
        f32 w1 = fontSize.width * 0.5f;
        f32 h1 = fontSize.height * 0.5f;
        if (w1 < UnkTextThing::getField0x758() || h1 < UnkTextThing::getField0x758()) {
            if (w1 < h1) {
                f32 tmp = UnkTextThing::getField0x758();
            } else {
                f32 tmp2 = UnkTextThing::getField0x758();
            }
        }
        f32 tmp3 = UnkTextThing::getField0x758();
        // End unused code

        f32 f7 = (fontSize.height + getTextBox()->GetLineSpace()) / 2.0f;
        ret = -(f7 * (getMaxNumLines(mMsgWindowSubtype) - v));
        if (getTextBox()->GetTextPositionV() == 2) {
            ret -=
                (size.height - (fontSize.height * getMaxNumLines(mMsgWindowSubtype) +
                                (mpTextBox->GetLineSpace() * (getMaxNumLines(mMsgWindowSubtype) - 1))));
        }
        if (ret > 0.0f) {
            ret = 0.0f;
        }
    }
    if (rem > 0) {
        ret -= rem * (fontSize.height + getTextBox()->GetLineSpace());
    }
    return ret;
}

s32 dTagProcessor_c::getMaxNumLines(s32 windowType) {
    if (windowType >= dLytMsgWindow_c::MSG_WINDOW_WOOD && windowType < dLytMsgWindow_c::MSG_WINDOW_WOOD + 2) {
        return 4;
    } else if (windowType == dLytMsgWindow_c::MSG_WINDOW_LINK) {
        return 2;
    } else if (windowType == dLytMsgWindow_c::MSG_WINDOW_DEMO) {
        return 2;
    }
    return 4;
}

void dTagProcessor_c::getTextCommand(
    wchar_t _0xe, const wchar_t *src, u8 *outCmdLen, s32 *outCmd, wchar_t **outEndPtr
) {
    *outCmdLen = READ_U16(src, 4) + 6;
    *outCmd = READ_U32(src, 0);
    if (*outCmdLen > 6) {
        *outEndPtr = (wchar_t *)src + 3;
    } else {
        *outEndPtr = nullptr;
    }
}

void dTagProcessor_c::process0xFCommand(wchar_t _0xf, const wchar_t *src, s32 *outCmd) {
    *outCmd = READ_U32(src, 0);
}

s32 dTagProcessor_c::tick0x830() {
    if (field_0x830 >= 0) {
        field_0x830--;
    }
    if (field_0x830 < -1) {
        field_0x830 = -1;
    }
    return field_0x830;
}

void dTagProcessor_c::resetSomething() {
    field_0x810 = 0.0f;
    field_0x818 = 0.0f;
    field_0x814 = 0.0f;
    field_0xEE4 = 0;
}

s32 dTagProcessor_c::tickPauseFrame() {
    if (mPauseFramesLeft > 0) {
        mPauseFramesLeft--;
    }
    if (mPauseFramesLeft < 0) {
        mPauseFramesLeft = 0;
    }
    return mPauseFramesLeft;
}

void dTagProcessor_c::resetLineData() {
    for (int i = 0; i < 0x32; i++) {
        mLineData.mLineWidths[i] = 0.0f;
    }
    mLineData.mNumLines = 0;
}

f32 dTagProcessor_c::getLineWidth(s32 i) {
    return mLineData.mLineWidths[i];
}

void dTagProcessor_c::setNumericArg0(s32 arg) {
    mNumericArgs[0] = arg;
    mFirstNumericArg = arg;
}

void dTagProcessor_c::setNumericArgs(const s32 *args, s32 numArgs) {
    // Holy unrolling, Batman!
    for (int i = 0; i < numArgs; i++) {
        mNumericArgs[i] = args[i];
    }
}

void dTagProcessor_c::executeBtnPressTimer() {
    mButtonPressAnimTimer++;
    if (mButtonPressAnimTimer >= UnkTextThing::getAnimBtnTotalDuration() * 2) {
        mButtonPressAnimTimer = 0;
    }
}

void dTagProcessor_c::executeBtnWiggleTimer() {
    mButtonWiggleAnimTimer++;
    if (mButtonWiggleAnimTimer >= UnkTextThing::getAnimBtnTotalDuration() * 4) {
        mButtonWiggleAnimTimer = 0;
    }
}

void dTagProcessor_c::execute() {
    executeBtnPressTimer();
    executeBtnWiggleTimer();
}
