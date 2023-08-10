#include <types.h>

class BitwiseFlagHelper {
    public:
    bool checkFlag(u16 slot, u16 shift, u16 *pData);
    void setFlag(u16 slot, u16 shift, u16 *pData);
    void unsetFlag(u16 slot, u16 shift, u16 *pData);
};

bool BitwiseFlagHelper::checkFlag(u16 slot, u16 shift, u16 *pData) {
    return (pData[slot] >> shift) & 1;
}
void BitwiseFlagHelper::setFlag(u16 slot, u16 shift, u16 *pData) {
    pData[slot] |= (1 << shift);
}
void BitwiseFlagHelper::unsetFlag(u16 slot, u16 shift, u16 *pData) {
    pData[slot] &= ~(1 << shift);
}