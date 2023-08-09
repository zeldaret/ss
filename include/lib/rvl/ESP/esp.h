#ifndef RVL_SDK_ESP_H
#define RVL_SDK_ESP_H
#include <lib/rvl/ARC.h>
#include <lib/rvl/IPC.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ES_IOCTLV_LAUNCH_TITLE = 8,
    ES_IOCTLV_GET_NUM_TICKET_VIEWS = 18,
    ES_IOCTLV_GET_TICKET_VIEWS = 19,
    ES_IOCTLV_GET_DATA_DIR = 29,
    ES_IOCTLV_GET_TITLE_ID = 32,
} ESIoctl;

typedef struct ESPTicket {
    u8 dummy[0x2A4];
    char padding[0x2C0 - 0x2A4];
} ESPTicket;

typedef struct ESPTmd {
    u8 dummy[0x49E4];
    char padding[0x4A00 - 0x49E4];
} ESPTmd;

s32 ESP_ReadContentFile(UNKWORD, void*, u32);
s32 ESP_SeekContentFile(UNKWORD, s32, UNKWORD);

#ifdef __cplusplus
}
#endif
#endif
