#ifndef D_FLAG_FLAG_MANAGERS_H
#define D_FLAG_FLAG_MANAGERS_H

#include "egg/core/eggHeap.h"

void setupFlagManagers(EGG::Heap *heap);
void copyAllFlagManagersFromSave();
void updateFlagForFlagIndex(u16 stage);
void commitAllFlagManagers();

bool checkedMemcpy(void *dest, u32 destLen, const void *src, u32 count);

#endif
