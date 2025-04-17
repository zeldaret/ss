#ifndef D_CAMERA_H
#define D_CAMERA_H

#include "d/d_base.h"
#include "m/m_vec.h"

extern "C" bool fn_80081FE0(void *, const char *);

class dCamera_c : public dBase_c {
public:
    bool isCurrentTrend(const char *name) const {
        return fn_80081FE0(field_0xD98, name);
    }

    void *getField_0xD98() const {
        return field_0xD98;
    }

    const mVec3_c &getPositionMaybe() const {
        return mPositionMaybe;
    }

private:
    /* 0x068 */ u8 _0x068[0x6C - 0x068];
    /* 0x06C */ mVec3_c mPositionMaybe;
    /* 0x078 */ u8 _0x078[0xD98 - 0x078];
    /* 0xD98 */ void *field_0xD98;
};

#endif
