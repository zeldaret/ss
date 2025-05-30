#ifndef D_SOUND_SOURCE_H
#define D_SOUND_SOURCE_H

#include "common.h"
#include "d/snd/d_snd_3d_actor.h"
#include "nw4r/ut/ut_list.h"


class dAcBase_c;

class dSoundSourceIf_c {
public:
    virtual ~dSoundSourceIf_c() {}
#define SOUNDSOURCE_VIRTUAL(offset) virtual void vt_##offset() = 0;
    SOUNDSOURCE_VIRTUAL(0x0C);
    SOUNDSOURCE_VIRTUAL(0x10);
    SOUNDSOURCE_VIRTUAL(0x14);
    SOUNDSOURCE_VIRTUAL(0x18);
    SOUNDSOURCE_VIRTUAL(0x1C);
    SOUNDSOURCE_VIRTUAL(0x20);
    SOUNDSOURCE_VIRTUAL(0x24);
    SOUNDSOURCE_VIRTUAL(0x28);
    SOUNDSOURCE_VIRTUAL(0x2C);
    SOUNDSOURCE_VIRTUAL(0x30);
    SOUNDSOURCE_VIRTUAL(0x34);
    SOUNDSOURCE_VIRTUAL(0x38);
    SOUNDSOURCE_VIRTUAL(0x3C);
    SOUNDSOURCE_VIRTUAL(0x40);
    SOUNDSOURCE_VIRTUAL(0x44);
    virtual bool hasPlayingSounds() const = 0; // 0x48
    SOUNDSOURCE_VIRTUAL(0x4C);
    SOUNDSOURCE_VIRTUAL(0x50);
    SOUNDSOURCE_VIRTUAL(0x54);
    SOUNDSOURCE_VIRTUAL(0x58);
    SOUNDSOURCE_VIRTUAL(0x5C);
    SOUNDSOURCE_VIRTUAL(0x60);
    SOUNDSOURCE_VIRTUAL(0x64);
    SOUNDSOURCE_VIRTUAL(0x68);
    SOUNDSOURCE_VIRTUAL(0x6C);
    SOUNDSOURCE_VIRTUAL(0x70);
    SOUNDSOURCE_VIRTUAL(0x74);
    SOUNDSOURCE_VIRTUAL(0x78);
    SOUNDSOURCE_VIRTUAL(0x7C);
    SOUNDSOURCE_VIRTUAL(0x80);
    SOUNDSOURCE_VIRTUAL(0x84);
    SOUNDSOURCE_VIRTUAL(0x88);
    SOUNDSOURCE_VIRTUAL(0x8C);
    SOUNDSOURCE_VIRTUAL(0x90);
    SOUNDSOURCE_VIRTUAL(0x94);
    SOUNDSOURCE_VIRTUAL(0x98);
    SOUNDSOURCE_VIRTUAL(0x9C);
    SOUNDSOURCE_VIRTUAL(0xA0);
    SOUNDSOURCE_VIRTUAL(0xA4);
    SOUNDSOURCE_VIRTUAL(0xA8);
    SOUNDSOURCE_VIRTUAL(0xAC);
    SOUNDSOURCE_VIRTUAL(0xB0);
    SOUNDSOURCE_VIRTUAL(0xB4);
    SOUNDSOURCE_VIRTUAL(0xB8);
    SOUNDSOURCE_VIRTUAL(0xBC);
    SOUNDSOURCE_VIRTUAL(0xC0);
    SOUNDSOURCE_VIRTUAL(0xC4);
    SOUNDSOURCE_VIRTUAL(0xC8);
    SOUNDSOURCE_VIRTUAL(0xCC);
    SOUNDSOURCE_VIRTUAL(0xD0);
    SOUNDSOURCE_VIRTUAL(0xD4);
    SOUNDSOURCE_VIRTUAL(0xD8);
    SOUNDSOURCE_VIRTUAL(0xDC);
    SOUNDSOURCE_VIRTUAL(0xE0);
    SOUNDSOURCE_VIRTUAL(0xE4);
    SOUNDSOURCE_VIRTUAL(0xE8);
    SOUNDSOURCE_VIRTUAL(0xEC);
    SOUNDSOURCE_VIRTUAL(0xF0);
    SOUNDSOURCE_VIRTUAL(0xF4);
    SOUNDSOURCE_VIRTUAL(0xF8);
    SOUNDSOURCE_VIRTUAL(0xFC);

