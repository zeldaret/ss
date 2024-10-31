#include "d/d_tag_processor.h"

#include "common.h"
#include "d/d_font_manager.h"
#include "d/d_textunk.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/ut/ut_CharWriter.h"
#include "nw4r/ut/ut_Color.h"
#include "nw4r/ut/ut_Font.h"
#include "nw4r/ut/ut_Rect.h"
#include "nw4r/ut/ut_TagProcessorBase.h"
#include "nw4r/ut/ut_TextWriterBase.h"
#include "sized_string.h"
#include "toBeSorted/file_manager.h"

#include <libc.h>

nw4r::ut::Color FontColors1[] = {
    nw4r::ut::Color(0xff, 0x4b, 0x32, 0xff),
    nw4r::ut::Color(0xff, 0x4b, 0x32, 0xff),
    nw4r::ut::Color(0xfa, 0xaf, 0x05, 0xff),
    nw4r::ut::Color(0xdc, 0xf5, 0xfa, 0xff),
    nw4r::ut::Color(0x91, 0xff, 0x6e, 0xff),
    nw4r::ut::Color(0xff, 0xbe, 0x1e, 0xff),
    nw4r::ut::Color(0xeb, 0xc8, 0xff, 0xff),
    nw4r::ut::Color(0x14, 0xd7, 0x46, 0xff),
    nw4r::ut::Color(0x19, 0xa5, 0xeb, 0xff),
    nw4r::ut::Color(0xf0, 0x3c, 0x1e, 0xff),
    nw4r::ut::Color(0xc8, 0xc8, 0xc8, 0xff),
    nw4r::ut::Color(0xfa, 0xaf, 0x05, 0xff),
    nw4r::ut::Color(0x32, 0x23, 0x46, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff),
    nw4r::ut::Color(0xf6, 0xf6, 0xde, 0xff),
    nw4r::ut::Color(0xe6, 0xe6, 0xa0, 0xff),
    nw4r::ut::Color(0xe8, 0xad, 0x55, 0xff),
    nw4r::ut::Color(0xff, 0xfa, 0xe6, 0xff),
    nw4r::ut::Color(0xf5, 0xf2, 0xeb, 0xff),
    nw4r::ut::Color(0x64, 0x5f, 0x69, 0xff),
    nw4r::ut::Color(0x5e, 0x51, 0x46, 0xff),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xfa, 0xf6, 0xe7, 0xff),
    nw4r::ut::Color(0xff, 0xff, 0xff, 0xff),
    nw4r::ut::Color(0x4c, 0x32, 0x0f, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xf6, 0xf2, 0xea, 0xff),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xff, 0xff, 0xff, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xed, 0xe2, 0xd0, 0xff),
    nw4r::ut::Color(0x20, 0x20, 0x20, 0xff),
    nw4r::ut::Color(0x74, 0x6d, 0x5c, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
};

