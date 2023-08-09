#ifndef RVL_SDK_MEM_LIST_H
#define RVL_SDK_MEM_LIST_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMList {
    void* head; // at 0x0
    void* tail; // at 0x4
    u16 length; // at 0x8
    u16 offset; // at 0xA
} MEMList;

typedef struct MEMLink {
    void* prev; // at 0x0
    void* next; // at 0x4
} MEMLink;

void MEMInitList(MEMList* list, u16 offset);
void MEMAppendListObject(MEMList* list, void* object);
void MEMRemoveListObject(MEMList* list, void* object);
void* MEMGetNextListObject(MEMList* list, void* object);

#ifdef __cplusplus
}
#endif
#endif
