#pragma once

#include <types.h>

class BitwiseFlagHelper {
    public:
    bool checkFlag(u16 slot, u16 shift, const u16 *pData, u16 flagCount);
    void setFlag(u16 slot, u16 shift, u16 *pData, u16 flagCount);
    void unsetFlag(u16 slot, u16 shift, u16 *pData, u16 flagCount);
};