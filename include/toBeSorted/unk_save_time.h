#ifndef UNK_SAVE_TIME_H
#define UNK_SAVE_TIME_H

#include "common.h"

class SaveTimeRelated {
public:
    static SaveTimeRelated *GetInstance() {
        return sInstance;
    }

    s64 getField_0x08() const {
        return field_0x08;
    }

    s64 getField_0x30() const {
        return field_0x30;
    }

    s64 fn_801907D0();
    void fn_801907F0();
    void fn_801909A0();

private:
    static SaveTimeRelated *sInstance;

    /* 0x00 */ u8 _0x00[0x08 - 0x00];
    /* 0x08 */ s64 field_0x08;
    /* 0x10 */ u8 field_0x10[0x30 - 0x10];
    /* 0x30 */ s64 field_0x30;
};

#endif
