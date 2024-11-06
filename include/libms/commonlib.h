#ifndef LIBMS_COMMONLIB_H
#define LIBMS_COMMONLIB_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct MsbBlockHeader {
    /* 0x00 */ char name[4];
    /* 0x04 */ int sectionLength;
    /* 0x08 */ unsigned short field_0x08;
    /* 0x0A */ unsigned char field_0x0A[0x10 - 0x0A];
    /* 0x10 */ int numEntries;
};

struct MsbBlock {
    /* 0x00 */ int *ptr;
    /* 0x04 */ char name[4];
    /* 0x08 */ int sectionLength;
    /* 0x0C */ u16 field_0x0C;
};

#pragma push
#pragma pack(2)

struct MsbHeader {
    /* 0x00 */ unsigned char field_0x00[0x0C - 0x00];
    /* 0x0C */ unsigned char version;
    /* 0x0E */ unsigned short sectionCount;
    /* 0x10 */ unsigned char field_0x10[0x12 - 0x10];
    /* 0x12 */ int fileLength;
};

#pragma pop

struct MsbInfo {
    /* 0x00 */ struct MsbHeader *header;
    /* 0x04 */ int fileLength;
    /* 0x08 */ unsigned char version;
    /* 0x0A */ unsigned short sectionCount;
    /* 0x0C */ struct MsbBlock *sectionInfos;
};

void LMSi_AnalyzeMessageHeader(struct MsbInfo *info);
void LMSi_AnalyzeMessageBlocks(struct MsbInfo *info);
void LMSi_AnalyzeMessageBinary(struct MsbInfo *info, const char *type);
int LMSi_SearchBlockByName(struct MsbInfo *info, const char *name);
int LMSi_GetHashTableIndexFromLabel(const char *label, int tableSize);

#ifdef __cplusplus
}
#endif

#endif
