#include "libms/msgfile.h"

#include "libms/commonlib.h"
#include "libms/libms.h"

struct MsbtInfo {
    /* 0x00 */ struct MsbInfo base;
    /* 0x10 */ int lbl1Index;
    /* 0x14 */ int txt2Index;
    /* 0x18 */ int atr1Index;
    /* 0x1C */ int tsy1Index;
};

struct MsbtInfo *LMS_InitMessage(void *data) {
    struct MsbtInfo *info = (struct MsbtInfo *)LMSi_Malloc(sizeof(struct MsbtInfo));
    info->base.header = (struct MsbHeader *)data;
    LMSi_AnalyzeMessageBinary(&info->base, "MsgStdBn");
    info->lbl1Index = LMSi_SearchBlockByName(&info->base, "LBL1");
    info->txt2Index = LMSi_SearchBlockByName(&info->base, "TXT2");
    info->atr1Index = LMSi_SearchBlockByName(&info->base, "ATR1");
    info->tsy1Index = LMSi_SearchBlockByName(&info->base, "TSY1");
    return info;
}

void LMS_CloseMessage(struct MsbtInfo *info) {
    if (info->base.sectionInfos != nullptr) {
        LMSi_Free(info->base.sectionInfos);
    }
    LMSi_Free(info);
}

int LMS_GetTextIndexByLabel(struct MsbtInfo *info, const char *label) {
    const struct MsbBlock *infos;
    int hashIndex;
    unsigned i;
    unsigned bucketLen;
    int offset;
    int entryLen;
    int sectionIndex;
    int labelLen;

    sectionIndex = info->lbl1Index;
    labelLen = 0;
    if (info->lbl1Index == -1) {
        return LMS_MISSING_LBL1_DATA;
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

const wchar_t *LMS_GetText(struct MsbtInfo *info, int index) {
    if (info->txt2Index == -1) {
        return NULL;
    }

    int *header = info->base.sectionInfos[info->txt2Index].ptr;
    if (header[0] <= index) {
        return NULL;
    }

    return LMS_OFS_TO_PTR(const wchar_t, header, header[index + 1]);
}

const wchar_t *LMS_GetTextByLabel(struct MsbtInfo *info, const char *label) {
    int index = LMS_GetTextIndexByLabel(info, label);
    if (index < 0) {
        return NULL;
    } else {
        return LMS_GetText(info, index);
    }
}

int LMS_GetLabelByTextIndex(struct MsbtInfo *info, int index, char *outLabel) {
    const struct MsbBlock *block;

    int existingIndex;
    unsigned offset;
    unsigned labelLen;

    block = &info->base.sectionInfos[info->lbl1Index];
    offset = block->ptr[0] * 8 + 4;

    while (1) {
        if (offset >= block->sectionLength) {
            return 0;
        }
        labelLen = *LMS_OFS_TO_PTR(unsigned char, block->ptr, offset);
        existingIndex = *LMS_OFS_TO_PTR(int, block->ptr, (offset + labelLen + 1));
        if (existingIndex == index) {
            LMSi_MemCopy(outLabel, LMS_OFS_TO_PTR(char, block->ptr, (offset + 1)), labelLen);
            outLabel[labelLen] = '\0';
            return 1;
        }
        offset = offset + labelLen + 5;
    }
}

struct MsbtAttrInfo *LMS_GetAttribute(struct MsbtInfo *info, int index) {
    int *p = info->base.sectionInfos[info->atr1Index].ptr;
    return LMS_OFS_TO_PTR(struct MsbtAttrInfo, p, p[1] * index + 8);
}
