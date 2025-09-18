#ifndef RVL_CX_H
#define RVL_CX_H

#include "common.h"

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

void CXInitUncompContextRL(CXUncompContextRL *);
void CXInitUncompContextLZ(CXUncompContextLZ *);
s32 CXReadUncompRL(CXUncompContextRL *, const void *, u32);
s32 CXReadUncompLZ(CXUncompContextLZ *, const void *, u32);
void CXInitUncompContextLH(CXUncompContextLH *);
s32 CXReadUncompLH(CXUncompContextLH *, const void *, u32);
void RCAddCount_();
void RCSearch_();
void RCGetData_();
void CXInitUncompContextLRC(CXUncompContextLRC *);
s32 CXReadUncompLRC(CXUncompContextLRC *, const void *, u32);
u32 CXGetUncompressedSize(const void *);
void CXUncompressLZ();
void CXiLHVerifyTable();

#ifdef __cplusplus
}
#endif

#endif