nw4r::ut::Color FontColors2[] = {
    nw4r::ut::Color(0xff, 0x4b, 0x32, 0xff),
    nw4r::ut::Color(0xcd, 0x4b, 0x32, 0xff),
    nw4r::ut::Color(0xff, 0xe6, 0x8c, 0xff),
    nw4r::ut::Color(0x7d, 0xc3, 0xff, 0xff),
    nw4r::ut::Color(0x64, 0xc8, 0x41, 0xff),
    nw4r::ut::Color(0xff, 0xbe, 0x1e, 0xff),
    nw4r::ut::Color(0x91, 0x5f, 0xff, 0xff),
    nw4r::ut::Color(0xaf, 0xe6, 0x2d, 0xff),
    nw4r::ut::Color(0xb4, 0xe6, 0xff, 0xff),
    nw4r::ut::Color(0xff, 0x91, 0x78, 0xff),
    nw4r::ut::Color(0xe6, 0xe6, 0xc8, 0xff),
    nw4r::ut::Color(0xff, 0xe6, 0x8c, 0xff),
    nw4r::ut::Color(0x50, 0x4b, 0x5f, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff),
    nw4r::ut::Color(0xe7, 0xd7, 0xed, 0xff),
    nw4r::ut::Color(0xa5, 0x96, 0x6e, 0xff),
    nw4r::ut::Color(0xb5, 0x52, 0x2e, 0xff),
    nw4r::ut::Color(0xff, 0xfa, 0xe6, 0xff),
    nw4r::ut::Color(0xe1, 0xc8, 0x9b, 0xff),
    nw4r::ut::Color(0x55, 0x50, 0x5a, 0xff),
    nw4r::ut::Color(0x35, 0x2d, 0x27, 0xff),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xe3, 0xcc, 0x8f, 0xff),
    nw4r::ut::Color(0xff, 0xff, 0xff, 0xff),
    nw4r::ut::Color(0x3a, 0x25, 0x09, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xe1, 0xc7, 0x9b, 0xff),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xff, 0xff, 0xff, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(0xca, 0xaf, 0x81, 0xff),
    nw4r::ut::Color(0x20, 0x20, 0x20, 0xff),
    nw4r::ut::Color(0x4b, 0x47, 0x3f, 0xff),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
    nw4r::ut::Color(),
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
    resetSomeFloats();
    field_0x81C = 0;
    field_0x820 = 0;
    setNumericArg0(0);
    mNumericArgsCopy[1] = 0;
    mNumericArgsCopy[2] = 0;
    mNumericArgsCopy[0] = 0;
    mNumericArgsCopy[3] = 0;
    mNumericArgsCopy[4] = 0;
    mNumericArgsCopy[5] = 0;
    mNumericArgsCopy[6] = 0;
    mNumericArgsCopy[7] = 0;
    mNumericArgsCopy[8] = 0;
    mNumericArgsCopy[9] = 0;
    field_0x8EC = 0;
    field_0x8F0 = 0;
    field_0x8F4 = 0;
    field_0x8F8 = 0;

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
    field_0x004 = nullptr;
    field_0x90C = 0x24;
    field_0x90D = 4;
    field_0xEF0 = 0;
    field_0xEF1 = 0;
}

dTagProcessor_c::~dTagProcessor_c() {}

struct StackThing {
    wchar_t us[4];
};

