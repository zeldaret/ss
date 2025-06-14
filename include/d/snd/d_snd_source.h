#ifndef D_SOUND_SOURCE_H
#define D_SOUND_SOURCE_H

#include "common.h"
#include "d/snd/d_snd_3d_actor.h"
#include "d/snd/d_snd_source_if.h"
#include "d/snd/d_snd_types.h"
#include "nw4r/math/math_types.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/ut/ut_list.h"

/** Manages sound relating to a particular actor. */
/** Size: probably 0x15C */
class dSoundSource_c : public dSoundSourceIf_c, public dSnd3DActor_c {
public:
    dSoundSource_c(u8 sourceType, dAcBase_c *, const char *name, dSndSourceGroup_c *pOwnerGroup);
    virtual ~dSoundSource_c();

    u32 modifySoundId(u32 baseSoundId);
    static u32 modifySoundId(u32 baseSoundId, dSoundSource_c *source);
    u32 getRemoConSoundVariant(u32 soundId) const;
    s32 getRoomId() const;
    bool isInaudible();

    // This is where it gets a bit wild and this class starts mixing in overrides between
    // new virtual functions, which causes the vtable to list these functions in exactly this
    // order.
    virtual const char *getName() const {
        return mpName;
    } // 0x17C
    virtual void d_s_vt_0x180();
    virtual void d_s_vt_0x184();
    virtual void d_s_vt_0x188();
    virtual void d_s_vt_0x18C();
    virtual void d_s_vt_0x190();
    virtual u32 d_s_vt_0x194(u32 soundId);

    virtual void d_vt_0x58(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id) override;

    virtual void d_s_vt_0x19C();
    virtual u32 d_s_vt_0x1A0(u32 soundId, UNKWORD);

    virtual void d_vt_0x5C(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id, UNKWORD) override;
    virtual StartResult
    SetupSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *) override;

    virtual bool d_s_vt_0x1AC(u32 soundId);
    virtual StartResult
    setupSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *arg, bool holdFlag);
    virtual void attachDistantSound(dSndDistantSoundActor_c *);
    virtual void detachDistantSound(dSndDistantSoundActor_c *);
    virtual void detachAllDistantSounds();
    virtual bool hasDistantSounds() const {
        return nw4r::ut::List_GetFirstConst(&mDistantSoundList) != nullptr;
    }
    virtual void pauseAllDistantSounds(bool flag, int fadeFrames);
    virtual void d_s_vt_0x1C8();
    virtual void d_s_vt_0x1CC(u32 playingId, u32 requestedId, dSndSeSound_c *seSound);

    virtual void setPause(bool flag, int fadeFrames) override;

    virtual void d_s_vt_0x1D4();
    virtual void d_s_vt_0x1D8();
    virtual void d_s_vt_0x1DC();
    virtual void d_s_vt_0x1E0();
    virtual void d_s_vt_0x1E4();
    virtual u32 d_s_vt_0x1E8(u32 soundId);

    bool startRemoConSound(u32 soundId);
    nw4r::snd::SoundHandle *startSound(u32 soundId, nw4r::snd::SoundHandle *handle);
    dSndDistantSoundActor_c *
    startSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position, nw4r::snd::SoundHandle *handle);
    nw4r::snd::SoundHandle *holdSound(u32 soundId, nw4r::snd::SoundHandle *handle);

    // Overrides of dSoundSourceIf_c - always in the first section of
    // the vtable, so the order is not certain. May have to reorder for weak
    // function order.
    virtual s32 getCategory() const override {
        return mSourceCategory;
    } // 0x010
    virtual bool isCategory(s32 category) const override {
        return mSourceCategory == category;
    } // 0x014

    virtual s32 getSourceType() const override {
        return mSourceType;
    } // 0x018
    virtual bool isSourceType(s32 type) const override {
        return mSourceType == type;
    } // 0x01C
    virtual const nw4r::math::VEC3 &getListenerPosition() const override; // 0x028
    virtual void stopAllSound(s32 fadeFrames) override {
        SoundActor::StopAllSound(fadeFrames);
    } // 0x040
    virtual bool hasPlayingSounds() const override;                      // 0x048
    virtual bool isPlayingSound(u32 soundId) override;                   // 0x04C
    virtual bool isPlayingSound(const char *soundId) override;           // 0x050
    virtual bool startSound(u32 soundId) override;                       // 0x060
    virtual bool startSound(const char *label) override;                 // 0x070
    virtual void stopSounds(u32 soundId, s32 fadeFrames) override;       // 0x0A4
    virtual void stopSounds(const char *label, s32 fadeFrames) override; // 0x0A8
    virtual bool holdSound(u32 soundId) override;                        // 0x0AC
    virtual bool holdSound(const char *label) override;                  // 0x0BC

    virtual bool isReadyMaybe() override {
        return false;
    } // 0x100
    virtual void load(void *data, const char *name) override {}     // 0x104
    virtual void setFrame(f32 frame) override {}                    // 0x108
    virtual void setRate(f32 frame) override {}                     // 0x10C
    virtual void setPolyAttrs(u8 polyAttr0, u8 polyAttr1) override; // 0x110

