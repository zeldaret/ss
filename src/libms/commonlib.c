#include "libms/commonlib.h"

#include "libms/libms.h"

void LMSi_AnalyzeMessageHeader(struct MsbInfo *info) {
    info->version = info->header->version;
    info->sectionCount = info->header->sectionCount;
    if (info->sectionCount == 0) {
        info->sectionInfos = NULL;
    } else {
        info->sectionInfos = (struct MsbBlock *)LMSi_Malloc(sizeof(struct MsbBlock) * info->sectionCount);
    }
    info->fileLength = info->header->fileLength;
}

// Complete fakematch here but hey, it works
inline static struct MsbBlockHeader *GetBlockHeader(struct MsbInfo *info, int offset) {
    return (struct MsbBlockHeader *)((unsigned int)info->header + offset);
}

inline static struct MsbBlockHeader *GetBlockHeader2(struct MsbInfo *info, int offset) {
    return (struct MsbBlockHeader *)(offset + (unsigned int)info->header);
}

void LMSi_AnalyzeMessageBlocks(struct MsbInfo *info) {
    int i;
    int offset;
    struct MsbBlock *section;

    offset = ROUND_UP(sizeof(struct MsbHeader), 0x20); // file header
    for (i = 0; i < info->sectionCount; i++) {
        section = &info->sectionInfos[i];
        section->ptr = &GetBlockHeader(info, offset)->numEntries;
        section->name[0] = GetBlockHeader(info, offset)->name[0];
        section->name[1] = GetBlockHeader2(info, offset)->name[1];
        section->name[2] = GetBlockHeader2(info, offset)->name[2];
        section->name[3] = GetBlockHeader2(info, offset)->name[3];
        section->sectionLength = GetBlockHeader2(info, offset)->sectionLength;
        section->field_0x0C = GetBlockHeader2(info, offset)->field_0x08;
        offset = offset + 0x10;                   // section header
        offset = offset + section->sectionLength; // section
        offset = ROUND_UP(offset, 0x10);          // align
    }
}

void LMSi_AnalyzeMessageBinary(struct MsbInfo *info, const char *type) {
    LMSi_AnalyzeMessageHeader(info);
    LMSi_AnalyzeMessageBlocks(info);
}

int LMSi_SearchBlockByName(struct MsbInfo *info, const char *name) {
    for (unsigned short i = 0; i < info->sectionCount; i++) {
        if (LMSi_MemCmp(info->sectionInfos[i].name, name, 4)) {
            return i;
        }
    }

    return -1;
}

int LMSi_GetHashTableIndexFromLabel(const char *label, int tableSize) {
    unsigned int hash = 0;
    int c;
    while (1) {
        c = *label;
        if (c == '\0') {
            break;
        }
        hash = hash * 0x492 + c;
        label++;
    }
    return hash % tableSize;
}
