#ifndef D_LYT_METER_TIMER_H
#define D_LYT_METER_TIMER_H

#include <d/lyt/d2d.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <m/m_color.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>


class LytMeterTimerPart1_c : public dLytMeterBase {
public:
    LytMeterTimerPart1_c() {
        sInstance = this;
    }
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~LytMeterTimerPart1_c() {
        sInstance = nullptr;
    }

    static LytMeterTimerPart1_c *sInstance;
private:
    /* 0x008 */ d2d::dLytSub mLyt;
    /* 0x09C */ d2d::AnmGroup_c mAnmGroups[26];
    /* 0x71C */ u8 field_0x71C[0x768 - 0x71C];
    /* 0x768 */ mColor mColors1[3];
    /* 0x774 */ mColor mColors2[3];
};

class LytMeterTimerPart2_c : public dLytMeterBase {
public:
    LytMeterTimerPart2_c() {
        sInstance = this;
    }
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~LytMeterTimerPart2_c() {
        sInstance = nullptr;
    }

    static LytMeterTimerPart2_c *sInstance;
private:
    /* 0x008 */ d2d::dLytSub mLyt;
    /* 0x09C */ d2d::AnmGroup_c mAnmGroups[30];
    /* 0x81C */ u8 field_0x81C[0x828 - 0x81C];
    /* 0x828 */ mColor mColors1[2];
    /* 0x830 */ mColor mColors2[2];
};

class dLytMeterTimer_c {
public:
    dLytMeterTimer_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeterTimer_c() {}
    bool build();
    bool remove();
private:
    STATE_FUNC_DECLARE(dLytMeterTimer_c, ChangeSiren);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, Siren);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, ChangeSafeBloom);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, ChangeSafe);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, Safe);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, ChangeFruits);
    STATE_FUNC_DECLARE(dLytMeterTimer_c, Fruits);

    UI_STATE_MGR_DECLARE(dLytMeterTimer_c);
};

#endif
