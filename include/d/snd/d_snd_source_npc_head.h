#ifndef D_SND_SOURCE_NPC_HEAD_H
#define D_SND_SOURCE_NPC_HEAD_H

#include "common.h"
#include "d/snd/d_snd_source.h"

class dSndSourceNpcHead_c : public dSoundSource_c {
public:
    dSndSourceNpcHead_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

    void setMainName(const char *name) {
        mpMainName = name;
    }

    /* 0x0D4 */ virtual bool startVoiceLine(u32 id) override;
    /* 0x0DC */ virtual bool vt_0xDC(u32 id) override;

    /* 0x180 */ virtual void setOrigName(const char *arg) override;

    // id can be a WZSound ID or an offset from LABEL_NV_START
    /* 0x1EC */ virtual bool npcSpeak(u32 id);

    /* 0x1CC */ virtual void postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) override;

private:
    /* 0x15C */ UNKWORD field_0x15C;
    /* 0x160 */ const char *mpMainName;
};

#endif
