#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

struct MsbfInfo;

struct MsbFlowInfo {
    /* 0x00 */ char type;
    /* 0x01 */ char subType;
    /* 0x04 */ short params1n2;
    /* 0x08 */ short next;
    /* 0x0A */ short param3;
    /* 0x0C */ short param4;
    /* 0x0E */ short param5;
};

// Func names are made up

struct MsbfInfo *LMS_InitFlow(void *data);
void LMS_CloseFlow(struct MsbfInfo *info);
int LMS_GetFlow(struct MsbfInfo *info, int index);
int LMS_GetEntrypoint(struct MsbfInfo *info, const char *label);
struct MsbFlowInfo *LMS_GetFlowElement(struct MsbfInfo *info, int index);
const unsigned short *LMS_GetBranchPoints(struct MsbfInfo *info, int index);

#ifdef __cplusplus
}
#endif
