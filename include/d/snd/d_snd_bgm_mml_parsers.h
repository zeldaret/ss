#ifndef D_SND_BGM_MML_PARSERS__H
#define D_SND_BGM_MML_PARSERS__H

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

#endif
