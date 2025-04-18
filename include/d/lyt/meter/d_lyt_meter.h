#ifndef D_LYT_METER_H
#define D_LYT_METER_H
// TODO sort includes for vtable order
// clang-format off
#include "common.h"
#include "d/lyt/d2d.h"
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

#include "d/lyt/meter/d_lyt_meter_key.h"
#include "d/lyt/d_lyt_unknowns.h"

#include "d/lyt/meter/d_lyt_meter_drink.h"
#include "d/lyt/meter/d_lyt_meter_timer.h"
#include "d/lyt/d_lyt_bird_gauge.h"
#include "d/lyt/d_lyt_boss_gauge.h"
#include "d/lyt/d_lyt_sky_gauge.h"
#include "m/m_vec.h"

enum Meter_e {
    METER_GANBARI = 0,
    METER_RUPY = 1,
    METER_ITEM_SELECT = 2,
    METER_CROSS_BTN = 3,
    METER_PLUS_BTN = 4,
    METER_MINUS_BTN = 5,
    METER_A_BTN = 6,
    METER_DOWSING = 7,
    METER_Z_BTN = 8,
    METER_NUN_STK = 9,
    METER_REMOCON_BG = 10,
    METER_NUN_BG = 11,
    METER_HEART = 12,
    METER_SHIELD = 13,
    METER_1_BTN = 14,
    METER_2_BTN = 15,
};

// clang-format on

class dLytMeter_c {
public:
    dLytMeter_c();
    virtual ~dLytMeter_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool draw();

    bool fn_800D5380(u8);
    bool fn_800D5650();
    bool fn_800D5680();
    bool fn_800C9FE0();

    bool isSilentRealm();

    void setUiMode(u16 value) const;
    u8 getUiMode();

    u8 getField_0x13792() const {
        return field_0x13792[0];
    }

    u8 getField_0x13793() const {
        return field_0x13792[1];
    }

    u8 getField_0x1377E() const {
        return field_0x1377E;
    }

    u8 getField_0x1377F() const {
        return field_0x1377F;
    }

    dLytMeterNunStk_c *getNunStk() {
        return &mNunStk;
    }

    /* 0x00004 */ d2d::ResAccIf_c mResAcc;
    /* 0x00374 */ d2d::LytBase_c mLyt;
    /* 0x00404 */ d2d::AnmGroup_c mAnmGroups[34];
    u8 field_0x00C84[0x00C8C - 0x00C84];
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
    /* 0x13200 */ u8 field_0x13200[0x13204 - 0x13200];
    /* 0x13204 */ dLytMeterParts_c mParts[16];

    /* 0x136C4 */ mVec3_c mShieldPositions[2];
    /* 0x136DC */ mVec3_c mRupyPositions[7];
    /* 0x13730 */ mVec3_c mShieldPos;
    /* 0x1373C */ mVec3_c mRupyPos;
    /* 0x13748 */ s32 field_0x13748;
    /* 0x1374C */ s32 field_0x1374C;
    /* 0x13750 */ s32 field_0x13750;
    /* 0x13754 */ s32 field_0x13754;
    /* 0x13758 */ s32 field_0x13758;
    /* 0x1375C */ s32 field_0x1375C;
    /* 0x13760 */ s32 field_0x13760;
    /* 0x13764 */ s32 field_0x13764;
    /* 0x13768 */ s32 field_0x13768;
    /* 0x1376C */ s32 field_0x1376C;
    /* 0x13770 */ u8 field_0x13770;
    /* 0x13771 */ u8 field_0x13771;
    /* 0x13772 */ u8 field_0x13772;
    /* 0x13773 */ u8 field_0x13773;
    /* 0x13774 */ u8 field_0x13774;
    /* 0x13775 */ u8 field_0x13775;
    /* 0x13776 */ u8 field_0x13776;
    /* 0x13777 */ u8 field_0x13777;
    /* 0x13778 */ u8 field_0x13778;
    /* 0x13779 */ u8 field_0x13779;
    /* 0x1377A */ u8 field_0x1377A;
    /* 0x1377B */ u8 field_0x1377B;
    /* 0x1377C */ u8 field_0x1377C;
    /* 0x1377D */ u8 field_0x1377D;
    /* 0x1377E */ u8 field_0x1377E;
    /* 0x1377F */ u8 field_0x1377F;
    /* 0x13780 */ u8 field_0x13780;
    /* 0x13781 */ u8 field_0x13781;
    /* 0x13782 */ u8 field_0x13782[16];
    /* 0x13792 */ u8 field_0x13792[16];
    /* 0x137A2 */ u8 field_0x137A2[16];
    /* 0x137B2 */ u8 field_0x137B2;
    /* 0x137B4 */ mVec3_c mPos3;
    /* 0x137C0 */ u32 field_0x137C0;
};

