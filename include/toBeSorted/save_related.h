#ifndef SAVE_RELATED_H
#define SAVE_RELATED_H

#include "common.h"

class SaveRelated {
public:
    static void create();
    static void remove();

    static SaveRelated *GetInstance() {
        return sInstance;
    }

    void fn_80015EA0();
    void fn_80015EC0();
    void fn_80015F40();

    bool getField_0x09() const {
        return field_0x09;
    }

private:
    static SaveRelated *sInstance;

    /* 0x00 */ u8 _0x00[0x08 - 0x00];
    /* 0x08 */ u8 field_0x08;
    /* 0x09 */ bool field_0x09;
};

#endif
