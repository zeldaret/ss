#ifndef EFFECTS_STRUCT_H
#define EFFECTS_STRUCT_H

#include "common.h"
#include "d/d_base.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"

class EffectsStruct {
private:
    s32 field_0x00;
    u8 field_0x04[0x1C - 0x04];

public:
    // vt at 0x1C
    EffectsStruct();
    EffectsStruct(dBase_c *);
    virtual ~EffectsStruct();

    inline void init(dBase_c *owner) {
        mpOwner = owner;
    }

    void remove(bool);
    void fn_80029929(u16 effect, mVec3_c *pos, void *, void *, void *, void *);
    void setMtx(const mMtx_c&);

    bool checkField0x00() const {
        return field_0x00 != 0;
    }

private:
    u8 field_0x20[0x28 - 0x20];
    /* 0x28 */ dBase_c *mpOwner;
    u8 field_0x2C[0x34 - 0x2C];
};

#endif
