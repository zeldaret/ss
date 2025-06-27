#ifndef D_SOUND_SOURCE_H
#define D_SOUND_SOURCE_H

#include "common.h"
#include "d/snd/d_snd_3d_actor.h"
#include "d/snd/d_snd_source_if.h"
#include "d/snd/d_snd_types.h"
#include "d/snd/d_snd_util.h"
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
    bool isInaudibleInternal();
    void stopAllPlaySounds(s32 fadeFrames);
    f32 timeAreaCheck();
    void handleRemoCon(nw4r::snd::SoundHandle *pHandle, u32 baseId);
    bool startRemoConSound(u32 soundId);
    nw4r::snd::SoundHandle *startSound(u32 soundId, nw4r::snd::SoundHandle *handle);
    dSndDistantSoundActor_c *
    startSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position, nw4r::snd::SoundHandle *handle);
    nw4r::snd::SoundHandle *holdSound(u32 soundId, nw4r::snd::SoundHandle *handle);
    u32 specializeBgHitSoundId(u32 baseSoundId, u32 polyAttr0, u32 polyAttr1);


    // This is where it gets a bit wild and this class starts mixing in overrides between
    // new virtual functions, which causes the vtable to list these functions in exactly this
    // order.
    /* 0x17C */ virtual const char *getName() const {
        return mpName;
    }
    /* 0x180 */ virtual void d_s_vt_0x180(u32 arg) {
        field_0x0F4 = arg;
    }
    /* 0x184 */ virtual void preCalc();
    /* 0x188 */ virtual void postSetup() {}
    /* 0x18C */ virtual void postCalc();
    /* 0x190 */ virtual bool d_s_vt_0x190() const;
    /* 0x194 */ virtual u32 overrideStartSoundId(u32 soundId);

    /* 0x198 */ virtual void postStartSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id) override;

    /* 0x19C */ virtual u32 overrideHitObjSoundId(u32 soundId, dSoundSource_c *source);
    /* 0x1A0 */ virtual u32 overrideHoldSoundId(u32 soundId, bool initial);

    /* 0x1A4 */ virtual void
    postHoldSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id, UNKWORD) override;
    /* 0x1A8 */ virtual StartResult
    SetupSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *) override;

    /* 0x1AC */ virtual bool d_s_vt_0x1AC(u32 soundId);
    /* 0x1B0 */ virtual StartResult
    setupSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *arg, bool holdFlag);
    /* 0x1B4 */ virtual void attachDistantSound(dSndDistantSoundActor_c *);
    /* 0x1B8 */ virtual void detachDistantSound(dSndDistantSoundActor_c *);
    /* 0x1BC */ virtual void detachAllDistantSounds();
    /* 0x1C0 */ virtual bool hasDistantSounds() const {
        return nw4r::ut::List_GetFirstConst(&mDistantSoundList) != nullptr;
    }
    /* 0x1C4 */ virtual void pauseAllDistantSounds(bool flag, int fadeFrames);
    /* 0x1C8 */ virtual void resetTempParams();
    /* 0x1CC */ virtual void postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) {}

    /* 0x1D0 */ virtual void setPause(bool flag, int fadeFrames) override;

    /* 0x1D4 */ virtual dSndAnimSound_c *getAnimSound() {
        return nullptr;
    }
    /* 0x1D8 */ virtual void registerAdditionalSource(dSoundSource_c *other) {
        return;
    }
    /* 0x1DC */ virtual bool isMultiSource() const {
        return 0;
    }
    /* 0x1E0 */ virtual void onAnimSoundEvent(s32 arg) {
        mAnimEventValue = arg;
    }
    /* 0x1E4 */ virtual void d_s_vt_0x1E4_noop();
    /* 0x1E8 */ virtual u32 d_s_vt_0x1E8(u32 soundId);

    // Overrides of dSoundSourceIf_c - always in the first section of
    // the vtable, so the order is not certain. May have to reorder for weak
    // function order.
    /* 0x00C */ virtual void setup() override;
    /* 0x010 */ virtual s32 getCategory() const override {
        return mSourceCategory;
    }
    /* 0x014 */ virtual bool isCategory(s32 category) const override {
        return category == mSourceCategory;
    }
    /* 0x018 */ virtual s32 getSourceType() const override {
        return mSourceType;
    }
    /* 0x01C */ virtual bool isSourceType(s32 type) const override {
        return type == mSourceType;
    }
    /* 0x020 */ virtual void vt_0x020() override {}
    /* 0x024 */ virtual void setSubtype(u8 subtype) override {
        mSubtype = subtype;
    }
    /* 0x028 */ virtual const nw4r::math::VEC3 &getListenerPosition() const override;
    /* 0x02C */ void calc(const nw4r::math::VEC3 &) override;
    /* 0x030 */ virtual void onFlag(u32 mask) override {
        mFlagsPersistent |= mask;
    }
    /* 0x034 */ virtual void offFlag(u32 mask) override {
        mFlagsPersistent &= ~mask;
    }
    /* 0x038 */ virtual bool checkFlag(u32 mask) const override {
        return (mFlagsPersistent & mask) != 0 || (mFlagsHold & mask) != 0;
    }
    /* 0x03C */ virtual void holdFlag(u32 mask) override {
        mFlagsHold |= mask;
    }
    /* 0x040 */ virtual void stopAllSound(s32 fadeFrames) override {
        SoundActor::StopAllSound(fadeFrames);
    }
    /* 0x044 */ virtual void shutdown() override;
    /* 0x048 */ virtual bool hasPlayingSounds() const override;
    /* 0x04C */ virtual bool isPlayingSound(u32 soundId) override;
    /* 0x050 */ virtual bool isPlayingSound(const char *soundId) override;
    /* 0x054 */ virtual void setCtxParam(const dSndSoundCtxParam *arg) override {
        mpCtxParam = arg;
    }
    /* 0x058 */ virtual void setCtxFloat(f32 value) override;
    /* 0x05C */ virtual void setCtxInt(s32 value) override;
    /* 0x060 */ virtual bool startSound(u32 soundId) override;
    /* 0x064 */ virtual bool startSoundWithIntParam(u32 soundId, s32 value) override;
    /* 0x068 */ virtual bool startSoundWithFloatParam(u32 soundId, f32 value) override;
    /* 0x06C */ virtual bool startSoundWithParams(u32 soundId, f32 fValue, s32 value) override;
    /* 0x070 */ virtual bool startSound(const char *label) override;
    /* 0x074 */ virtual bool startSoundWithIntParam(const char *label, s32 value) override;
    /* 0x078 */ virtual bool startSoundWithFloatParam(const char *label, f32 value) override;
    /* 0x07C */ virtual bool startSoundWithParams(const char *label, f32 fValue, s32 value) override;
    /* 0x080 */ virtual bool startSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position) override;
    /* 0x084 */ virtual bool
    startObjHitSound(u32 baseSoundId, dSoundSourceIf_c *source, const nw4r::math::VEC3 *position) override;
    /* 0x088 */ virtual bool
    startBgHitSound(u32 baseSoundId, u32 polyAttr0, u32 polyAttr1, const nw4r::math::VEC3 *position) override;
    /* 0x08C */ virtual bool startSoundAtPosition(const char *label, const nw4r::math::VEC3 *position) override;
    /* 0x090 */ virtual bool
    startObjHitSound(const char *label, dSoundSourceIf_c *source, const nw4r::math::VEC3 *position) override;
    /* 0x094 */ virtual bool
    startBgHitSound(const char *label, u32 polyAttr0, u32 polyAttr1, const nw4r::math::VEC3 *position) override;
    /* 0x098 */ virtual bool startSoundAtPosition2(u32 soundId, const nw4r::math::VEC3 *position) override;
    /* 0x09C */ virtual bool startSoundAtPosition2(const char *label, const nw4r::math::VEC3 *position) override;
    /* 0x0A0 */ virtual bool maybeUnusedEnemyDeath() override;
    /* 0x0A4 */ virtual void stopSounds(u32 soundId, s32 fadeFrames) override;
    /* 0x0A8 */ virtual void stopSounds(const char *label, s32 fadeFrames) override;
    /* 0x0AC */ virtual bool holdSound(u32 soundId) override;
    /* 0x0B0 */ virtual bool holdSoundWithIntParam(u32 soundId, s32 value) override;
    /* 0x0B4 */ virtual bool holdSoundWithFloatParam(u32 soundId, f32 value) override;
    /* 0x0B8 */ virtual bool holdSoundWithParams(u32 soundId, f32 fValue, s32 value) override;
    /* 0x0BC */ virtual bool holdSound(const char *label) override;
    /* 0x0C0 */ virtual bool holdSoundWithIntParam(const char *label, s32 value) override;
    /* 0x0C4 */ virtual bool holdSoundWithFloatParam(const char *label, f32 value) override;
    /* 0x0C8 */ virtual bool holdSoundWithParams(const char *label, f32 fValue, s32 value) override;
    /* 0x0CC */ virtual bool holdSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position) override;
    /* 0x0D0 */ virtual bool holdSoundAtPosition(const char *label, const nw4r::math::VEC3 *position) override;
    /* 0x0D4 */ virtual bool startVoiceLine(u32 id) override {
        return false;
    }
    /* 0x0D8 */ virtual bool startVoiceLine(const char *label) override;
    /* 0x0DC */ virtual bool vt_0xDC() override {
        return false;
    }
    /* 0x0E0 */ virtual void stopCurrentActionSound() override {}
    /* 0x0E4 */ virtual void stopActionSound(u32 id) override {}
    /* 0x0E8 */ virtual bool isPlayingActionSound() const override {
        return false;
    }
    /* 0x0EC */ virtual bool isCurrentActionSound(u32 id) const override {
        return false;
    }
    /* 0x0F0 */ virtual void vt_0x0F0_noop() override {}
    /* 0x0F4 */ virtual void setField0x101(u8 value) override {
        field_0x101 = value;
    }
    /* 0x0F8 */ virtual void setField0x102(u8 value) override {
        field_0x102 = value;
    }
    /* 0x0FC */ virtual bool isInaudible() override;
    /* 0x100 */ virtual bool hasAnimSound() override {
        return false;
    }
    /* 0x104 */ virtual void load(void *data, const char *name) override {}
    /* 0x108 */ virtual void setFrame(f32 frame) override {}
    /* 0x10C */ virtual void setRate(f32 frame) override {}
    /* 0x110 */ virtual void setPolyAttrs(u8 polyAttr0, u8 polyAttr1) override;
    /* 0x114 */ virtual void setBattleBgmRelated(UNKWORD) override {}
    /* 0x118 */ virtual UNKWORD checkBattleBgmRelated() override {
        return 0;
    }

    dSndSeSound2_c *getHandleType2ForSoundId(u32 soundId);

