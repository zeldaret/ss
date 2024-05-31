#ifndef NW4R_DB_DIRECT_PRINT_H
#define NW4R_DB_DIRECT_PRINT_H

#include <Runtime.PPCEABI.H/__va_arg.h>
#include <common.h>
#include <rvl/GX.h>

namespace nw4r {

namespace db {

struct FrameBufferInfo {
    /* 0x00 */ u8 *frameMemory;
    /* 0x04 */ u32 frameSize;
    /* 0x08 */ u16 frameWidth;
    /* 0x0A */ u16 frameHeight;
    /* 0x0C */ u16 frameRow;
    /* 0x0E */ u16 reserved;
};

struct YUVColorInfo {
    /* 0x00 */ GXColor colorRGBA;
    /* 0x04 */ u16 colorY256;
    /* 0x06 */ u16 colorU;
    /* 0x08 */ u16 colorU2;
    /* 0x0A */ u16 colorU4;
    /* 0x0C */ u16 colorV;
    /* 0x0E */ u16 colorV2;
    /* 0x10 */ u16 colorV4;
    /* 0x12 */ u16 reserved;
};

/* 804342a0 */ void DirectPrint_Init();
/* 80434360 */ bool DirectPrint_IsActive();
/* 80434390 */ void DirectPrint_EraseXfb(int posh, int posv, int sizeh, int sizev);
/*          */ void DirectPrint_ChangeXfb(void *framBuf);
/* 80434520 */ void DirectPrint_ChangeXfb(void *framBuf, u16 width, u16 height);
/* 80434560 */ void DirectPrint_StoreCache();
/* 80434580 */ void DirectPrint_Printf(int posh, int posv, const char *format, ...);
/* 80434650 */ void DirectPrint_printfsub(int posh, int posv, const char *format,
        __va_list_struct *args); // ????? Not from a symbol, needs the arg?
/*          */ void DirectPrint_Printf(int posh, int posv, bool turnOver, const char *format, ...);
/*          */ void DirectPrint_DrawString(int posh, int posv, const char *format, ...);
/* 804346c0 */ void DirectPrint_DrawString(int posh, int posv, bool turnOver, const char *format, ...);
/*          */ void DirectPrint_SetColor(GXColor color);
/* inlined  */ void DirectPrint_SetColor(u8 r, u8 g, u8 b);
/*          */ GXColor DirectPrint_GetColor();
namespace detail {
/* inlined  */ void DirectPrint_DrawStringToXfb(int posh, int posv, const char *format, __va_list_struct *args,
        bool turnover, bool backErase);

/* local    */ void WaitVIRetrace_();

/* local    */ void *CreateFB_(const _GXRenderModeObj *rmode);
/* 80434cb0 */ void *DirectPrint_SetupFB(const _GXRenderModeObj *rmode);

} // namespace detail

} // namespace db

} // namespace nw4r
#endif
