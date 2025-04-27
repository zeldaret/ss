#ifndef D_LYT_METER_H
#define D_LYT_METER_H

// clang-format off
#include "common.h"

#include "d/lyt/d2d.h"
#include "d/d_cursor_hit_check.h"
#include "d/lyt/d_lyt_do_button.h"
#include "d/lyt/meter/d_lyt_meter_shield_gauge.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/meter/d_lyt_meter_cross_btn.h"
#include "d/lyt/meter/d_lyt_meter_plus_btn.h"
#include "d/lyt/meter/d_lyt_meter_minus_btn.h"
#include "d/lyt/meter/d_lyt_meter_dowsing.h"
#include "d/lyt/meter/d_lyt_meter_a_btn.h"
#include "d/lyt/meter/d_lyt_meter_z_btn.h"
#include "d/lyt/meter/d_lyt_meter_nun_stk.h"
#include "d/lyt/meter/d_lyt_meter_heart.h"
#include "d/lyt/meter/d_lyt_meter_remocon_bg.h"
#include "d/lyt/meter/d_lyt_meter_nunchaku_bg.h"
#include "d/lyt/meter/d_lyt_meter_rupy.h"
#include "d/lyt/meter/d_lyt_meter_ganbari_gauge.h"
#include "d/lyt/meter/d_lyt_meter_event_skip.h"
#include "d/lyt/meter/d_lyt_meter_item_select.h"
#include "d/lyt/meter/d_lyt_meter_parts.h"
#include "d/lyt/d_lyt_unknowns.h"

#include "m/m_vec.h"
// clang-format on

class dLytMeter1Button_c;
class dLytMeter2Button_c;
class dLytMeterTimer_c;
class LytMeterTimerPart1_c;
class LytMeterTimerPart2_c;
class dLytMeterKakeraKey_c;
class dLytMeterBossKey_c;
class dLytMeterSmallKey_c;
class dLytMeterDrink_c;
class dLytSkyGauge_c;
class dLytBirdGauge_c;
class dLytBossGauge_c;

// apart from dLytMeterParts_c the names here are made up

enum MeterFlag_e {
    METER_BTN_CROSS_UP = 0x1,
    METER_BTN_CROSS_DOWN = 0x2,
    METER_BTN_CROSS_LEFT = 0x4,
    METER_BTN_CROSS_RIGHT = 0x8,
    METER_BTN_B = 0x10,
    METER_BTN_PLUS = 0x20,
    METER_BTN_MINUS = 0x40,
    METER_BTN_1 = 0x80,
    METER_BTN_2 = 0x100,
    METER_BTN_NUN_STK = 0x200,
    METER_BTN_C = 0x400,
    METER_BTN_Z = 0x800,
    METER_BTN_0x1000 = 0x1000,


    METER_BTN_CROSS = METER_BTN_CROSS_UP | METER_BTN_CROSS_DOWN | METER_BTN_CROSS_LEFT | METER_BTN_CROSS_RIGHT,
};

class dLytMeterMain_c {
    friend class dLytMeter_c;

public:
    dLytMeterMain_c();
    virtual ~dLytMeterMain_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool draw();

    bool isNotSilentRealmOrLoftwing();

    bool fn_800C9F70();
    bool fn_800C9FE0();
    bool fn_800CA040();

    bool fn_800D5350();
    bool fn_800D5380(u8);
    void fn_800D5290();
    bool fn_800D5590();
    void fn_800D5630();
    bool fn_800D5650();
    bool fn_800D5680();
    bool fn_800D56B0();
    void checkPaneVisibility();
    bool execute();

    bool isSilentRealm();

    bool fn_800D53D0();
    bool fn_800D5420();

    void setUiMode(u16 value) const;
    u8 getUiMode();

    bool getItemSelectNotHiddenByAreaCaption() const {
        return mPanesNotHiddenByAreaCaption[0];
    }

