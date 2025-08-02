#ifndef NAND_RESULT_TRACKER_H
#define NAND_RESULT_TRACKER_H

#include "egg/core/eggHeap.h"

#include "rvl/NAND.h"

class NandResultTracker {
    enum Mode_e {
        Normal = 0,
        Error = 1,
    };

public:
    enum ErrorCategory_e {
        // Everything is ok
        ERR_CAT_NONE = 0,
        // Error handled by us
        ERR_CAT_FATAL = 3,
        // Error handled by save manager
        ERR_CAT_SAVE_MGR = 4,
    };
    static void create(EGG::Heap *heap);
    static NandResultTracker *GetInstance();
    bool isFailure(NANDResult status);

    void init();
    void draw();
    void execute();

    s32 getErrorCategory() const {
        return mErrorCategory;
    }

    bool isError() const {
        return mIsError;
    }

private:
    typedef void (NandResultTracker::*ModeFunc)();

    void ModeRequestNormal();
    void ModeProc_Normal();
    void ModeRequestError();
    void ModeProc_Error();

    void ModeRequest(Mode_e mode);

    static NandResultTracker *sInstance;

    /* 0x00 */ s32 mErrorCategory;
    /* 0x04 */ Mode_e mMode;
    /* 0x08 */ s32 mStep;
    /* 0x0C */ bool mIsError;
};

#endif