void dTagProcessor_c::eventFlowTextProcessingRelated(
    dTextBox_c *textBox, const wchar_t *src, wchar_t *dest, u32 destLen, u32 *pOutLen
) {
    s32 state3 = 0;
    s32 state4 = 0;

    s32 state1 = -1;
    s32 state2 = -1;

    // FPR regswap between float1 and float2
    f32 float1 = fn_800B8040(0, field_0x90C);
    f32 float2 = float1;
    if (textBox != nullptr) {
        float2 *= textBox->getMyScale();
        resetSomeFloats();
        textBox->set0x1F8(0);
    }

    s32 local_b4 = 0;

    StackThing x = {{0x0F0F, 0x000E, 0x0002, 0x0F0F}};

    wchar_t *writePtr = dest;
    if (textBox != nullptr) {
        writePtr += 5;
        dest[0] = x.us[0];
        dest[1] = x.us[1];
        dest[2] = x.us[2];
        dest[3] = x.us[3];
        dest[4] = mCommandInsert;
    }

    StackThing yTmp;
    StackThing y = {0x0F0F, 0x000E, 0x0002, 0x0F0F};

    do {
        wchar_t c = *src;
        if (c == nullptr) {
            if (textBox != nullptr) {
                mCommandInsert++;
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
                            field_0x008[field_0x90E - 1][local_b4] = *(t++);
                            if (field_0x90E - 1 < 4) {
                                field_0x808[field_0x90E - 1]++;
                            }
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
                    // This pattern is too efficient and needs
                    // to use 1 more reg
                    u8 a = ((u8 *)endPtr)[0];
                    u8 b = ((u8 *)endPtr)[1];
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
                    u8 a = ((u8 *)endPtr)[0];
                    u8 b = ((u8 *)endPtr)[1];
                    switch (a) {
                        case 0: state2 = 1; break;
                        case 1: state1 = 1; break;
                    }
                    field_0x90F[1] = b;
                    state3 = 2;
                    bVar3 = true;
                } break;
                case 0x10002: {
                    u8 a = ((u8 *)endPtr)[0];
                    u8 b = ((u8 *)endPtr)[1];
                    switch (a) {
                        case 0: state2 = 2; break;
                        case 1: state1 = 2; break;
                    }
                    field_0x90F[2] = b;
                    state3 = 3;
                    bVar3 = true;
                } break;
                case 0x10003: {
                    u8 a = ((u8 *)endPtr)[0];
                    u8 b = ((u8 *)endPtr)[1];
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
                        float2 = fn_800B8040(((u8 *)endPtr)[0], field_0x90C);
                        float2 *= textBox->getMyScale();
                    }
                    writePtr = writeTextNormal(src, writePtr, &local_b4, cmdLen, state4);
                    break;
                case 0x30000: {
                    f32 tmp = float2;
                    if (textBox != nullptr) {
                        float1 = float2;
                        tmp *= UnkTextThing::getField0x768();
                        tmp *= textBox->getMyScale();
                    }
                    writePtr = writeTextNormal(src, writePtr, &local_b4, cmdLen, state4);
                    float2 = tmp;
                } break;
                case 0x10010: fn_800B5520(endPtr); break;
                case 0x20004:
                    if (textBox != nullptr) {
                        fn_800B6320(textBox, endPtr, float2);
                    }
                    writePtr = writeTextNormal(src, writePtr, &local_b4, cmdLen, state4);
                    break;
                case 0x20000: writePtr = fn_800B5570(writePtr, &local_b4, state4); break;
                case 0x20001: writePtr = fn_800B5680(writePtr, endPtr, &local_b4, state4); break;
                case 0x20002: writePtr = fn_800B5860(writePtr, endPtr, &local_b4, state4); break;
                case 0x20003: writePtr = fn_800B5A20(writePtr, endPtr, &local_b4, state4); break;

                case 0x30004: writePtr = fn_800B5DD0(writePtr, endPtr, &local_b4, state4); break;
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
                float2 = float1;
            }
            writePtr[0] = src[0];
            writePtr[1] = src[1];
            writePtr[2] = src[2];
            writePtr += 3;
            src += 3;
        } else if (state4 != 0 && field_0x90E != 0) {
            // Note: Return ignored here
            fn_800B5FD0(c, &field_0x008[field_0x90E - 1][local_b4], &local_b4);
            src++;
            // This looks like an inline tbh
            if (field_0x90E - 1 < 4) {
                field_0x808[field_0x90E - 1]++;
            }
        } else {
            if (textBox != nullptr) {
                if (c == 10) {
                    *writePtr = c;
                    src++;
                    writePtr++;
                    mCommandInsert++;
                    s32 i10 = getNumLines(field_0x90C);
                    if (mCommandInsert % i10 == 0) {
                        float2 = fn_800B8040(0, field_0x90C);
                        float2 *= textBox->getMyScale();
                    }
                    if (textBox != nullptr) {
                        wchar_t *buf = (wchar_t *)&x;
                        writePtr[0] = buf[0];
                        writePtr[1] = buf[1];
                        writePtr[2] = buf[2];
                        writePtr[3] = buf[3];
                        writePtr[4] = mCommandInsert;
                        writePtr += 5;
                    }
                } else {
                    const nw4r::ut::Font *fnt = textBox->GetFont();
                    field_0x914[mCommandInsert] += float2 * fnt->GetCharWidth(*src) + textBox->GetCharSpace();
                    writePtr = fn_800B5FD0(*src, writePtr, nullptr);
                    src++;
                }

            } else {
                writePtr = fn_800B5FD0(c, writePtr, nullptr);
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
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEED == 0 && field_0x8F0 == field_0x838) {
                    setFramesLeftOnPause(rect, ctx, cmdLen, endPtr);
                    field_0xEED = 1;
                    field_0x838++;
                }
                field_0x8F0++;
            }
            break;
        case 0x10005:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEE == 0 && field_0x8F4 == field_0x83C) {
                    fn_800B5500(cmdLen, endPtr);
                    field_0xEEE = 1;
                    field_0x83C++;
                }
                field_0x8F4++;
            }
            break;
        case 0x10007:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEE5 == 0 && mNumericArgsCopy[3] == field_0x87C) {
                    fn_800B60E0(cmdLen, endPtr);
                    field_0xEE5 = 1;
                    field_0x87C++;
                }
                mNumericArgsCopy[3]++;
            }
            break;
        case 0x10008: fn_800B61D0(rect, ctx, cmdLen, endPtr); break;
        case 0x30000: changeScale(rect, ctx, false); break;
        case 0x10009:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEE6 == 0 && mNumericArgsCopy[4] == field_0x880) {
                    fn_800B6110(cmdLen, endPtr);
                    field_0xEE6 = 1;
                    field_0x880++;
                }
                mNumericArgsCopy[4]++;
            }
            break;
        case 0x1000A:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEE7 == 0 && mNumericArgsCopy[5] == field_0x884) {
                    fn_800B6140(cmdLen, endPtr);
                    field_0xEE7 = 1;
                    field_0x884++;
                }
                mNumericArgsCopy[5]++;
            }
            break;
        case 0x1000B:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEE8 == 0 && mNumericArgsCopy[6] == field_0x888) {
                    fn_800B6160(cmdLen, endPtr);
                    field_0xEE8 = 1;
                    field_0x888++;
                }
                mNumericArgsCopy[6]++;
            }
            break;
        case 0x1000C:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEA == 0 && mNumericArgsCopy[8] == field_0x890) {
                    fn_800B6170(cmdLen, endPtr);
                    field_0xEEA = 1;
                    field_0x890++;
                }
                mNumericArgsCopy[8]++;
            }
            break;
        case 0x1000D:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEB == 0 && mNumericArgsCopy[9] == field_0x894) {
                    fn_800B6190(cmdLen, endPtr);
                    field_0xEEB = 1;
                    field_0x894++;
                }
                mNumericArgsCopy[9]++;
            }
            break;
        case 0x1000E:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEC == 0 && field_0x8EC == field_0x898) {
                    fn_800B61B0(cmdLen, endPtr);
                    field_0xEEC = 1;
                    field_0x898++;
                }
                field_0x8EC++;
            }
            break;
        case 0x20004:
            if (field_0xEE3 != 0) {
                if (rect == nullptr) {
                    fn_800B6450(rect, ctx, cmdLen, endPtr);
                } else {
                    fn_800B6EE0(rect, ctx, endPtr);
                }
            }
            break;
        case 0x10011:
            if (rect == nullptr && field_0xEE1 == 0) {
                if (field_0xEEF == 0 && field_0x8F8 == field_0x840) {
                    fn_800B5540(endPtr);
                    field_0xEEF = 1;
                    field_0x840++;
                }
                field_0x8F8++;
            }
            break;
    }
    ctx->str += cmdLen / 2;
    return nw4r::ut::OPERATION_DEFAULT;
}

