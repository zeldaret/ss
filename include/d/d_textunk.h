#ifndef D_TEXTUNK_H
#define D_TEXTUNK_H

#include "common.h"
#include "libms/flowfile.h"
#include "libms/msgfile.h"
#include "nw4r/ut/ut_Color.h"
#include "sized_string.h"

class UnkTextThing {
public:
    UnkTextThing();
    virtual ~UnkTextThing() {}

    void destroy();

    inline static f32 getField0x758() {
        if (sInstance != nullptr) {
            return sInstance->field_0x758;
        }
        return 0.0f;
    }

    inline static f32 getField0x75C() {
        if (sInstance != nullptr) {
            return sInstance->field_0x75C;
        }
        return 0.0f;
    }

    inline static f32 getField0x760() {
        if (sInstance != nullptr) {
            return sInstance->field_0x760;
        }
        return 0.0f;
    }

    inline static f32 getField0x768() {
        if (sInstance != nullptr) {
            return sInstance->field_0x768;
        }
        return 0.0f;
    }

    inline static f32 getField0x76C() {
        if (sInstance != nullptr) {
            return sInstance->field_0x76C;
        }
        return 0.0f;
    }

    inline static f32 getFn800B1F70() {
        if (sInstance != nullptr) {
            return sInstance->fn_800B1F70();
        }
        return 1.0f;
    }

    inline static f32 getFn800B1FD0() {
        if (sInstance != nullptr) {
            return sInstance->fn_800B1FD0();
        }
        return 1.0f;
    }

    inline static f32 getFn800B1F10() {
        return sInstance->fn_800B1F10();
    }

    inline static f32 getFn800B1FF0() {
        if (sInstance != nullptr) {
            return sInstance->fn_800B1FF0();
        }
        return 0.0f;
    }

    inline static f32 getFn800B2000() {
        if (sInstance != nullptr) {
            return sInstance->fn_800B2000();
        }
        return 0.0f;
    }

    inline static f32 getFn800B2010() {
        if (sInstance != nullptr) {
            return sInstance->fn_800B2010();
        }
        return 0.0f;
    }

    inline static f32 getFn800B2020() {
        if (sInstance != nullptr) {
            return sInstance->fn_800B2020();
        }
        return 0.0f;
    }

    inline static f32 getFn800B2030() {
        if (sInstance != nullptr) {
            return sInstance->fn_800B2030();
        }
        return 0.0f;
    }

    inline static f32 getFn800B2040() {
        if (sInstance != nullptr) {
            return sInstance->fn_800B2040();
        }
        return 0.0f;
    }

    static UnkTextThing *getInstance() {
        return sInstance;
    }

    void reset0(const char *str) {
        field_0x004 = str;
    }

    void reset1(const char *str) {
        field_0x084 = str;
    }

    void reset2(const char *str) {
        field_0x104 = str;
    }

    s16 getField_0x7AE() const {
        return field_0x7AE;
    }

    bool getField_0x7B4() const {
        return field_0x7B4;
    }

    void *getField_0x74C() const {
        return field_0x74C;
    }

    void *getField_0x750() const {
        return field_0x750;
    }

    bool getField_0x7B6() const {
        return field_0x7B6;
    }

    MsbtInfo *getMsbtInfo(s32 index) const {
        return field_buf2[index];
    }

    MsbfInfo *getMsbfInfo(s32 index) const {
        return field_buf3[index];
    }

    const char *getStr1() const {
        return field_0x004;
    }

    const char *getStr2() const {
        return field_0x084;
    }

    const char *getStr3() const {
        return field_0x104;
    }

    void setStr1(const char *s) {
        field_0x004 = s;
    }

    void setStr2(const char *s) {
        field_0x084 = s;
    }

    void setStr3(const char *s) {
        field_0x104 = s;
    }

    void *allocUnk(size_t size, int align);
    void destroyUnk(void *thing);

private:
    static UnkTextThing *sInstance;

    void destroyFlow(MsbfInfo *flow);
    void destroyMsg(MsbtInfo *msg);

    f32 fn_800B1F10();
    f32 fn_800B1F70();
    f32 fn_800B1FC0();
    f32 fn_800B1FD0();
    f32 fn_800B1FE0();
    f32 fn_800B1FF0();
    f32 fn_800B2000();
    f32 fn_800B2010();
    f32 fn_800B2020();
    f32 fn_800B2030();
    f32 fn_800B2040();

    /* 0x004 */ SizedString<0x80> field_0x004;
    /* 0x084 */ SizedString<0x80> field_0x084;
    /* 0x104 */ SizedString<0x80> field_0x104;
    /* 0x184 */ void *field_buf0[82];
    /* 0x2CC */ void *field_buf4[23];
    /* 0x328 */ void *field_buf1[80];
    /* 0x468 */ MsbtInfo *field_buf2[82];
    /* 0x5B0 */ MsbtInfo *field_buf5[23];
    /* 0x60C */ MsbfInfo *field_buf3[80];
    /* 0x74C */ void *field_0x74C;
    /* 0x750 */ void *field_0x750;
    /* 0x754 */ f32 field_0x754;
    /* 0x758 */ f32 field_0x758;
    /* 0x75C */ f32 field_0x75C;
    /* 0x760 */ f32 field_0x760;
    /* 0x764 */ f32 field_0x764;
    /* 0x768 */ f32 field_0x768;
    /* 0x76C */ f32 field_0x76C;
    /* 0x770 */ f32 field_0x770;
    /* 0x774 */ f32 field_0x774;
    /* 0x778 */ f32 field_0x778;
    /* 0x77C */ f32 field_0x77C;
    /* 0x780 */ f32 field_0x780;
    /* 0x784 */ f32 field_0x784;
    /* 0x788 */ f32 field_0x788;
    /* 0x78C */ f32 field_0x78C;
    /* 0x790 */ f32 field_0x790;
    /* 0x794 */ s16 field_0x794;
    /* 0x796 */ s16 field_0x796;
    /* 0x798 */ UNKWORD field_0x798;
    /* 0x79C */ UNKWORD field_0x79C;
    /* 0x7A0 */ UNKWORD field_0x7A0;
    /* 0x7A4 */ s16 field_0x7A4;
    /* 0x7A6 */ s16 field_0x7A6;
    /* 0x7A8 */ s16 field_0x7A8;
    /* 0x7AA */ s16 field_0x7AA;
    /* 0x7AC */ s16 field_0x7AC;
    /* 0x7AE */ s16 field_0x7AE;
    /* 0x7B0 */ s16 field_0x7B0;
    /* 0x7B2 */ bool field_0x7B2;
    /* 0x7B3 */ bool field_0x7B3;
    /* 0x7B4 */ bool field_0x7B4;
    /* 0x7B5 */ bool field_0x7B5;
    /* 0x7B6 */ bool field_0x7B6;
    /* 0x7B6 */ bool field_0x7B7;
    /* 0x7B8 */ nw4r::ut::Color field_0x7B8;
    /* 0x7BC */ nw4r::ut::Color field_0x7BC;
};

#endif
