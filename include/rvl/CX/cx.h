#pragma once

#include "types.h"

// .text: [0x803cee90, 0x803d0b20]

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CXUncompContextLZ {
    u8 _[0x18];
} CXUncompContextLZ;

typedef struct CXUncompContextRL {
    u8 _[0x10];
} CXUncompContextRL;

typedef struct CXUncompContextLH {
    // u8 _[0x8a8];
    // taken from xenoblade
	u8* destp;
	s32 destCount;
	s32 forceDestCount;
	u16 huffTable9[1 << (9 + 1)];
	u16 huffTable12[1 << (5 + 1)];
	u16* nodep;
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
/* 803cee90 */ void CXInitUncompContextRL();
/* 803ceec0 */ void CXInitUncompContextLZ();
/* 803cef00 */ void CXReadUncompRL();
/* 803cf1b0 */ void CXReadUncompLZ();
/* 803cf570 */ void CXInitUncompContextLH();
/* 803cf5b0 */ void CXReadUncompLH();
/* 803cfda0 */ void RCAddCount_();
/* 803cff90 */ void RCSearch_();
/* 803d0030 */ void RCGetData_();
/* 803d0140 */ void CXInitUncompContextLRC();
/* 803d0390 */ void CXReadUncompLRC();
/* 803d0790 */ void CXGetUncompressedSize();
/* 803d07d0 */ void CXUncompressLZ();
/* 803d0920 */ void CXiLHVerifyTable();


#ifdef __cplusplus
}
#endif