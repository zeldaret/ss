#ifndef D_SND_SOURCE_HARP_RELATED_H
#define D_SND_SOURCE_HARP_RELATED_H

#include "d/snd/d_snd_source.h"

class dSndSourceHarpRelated_c : public dSoundSource_c {
public:
    dSndSourceHarpRelated_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);
    virtual ~dSndSourceHarpRelated_c();

    /* 0x18C */ virtual void postCalc() override;
    /* 0x1D0 */ virtual void setPause(bool flag, int fadeFrames) override;

    /* 0x1EC */ virtual UNKWORD d_s_harp_vt_0x1EC() const {
        return 0;
    }

    /* 0x1F0 */ virtual void d_s_harp_vt_0x1F0();
    /* 0x1F4 */ virtual void d_s_harp_vt_0x1F4();
    /* 0x1F8 */ virtual void d_s_harp_vt_0x1F8();

private:
};

class dSndSourceHarpTg_c : public dSndSourceHarpRelated_c {
public:
    dSndSourceHarpTg_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceHarpRelated_c(sourceType, ac, name, pOwnerGroup) {}

private:
};

#endif
