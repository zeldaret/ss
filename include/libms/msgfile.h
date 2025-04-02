#ifndef LIBMS_MSGFILE_H
#define LIBMS_MSGFILE_H

#include "common.h"
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

struct MsbtInfo;

struct MsbtAttrInfo {
    char c_0x00;
    char c_0x01;
};

struct MsbtInfo *LMS_InitMessage(void *data);
void LMS_CloseMessage(struct MsbtInfo *info);
int LMS_GetTextIndexByLabel(const struct MsbtInfo *info, const char *label);
const wchar_t *LMS_GetText(const struct MsbtInfo *info, int index);
const wchar_t *LMS_GetTextByLabel(const struct MsbtInfo *info, const char *label);
const char *LMS_GetLabelByTextIndex(const struct MsbtInfo *info, int index);
struct MsbtAttrInfo *LMS_GetAttribute(const struct MsbtInfo *info, int index);

#ifdef __cplusplus
}
#endif

#endif