private:
    /**
     * Sound sources can cause other sounds to be started. E.g when
     * walking, Link will produce a walk sound, but an additional sound
     * will be played, a different one per surface material.
     *
     * If this flag isn't set, the code will find the correct variant for
     * a base sound, play the variant, and then play the base sound with this
     * flag set so that we don't attempt to find and play the variant again.
     */
    static bool sIsStartingBaseSound;
    f32 getBaseSoundVolume(u32 variantSoundId, u32 baseSoundId);
    dSndDistantSoundActor_c *startBaseSoundAtPosition(u32 baseSoundId, const nw4r::math::VEC3 *position, f32 volume);
    nw4r::snd::SoundHandle *startBaseSound(u32 baseSoundId, f32 volume);

    nw4r::snd::SoundStartable::StartResult onSetupError();

    nw4r::snd::SoundStartable::StartResult
    startSound(u32 soundId, nw4r::snd::SoundHandle *handle, nw4r::snd::SoundHandle **pOutHandle);

    nw4r::snd::SoundHandle *continueHoldingSound(
        dSndSeSound2_c *seHandle, u32 soundId, dSndDistantSoundActor_c *distant, const nw4r::math::VEC3 *position
    );

    void addSeHandleType1(dSndSeSound1_c *handle);
    void addSeHandleType2(dSndSeSound2_c *handle);
    void removeSeHandleType1(dSndSeSound1_c *handle, bool stop);
    void removeSeHandleType2(dSndSeSound2_c *handle);
    void removeAllSeHandles();
    void removeAllSeHandlesType1();
    void removeAllSeHandlesType2();

    void stopSoundHandles(u32 soundId, s32 fadeFrames);

    void calcHandles();
    void calcHandlesType1();
    void calcHandlesType2();

    dSndSeSound1_c *getHandleType1ForSoundId(u32 soundId);
    dSndSeSound1_c *getHandleType1ForSoundHandle(nw4r::snd::SoundHandle *handle);
    dSndSeSound2_c *getHandleType2ForSoundId(u32 soundId);

    void initVolumeFade();
    void setVolumeFade(f32 volume, u32 fadeFrames);
    void calcVolumeFade();

    // at 0x00: dSoundSourceIf_c vtable
    // at 0x04: dSnd3DActor_c sub-object
    // at 0x58: thunk-vtable

    /* 0x0E8 */ nw4r::ut::Node mMgrLink;
    /* 0x0F0 */ const char *mpName;
    /* 0x0F4 */ UNKWORD field_0x0F4;
    /* 0x0F8 */ dAcBase_c *mpActor;
    /* 0x0FC */ u8 mSourceCategory;
    /* 0x0FD */ u8 mSourceType;
    /* 0x0FE */ u8 field_0x0FE;
    /* 0x0FF */ u8 field_0x0FF;
    /* 0x100 */ u8 field_0x100;
    /* 0x101 */ u8 field_0x101;
    /* 0x102 */ u8 field_0x102;
    /* 0x104 */ UNKWORD field_0x104;
    /* 0x108 */ UNKWORD field_0x108;
    /* 0x10C */ UnkSeSoundStruct *mpUnkSe;
    /* 0x110 */ nw4r::ut::List mDistantSoundList; // node offset 0xEC -> dSndDistantSoundActor_c
    /* 0x11C */ UNKWORD field_0x11C;
    /* 0x120 */ nw4r::ut::List mHandleType1List; // node offset 0x4 -> dSndSeSound_c
    /* 0x12C */ nw4r::ut::List mHandleType2List; // node offset 0x4 -> dSndSeSound_c
    /* 0x138 */ nw4r::ut::Node mGroupLink;       // node for list in dSndSourceGroup_c
    /* 0x140 */ dSndSourceGroup_c *mpOwnerGroup;
    /* 0x144 */ f32 mVolumeFadeTarget;
    /* 0x148 */ f32 mVolumeFadeStepSize;
    /* 0x150 */ u8 _0x14C[0x154 - 0x14C];
    /* 0x154 */ UNKWORD field_0x154;
    /* 0x158 */ s16 mPolyAttr0;
    /* 0x15A */ s16 mPolyAttr1;
};

#endif