void dTagProcessor_c::fn_800B4FF0(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr) {
    if (field_0x90C != 22 && field_0x90C != 30 && field_0xEE0 != 0) {
        int arg = ptr[0];
        nw4r::lyt::Size textBoxSize = field_0x004->GetSize();
        nw4r::lyt::Size fontSize = field_0x004->GetFontSize();
        int i1 = getNumLines(field_0x90C);
        if (arg % i1 == 0 && field_0x90C != 31 && field_0x90C != 8) {
            int u = 0;
            int v = 0;
            for (int i = arg; i < arg + getNumLines(field_0x90C) && i < 0x32; i++) {
                f32 f6 = getFloat(i);
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

            // Unfortunately, the code then proceeds to access
            // some global text stuff at 0x805753b0, which is
            // completely hidden in the Ghidra decompiler, apparently
            // because the results aren't used?

            if (u < getNumLines(field_0x90C)) {
                f32 w1 = fontSize.width * 0.5f;
                f32 h1 = fontSize.height * 0.5f;
                if (!(w1 < UnkTextThing::getField0x758())) {
                    w1 = UnkTextThing::getField0x758();
                }
                if (!(h1 < UnkTextThing::getField0x758())) {
                    h1 = UnkTextThing::getField0x758();
                }

                f32 f7 = (fontSize.height + field_0x004->GetLineSpace()) * 0.5f * (getNumLines(field_0x90C) - v);
                field_0x814 = ctx->writer->GetCursorY();
                field_0x818 = ctx->y;
                field_0xEE4 = 1;
                if (f7 > 0.0f) {
                    ctx->writer->SetCursorY(ctx->writer->GetCursorY() + f7);
                    field_0x810 = f7;
                }
            }
        }

        f32 f7 = getFloat(arg);
        f32 f8 = (textBoxSize.width - f7) * 0.5f;
        if ((field_0x90C < 6 || field_0x90C >= 9) && field_0x90C != 30) {
            f8 = 0.0f;
        }
        if (f8 > 0.0f) {
            ctx->writer->SetCursorX(ctx->writer->GetCursorX() + f8);
        }
    }
}

void dTagProcessor_c::setColor(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *buf) {
    u16 cmd = buf[0];
    if (cmd == 0xFFFF) {
        restoreColor(ctx, field_0x90C);
        return;
    }
    nw4r::ut::Color c1 = FontColors1[cmd & 0xFFFF];
    nw4r::ut::Color c2 = FontColors2[cmd & 0xFFFF];
    if (cmd == 0) {
        if (field_0x90C == 2) {
            c1.r = 0xFF;
            c1.g = 0x6E;
            c1.b = 0x64;
            c2.r = 0xFF;
            c2.g = 0x6E;
            c2.b = 0x64;
        } else if (field_0x90C == 7) {
            c1.r = 0xE6;
            c1.g = 0x4B;
            c1.b = 0x32;
            c2.r = 0xE6;
            c2.g = 0x4B;
            c2.b = 0x32;
        }
    } else if (cmd == 1) {
        if (field_0x90C == 2) {
            c1.r = 0xF5;
            c1.g = 0x64;
            c1.b = 0x5A;
            c2.r = 0xC8;
            c2.g = 0x64;
            c2.b = 0x5A;
        } else if (field_0x90C == 7) {
            c1.r = 0xB4;
            c1.g = 0x50;
            c1.b = 0x50;
            c2.r = 0x8C;
            c2.g = 0x40;
            c2.b = 0x40;
        }
    } else if (cmd == 3 && field_0x90C >= 2 && field_0x90C < 5) {
        c1.r = 0x50;
        c1.g = 0xE6;
        c1.b = 0xFA;
        c2.r = 0xC8;
        c2.g = 0xE6;
        c2.b = 0xF5;
    }

    u8 u5 = 0xFF;
    if (field_0x004 != nullptr) {
        u5 = field_0x004->GetGlobalAlpha();
    }

    c2.a = u5;
    c1.a = u5;
    ctx->writer->SetGradationMode(nw4r::ut::CharWriter::GRADMODE_V);
    ctx->writer->SetTextColor(c1, c2);
}

void dTagProcessor_c::setScale(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *buf) {
    u16 scale = buf[0];
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
    mPauseFramesLeft = ptr[0];
}

void dTagProcessor_c::fn_800B5500(u8 cmdLen, wchar_t *ptr) {
    u16 val = ptr[0];
    if (field_0x830 == -1) {
        field_0x830 = val;
    }
}

void dTagProcessor_c::fn_800B5520(wchar_t *src) {
    field_0x81C = ((u32 *)src)[0];
    field_0x820 = ((u32 *)src)[1];
}

wchar_t *dTagProcessor_c::fn_800B5570(wchar_t *dest, s32 *outArg, s32 arg) {
    if (FileManager::sInstance->getHeroname()[0] != '\0') {
        for (int i = 0; FileManager::sInstance->getHeroname()[i] != '\0'; i++) {
            if (arg != 0 && field_0x90E != 0) {
                wchar_t c = FileManager::sInstance->getHeroname()[i];
                fn_800B5FD0(c, &field_0x008[field_0x90E - 1][*outArg], outArg);
                if (field_0x90E - 1 < 4) {
                    field_0x808[field_0x90E - 1]++;
                }
            } else {
                dest = fn_800B5FD0(FileManager::sInstance->getHeroname()[i], dest, nullptr);
            }
        }
    }
    return dest;
}

void dTagProcessorDataStuff() {
    SizedString<32> s;
    s.sprintf("NAME_ITEM_%03d");
    s.sprintf("lang:word:%03d:%02d");
}

void dTagProcessor_c::fn_800B60E0(u8 cmdLen, wchar_t *ptr) {
    field_0x844 = *((u8 *)ptr);
    field_0x848 = *((u8 *)ptr + 1);
    field_0x84C = *((u8 *)ptr + 2);
    field_0x850 = *((u8 *)ptr + 3);
}
void dTagProcessor_c::fn_800B6110(u8 cmdLen, wchar_t *ptr) {
    field_0x854 = *((u8 *)ptr);
    field_0x858 = *((u8 *)ptr + 1);
    field_0x85C = *((u8 *)ptr + 2);
    field_0x860 = *((u8 *)ptr + 3);
}
void dTagProcessor_c::fn_800B6140(u8 cmdLen, wchar_t *ptr) {
    field_0x864 = ptr[0];
    field_0x868 = ptr[1];
}
void dTagProcessor_c::fn_800B6160(u8 cmdLen, wchar_t *ptr) {}
void dTagProcessor_c::fn_800B6170(u8 cmdLen, wchar_t *ptr) {
    field_0x8FC = ptr[0];
    field_0x900 = ptr[1];
}
void dTagProcessor_c::fn_800B6190(u8 cmdLen, wchar_t *ptr) {
    field_0x86C = *((char *)ptr);
    field_0x870 = *((char *)ptr + 1);
}
void dTagProcessor_c::fn_800B61B0(u8 cmdLen, wchar_t *ptr) {
    field_0x874 = *((char *)ptr);
    field_0x878 = *((s16 *)((char *)ptr + 1));
}

void dTagProcessor_c::fn_800B61D0(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr) {
    f32 scale = fn_800B8040(*(char *)ptr, field_0x90C);
    if (field_0x004 != nullptr) {
        scale *= field_0x004->getMyScale();
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

void dTagProcessor_c::fn_800B6EE0(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, wchar_t *ptr) {
    nw4r::ut::Font *f = dFontMng_c::getFont(4);
    u32 c3 = fn_800B7880(*(u8 *)ptr);
    nw4r::ut::TextWriterBase<wchar_t> copy = *ctx->writer;
    f32 charWidth = f->GetCharWidth(c3);
    fn_800B70D0(&copy, ctx, c3, 0xFF);
    ctx->writer->MoveCursorX(charWidth * copy.GetScaleH());

    f32 cursorX = ctx->writer->GetCursorX();
    rect->left = rect->left > cursorX ? cursorX : rect->left;
    rect->right = rect->right < cursorX ? cursorX : rect->right;
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
        case 5:  colorIndex = 19; break;
        case 6:  colorIndex = 20; break;
        case 7:  colorIndex = 21; break;
        case 8:  colorIndex = 22; break;
        case 9:  colorIndex = 24; break;
        case 11: colorIndex = 25; break;
        case 12: colorIndex = 26; break;
        case 13: colorIndex = 30; break;
        case 17: colorIndex = 31; break;
        case 18: colorIndex = 32; break;
        case 19: colorIndex = 43; break;
        case 20: colorIndex = 33; break;
        case 22: colorIndex = 35; break;
        case 23: colorIndex = 36; break;
        case 24: colorIndex = 37; break;
        case 27: colorIndex = 40; break;
        case 29: colorIndex = 42; break;
        case 30: colorIndex = 44; break;
        case 31: colorIndex = 18; break;
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
    if (field_0x004 != nullptr) {
        u5 = field_0x004->GetGlobalAlpha();
    }

    c1.a = c1.a * u5 / 255.0f;
    c2.a = c2.a * u5 / 255.0f;
    ctx->writer->SetGradationMode(nw4r::ut::CharWriter::GRADMODE_V);
    ctx->writer->SetTextColor(c1, c2);
}

wchar_t *dTagProcessor_c::writeTextNormal(const wchar_t *src, wchar_t *dest, s32 *pArg, u8 cmdLen, s32 arg) {
    if (arg != 0 && field_0x90E != 0) {
        for (u32 i = 0; i < (cmdLen / 2 + 1); i++) {
            field_0x008[field_0x90E - 1][*pArg] = *(src++);
            if (field_0x90E - 1 < 4) {
                field_0x808[field_0x90E - 1]++;
            }
            (*pArg)++;
        }
    } else {
        memcpy(dest, src, cmdLen + 2);
        dest += (cmdLen / 2 + 1);
    }

    return dest;
}

u8 dTagProcessor_c::symbolToFontIdx(s32 s) {
    static const u8 alphabet[] = " !\"#$%&\'()*+,)+-/0123456789";
    return alphabet[s];
}

void dTagProcessor_c::setStringArg(const wchar_t *arg, s32 index) {
    for (int i = 0; i < 0x40; i++) {
        mStringArgs[index][i] = arg[i];
    }
}

f32 dTagProcessor_c::fn_800B8040(s8 factor, u32 windowType) {
    // Fun little recursion here
    f32 f1 = UnkTextThing::getFn800B1F70();
    if (windowType == 6) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.93f);
    } else if (windowType == 7) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.93f);
    } else if (windowType == 9) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.93f);
    } else if (windowType == 30) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.93f);
    } else if (windowType == 34) {
        f32 f2 = fn_800B8040(0, 0);
        f32 f3 = fn_800B8040(factor, 0);
        return f1 * ((f3 / f2) * 0.93f);
    } else {
        f32 x;
        switch (factor) {
            case -2: x = 0.55f; break;
            case -1: x = 0.55f; break;
            case -0: x = 0.55f; break;
            case 1:  x = 0.55f; break;
            case 2:
                x = 0.55f;
                break;
                // @bug: No default, so uninitialized is possible
        }
        x *= UnkTextThing::getFn800B1F10();
        return x * f1;
    }
}

