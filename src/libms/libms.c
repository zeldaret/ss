#include "libms/libms.h"

static void *(*MSB_ALLOC_FUNC)(size_t size);
static void (*MSB_FREE_FUNC)(void *arg);

void LMS_SetMemFuncs(void *(*alloc)(size_t size), void (*free)(void *ptr)) {
    MSB_ALLOC_FUNC = alloc;
    MSB_FREE_FUNC = free;
}

// internal
void *LMSi_Malloc(size_t size) {
    return (MSB_ALLOC_FUNC)(size);
}

void LMSi_Free(void *ptr) {
    return (MSB_FREE_FUNC)(ptr);
}

int LMSi_MemCmp(const char *p1, const char *p2, int n) {
    for (int i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return 0;
        }
    }

    return 1;
}

void LMSi_MemCopy(char *p1, const char *p2, int n) {
    // TODO register usage too optimal
    for (int i = 0; i < n; i++) {
        *p1 = *p2;
        p1++;
        p2++;
    }
}