    bool getMinusBtnNotHiddenByAreaCaption() const {
        return mPanesNotHiddenByAreaCaption[1];
    }

    bool getDowsingNotHiddenByAreaCaption() const {
        return mPanesNotHiddenByAreaCaption[8];
    }

    void setGanbariGaugeHiddenByAreaCaption(bool visible) {
        mPanesNotHiddenByAreaCaption[15] = visible;
    }

    bool getField_0x1377E() const {
        return field_0x1377E;
    }

    bool getField_0x1377F() const {
        return field_0x1377F;
    }

    dLytMeterNunStk_c *getNunStk() {
        return &mNunStk;
    }

private:
    /* 0x00004 */ d2d::ResAccIf_c mResAcc;
    /* 0x00374 */ d2d::LytBase_c mLyt;
    /* 0x00404 */ d2d::AnmGroup_c mAnmGroups[34];
    /* 0x00C84 */ u8 _0x00C84[0x00C8C - 0x00C84];
    /* 0x00C8C */ dLytMeterGanbariGauge_c mGanbariGauge;
    /* 0x011E8 */ dLytMeterRupy_c mRupy;
    /* 0x01A98 */ dLytMeterItemSelect_c mItemSelect;
    /* 0x07260 */ dLytMeterCrossBtn_c mCrossBtn;
    /* 0x078A0 */ dLytMeterPlusBtn_c mPlusBtn;
    /* 0x07A64 */ dLytMeterMinusBtn_c mMinusBtn;
    /* 0x0C928 */ dLytMeterDowsing_c mDowsing;
    /* 0x11E34 */ dLytMeterABtn_c mABtn;
    /* 0x12000 */ dLytMeterZBtn_c mZBtn;
    /* 0x121C8 */ dLytMeterNunStk_c mNunStk;
    /* 0x12350 */ dLytMeterRemoconBg_c mRemoCon;
    /* 0x12440 */ dLytMeterNunchakuBg_c mNunBg;
    /* 0x12608 */ dLytMeterHeart_c mHeart;
    /* 0x12DA4 */ dLytMeterShieldGauge_c mShield;
    /* 0x130C4 */ dLytMeter1Button_c *mp1Button;
    /* 0x130C8 */ dLytMeter2Button_c *mp2Button;
    /* 0x130CC */ dLytMeterTimer_c *mpTimer;
    /* 0x130D0 */ LytMeterTimerPart1_c *mpTimerPart1;
    /* 0x130D4 */ LytMeterTimerPart2_c *mpTimerPart2;
    /* 0x130D8 */ dLytMeterKakeraKey_c *mpKakeraKey;
    /* 0x130DC */ dLytMeterBossKey_c *mpBossKey;
    /* 0x130E0 */ dLytMeterSmallKey_c *mpSmallKey;
    /* 0x130E4 */ dLytMeterDrink_c *mpDrink;
    /* 0x130E8 */ dLytSkyGauge_c *mpSkyGauge;
    /* 0x130EC */ dLytBirdGauge_c *mpBirdGauge;
    /* 0x130F0 */ dLytBossGauge_c *mpBossGauge;
    /* 0x130F4 */ d2d::SubPaneList mMeters;

    /* 0x13100 */ d2d::SubPaneListNode mNodes[16];
    /* 0x13200 */ u8 _0x13200[0x13204 - 0x13200];
    /* 0x13204 */ dLytMeterParts_c mParts[16];

