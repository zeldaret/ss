#ifndef D_THP_PLAYER_H
#define D_THP_PLAYER_H

#include "THPPlayer/THPInfo.h"
#include "common.h"
#include "d/d_base.h"
#include "m/m2d.h"
#include "m/m_dvd.h"
#include "sized_string.h"

class ThpVideoScreen : public m2d::Base_c {
public:
    ThpVideoScreen() {
        mEfbWidth = 0;
        mEfbHeight = 0;
        mVideoWidth = 0;
        mVideoHeight = 0;
    }
    virtual void draw() override;
    void setEfbWidth(s16 width) {
        mEfbWidth = width;
    }
    void setEfbHeight(s16 height) {
        mEfbHeight = height;
    }
    void setVideoDimensions(u32 width, u32 height) {
        mVideoWidth = width;
        mVideoHeight = height;
    }

private:
    /* 0x10 */ s32 mEfbWidth;
    /* 0x14 */ s32 mEfbHeight;
    /* 0x18 */ s32 mVideoWidth;
    /* 0x1C */ s32 mVideoHeight;
};

class dThpPlayer_c : public dBase_c {
public:
    dThpPlayer_c();
    ~dThpPlayer_c();

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

    void reallyExecute();
    void reallyCreate(u16, u16);
    s32 isDone();
    s32 play();
    s32 destroy();
    void open();
    void setupFile(char *filename, s32 loadAllAtOnce);

private:
    /* 0x68 */ mDvd_callback_c *mDvdCallback;
    /* 0x6C */ SizedString<0x20> mFilename;

    /* 0x8C */ THPVideoInfo mVideoComponent;
    /* 0x98 */ THPAudioInfo mAudioComponent;
    /* 0xA8 */ s32 mLoadAllAtOnce;
    /* 0xAC */ s32 mFilenameSet;
    /* 0xB0 */ s32 mSetupDone;
    /* 0xB4 */ s32 isPlaying;
    /* 0xB8 */ u8 *mBuffer;
    /* 0xBC */ ThpVideoScreen mScreen;
    /* 0xDC */ s32 mSpecialType;
};

#endif
