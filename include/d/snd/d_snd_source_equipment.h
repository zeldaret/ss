#ifndef D_SND_SOURCE_EQUIPMENT_H
#define D_SND_SOURCE_EQUIPMENT_H

#include "d/snd/d_snd_source.h"

class dSndSourceEquipment_c : public dSoundSource_c {
public:
    dSndSourceEquipment_c(u8 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

    /* 0x040 */ virtual void shutdown() override;
    /* 0x194 */ virtual u32 overrideStartSoundId(u32 soundId) override;
    /* 0x198 */ virtual void postStartSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id) override;
    /* 0x19C */ virtual u32 overrideHitObjSoundId(u32 soundId, dSoundSource_c *source) override;

    /* 0x1CC */ virtual void postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) override;
    
    /* 0x1E8 */ virtual u32 d_s_vt_0x1E8(u32 soundId) override {
        return specializeBgHitSoundId(soundId, mPolyAttr0, mPolyAttr1);
    }

private:
    static void cbBeFlyLv(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle);
    static void cbBeThrowRc(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle);
    static void cbBmIgnitionLv(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle);
    static void cbBmBound(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle);
    static void cbWiFxLv(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle);
    static void cbPcHit(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle);
};


class dSndSourceEquipmentWhip_c : public dSndSourceEquipment_c {
public:
    dSndSourceEquipmentWhip_c(u8 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

    /* 0x1CC */ virtual void postCalc() override;
};

#endif