    /* 0x136C4 */ mVec3_c mShieldPositions[2];
    /* 0x136DC */ mVec3_c mRupyPositions[7];
    /* 0x13730 */ mVec3_c mShieldPos;
    /* 0x1373C */ mVec3_c mRupyPos;
    /* 0x13748 */ s32 field_0x13748;
    /* 0x1374C */ s32 field_0x1374C;
    /* 0x13750 */ s32 field_0x13750;
    /* 0x13754 */ s32 field_0x13754;
    /* 0x13758 */ s32 mShieldPosIndex;
    /* 0x1375C */ s32 mRupyPosIndex;
    /* 0x13760 */ s32 mRupyPosInterpFrame;
    /* 0x13764 */ s32 mShieldPosInterpFrame;
    /* 0x13768 */ s32 mOldShieldPosIndex;
    /* 0x1376C */ s32 mOldRupyPosIndex;
    /* 0x13770 */ u8 field_0x13770;
    /* 0x13771 */ bool field_0x13771;
    /* 0x13772 */ bool field_0x13772;
    /* 0x13773 */ bool field_0x13773;
    /* 0x13774 */ bool field_0x13774;
    /* 0x13775 */ bool field_0x13775;
    /* 0x13776 */ bool mTimerVisible;
    /* 0x13777 */ bool mBirdGaugeVisible;
    /* 0x13778 */ bool mSkyGaugeVisible;
    /* 0x13779 */ bool mBossGaugeVisible;
    /* 0x1377A */ bool mKakeraKeyVisible;
    /* 0x1377B */ bool mBossKeyVisible;
    /* 0x1377C */ bool mSmallKeyVisible;
    /* 0x1377D */ bool mDrinkVisible;
    /* 0x1377E */ bool field_0x1377E;
    /* 0x1377F */ bool field_0x1377F;
    /* 0x13780 */ bool field_0x13780;
    /* 0x13781 */ bool field_0x13781;
    /* 0x13782 */ bool mPanesVisible[16];
    /* 0x13792 */ bool mPanesNotHiddenByAreaCaption[16];
    /* 0x137A2 */ bool mPanesForceShown[16];
    /* 0x137B2 */ bool field_0x137B2;
    /* 0x137B4 */ mVec3_c mPos3;
    /* 0x137C0 */ u32 field_0x137C0;
};

class dLytMeter_c {
public:
    dLytMeter_c();
    virtual ~dLytMeter_c() {
        sInstance = nullptr;
    }

    bool build();
    bool remove();
    bool execute();
    bool draw();

    bool fn_800D5670();
    bool fn_800D56F0();
    bool fn_800D97A0();
    /** Running out of good names for this function, but this allows
        the area caption to temporarily hide the main HUD. */
    void setAreaCaptionOverrideVisibility(bool visible);
    void fn_800D9710();
    void fn_800D9730(u8 val);
    void fn_800D9780(bool val);
    static void fn_800D97E0(u8);
    static void setVisible(bool);

    static void setStaminaWheelPercent(f32 percent);
    void setStaminaWheelPercentInternal(f32 percent);

    static dLytMeter_c *GetInstance() {
        return sInstance;
    }

    static dLytMeterMain_c *GetMain() {
        return &sInstance->mMain;
    }

    // Not all of these inlines exist on dLytMeterMain_c
    // because accessing via GetMeter->get... causes
    // different instructions sometimes

    s32 getMeterField_0x13750() const {
        return mMain.field_0x13750;
    }

    u8 getMeterField_0x13770() const {
        return mMain.field_0x13770;
    }

    bool getMeterField_0x13774() const {
        return mMain.field_0x13774;
    }

    bool getMeterField_0x1377F() const {
        return mMain.field_0x1377F;
    }

    bool checkAllFlags(u32 mask) const {
        return (mFlags & mask) == mask;
    }

    void clearFlags(u32 mask) {
        mFlags = mFlags & ~mask;
    }

    void setFlags(u32 mask) {
        mFlags = mFlags | mask;
    }

    void resetFlags() {
        mFlags = 0xFFFFFFFF;
    }

    static s32 getCrossBtn0x7BF8() {
        if (sInstance != nullptr) {
            return sInstance->mMain.mCrossBtn.getField_0x620();
        } else {
            return 6;
        }
    }

    static bool getItemSelect0x75A2() {
        if (sInstance != nullptr) {
            return sInstance->mMain.mItemSelect.getField_0x57A2();
        } else {
            return 0;
        }
    }

