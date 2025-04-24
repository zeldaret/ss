#include "libms/flowfile.h"

#include "libms/commonlib.h"
#include "libms/libms.h"

struct MsbfInfo {
    /* 0x00 */ struct MsbInfo base;
    /* 0x10 */ int flw3Index;
    /* 0x14 */ int fen1Index;
    /* 0x18 */ int ref1Index;
};

struct MsbfInfo *LMS_InitFlow(void *data) {
    struct MsbfInfo *info = (struct MsbfInfo *)LMSi_Malloc(sizeof(struct MsbfInfo));
    info->base.header = (struct MsbHeader *)data;
    LMSi_AnalyzeMessageBinary(&info->base, "MsgFlwBn");
    info->flw3Index = LMSi_SearchBlockByName(&info->base, "FLW3");
    info->fen1Index = LMSi_SearchBlockByName(&info->base, "FEN1");
    info->ref1Index = LMSi_SearchBlockByName(&info->base, "REF1");
    return info;
}

void LMS_CloseFlow(struct MsbfInfo *info) {
    if (info->base.sectionInfos != nullptr) {
        LMSi_Free(info->base.sectionInfos);
    }
    LMSi_Free(info);
}

int LMS_GetFlow(struct MsbfInfo *info) {
    if (info->flw3Index == -1) {
        return LMS_MISSING_FLW3_DATA;
    }
    return *LMS_OFS_TO_PTR(unsigned short, info->base.sectionInfos[info->flw3Index].ptr, 0);
}

// Basically copied from msgfile
int LMS_GetEntrypoint(struct MsbfInfo *info, const char *label) {
    const struct MsbBlock *infos;
    int hashIndex;
    unsigned i;
    unsigned bucketLen;
    int offset;
    int entryLen;
    int sectionIndex;
    int labelLen;

    sectionIndex = info->fen1Index;
    labelLen = 0;
    if (info->fen1Index == -1) {
        return LMS_MISSING_FEN1_DATA;
    } else {
        while (label[labelLen] != '\0') {
            labelLen++;
        }
        infos = info->base.sectionInfos;
        hashIndex = LMSi_GetHashTableIndexFromLabel(label, infos[sectionIndex].ptr[0]);
        bucketLen = infos[sectionIndex].ptr[hashIndex * 2 + 1];
        offset = infos[sectionIndex].ptr[hashIndex * 2 + 2];
        for (i = 0; i < bucketLen; i++) {
            entryLen = *LMS_OFS_TO_PTR(unsigned char, infos[sectionIndex].ptr, offset);
            if (entryLen == labelLen &&
                LMSi_MemCmp(label, LMS_OFS_TO_PTR(const char, infos[sectionIndex].ptr, offset + 1), entryLen)) {
                return *LMS_OFS_TO_PTR(int, infos[sectionIndex].ptr, offset + entryLen + 1);
            }
            offset += entryLen + 5;
        }
    }
    return LMS_NOT_FOUND;
}

struct MsbFlowInfo *LMS_GetFlowElement(struct MsbfInfo *info, int index) {
    if (info->flw3Index == -1) {
        return NULL;
    }

    return LMS_OFS_TO_PTR(struct MsbFlowInfo, info->base.sectionInfos[info->flw3Index].ptr, index * 16) + 1;
}

inline unsigned short *LMSi_GetBranchPoints(int *ptr, int off1, int off2) {
    return (unsigned short *)((char *)ptr + off1 * sizeof(struct MsbFlowInfo) + sizeof(struct MsbFlowInfo) + off2 * 2);
}

unsigned short *LMS_GetBranchPoints(struct MsbfInfo *info, int index) {
    const struct MsbBlock *block;
    struct MsbFlowInfo *element;

    int flow;
    int idx;

    flow = LMS_GetFlow(info);
    block = &info->base.sectionInfos[info->flw3Index];
    element = LMS_GetFlowElement(info, index);

    if (element == NULL) {
        return NULL;
    }

    if (element->type != 2) {
        return NULL;
    }

    return LMSi_GetBranchPoints(block->ptr, flow, element->param5);
}
