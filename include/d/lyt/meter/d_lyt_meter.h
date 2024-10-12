#ifndef D_LYT_METER_H
#define D_LYT_METER_H
// TODO sort includes for vtable order
// clang-format off
#include "d/lyt/d2d.h"
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

#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/d_lyt_do_button.h"

#include <d/lyt/meter/d_lyt_meter_timer.h>
#include <d/lyt/d_lyt_sky_gauge.h>
#include <d/lyt/d_lyt_bird_gauge.h>
#include <d/lyt/d_lyt_boss_gauge.h>
#include <d/lyt/meter/d_lyt_meter_key.h>
#include <d/lyt/meter/d_lyt_meter_drink.h>

// clang-format on

class dLytMeter1Button_c : public dLytMeterBase {
public:
    dLytMeter1Button_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeter1Button_c() {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;
private:
    STATE_FUNC_DECLARE(dLytMeter1Button_c, Wait);
    STATE_FUNC_DECLARE(dLytMeter1Button_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeter1Button_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeter1Button_c, Unuse);

    UI_STATE_MGR_DECLARE(dLytMeter1Button_c);
    d2d::dLytSub mLyt;
    d2d::AnmGroup_c mAnmGroups[3];
    /* 0x198 */ dWindow_c *mpWindow;
    /* 0x19C */ dTextBox_c *mpTextbox;
    /* 0x1A0 */ u32 field_0x1A0;
    /* 0x1A4 */ s32 field_0x1A4;
    /* 0x1A8 */ s32 field_0x1A8;
    /* 0x1AC */ u8 field_0x1AC;
    /* 0x1AD */ u8 field_0x1AD;
};

class dLytMeter2Button_c : public dLytMeterBase {
public:
    dLytMeter2Button_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeter2Button_c() {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;
private:
    STATE_FUNC_DECLARE(dLytMeter2Button_c, Wait);
    STATE_FUNC_DECLARE(dLytMeter2Button_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeter2Button_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeter2Button_c, Unuse);

    UI_STATE_MGR_DECLARE(dLytMeter2Button_c);
    d2d::dLytSub mLyt;
    d2d::AnmGroup_c mAnmGroups[3];
    /* 0x198 */ dWindow_c *mpWindow;
    /* 0x19C */ dTextBox_c *mpTextbox;
    /* 0x1A0 */ u32 field_0x1A0;
    /* 0x1A4 */ s32 field_0x1A4;
    /* 0x1A8 */ s32 field_0x1A8;
    /* 0x1AC */ u8 field_0x1AC;
    /* 0x1AD */ u8 field_0x1AD;
};

class dLytMeterParts_c {
public:
    dLytMeterParts_c() : mStateMgr(*this, sStateID::null) {}

private:
    STATE_FUNC_DECLARE(dLytMeterParts_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterParts_c, In);
    STATE_FUNC_DECLARE(dLytMeterParts_c, Visible);
    STATE_FUNC_DECLARE(dLytMeterParts_c, Out);

    UI_STATE_MGR_DECLARE(dLytMeterParts_c);
    u8 field_0x3C[0x10];
};

class dLytMeter_c {
public:
    dLytMeter_c();
    virtual ~dLytMeter_c() {}

    bool build(d2d::ResAccIf_c *resAcc);

    bool isSilentRealm();

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
    /* 0x130F4 */ LytMeterGroup mMeters;

    /* 0x13100 */ LytMeterListNode mNodes[16];
    /* 0x13200 */ u8 field_0x13200[0x13204 - 0x13200];
    /* 0x13204 */ dLytMeterParts_c mParts[16];

    /* 0x136C4 */ mVec3_c mPosArray1[2];
    /* 0x136DC */ mVec3_c mPosArray2[7];
    /* 0x13730 */ mVec3_c mPos1;
    /* 0x1373C */ mVec3_c mPos2;
    /* 0x13748 */ u8 field_0x13748[0x137B4 - 0x13748];
    /* 0x137B4 */ mVec3_c mPos3;
    /* 0x13B38 */ u32 field_0x13B38;
};

// made up name
class dLytMeterContainer_c {
public:
    dLytMeterContainer_c();
    virtual ~dLytMeterContainer_c() {
        sInstance = nullptr;
    }

    bool build();

    static dLytMeterContainer_c *sInstance;

    // FIXME This goes to doButton!!!
    bool isButtonFlag(u32 flag) {
        return (mMeter.field_0x13B38 & flag) == flag;
    }
private:
    /* 0x00004 */ d2d::ResAccIf_c mResAcc;
    /* 0x00374 */ dLytMeterEventSkip_c *mpEventSkip;
    /* 0x00378 */ dLytMeter_c mMeter;
    /* 0x13B3C */ dLytDobutton_c *mpDoButton;
    /* 0x13B40 */ LytDoButtonRelated *mpDoButtonRelated;
    /* 0x13B44 */ LytBirdButtonRelated *mpBirdRelated;
};

#endif