    virtual bool isReadyMaybe() = 0;                     // 0x100
    virtual bool load(void *data, const char *name) = 0; // 0x104
    virtual void setFrame(f32 frame) = 0;                // 0x108
    virtual void setRate(f32 frame) = 0;                 // 0x10C
    virtual void set_0x164(UNKWORD val) = 0;             // 0x114

    SOUNDSOURCE_VIRTUAL(0x118);
    SOUNDSOURCE_VIRTUAL(0x11C);
};

class dSoundSource_c : public dSoundSourceIf_c, public dSnd3DActor_c {
public:
    dSoundSource_c(u8, dAcBase_c *, UNKWORD, UNKWORD);
    virtual ~dSoundSource_c();

    u32 getRemoConSoundVariant(u32 soundId) const;

    // This is where it gets a bit wild and this class starts mixing in overrides between
    // new virtual functions, which causes the vtable to list these functions in exactly this
    // order.
    virtual void d_s_vt_0x17C();
    virtual void d_s_vt_0x180();
    virtual void d_s_vt_0x184();
    virtual void d_s_vt_0x188();
    virtual void d_s_vt_0x18C();
    virtual void d_s_vt_0x190();
    virtual void d_s_vt_0x194();

    virtual void d_vt_0x58() override;

    virtual void d_s_vt_0x19C();
    virtual void d_s_vt_0x1A0();

    virtual void d_vt_0x5C() override;
    virtual StartResult
    SetupSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *) override;

    virtual void d_s_vt_0x1AC();
    virtual void d_s_vt_0x1B0();
    virtual void d_s_vt_0x1B4();
    virtual void d_s_vt_0x1B8();
    virtual void d_s_vt_0x1BC();
    virtual void d_s_vt_0x1C0();
    virtual void d_s_vt_0x1C4(bool flag, int fadeFrames);
    virtual void d_s_vt_0x1C8();
    virtual void d_s_vt_0x1CC();

    virtual void d_vt_0x38(bool flag, int fadeFrames) override;

    virtual void d_s_vt_0x1D4();
    virtual void d_s_vt_0x1D8();
    virtual void d_s_vt_0x1DC();
    virtual void d_s_vt_0x1E0();
    virtual void d_s_vt_0x1E4();
    virtual void d_s_vt_0x1E8();

    // Overrides of dSoundSourceIf_c
    virtual bool hasPlayingSounds() const override;

private:
    // at 0x00: dSoundSourceIf_c vtable
    // at 0x04: dSnd3DActor_c sub-object
    // at 0x58: thunk-vtable
    /* 0xE8 */ u8 field_0xE8[0xF0 - 0xE8];

    /* 0x0F0 */ UNKWORD field_0x0F0;
    /* 0x0F4 */ UNKWORD field_0x0F4;
    /* 0x0F8 */ dAcBase_c *mpPlayer;
    /* 0x0FC */ u8 field_0x0FC;
    /* 0x0FD */ u8 field_0x0FD;
    /* 0x0FE */ u8 field_0x0FE;
    /* 0x0FF */ u8 field_0x0FF;
    /* 0x100 */ u8 field_0x100;
    /* 0x101 */ u8 field_0x101;
    /* 0x102 */ u8 field_0x102;
    /* 0x104 */ UNKWORD field_0x104;
    /* 0x108 */ UNKWORD field_0x108;
    /* 0x10C */ UNKWORD field_0x10C;
    /* 0x110 */ nw4r::ut::List field_0x110; // node offset 0xEC
    /* 0x11C */ UNKWORD field_0x11C;
    /* 0x120 */ nw4r::ut::List field_0x120; // node offset 0x4
    /* 0x12C */ nw4r::ut::List field_0x12C; // node offset 0x4
    /* 0x138 */ nw4r::ut::Node mNode;
    /* 0x140 */ UNKWORD field_0x140;
    /* 0x144 */ u8 _0x144[0x154 - 0x144];
    /* 0x154 */ UNKWORD field_0x154;
    /* 0x158 */ s16 field_0x158;
    /* 0x15A */ s16 field_0x15A;
};

#endif
