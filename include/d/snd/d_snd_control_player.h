#ifndef D_SND_CONTROL_PLAYER_H
#define D_SND_CONTROL_PLAYER_H

#include "d/snd/d_snd_sound.h"
#include "nw4r/snd/snd_SoundPlayer.h"

class dSndControlPlayer_c : public dSndControl_c {
public:
    dSndControlPlayer_c(f32 value, f32 min, f32 max) : dSndControl_c(value, min, max) {}
    virtual void reset() override; // vt 0x0C

    virtual void stop() override;      // vt 0x18
    virtual void apply() = 0;          // vt 0x1C
    virtual f32 getAppliedValue() = 0; // vt 0x20

    void setIndex1(s32 idx);
    void setIndex2(s32 idx);

protected:
    /* 0x30 */ nw4r::snd::SoundPlayer *mpPlayer;
    /* 0x34 */ s32 mIndex;
};

class dSndControlPlayerVolumeBase_c : public dSndControlPlayer_c {
public:
    dSndControlPlayerVolumeBase_c();
};

class dSndControlPlayerVolume_c : public dSndControlPlayerVolumeBase_c {
public:
    dSndControlPlayerVolume_c() {}

    virtual void apply() override;          // vt 0x1C
    virtual f32 getAppliedValue() override; // vt 0x20
};

class dSndControlPlayerLpfFreq_c : public dSndControlPlayer_c {
public:
    dSndControlPlayerLpfFreq_c() : dSndControlPlayer_c(0.0f, -1.0f, 0.0f) {}

    virtual void apply() override;          // vt 0x1C
    virtual f32 getAppliedValue() override; // vt 0x20
};

class dSndControlPlayerFxSend_c : public dSndControlPlayer_c {
public:
    dSndControlPlayerFxSend_c() : dSndControlPlayer_c(0.0f, 0.0f, 1.0f) {}

    virtual void apply() override;          // vt 0x1C
    virtual f32 getAppliedValue() override; // vt 0x20
};

#endif
