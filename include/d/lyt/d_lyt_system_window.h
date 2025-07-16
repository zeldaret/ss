#ifndef D_LYT_SYSTEM_WINDOW_H
#define D_LYT_SYSTEM_WINDOW_H

#include "common.h"

class dLytSystemWindow_c {
public:
    virtual ~dLytSystemWindow_c();
    // TODO fill out
    static dLytSystemWindow_c *GetInstance() {
        return sInstance;
    }

    static bool create();

    bool setProperties(const char *label, bool, const wchar_t *);
    void showMaybe();
    void showMaybe(s32 arg);

    s32 getField_0xE10() const {
        return field_0xE10;
    }

    s32 getField_0xDE0() const;
    bool getField_0xDFC() const;
    void fn_80152F10();
    bool fn_80152F50() const;
    bool fn_80152F60() const;
    bool fn_80152F70() const;
    bool fn_80152F80() const;

private:
    static dLytSystemWindow_c *sInstance;

    /* 0x004 */ u8 _0x004[0xE10 - 0x004];
    /* 0xE10 */ u8 field_0xE10;
};

#endif