protected:
    StartResult setupSoundCommon(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *arg);
    void setVolumeFade(f32 volume, u32 fadeFrames);
    nw4r::snd::SoundStartable::StartResult onSetupError();

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

    dSndSeSound1_c *getHandleType1ForSoundHandle_Dupe(nw4r::snd::SoundHandle *pHandle);
    dSndSeSound2_c *getHandleType2ForSoundHandle_Dupe(nw4r::snd::SoundHandle *pHandle);

    void initVolumeFade();
    void calcVolumeFade();

    // at 0x00: dSoundSourceIf_c vtable
    // at 0x04: dSnd3DActor_c sub-object
    // at 0x58: thunk-vtable
protected:
    /* 0x0E8 */ nw4r::ut::Node mMgrLink;
    /* 0x0F0 */ const char *mpName;
    /* 0x0F4 */ UNKWORD field_0x0F4;
    /* 0x0F8 */ dAcBase_c *mpActor;
    /* 0x0FC */ u8 mSourceCategory;
    /* 0x0FD */ u8 mSourceType;
    /* 0x0FE */ u8 mSubtype;
    /* 0x0FF */ bool mIsSetup;
    /* 0x100 */ u8 field_0x100;
    /* 0x101 */ u8 field_0x101;
    /* 0x102 */ u8 field_0x102;
    /* 0x104 */ u32 mFlagsPersistent;
    /* 0x108 */ u32 mFlagsHold;
    /* 0x10C */ const dSndSoundCtxParam *mpCtxParam;
    /* 0x110 */ LIST_MEMBER(dSndDistantSoundActor_c, DistantSound); // node offset 0xEC
    /* 0x11C */ UNKWORD field_0x11C;
    /* 0x120 */ LIST_MEMBER(dSndSeSound1_c, HandleType1); // node offset 0x4
    /* 0x12C */ LIST_MEMBER(dSndSeSound2_c, HandleType2); // node offset 0x4
    /* 0x138 */ nw4r::ut::Node mGroupLink;                // node for list in dSndSourceGroup_c
    /* 0x140 */ dSndSourceGroup_c *mpOwnerGroup;
    /* 0x144 */ f32 mVolumeFadeTarget;
    /* 0x148 */ f32 mVolumeFadeStepSize;
    /* 0x14C */ nw4r::ut::Node mSubSourceLink; // node for list in dSndSourceEnemyMultiBase_c
    /* 0x154 */ s32 mAnimEventValue;
    /* 0x158 */ s16 mPolyAttr0;
    /* 0x15A */ s16 mPolyAttr1;
};

#endif
