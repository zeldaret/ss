#ifndef D_LYT_PAUSE_H
#define D_LYT_PAUSE_H

#include "d/lyt/d2d.h"
#include "egg/gfx/eggCpuTexture.h"
#include "nw4r/lyt/lyt_pane.h"

class dLytPauseMgr_c {
public:
    static dLytPauseMgr_c *GetInstance() {
        return sInstance;
    }

    d2d::ResAccIf_c *getResAcc1() {
        return &mResAcc1;
    }

    d2d::ResAccIf_c *getResAcc2() {
        return &mResAcc2;
    }

    EGG::CpuTexture *getBgTexture() {
        return mpBgTexture;
    }

    bool getField_0x0831() const {
        return field_0x0831;
    }

    bool getField_0x0832() const {
        return field_0x0832;
    }

    bool getField_0x083B() const {
        return field_0x083B;
    }

    bool getField_0x083E() const {
        return field_0x083E;
    }

    bool getField_0x0840() const {
        return field_0x0840;
    }

    bool isStateWait() const;

    enum SelectionType_e {
        SELECT_DOWSING = 2,
        SELECT_INSECT = 4,
        SELECT_QUEST_MATERIAL = 5,
        SELECT_ITEM = 6,
        SELECT_CATEGORY = 7,
        SELECT_FIRE = 8,
        SELECT_RING = 9,
        SELECT_NONE = 10,
    };

    void setSelection(SelectionType_e type, s32 id, bool restricted);

    bool isCurrentSelectionRestricted() const {
        return mCurrentSelectionIsRestricted;
    }
    SelectionType_e getCurrentSelectionType() const {
        return mCurrentSelectionType;
    }

    s32 getCurrentSelectionId() const {
        return mCurrentSelectionId;
    }

    nw4r::lyt::Pane *getArrowBounding(int idx) const;
    nw4r::lyt::Pane *setSelectedArrowBounding(int idx) const;

private:
    static dLytPauseMgr_c *sInstance;

    /* 0x0000 */ u8 _0x0000[0x00C8 - 0x0000];

    /* 0x00C8 */ d2d::ResAccIf_c mResAcc1;
    /* 0x0438 */ d2d::ResAccIf_c mResAcc2;

    /* 0x07A8 */ u8 _0x0438[0x0814 - 0x07A8];

    /* 0x0814 */ EGG::CpuTexture *mpBgTexture;
    /* 0x0818 */ SelectionType_e mCurrentSelectionType;

    /* 0x081C */ u8 _0x081C[0x082C - 0x081C];

    /* 0x082C */ u16 mCurrentSelectionId;

    /* 0x082E */ u8 _0x082E[0x0831 - 0x082E];

    /* 0x0831 */ bool field_0x0831;
    /* 0x0832 */ bool field_0x0832;

    /* 0x0832 */ u8 _0x0832[0x083B - 0x0833];

    /* 0x083B */ bool field_0x083B;

    /* 0x083C */ u8 _0x083C[0x083E - 0x083C];

    /* 0x083E */ bool field_0x083E;
    /* 0x083F */ bool field_0x083F;
    /* 0x0840 */ bool field_0x0840;
    /* 0x0841 */ bool mCurrentSelectionIsRestricted;
};

#endif