    static void setRupyField_0x8A9(u8 val) {
        if (sInstance != nullptr) {
            sInstance->mMain.mRupy.setField_0x8A9(val);
        }
    }

    static void setRupyField_0x8AA(u8 val) {
        if (sInstance != nullptr) {
            sInstance->mMain.mRupy.setField_0x8AA(val);
        }
    }

    static void setRupyField_0x8AC(u8 val) {
        if (sInstance != nullptr) {
            sInstance->mMain.mRupy.setField_0x8AC(val);
        }
    }

    static u8 getRupyField_0x8AC() {
        if (sInstance != nullptr) {
            return sInstance->mMain.mRupy.getField_0x8AC();
        } else {
            return 0;
        }
    }

    static s32 getHeartField_0x78C() {
        if (sInstance != nullptr) {
            return sInstance->mMain.mHeart.getField_0x78C();
        } else {
            return 0;
        }
    }

    static void setField_0x13B61(u8 val) {
        if (sInstance != nullptr) {
            sInstance->field_0x13B61 = val;
        }
    }

    static void setField_0x13B63(u8 val) {
        if (sInstance != nullptr) {
            sInstance->field_0x13B63 = val;
        }
    }

    static u8 getField_0x13B63() {
        if (sInstance != nullptr) {
            return sInstance->field_0x13B63;
        } else {
            return 0;
        }
    }

    static void setField_0x13B64(u8 val) {
        if (sInstance != nullptr) {
            sInstance->field_0x13B64 = val;
        }
    }

    static u8 getField_0x13B66() {
        if (sInstance != nullptr) {
            return sInstance->field_0x13B66;
        } else {
            return 0;
        }
    }

    static u8 getDowsing0x550A() {
        if (sInstance != nullptr) {
            return sInstance->mMain.mDowsing.getField_0x550A();
        } else {
            return 0;
        }
    }

    static bool getfn_800C9FE0() {
        if (sInstance != nullptr) {
            return sInstance->mMain.fn_800C9FE0();
        } else {
            return false;
        }
    }

    static bool getfn_800D97A0() {
        if (sInstance != nullptr) {
            return sInstance->fn_800D97A0();
        } else {
            return false;
        }
    }

    static bool getMinusBtnFn800F7600() {
        if (sInstance != nullptr) {
            return sInstance->mMain.mMinusBtn.fn_800F7600();
        } else {
            return false;
        }
    }

    static bool getDowsingFn800FE4B0() {
        if (sInstance != nullptr) {
            return sInstance->mMain.mDowsing.fn_800FE4B0();
        } else {
            return false;
        }
    }

private:
    /* 0x00004 */ d2d::ResAccIf_c mResAcc;
    /* 0x00374 */ dLytMeterEventSkip_c *mpEventSkip;
    /* 0x00378 */ dLytMeterMain_c mMain;
    /* 0x13B3C */ dLytDobutton_c *mpDoButton;
    /* 0x13B40 */ LytDoButtonRelated *mpDoButtonRelated;
    /* 0x13B44 */ LytBirdButtonRelated *mpBirdRelated;
    /* 0x13B48 */ bool mVisible;
    /* 0x13B49 */ u8 _0x13B49[0x13B50 - 0x13B49];
    /* 0x13B50 */ s32 mFlags;
    /* 0x13B54 */ s32 field_0x13B54;
    /* 0x13B58 */ s32 field_0x13B58;
    /* 0x13B5C */ s32 field_0x13B5C;
    /* 0x13B60 */ bool field_0x13B60;
    /* 0x13B61 */ bool field_0x13B61;
    /* 0x13B62 */ bool field_0x13B62;
    /* 0x13B63 */ bool field_0x13B63;
    /* 0x13B64 */ bool field_0x13B64;
    /* 0x13B65 */ bool field_0x13B65;
    /* 0x13B66 */ bool field_0x13B66;

    static dLytMeter_c *sInstance;
};

#endif
