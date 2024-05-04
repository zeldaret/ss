
#include <MSL_C/stdio.h>
#include <MSL_C/string.h>
#include <nw4r/db/db_directPrint.h>
#include <rvl/OS.h>
#include <rvl/VI.h>

/*

NOTE: This file does not match fully yet, Inlining + function ordering needs to still be addressed

This worked for SS: as seen in egg stuff ive worked on, not sure if it works for others ¯\_(ツ)_/¯
- Especially with inlines. DWARF provides info of local vars and maps provide calls to functions,
  but outside of that I made some guesses.

*/

namespace nw4r {
namespace db {
static FrameBufferInfo sFrameBufferInfo;
static YUVColorInfo sFrameBufferColor;
static int sInitialized = 0;

// clang-format off
static const u8 sAsciiTable[128] = {
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0xFD, 0xFE, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x29, 0x64, 0x65, 0x66, 0x2B, 0x67, 0x68, 0x25, 0x26, 0x69, 0x2A, 0x6A, 0x27, 0x2C, 0x6B,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x24, 0x6C, 0x6D, 0x6E, 0x6F, 0x28,
    0x70, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x71, 0x72, 0x73, 0x74, 0x75,
    0xFF, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B,
    0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x76, 0x77, 0x78, 0x79, 0x7A,
};

static const u32 sFontData[64] = {
    0x70871C30, 0x8988A250, 0x88808290, 0x88830C90, 0x888402F8, 0x88882210, 0x71CF9C10, 0xF9CF9C70,
    0x8208A288, 0xF200A288, 0x0BC11C78, 0x0A222208, 0x8A222208, 0x71C21C70, 0x23C738F8, 0x5228A480,
    0x8A282280, 0x8BC822F0, 0xFA282280, 0x8A28A480, 0x8BC738F8, 0xF9C89C08, 0x82288808, 0x82088808,
    0xF2EF8808, 0x82288888, 0x82288888, 0x81C89C70, 0x8A08A270, 0x920DA288, 0xA20AB288, 0xC20AAA88,
    0xA208A688, 0x9208A288, 0x8BE8A270, 0xF1CF1CF8, 0x8A28A220, 0x8A28A020, 0xF22F1C20, 0x82AA0220,
    0x82492220, 0x81A89C20, 0x8A28A288, 0x8A28A288, 0x8A289488, 0x8A2A8850, 0x894A9420, 0x894AA220,
    0x70852220, 0xF8011000, 0x08020800, 0x10840400, 0x20040470, 0x40840400, 0x80020800, 0xF8011000,
    0x70800000, 0x88822200, 0x08820400, 0x108F8800, 0x20821000, 0x00022200, 0x20800020, 0x00000000,
};

static const u32 sFontData2[77] = {
    0x51421820, 0x53E7A420, 0x014A2C40, 0x01471000, 0x0142AA00, 0x03EAA400, 0x01471A78, 0x00000000,
    0x50008010, 0x20010820, 0xF8020040, 0x20420820, 0x50441010, 0x00880000, 0x00070E00, 0x01088840,
    0x78898820, 0x004A8810, 0x788A8810, 0x01098808, 0x00040E04, 0x70800620, 0x11400820, 0x12200820,
    0x10001020, 0x10000820, 0x100F8820, 0x70000620, 0x60070000, 0x110F82A0, 0x12AA8AE0, 0x084F92A0,
    0x100FBE1C, 0x10089008, 0x60070808, 0x00000000, 0x02000200, 0x7A078270, 0x8BC81E88, 0x8A2822F8,
    0x9A282280, 0x6BC79E78, 0x30000000, 0x48080810, 0x41E80000, 0x422F1830, 0xFBE88810, 0x40288890,
    0x43C89C60, 0x81000000, 0x81000000, 0x990F3C70, 0xA10AA288, 0xE10AA288, 0xA10AA288, 0x98CAA270,
    0x00000000, 0x00000020, 0xF1EF1E20, 0x8A28A0F8, 0x8A281C20, 0xF1E80220, 0x80283C38, 0x00000000,
    0x00000000, 0x8A28B688, 0x8A2A8888, 0x8A2A8878, 0x894A8808, 0x788536F0, 0x00000000, 0x00000000,
    0xF8000000, 0x10000000, 0x20000000, 0x40000000, 0xF8000000,
};

static u32 twiceBit[4] = {0, 3, 12, 15};
// clang-format on

void DirectPrint_Init() {
    if (!sInitialized) {
        DirectPrint_ChangeXfb(NULL, 0x280, 0x1e0);
        DirectPrint_SetColor(0xFF, 0xFF, 0xFF);
        sInitialized = 1;
    }
}

bool DirectPrint_IsActive() {
    return (sInitialized && sFrameBufferInfo.frameMemory);
}

int GetDotWidth_() {
    return sFrameBufferInfo.frameWidth < 400 ? 1 : 2;
}

int GetDotHeight_() {
    return sFrameBufferInfo.frameHeight < 300 ? 1 : 2;
}

void DirectPrint_EraseXfb(int posh, int posv, int sizeh, int sizev) {
    if (!sFrameBufferInfo.frameMemory) {
        return;
    }

    if (GetDotWidth_() == 2) {
        posh *= 2;
        sizeh *= 2;
    }
    int endPosH = posh + sizeh;
    // The MIN/MAX Defines do not work due to the need for the = sign.
    posh = posh >= 0 ? posh : 0;
    endPosH = (endPosH <= sFrameBufferInfo.frameWidth ? endPosH : sFrameBufferInfo.frameWidth);
    sizeh = endPosH - posh;

    if (GetDotHeight_() == 2) {
        posv *= 2;
        sizev *= 2;
    }
    int endPosV = posv + sizev;
    posv = posv >= 0 ? posv : 0;
    endPosV = (endPosV <= sFrameBufferInfo.frameHeight ? endPosV : sFrameBufferInfo.frameHeight);
    sizev = endPosV - posv;

    u16 *pixel = ((u16 *)sFrameBufferInfo.frameMemory) + sFrameBufferInfo.frameRow * posv + posh;

    for (int i = 0; i < sizev; i++) {
        for (int j = 0; j < sizeh; j++) {
            *pixel++ = 0x1080;
        }

        pixel += sFrameBufferInfo.frameRow - sizeh;
    }
}

void DirectPrint_ChangeXfb(void *framBuf) {
    sFrameBufferInfo.frameMemory = (u8 *)framBuf;
}

void DirectPrint_ChangeXfb(void *framBuf, u16 width, u16 height) {
    sFrameBufferInfo.frameMemory = reinterpret_cast<u8 *>(framBuf);
    sFrameBufferInfo.frameWidth = width;
    sFrameBufferInfo.frameHeight = height;
    sFrameBufferInfo.frameRow = ROUND_UP(width, 16);
    sFrameBufferInfo.frameSize = sFrameBufferInfo.frameRow * sFrameBufferInfo.frameHeight * sizeof(u16);
}

void DirectPrint_SetColor(u8 r, u8 g, u8 b) {
    const int y = (0.257f * r + 0.504f * g + 0.098f * b + 16.0f);
    const int u = (-0.148f * r - 0.291f * g + 0.439f * b + 128.0f);
    const int v = (0.439f * r - 0.368f * g - 0.071f * b + 128.0f);
    sFrameBufferColor.colorRGBA.r = r;
    sFrameBufferColor.colorRGBA.g = g;
    sFrameBufferColor.colorRGBA.b = b;
    sFrameBufferColor.colorRGBA.a = 0xff;
    sFrameBufferColor.colorY256 = y << 8;
    sFrameBufferColor.colorU = u;
    sFrameBufferColor.colorU2 = u >> 1;
    sFrameBufferColor.colorU4 = u >> 2;
    sFrameBufferColor.colorV = v;
    sFrameBufferColor.colorV2 = v >> 1;
    sFrameBufferColor.colorV4 = v >> 2;
}

void DirectPrint_StoreCache() {
    DCStoreRange(sFrameBufferInfo.frameMemory, sFrameBufferInfo.frameSize);
}

void DirectPrint_Printf(int posh, int posv, const char *format, ...) {
    if (!sFrameBufferInfo.frameMemory) {
        return;
    }
    __va_list_struct list;
    va_start(list, format);
    detail::DirectPrint_DrawStringToXfb(posh, posv, format, &list, true, true);
    va_end(list);
}

// I dont know how else this couldve been written? keeping the args as variable doesnt work
void DirectPrint_printfsub(int posh, int posv, const char *format, __va_list_struct *args) {
    if (!sFrameBufferInfo.frameMemory) {
        return;
    }
    detail::DirectPrint_DrawStringToXfb(posh, posv, format, args, true, true);
}

void DirectPrint_DrawString(int posh, int posv, bool turnOver, const char *format, ...) {
    if (!sFrameBufferInfo.frameMemory) {
        return;
    }
    __va_list_struct list;

    va_start(list, format);
    detail::DirectPrint_DrawStringToXfb(posh, posv, format, &list, turnOver, false);
    va_end(list);
}

static int StrLineWidth_(const char *str) {
    int len = 0;
    do {
        int c = *str++;
        if (c == '\0' || c == '\n') {
            break;
        }
        if (c == '\t') {
            len++;
            len = ROUND_UP(len, 4);
        } else {
            len++;
        }
    } while (1);

    return len;
}

static void DrawCharToXfb_(int posh, int posv, int code);
static void DrawStringToXfb_(int posh, int posv, const char *str, bool turnOver, bool backErase);
static const char *DrawStringLineToXfb_(int posh, int posv, const char *str, int width);

static void DrawStringToXfb_(int posh, int posv, const char *str, bool turnOver, bool backErase) {
    int basePosH = posh;
    int frameWidth = sFrameBufferInfo.frameWidth;
    int width = frameWidth / GetDotWidth_();

    while (*str != '\0') {
        if (backErase) {
            int len = StrLineWidth_(str);
            DirectPrint_EraseXfb(posh - 6, posv - 3, (len + 2) * 6, 13);
        }
        str = DrawStringLineToXfb_(posh, posv, str, (width - posh) / 6);
        posv += 10;

        if (*str == '\n') {
            str++;
            posh = basePosH;
        } else if (*str != '\0') {
            str++;
            if (!turnOver) {
                str = strchr(str, '\n');
                if (str == NULL) {
                    break;
                }
                str++;
                posh = basePosH;
            } else {
                posh = 0;
            }
        }
    }
}


static const char *DrawStringLineToXfb_(int posh, int posv, const char *str, int width) {
    // Vars from DWARF info
    char c;
    int code, cnt, tab_size;

    for (cnt = 0; (c = *str) != '\0'; str++) {
        if (c == '\n' || c == '\0') {
            return str;
        }
        code = sAsciiTable[c & 0x7f];
        if (code == 0xfd) {
            tab_size = 4 - (cnt & 3);
            cnt += tab_size;
            posh += tab_size * 6;
        } else {
            if (code != 0xFF) {
                DrawCharToXfb_(posh, posv, code);
            }
            posh += 6;
            cnt++;
        }
        if (cnt >= width) {
            if (str[1] == '\n') {
                str++;
            }
            return str;
        }
    }
    return str;
}

static void DrawCharToXfb_(int posh, int posv, int code) {
    int ncode = (100 <= code) ? code - 100 : code;
    int fontv = (ncode % 5) * 6;
    int fonth = (ncode / 5) * 7;

    const u32 *fontLine = (100 > code) ? sFontData + fonth : sFontData2 + fonth;

    int wH = GetDotWidth_();
    int wV = GetDotHeight_();

    u16 *pixel =
            reinterpret_cast<u16 *>(sFrameBufferInfo.frameMemory) + sFrameBufferInfo.frameRow * posv * wV + posh * wH;

    if (posv < 0 || posh < 0) {
        return;
    }
    if (sFrameBufferInfo.frameWidth <= wH * (posh + 6) || sFrameBufferInfo.frameHeight <= wV * (posv + 7)) {
        return;
    }
    for (int cntv = 0; cntv < 7; cntv++) {
        u32 fontBits = *fontLine++ << fontv;
        if (wH == 1) {
            fontBits = (fontBits & 0xFC000000) >> 1;
        } else {
            fontBits = (twiceBit[fontBits >> 26 & 0x3] << 0 | twiceBit[fontBits >> 28 & 0x3] << 4 |
                               twiceBit[fontBits >> 30 & 0x3] << 8)
                    << 19;
        }
        for (int cnth = 0; cnth < 6 * wH; cnth += 2) {
            u16 pixColor;
            pixColor = ((fontBits & 0x40000000) ? sFrameBufferColor.colorY256 : 0x0) |
                    ((fontBits & 0x80000000) ? sFrameBufferColor.colorU4 : 0x20) +
                            ((fontBits & 0x40000000) ? sFrameBufferColor.colorU2 : 0x40) +
                            ((fontBits & 0x20000000) ? sFrameBufferColor.colorU4 : 0x20);
            pixel[0] = pixColor;
            if (wV > 1) {
                pixel[sFrameBufferInfo.frameRow] = pixColor;
            }
            pixColor = ((fontBits & 0x20000000) ? sFrameBufferColor.colorY256 : 0x0) |
                    ((fontBits & 0x40000000) ? sFrameBufferColor.colorV4 : 0x20) +
                            ((fontBits & 0x20000000) ? sFrameBufferColor.colorV2 : 0x40) +
                            ((fontBits & 0x10000000) ? sFrameBufferColor.colorV4 : 0x20);
            pixel[1] = pixColor;
            if (wV > 1) {
                pixel[sFrameBufferInfo.frameRow + 1] = pixColor;
            }
            fontBits <<= 2;
            pixel += 2;
        }
        pixel += sFrameBufferInfo.frameRow * wV - 6 * wH;
    }
}

void detail::DirectPrint_DrawStringToXfb(int posh, int posv, const char *format, __va_list_struct *args, bool turnOver,
        bool backErase) {
    // Vars from dwarf info
    char string[0x100];
    int length = vsnprintf(string, 0x100, format, args);
    int posLeftStart = posh; // Guess?? Really have no clue why else it would be used

    if (length > 0) {
        DrawStringToXfb_(posLeftStart, posv, string, turnOver, backErase);
    }
}

void detail::WaitVIRetrace_() {
    // Vars from dwarf info
    int enabled = OSEnableInterrupts();
    u32 preCnt = VIGetRetraceCount();
    do {
    } while (preCnt == VIGetRetraceCount());
    OSRestoreInterrupts(enabled);
}

void *detail::CreateFB_(const _GXRenderModeObj *rmode) {
    // Vars from dwarf info
    u32 arenaHi, memSize, frameBuf;
    arenaHi = (u32)OSGetArenaHi();
    memSize = (rmode->fbWidth + 15 & 0xFFF0) * rmode->xfbHeight * 2;
    frameBuf = (arenaHi - memSize) & 0xFFFFFFE0;
    VIConfigure(rmode);
    return (void *)frameBuf;
}

void *detail::DirectPrint_SetupFB(const _GXRenderModeObj *rmode) {
    // Vars from dwarf info
    void *frameMemory;

    DirectPrint_Init();
    frameMemory = VIGetCurrentFrameBuffer();
    if (!frameMemory) {
        if (!rmode) {
            switch ((u32)VIGetTvFormat()) {
            case 0:
                rmode = &GXNtsc480IntDf;
                break;
            case 1:
                rmode = &GXPal528IntDf;
                break;
            case 5:
                rmode = &GXEurgb60Hz480IntDf;
                break;
            case 2:
                rmode = &GXMpal480IntDf;
                break;
            default:
                break;
            }
        }
        frameMemory = CreateFB_(rmode);
        VISetNextFrameBuffer((void *)frameMemory);
    }
    VISetBlack(FALSE);
    VIFlush();
    WaitVIRetrace_();
    if (rmode) {
        DirectPrint_ChangeXfb(frameMemory, rmode->fbWidth, rmode->xfbHeight);
    } else {
        DirectPrint_ChangeXfb(frameMemory);
    }
    return frameMemory;
}

} // namespace db

} // namespace nw4r
