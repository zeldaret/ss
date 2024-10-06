#ifndef C_BG_S_POLY_INFO_H
#define C_BG_S_POLY_INFO_H

#include "common.h"

class cBgS_PolyInfo {
private:
    /* 0x00 */ u32 mPolyIndex;
    /* 0x04 */ u32 mBgIndex;
    /* 0x08 */ void *unk_0x04;        // Collision Pointer?
    /* 0x0C */ unsigned int mActorId; // Matched the Unique Actor Id

public:
    cBgS_PolyInfo();
    bool ChkSetInfo() const;
    void ClearPi();
    void SetPolyInfo(const cBgS_PolyInfo &);
    void SetActorInfo(int, void *, unsigned int);
    bool ChkSafe(const void *, unsigned int) const;
    void SetPolyIndex(int);
    bool ChkBgIndex() const;

    virtual ~cBgS_PolyInfo();

    u32 GetPolyIndex() const {
        return mPolyIndex;
    }
    u32 GetBgIndex() const {
        return mBgIndex;
    }
};
#endif
