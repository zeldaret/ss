#ifndef RVL_CX_H
#define RVL_CX_H

#include <common.h>

// .text: [0x803cee90, 0x803d0b20]

#ifdef __cplusplus
extern "C" {
#endif

enum CXReadResult {
    CX_READ_ERR_4 = -4,
    CX_READ_ERR_3 = -3,
    CX_READ_ERR_2 = -2,
    CX_READ_ERR_1 = -1,
    CX_READ_OK = 0,
};

typedef struct CXUncompContextLZ {
    u8 _[0x18];
} CXUncompContextLZ;

typedef struct CXUncompContextRL {
    u8 _[0x10];
} CXUncompContextRL;

typedef struct CXUncompContextLH {
    // u8 _[0x8a8];
    // taken from xenoblade
    u8 *destp;
    s32 destCount;
    s32 forceDestCount;
    u16 huffTable9[1 << (9 + 1)];
    u16 huffTable12[1 << (5 + 1)];
    u16 *nodep;
    s32 tableSize9;
    s32 tableSize12;
    u32 tableIdx;
    u32 stream;
    u32 stream_len;
    u16 length;
    s8 offset_bits;
    u8 headerSize;
} CXUncompContextLH;

typedef struct CXUncompContextLRC {
    u8 _[0x902C];
} CXUncompContextLRC;

// TODO(Zeldex) : Fix Params
/* 803cee90 */ void CXInitUncompContextRL(CXUncompContextRL *);
/* 803ceec0 */ void CXInitUncompContextLZ(CXUncompContextLZ *);
/* 803cef00 */ s32 CXReadUncompRL(CXUncompContextRL *, const void *, u32);
/* 803cf1b0 */ s32 CXReadUncompLZ(CXUncompContextLZ *, const void *, u32);
/* 803cf570 */ void CXInitUncompContextLH(CXUncompContextLH *);
/* 803cf5b0 */ s32 CXReadUncompLH(CXUncompContextLH *, const void *, u32);
/* 803cfda0 */ void RCAddCount_();
/* 803cff90 */ void RCSearch_();
/* 803d0030 */ void RCGetData_();
/* 803d0140 */ void CXInitUncompContextLRC(CXUncompContextLRC *);
/* 803d0390 */ s32 CXReadUncompLRC(CXUncompContextLRC *, const void *, u32);
/* 803d0790 */ u32 CXGetUncompressedSize(const void *);
/* 803d07d0 */ void CXUncompressLZ();
/* 803d0920 */ void CXiLHVerifyTable();

#ifdef __cplusplus
}
#endif

#endif
