#ifndef D_SAVE_MANAGER_H
#define D_SAVE_MANAGER_H

#include "d/lyt/d_lyt_save_msg_window.h"
#include "egg/egg_types.h"
#include "toBeSorted/nand_request_thread.h"

#include "rvl/TPL.h"

class NandRequestCheckIsFile : public NandRequest {
    friend class NandRequestCheckIsFileHolder;

public:
    virtual bool execute() override;

private:
    NandRequestCheckIsFile(const char *path);

    /* 0x11 */ SizedString<0x40> mPath;
    /* 0x51 */ bool mIsFile;
};

class NandRequestCheckIsFileHolder : public NandRequestHolderBase {
public:
    NandRequestCheckIsFileHolder(NandRequestCheck *req) : NandRequestHolderBase(req) {}

    bool checkIsFile(const char *path);
    bool getCheckResult() const;
};

class NandRequestSaveBanner : public NandRequestCreate {
    friend class NandRequestSaveBannerHolder;
    NandRequestSaveBanner(
        const char *filePath, TPLPalette *palette, const wchar_t *title, const wchar_t *subtitle, u8 perm, u8 attr
    );

public:
    virtual bool execute() override;

private:
    TPLPalette *getPalette() const {
        return mpPalette;
    }

    const wchar_t *getTitle() const {
        return mpTitle;
    }

    const wchar_t *getSubTitle() const {
        return mpSubTitle;
    }

    u32 writeBannerTpl(NANDFileInfo *info);
    /* 0x54 */ TPLPalette *mpPalette;
    /* 0x58 */ const wchar_t *mpTitle;
    /* 0x5C */ const wchar_t *mpSubTitle;
    /* 0x60 */ bool mFailed;
};

class NandRequestSaveBannerHolder : public NandRequestHolderBase {
public:
    NandRequestSaveBannerHolder(NandRequestSaveBanner *req) : NandRequestHolderBase(req) {}

    bool saveBanner(
        const char *filePath, TPLPalette *palette, const wchar_t *title, const wchar_t *subtitle, u8 perm, u8 attr
    );
    bool hasFailed() const;
};

class NandRequestLoadSaveFile : public NandRequest {
    friend class NandRequestLoadSaveFileHolder;

public:
    virtual bool execute() override;

private:
    NandRequestLoadSaveFile(const char *path, s32 readSize, s32 mountDirection, EGG::Heap *heap);

    void remove();

    /* 0x11 */ SizedString<0x40> mPath;
    /* 0x54 */ EGG::Heap *mpHeap;
    /* 0x58 */ void *mpBuf;
    /* 0x5C */ u32 mBufLen;
    /* 0x60 */ s32 mMountDirection;
    /* 0x64 */ u32 mReadSize;
    /* 0x68 */ bool mUnused;
};

class NandRequestLoadSaveFileHolder : public NandRequestHolderBase {
public:
    NandRequestLoadSaveFileHolder(NandRequestLoadSaveFile *req) : NandRequestHolderBase(req) {}

    bool loadSave(const char *path, s32 readSize, s32 mountDirection, EGG::Heap *heap);
    void *getBuf() const;
    u32 getLen() const;
    void remove();
};

class NandRequestDelete : public NandRequest {
    friend class NandRequestDeleteHolder;

public:
    virtual bool execute() override;

private:
    NandRequestDelete(const char *path);

    /* 0x11 */ SizedString<0x40> mPath;
};

class NandRequestDeleteHolder : public NandRequestHolderBase {
public:
    NandRequestDeleteHolder(NandRequestCheck *req) : NandRequestHolderBase(req) {}

    bool doDelete(const char *path);
};

class SaveMgr {
public:
    static void create(EGG::Heap *heap);

    static SaveMgr *GetInstance() {
        return sInstance;
    }

    bool getField_0x83D() const {
        return field_0x83D;
    }
    bool getField_0x83F() const {
        return field_0x83F;
    }
    s32 getCheckForFreeSpaceResult() const {
        return mCheckForFreeSpaceResult;
    }
    bool isInState() {
        return mCurrentState != STATE_MAX;
    }

