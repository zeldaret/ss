#ifndef D_SOUND_SOURCE_H
#define D_SOUND_SOURCE_H

#include "common.h"
#include "d/snd/d_snd_3d_actor.h"
#include "d/snd/d_snd_source_if.h"
#include "nw4r/ut/ut_list.h"

#include <cstring>

class dSndSourceGroup_c;

/** Manages sound relating to a particular actor. */
/** Size: probably 0x15C */
class dSoundSource_c : public dSoundSourceIf_c, public dSnd3DActor_c {
public:
    dSoundSource_c(u8 sourceType, dAcBase_c *, const char *name, dSndSourceGroup_c *pOwnerGroup);
    virtual ~dSoundSource_c();

    static u32 getCharacterTalkSoundId(u32 baseSoundId, dSoundSource_c *source);
    u32 getRemoConSoundVariant(u32 soundId) const;

    // This is where it gets a bit wild and this class starts mixing in overrides between
    // new virtual functions, which causes the vtable to list these functions in exactly this
    // order.
    virtual const char *getName() const;
    bool isName(const char *name) const {
        return !std::strcmp(getName(), name);
    }
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

    // Overrides of dSoundSourceIf_c - always in the first section of
    // the vtable, so the order is not certain. May have to reorder for weak
    // function order.
    virtual s32 getCategory() const override {
        return mSourceCategory;
    }
    virtual const nw4r::math::VEC3 &getListenerPosition() const override;

    virtual bool hasPlayingSounds() const override;            // 0x48
    virtual bool isPlayingSound(u32 soundId) override;         // 0x4C
    virtual bool isPlayingSound(const char *soundId) override; // 0x50

    virtual bool isReadyMaybe() override {
        return false;
    } // 0x100
    virtual void load(void *data, const char *name) override {} // 0x104
    virtual void setFrame(f32 frame) override {}                // 0x108
    virtual void setRate(f32 frame) override {}                 // 0x10C

private:
    // at 0x00: dSoundSourceIf_c vtable
    // at 0x04: dSnd3DActor_c sub-object
    // at 0x58: thunk-vtable

    /* 0xE8 */ nw4r::ut::Node mMgrLink;
    /* 0x0F0 */ const char *mpName;
    /* 0x0F4 */ UNKWORD field_0x0F4;
    /* 0x0F8 */ dAcBase_c *mpPlayer;
    /* 0x0FC */ u8 mSourceCategory;
    /* 0x0FD */ u8 mSourceType;
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
    /* 0x138 */ nw4r::ut::Node mGroupLink;  // node for list in dSndSourceGroup_c
    /* 0x140 */ dSndSourceGroup_c *mpOwnerGroup;
    /* 0x144 */ u8 _0x144[0x154 - 0x144];
    /* 0x154 */ UNKWORD field_0x154;
    /* 0x158 */ s16 field_0x158;
    /* 0x15A */ s16 field_0x15A;
};

#endif
