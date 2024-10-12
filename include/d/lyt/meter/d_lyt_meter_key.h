#ifndef D_LYT_METER_KEY_H
#define D_LYT_METER_KEY_H

#include <d/lyt/d2d.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytMeterKakeraKey_c {
public:
    dLytMeterKakeraKey_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterKakeraKey_c() {}

    bool build(d2d::ResAccIf_c *);
private:


    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytMeterKakeraKey_c);
    /* 0x03C */ d2d::dLytSub mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnmGroups[8];
    /* 0x2D4 */ mVec3_c mVecs[2];
};

class dLytMeterBossKey_c {
public:
    dLytMeterBossKey_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterBossKey_c() {}

    virtual void BossKey0x8() {}
    virtual void BossKey0xC() {}

    bool build(d2d::ResAccIf_c *);
private:


    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterBossKey_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnmGroups[5];
    /* 0x218 */ mVec3_c mVecs[4];
};

class dLytMeterSmallKey_c {
public:
    dLytMeterSmallKey_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterSmallKey_c() {}

    virtual void SmallKey0x8() {}
    virtual void SmallKey0xC() {}

    bool build(d2d::ResAccIf_c *);
private:


    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterSmallKey_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnmGroups[5];
    /* 0x218 */ mVec3_c mVecs[4];
};

#endif
