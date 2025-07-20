#ifndef D_SND_BGM_MML_PARSERS_H
#define D_SND_BGM_MML_PARSERS_H

#include "d/snd/d_snd_bgm_mml_parser_base.h"
#include "d/snd/d_snd_types.h"

class dSndBgmMmlParserHarp_c : dSndBgmMmlParser_c {
public:
    dSndBgmMmlParserHarp_c(dSndBgmSoundHarpMgr_c *mgr);

    virtual void CommandProc(u32 trackNo, int wait, u32 command, s32 commandArg1, s32 commandArg2) const override;
    virtual void NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const override;

    bool parseData(const char *soundLabel, u32 baseSoundId, u32 trackMask);
    bool parseData(u32 baseSoundId, u32 trackMask);

private:
    /* 0x294 */ dSndBgmSoundHarpMgr_c *field_0x294;
    /* 0x298 */ dSndBgmSoundHarpMgr_c *field_0x298;
};

class dSndBgmMmlParserHarpSong_c : dSndBgmMmlParser_c {
public:
    dSndBgmMmlParserHarpSong_c();

    virtual void CommandProc(u32 trackNo, int wait, u32 command, s32 commandArg1, s32 commandArg2) const override;
    virtual void NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const override;

    bool parseData(u32 soundId, dSndHarpSongData_c *pData);

private:
    /* 0x294 */ dSndHarpSongData_c *field_0x294;
};

struct dSndTagData {
    /* 0x00 */ u32 size;
    /* 0x04 */ u32 data[];
};

class dSndBgmMmlParserTagBattle_c : dSndBgmMmlParser_c {
public:
    dSndBgmMmlParserTagBattle_c();

    virtual void NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const override;

    bool parseData(u32 soundId, dSndTagData **pOutData);

private:
    /* 0x294 */ dSndTagData *mpBuf;
    /* 0x298 */ mutable u32 mBufSize;
};

class dSndBgmMmlParserTagBgmBoss_c : dSndBgmMmlParser_c {
public:
    dSndBgmMmlParserTagBgmBoss_c();

    virtual void NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const override;

    void parseData(dSndTagData **pOutData1, dSndTagData **pOutData2);

private:
    /* 0x294 */ dSndTagData *mpBuf1;
    /* 0x298 */ dSndTagData *mpBuf2;
    /* 0x29C */ mutable u32 mBufSize1;
    /* 0x2A0 */ mutable u32 mBufSize2;
};

class dSndBgmMmlParserTagCompNote_c : dSndBgmMmlParser_c {
public:
    dSndBgmMmlParserTagCompNote_c();

    virtual void NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const override;

    void parseData(dSndTagData **pOutData);

private:
    /* 0x294 */ dSndTagData *mpBuf;
    /* 0x298 */ mutable u32 mBufSize;
};

#endif
