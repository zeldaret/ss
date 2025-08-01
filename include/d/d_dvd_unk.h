#ifndef D_DVD_UNK_H
#define D_DVD_UNK_H

#include "common.h"
#include "egg/core/eggHeap.h"

// This file is related to errors that are
// displayed on the screen, using a backup font
// included in the DOL.

namespace dDvdUnk {

class FontUnk {
private:
    enum Error_e {
        ERROR_NONE,
        ERROR_DISK,
        ERROR_PAD,
        ERROR_NAND,
    };

    enum Flag_e {
        FLAG_ERROR_REQUEST = 0x1,
        FLAG_0x2 = 0x2,
        FLAG_ERROR_STATE = 0x4,
    };

public:
    FontUnk() : mFlags(0) {}

    static FontUnk *GetInstance() {
        return sInstance;
    }

    static void create(EGG::Heap *heap);
    void init();
    void execute();
    void drawNonDriveError();
    void drawDriveError();

    void preExecute();

    bool isAnyError() const;
    void onError();
    void clearNandTrackerError();

    void fn_800529B0();
    void clearNandError();
    void fn_80052A20();
    void fn_80052CC0();
    void setNeedsPad(bool);
    bool padErrorsAllowed();
    void fn_80052D50();
    void fn_80052DD0();

    s32 getNandError() const {
        return mNandErrorCode;
    }

    bool isDiskError() const {
        return mIsDiskError;
    }

    bool isNandError() const {
        return mIsNandError;
    }

    void setField_0x2C(u8 val) {
        field_0x2C = val;
    }

    const nw4r::ut::ResFont &getFont() const {
        return mFont;
    }

private:
    static FontUnk *sInstance;

    void onFlag(u8 mask) {
        mFlags |= mask;
    }

    void offFlag(u8 mask) {
        mFlags &= ~mask;
    }

    bool checkFlag(u8 mask) const {
        return (mFlags & mask) != 0;
    }

    /* 0x00 */ nw4r::ut::ResFont mFont;
    /* 0x1C */ s32 mErrorType;
    /* 0x20 */ s32 mDiskErrorCode;
    /* 0x24 */ s32 mNandErrorCode;
    /* 0x28 */ bool mIsDiskError;
    /* 0x29 */ bool mIsNandError;
    /* 0x2A */ bool mIsPadError;
    /* 0x2B */ u8 field_0x2B;
    /* 0x2C */ u8 field_0x2C;
    /* 0x2D */ u8 mFlags;
};

} // namespace dDvdUnk

#endif