s32 dTagProcessor_c::getNumLines(s32 arg) {
    if (arg >= 6 && arg < 8) {
        return 4;
    } else if (arg == 9) {
        return 2;
    } else if (arg == 30) {
        return 2;
    }
    return 4;
}

void dTagProcessor_c::getTextCommand(
    wchar_t _0xe, const wchar_t *src, u8 *outCmdLen, s32 *outCmd, wchar_t **outEndPtr
) {
    *outCmdLen = src[2] + 6;
    *outCmd = *(s32 *)src;
    if (*outCmdLen > 6) {
        *outEndPtr = (wchar_t *)src + 3;
    } else {
        *outEndPtr = nullptr;
    }
}

void dTagProcessor_c::process0xFCommand(wchar_t _0xf, const wchar_t *src, s32 *outCmd) {
    *outCmd = *(s32 *)src;
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

void dTagProcessor_c::resetSomeFloats() {
    for (int i = 0; i < 0x32; i++) {
        field_0x914[i] = 0.0f;
    }
    mCommandInsert = 0;
}

f32 dTagProcessor_c::getFloat(s32 i) {
    return field_0x914[i];
}

void dTagProcessor_c::setNumericArg0(s32 arg) {
    mNumericArgs[0] = arg;
    mNumericArgsCopy[0] = arg;
}

void dTagProcessor_c::setNumericArgs(const s32 *args, s32 numArgs) {
    // Holy unrolling, Batman!
    for (int i = 0; i < numArgs; i++) {
        mNumericArgs[i] = args[i];
    }
}