    void init();
    void createSaveMsgWindow();
    void execute();
    void draw();

    bool checkForSave();
    bool checkForFreeSpace();
    bool createFiles();
    bool loadSave();
    bool save(bool entranceT1LoadFlag, bool full);
    bool saveAfterCredits();
    bool writeSkipDat();
    bool copySave(u8 to, u8 from);
    bool clearSelectedFile();

private:
    // TODO - Align with NSMBW ModeInit_ / ModeProc_ naming convention?
    void initializeCheckForSave();
    void initializeCheckForFreeSpace();
    void initializeCreateFiles();
    void initializeSaveBanner();
    void initializeLoadSave();
    void initializeWriteSave();
    void initializeWriteSkipDat();
    void initializeCopySave();
    void initializeClearSelectedFile();
    void initializeSave();
    void initializeDeleteAllData();
    void initializeError();
    void initializeNandError();

    void executeCheckForSave();
    void executeCheckForFreeSpace();
    void executeCreateFiles();
    void executeSaveBanner();
    void executeLoadSave();
    void executeWriteSave();
    void executeWriteSkipDat();
    void executeCopySave();
    void executeClearSelectedFile();
    void executeSave();
    void executeDeleteAllData();
    void executeError();
    void executeNandError();

    static s32 getFrameRate();

    enum SaveMgrState_e {
        STATE_CHECK_FOR_SAVE = 0,
        STATE_CHECK_FOR_FREE_SPACE = 1,
        STATE_CREATE_FILES = 2,
        STATE_SAVE_BANNER = 3,
        STATE_LOAD_SAVE = 4,
        STATE_WRITE = 5,
        STATE_WRITE_SKIP_DAT = 6,
        STATE_COPY_SAVE = 7,
        STATE_CLEAR_SELECTED_FILE = 8,
        STATE_SAVE = 9,
        STATE_DELETE_ALL_DATA = 10,
        STATE_ERROR = 11,
        STATE_NAND_ERROR = 12,
        STATE_MAX = 13,
    };

    typedef void (SaveMgr::*StateFunc)();
    static const StateFunc sStateFuncs[STATE_MAX];

    SaveMgr();
    ~SaveMgr();

    void beginState(SaveMgrState_e state);
    void endState();

    /* 0x000 */ NandRequestCheckIsFileHolder mCheckIsFileRequest;
    /* 0x004 */ NandRequestCheckHolder mCheckRequest;
    /* 0x008 */ NandRequestCreateHolder mCreateRequest;
    /* 0x00C */ NandRequestWriteHolder mWriteRequest;
    /* 0x010 */ NandRequestSaveBannerHolder mSaveBannerRequest;
    /* 0x014 */ NandRequestLoadSaveFileHolder mLoadSaveRequest;
    /* 0x018 */ NandRequestDeleteHolder mDeleteRequest;
    /* 0x01C */ dLytSaveMsgWindow_c *mpWindow;
    /* 0x020 */ wchar_t mTextBuf[0x400];
    /* 0x820 */ SaveMgrState_e mCurrentState;
    /* 0x824 */ s32 field_0x824;
    /* 0x828 */ s32 mCheckForFreeSpaceResult;
    /* 0x82C */ s32 mStep;
    /* 0x830 */ s32 mDelayTimer;
    /* 0x834 */ u8 mCopyToFile;
    /* 0x835 */ u8 mCopyFromFile;
    /* 0x836 */ u8 field_0x836;
    /* 0x837 */ u8 field_0x837;
    /* 0x838 */ u8 field_0x838;
    /* 0x839 */ u8 field_0x839;
    /* 0x83A */ u8 field_0x83A;
    /* 0x83B */ u8 field_0x83B;
    /* 0x83C */ u8 field_0x83C;
    /* 0x83D */ u8 field_0x83D;
    /* 0x83E */ u8 field_0x83E;
    /* 0x83F */ u8 field_0x83F;

    static SaveMgr *sInstance;
};

#endif
