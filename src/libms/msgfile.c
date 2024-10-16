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
    if (info->lbl1Index == -1) {
        return -2;
    } else {
        // TODO
    }
}

const char *LMS_GetText(struct MsbtInfo *info, int index) {
    if (info->txt2Index == -1) {
        return NULL;
    }

    int *header = info->base.sectionInfos[info->txt2Index].ptr;
    if (header[0] <= index) {
        return NULL;
    }

    return &((const char *)header)[header[index + 1]];
}

const char *LMS_GetTextByLabel(struct MsbtInfo *info, const char *label) {
    int index = LMS_GetTextIndexByLabel(info, label);
    if (index < 0) {
        return NULL;
    } else {
        return LMS_GetText(info, index);
    }
}

const char *LMS_GetLabelByTextIndex(struct MsbtInfo *info, int index) {
    // TODO
}

struct MsbtAttrInfo *LMS_GetAttribute(struct MsbtInfo *info, int index) {
    int *p = info->base.sectionInfos[info->atr1Index].ptr;
    return (struct MsbtAttrInfo *)&((const char *)p)[p[1] * index + 8];
}
