#ifndef D_LYT_COMMON_ICON_ITEM_H
#define D_LYT_COMMON_ICON_ITEM_H

#include <d/lyt/d2d.h>
#include <d/lyt/d_structd.h>
#include <d/lyt/d_lyt_sub.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytCommonIconItemPart1_c  {
public:
    dLytCommonIconItemPart1_c() { }
    virtual ~dLytCommonIconItemPart1_c() {}
    dLytSub lyt;
    d2d::AnmGroup_c mAnm[7];
    u32 unk[0x3];
    d2d::dLytStructD structD;
};

class dLytCommonIconItemPart2_c {
public:
    virtual ~dLytCommonIconItemPart2_c() {}
    dLytSub lyt;
    d2d::AnmGroup_c mAnm[10];
    u32 unk[0x3];
    d2d::dLytStructD structD;
};

class dLytCommonIconItemPart3_c {
public:
    virtual ~dLytCommonIconItemPart3_c() {}
    dLytSub lyt;
    d2d::AnmGroup_c mAnm[3];
    u32 unk;
    d2d::dLytStructD structD;
};

class dLytCommonIconItem_c : public dLytMeterBase {
public:
    dLytCommonIconItem_c() : mStateMgr(*this, sStateID::null), unk(3) {}
    ~dLytCommonIconItem_c() {}

    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;

    void init(void *, u8);

private:
    STATE_FUNC_DECLARE(dLytCommonIconItem_c, None);
    STATE_FUNC_DECLARE(dLytCommonIconItem_c, In);
    STATE_FUNC_DECLARE(dLytCommonIconItem_c, Wait);
    STATE_FUNC_DECLARE(dLytCommonIconItem_c, Out);

    UI_STATE_MGR_DECLARE(dLytCommonIconItem_c);
    u8 unk;
    dLytCommonIconItemPart1_c part1;
    u32 unk2[11];
    dLytCommonIconItemPart2_c part2;
    u32 unk3[13];
    dLytCommonIconItemPart3_c part3;
    u32 unk4;
};

#endif
