#include "d/d_tag_processor.h"

#include "common.h"
#include "nw4r/ut/ut_Color.h"
#include "nw4r/ut/ut_Rect.h"
#include "nw4r/ut/ut_TagProcessorBase.h"

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
    field_0x004 = 0;
    field_0x90C = 0x24;
    field_0x90D = 4;
    field_0xEF0 = 0;
    field_0xEF1 = 0;
}
dTagProcessor_c::~dTagProcessor_c() {}

nw4r::ut::Operation dTagProcessor_c::Process(u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) {
    return ProcessTags(nullptr, ch, ctx);
}

nw4r::ut::Operation dTagProcessor_c::CalcRect(nw4r::ut::Rect *rect, u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) {
    return ProcessTags(rect, ch, ctx);
}

nw4r::ut::Operation dTagProcessor_c::ProcessTags(nw4r::ut::Rect *rect, u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) {
    // Function is largely OK, but the switch below uses a slightly different partition
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
    const wchar_t *endPtr = nullptr;
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
        case 0x0F0F0F10:
            fn_800B4FF0(rect, ctx, cmdLen, endPtr);
            break;
        case 0x0F0F0F0E:
            field_0xEE2 = 1;
            break;
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
        case 0x10008:
            fn_800B61D0(rect, ctx, cmdLen, endPtr);
            break;
        case 0x30000:
            changeScale(rect, ctx, false);
            break;
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

void dTagProcessor_c::setStringArg(const wchar_t *arg, s32 index) {
    for (int i = 0; i < 0x40; i++) {
        mStringArgs[index][i] = arg[i];
    }
}

u8 dTagProcessor_c::symbolToFontIdx(s32 s) {
    static const u8 alphabet[] = " !\"#$%&\'()*+,)+-/0123456789";
    return alphabet[s];
}

u32 dTagProcessor_c::mapSomething(s32 arg) {
    // What is this pattern?
    if ((u32)arg - 6 <= 1) {
        return 4;
    } else if (arg == 9) {
        return 2;
    } else if (arg == 30) {
        return 2;
    }
    return 4;
}

void dTagProcessor_c::getTextCommand(
    wchar_t _0xe, const wchar_t *src, u8 *outCmdLen, s32 *outCmd, wchar_t const **outEndPtr
) {
    *outCmdLen = src[2] + 6;
    *outCmd = *(s32 *)src;
    if (*outCmdLen > 6) {
        *outEndPtr = src + 3;
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
