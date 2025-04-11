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

// clang-format on

class dLytMeter_c {
public:
    dLytMeter_c();
    virtual ~dLytMeter_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool draw();

    bool fn_800D5650();
    bool fn_800D5680();

    bool isSilentRealm();

    u8 getField_0x13792() const {
        return field_0x13792;
    }

    u8 getField_0x1377E() const {
        return field_0x1377E;
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

    /* 0x136C4 */ mVec3_c mPosArray1[2];
    /* 0x136DC */ mVec3_c mPosArray2[7];
    /* 0x13730 */ mVec3_c mPos1;
    /* 0x1373C */ mVec3_c mPos2;
    /* 0x13748 */ u8 field_0x13748[0x13750 - 0x13748];
    /* 0x13750 */ s32 field_0x13750;
    /* 0x13754 */ u8 field_0x13754[0x13770 - 0x13754];
    /* 0x13770 */ u8 field_0x13770;
    /* 0x13771 */ u8 field_0x13771[0x1377E - 0x13771];
    /* 0x1377E */ u8 field_0x1377E;
    /* 0x1377F */ u8 field_0x1375F[0x13792 - 0x1377F];
    /* 0x13792 */ u8 field_0x13792;
    /* 0x13793 */ u8 field_0x13793[0x137B4 - 0x13793];
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

    bool checkAllFlags(u32 mask) const {
        return (mFlags & mask) == mask;
    }

    static s32 getCrossBtn0x7BF8() {
        if (sInstance != nullptr) {
            return sInstance->mMeter.mCrossBtn.getField_0x620();
        } else {
            return 6;
        }
    }

    static bool getItemSelect0x75B2() {
        if (sInstance != nullptr) {
            return sInstance->mMeter.mItemSelect.getField_0x57A2();
        } else {
            return 0;
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

    static dLytMeterContainer_c *sInstance;
};

#endif
