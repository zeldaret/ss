#include "types.h"
#include "toBeSorted/bitwise_flag_helper.h"

bool BitwiseFlagHelper::checkFlag(u16 slot, u16 shift, const u16 *pData, u16 flagCount) {
    return (pData[slot] >> shift) & 1;
}
void BitwiseFlagHelper::setFlag(u16 slot, u16 shift, u16 *pData, u16 flagCount) {
    pData[slot] |= (1 << shift);
}
void BitwiseFlagHelper::unsetFlag(u16 slot, u16 shift, u16 *pData, u16 flagCount) {
    pData[slot] &= ~(1 << shift);
}