#ifndef D_SND_SOURCE_OBJ_H
#define D_SND_SOURCE_OBJ_H

#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/math/math_types.h"

class dSndSourceObj_c : public dSoundSource_c {
public:
    dSndSourceObj_c(u8 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSoundSource_c(sourceType, ac, name, pOwnerGroup) {}

    /* 0x188 */ virtual void postSetup() override;

    /* 0x1CC */ virtual void postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) override;

    /* 0x1E8 */ virtual u32 d_s_vt_0x1E8(u32 soundId) override {
        return specializeBgHitSoundId(soundId, mPolyAttr0, mPolyAttr1);
    }

private:
    // Probably not a problem for weak function order since getName is emitted
    // earlier through an explicit call
    bool isName(const char *name) const {
        return streq(name, getName());
    }
};

class dSndSourceObjLightShaft_c : public dSndSourceObj_c {
public:
    dSndSourceObjLightShaft_c(u8 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceObj_c(sourceType, ac, name, pOwnerGroup) {}

    /* 0x1EC */ virtual void setPosition(const nw4r::math::VEC3 &position) override;
};

#endif
