#ifndef D_DVD_UNK_H
#define D_DVD_UNK_H

#include "common.h"
#include "egg/core/eggHeap.h"

// This file is related to errors that are
// displayed on the screen, using a backup font
// included in the DOL.

namespace dDvdUnk {

class unkstruct_c {
public:
    static unkstruct_c *create(EGG::Heap *heap);
    void draw();
    void execute();
    void init();
    char getUnk();

    static void createFont(EGG::Heap *heap);

private:
    UNKWORD field_0x0;
    char field_0x4;
};

class FontUnk {
public:
    FontUnk() {}

    static FontUnk *GetInstance() {
        return sInstance;
    }

    void fn_80052D00(bool);
    void fn_80052D50();
    void fn_80052C90();

    s32 getField_0x24() const {
        return field_0x24;
    }

    u8 getField_0x28() const {
        return field_0x28;
    }

    u8 getField_0x29() const {
        return field_0x29;
    }

    void setField_0x2C(u8 val) {
        field_0x2C = val;
    }

private:
    static FontUnk *sInstance;

    /* 0x00 */ nw4r::ut::ResFont mFont;
    /* 0x1C */ s32 field_0x1C;
    /* 0x20 */ s32 field_0x20;
    /* 0x24 */ s32 field_0x24;
    /* 0x28 */ u8 field_0x28;
    /* 0x29 */ u8 field_0x29;
    /* 0x2A */ u8 field_0x2A;
    /* 0x2B */ u8 field_0x2B;
    /* 0x2C */ u8 field_0x2C;
    /* 0x2D */ u8 field_0x2D;
};

} // namespace dDvdUnk

#endif
