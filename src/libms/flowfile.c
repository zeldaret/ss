#include <libms/commonlib.h>
#include <libms/flowfile.h>
#include <libms/libms.h>


struct MsbfInfo {
    /* 0x00 */ struct MsbInfo base;
    /* 0x10 */ int flw3Index;
    /* 0x14 */ int fen1Index;
    /* 0x18 */ int ref1Index;
};

struct MsbfInfo *LMS_InitFlow(void *data) {
    struct MsbfInfo *info = (struct MsbfInfo *)LMSi_Malloc(sizeof(struct MsbfInfo));
    info->base.header = (struct MsbHeader *)data;
    LMSi_AnalyzeMessageBinary(&info->base, "MsgStdBn");
    info->flw3Index = LMSi_SearchBlockByName(&info->base, "LBL1");
    info->fen1Index = LMSi_SearchBlockByName(&info->base, "TXT2");
    info->ref1Index = LMSi_SearchBlockByName(&info->base, "ATR1");
    return info;
}

void LMS_CloseFlow(struct MsbfInfo *info) {
    if (info->base.sectionInfos != nullptr) {
        LMSi_Free(info->base.sectionInfos);
    }
    LMSi_Free(info);
}

int LMS_GetFlow(struct MsbfInfo *info, int index) {
    // TODO
}

int LMS_GetEntrypoint(struct MsbfInfo *info, const char *label) {
    // TODO
}

struct MsbFlowInfo *LMS_GetFlowElement(struct MsbfInfo *info, int index) {
    // TODO
}

const unsigned short *LMS_GetBranchPoints(struct MsbfInfo *info, int index) {
    // TODO
}
