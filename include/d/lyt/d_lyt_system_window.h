#ifndef D_LYT_SYSTEM_WINDOW_H
#define D_LYT_SYSTEM_WINDOW_H

#include "common.h"

// TODO - the Main vs not-main split may not make sense here,
// but I don't know what the second class here does

class dLytSystemWindowMain_c {
public:
    dLytSystemWindowMain_c();
    virtual ~dLytSystemWindowMain_c();

    bool draw();
    bool build();
    bool calc();

    void init();

    bool setProperties(const char *label, bool, const wchar_t *);
    void fn_80150F30(s32 arg);
    bool requestABtnOut();

    bool fn_80150EB0();
    bool fn_80150EE0();
    bool fn_80150FB0();
    void fn_80150FE0();
    void fn_80150FF0();
    void fn_80151000();
    bool isPointerWithinWindow();

    s32 getField_0xDDC() const {
        return field_0xDDC;
    }

    bool getField_0xDF7() const {
        return field_0xDF7;
    }

    bool getField_0xDF8() const {
        return field_0xDF8;
    }

    void setField_0xE03(bool val) {
        field_0xE03 = val;
    }

    void setField_0xE04(bool val) {
        field_0xE04 = val;
    }

private:
    /* 0x004 */ u8 _0x004[0xDDC - 0x004];
    /* 0xDDC */ s32 field_0xDDC;
    /* 0xDF0 */ u8 _0xDF0[0xDF7 - 0xDE0];
    /* 0xDF7 */ bool field_0xDF7;
    /* 0xDF8 */ bool field_0xDF8;
    /* 0xDF9 */ u8 _0xDF9[0xE03 - 0xDF9];
    /* 0xE03 */ bool field_0xE03;
    /* 0xE04 */ bool field_0xE04;
};

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
