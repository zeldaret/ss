#ifndef RVL_SDK_IPC_PROFILE_H
#define RVL_SDK_IPC_PROFILE_H
#include <rvl/IPC/ipcclt.h>
#include <rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void IPCiProfInit(void);
void IPCiProfQueueReq(IPCRequestEx* req, s32 fd);
void IPCiProfAck(void);
void IPCiProfReply(IPCRequestEx* req, s32 fd);

#ifdef __cplusplus
}
#endif
#endif
