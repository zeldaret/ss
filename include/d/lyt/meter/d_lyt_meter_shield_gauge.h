#ifndef D_LYT_METER_SHIELD_GAUGE_H
#define D_LYT_METER_SHIELD_GAUGE_H

#include "d/lyt/d2d.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytMeterShieldGauge_c : public d2d::dSubPane {
public:
    dLytMeterShieldGauge_c() : mStateMgr(*this, sStateID::null) {}
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

    virtual ~dLytMeterShieldGauge_c() {}

    void setContainerGroup(d2d::AnmGroup_c *g) {
        mpContainerAnmGroup = g;
    }

    void setOwnerPane(nw4r::lyt::Pane *pane) {
        mpOwnerPane = pane;
    }

    f32 getCurrentDurability() const {
        return mCurrentDurability;
    }

    void setfield_0x318(s32 v) {
        field_0x318 = v;
    }

    u8 getField_0x31D() const {
        return field_0x31D;
    }

    f32 getMaxDurability() const {
        return mMaxDurability;
    }

    void setMaxDurability(f32 durability) {
        mMaxDurability = durability;
    }

    void setShieldId(s32 id) {
        mShieldId = id;
    }

private:
    s32 getLytFrameForShield(s32) const;
    void setLevel(f32 level);
    f32 calcUpdownRatio(f32) const;

    STATE_FUNC_DECLARE(dLytMeterShieldGauge_c, Normal);
    STATE_FUNC_DECLARE(dLytMeterShieldGauge_c, Use);
    STATE_FUNC_DECLARE(dLytMeterShieldGauge_c, ToMax);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterShieldGauge_c);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytMeterShieldGauge_c);
    /* 0x044 */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnm[8];
    /* 0x2D8 */ d2d::AnmGroup_c *mpContainerAnmGroup;
    /* 0x2DC */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x2E0 */ f32 mCurrentDurability;
    /* 0x2E4 */ f32 field_0x2E4;
    /* 0x2E8 */ f32 mSavedDurability;
    /* 0x2EC */ f32 mAnimatingDurability;
    /* 0x2F0 */ f32 mMaxDurability;
    /* 0x2F4 */ f32 field_0x2F4;
    /* 0x2F8 */ f32 field_0x2F8;
    /* 0x2FC */ s32 field_0x2FC;
    /* 0x300 */ f32 field_0x300;
    /* 0x304 */ f32 field_0x304;
    /* 0x308 */ s32 mCurrentShieldPouchSlot;
    /* 0x30C */ u8 field_0x30C;
    /* 0x30D */ u8 field_0x30D;
    /* 0x30E */ u8 field_0x30E;
    /* 0x310 */ s32 mShieldId;
    /* 0x314 */ s32 mSavedShieldType;
    /* 0x318 */ s32 field_0x318;
    /* 0x31C */ u8 field_0x31C;
    /* 0x31D */ u8 field_0x31D;
    /* 0x31E */ u8 field_0x31E;
    /* 0x31F */ u8 field_0x31F;
};

#endif
