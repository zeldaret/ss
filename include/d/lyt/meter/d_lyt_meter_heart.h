#ifndef D_LYT_METER_HEART_H
#define D_LYT_METER_HEART_H

#include "d/lyt/d2d.h"
#include "nw4r/lyt/lyt_pane.h"

class dLytMeterHeart_c : public d2d::dSubPane {
public:
    dLytMeterHeart_c() {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    virtual ~dLytMeterHeart_c() {}
    
    s32 getField_0x78C() const {
        return field_0x78C;
    }

    u8 getCurrentHealthCapacity() const;

private:
    u8 getCurrentHealth() const;
    void realizeHeartsState();
    void executeInternal();
    s32 getNumDisplayedHearts() const;
    s32 getDisplayedHealthCapacity() const;

    /* 0x008 */ d2d::dLytSub mLyt;
    /* 0x09C */ d2d::AnmGroup_c mAnm[26];
    /* 0x71C */ nw4r::lyt::Pane *mpHeartPanes[20];
    /* 0x76C */ nw4r::lyt::Pane *mpAlphaPane;
    /* 0x770 */ nw4r::lyt::Pane *mpMainPane;
    /* 0x774 */ s32 mCurrentHealth;
    /* 0x778 */ s32 mCurrentHealthCapacity;
    /* 0x77C */ s32 mStoredHealth;
    /* 0x780 */ s32 mStoredHealthCapacity;
    /* 0x784 */ s32 mHealthCapacityIncreaseDelayTimer;
    /* 0x788 */ s32 mEffectHeartIdx;
    /* 0x78C */ s32 field_0x78C;
    /* 0x790 */ s32 field_0x790;
    /* 0x794 */ u8 field_0x794;
    /* 0x795 */ u8 field_0x795;
    /* 0x796 */ u8 field_0x796;
    /* 0x797 */ bool field_0x797;
    /* 0x798 */ bool mIsLoop;
    /* 0x799 */ bool mIsDanger;
    /* 0x79A */ u8 field_0x79A;
};

#endif