STATIC_ASSERT(offsetof(dLytMeter_c, mGanbariGauge) == 0x00C8C);
STATIC_ASSERT(offsetof(dLytMeter_c, mRupy) == 0x011E8);
STATIC_ASSERT(offsetof(dLytMeter_c, mItemSelect) == 0x01A98);
STATIC_ASSERT(offsetof(dLytMeter_c, mCrossBtn) == 0x07260);
STATIC_ASSERT(offsetof(dLytMeter_c, mPlusBtn) == 0x078A0);
STATIC_ASSERT(offsetof(dLytMeter_c, mMinusBtn) == 0x07A64);
STATIC_ASSERT(offsetof(dLytMeter_c, mDowsing) == 0x0C928);
STATIC_ASSERT(offsetof(dLytMeter_c, mABtn) == 0x11E34);
STATIC_ASSERT(offsetof(dLytMeter_c, mZBtn) == 0x12000);
STATIC_ASSERT(offsetof(dLytMeter_c, mNunStk) == 0x121C8);
STATIC_ASSERT(offsetof(dLytMeter_c, mRemoCon) == 0x12350);
STATIC_ASSERT(offsetof(dLytMeter_c, mNunBg) == 0x12440);
STATIC_ASSERT(offsetof(dLytMeter_c, mHeart) == 0x12608);
STATIC_ASSERT(offsetof(dLytMeter_c, mShield) == 0x12DA4);
STATIC_ASSERT(offsetof(dLytMeter_c, field_0x137C0) == 0x137C0);

// made up name
class dLytMeterContainer_c {
public:
    dLytMeterContainer_c();
    virtual ~dLytMeterContainer_c() {
        sInstance = nullptr;
    }

    bool build();
    bool remove();
    bool draw();

    bool fn_800D5670();
    bool fn_800D97A0();

    static dLytMeterContainer_c *GetInstance() {
        return sInstance;
    }

    static dLytMeter_c *GetMeter() {
        return &sInstance->mMeter;
    }

    // Not all of these inlines exist on dLytMeter_c
    // because accessing via GetMeter->get... causes
    // different instructions sometimes

    s32 getMeterField_0x13750() const {
        return mMeter.field_0x13750;
    }

    u8 getMeterField_0x13770() const {
        return mMeter.field_0x13770;
    }

    u8 getMeterField_0x13774() const {
        return mMeter.field_0x13774;
    }

    u8 getMeterField_0x1377F() const {
        return mMeter.field_0x1377F;
    }

    u8 getMeterField_0x1379A() const {
        return mMeter.field_0x13792[8];
    }

    bool checkAllFlags(u32 mask) const {
        return (mFlags & mask) == mask;
    }

    void clearFlags(u32 mask) {
        mFlags = mFlags & ~mask;
    }

    static s32 getCrossBtn0x7BF8() {
        if (sInstance != nullptr) {
            return sInstance->mMeter.mCrossBtn.getField_0x620();
        } else {
            return 6;
        }
    }

    static bool getItemSelect0x75A2() {
        if (sInstance != nullptr) {
            return sInstance->mMeter.mItemSelect.getField_0x57A2();
        } else {
            return 0;
        }
    }

    static u8 getField_0x13B63() {
        if (sInstance != nullptr) {
            return sInstance->field_0x13B63;
        } else {
            return 0;
        }
    }

    static void setRupyField_0x8A9(u8 val) {
        if (sInstance != nullptr) {
            sInstance->mMeter.mRupy.setField_0x8A9(val);
        }
    }

    static void setRupyField_0x8AA(u8 val) {
        if (sInstance != nullptr) {
            sInstance->mMeter.mRupy.setField_0x8AA(val);
        }
    }

    static void setRupyField_0x8AC(u8 val) {
        if (sInstance != nullptr) {
            sInstance->mMeter.mRupy.setField_0x8AC(val);
        }
    }

    static u8 getRupyField_0x8AC() {
        if (sInstance != nullptr) {
            return sInstance->mMeter.mRupy.getField_0x8AC();
        } else {
            return 0;
        }
    }

    static s32 getHeartField_0x78C() {
        if (sInstance != nullptr) {
            return sInstance->mMeter.mHeart.getField_0x78C();
        } else {
            return 0;
        }
    }

    static void setField_0x13B61(u8 val) {
        if (sInstance != nullptr) {
            sInstance->field_0x13B61 = val;
        }
    }

    static void setField_0x13B64(u8 val) {
        if (sInstance != nullptr) {
            sInstance->field_0x13B64 = val;
        }
    }

    static u8 getDowsing0x550A() {
        if (sInstance != nullptr) {
            return sInstance->mMeter.mDowsing.getField_0x550A();
        } else {
            return 0;
        }
    }

    static bool getfn_800C9FE0() {
        if (sInstance != nullptr) {
            return sInstance->mMeter.fn_800C9FE0();
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

private:
    /* 0x00004 */ d2d::ResAccIf_c mResAcc;
    /* 0x00374 */ dLytMeterEventSkip_c *mpEventSkip;
    /* 0x00378 */ dLytMeter_c mMeter;
    /* 0x13B3C */ dLytDobutton_c *mpDoButton;
    /* 0x13B40 */ LytDoButtonRelated *mpDoButtonRelated;
    /* 0x13B44 */ LytBirdButtonRelated *mpBirdRelated;
    /* 0x13B48 */ bool mVisible;
    /* 0x13B49 */ u8 _0x13B49[0x13B50 - 0x13B49];
    /* 0x13B50 */ s32 mFlags;
    /* 0x13B54 */ s32 field_0x13B54;
    /* 0x13B58 */ u8 _0x13B58[0x13B60 - 0x13B58];
    /* 0x13B60 */ u8 field_0x13B60;
    /* 0x13B61 */ u8 field_0x13B61;
    /* 0x13B62 */ u8 field_0x13B62;
    /* 0x13B63 */ u8 field_0x13B63;
    /* 0x13B64 */ u8 field_0x13B64;
    /* 0x13B65 */ u8 field_0x13B65;
    /* 0x13B66 */ u8 field_0x13B66;

    static dLytMeterContainer_c *sInstance;
};

#endif
