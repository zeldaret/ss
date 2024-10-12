#ifndef D_LYT_COMMON_ICON_ITEM_H
#define D_LYT_COMMON_ICON_ITEM_H

#include <d/lyt/d2d.h>
#include <d/lyt/d_lyt_sub.h>
#include <d/lyt/d_structd.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytCommonIconItemPart1_c {
public:
    dLytCommonIconItemPart1_c()
        : field_0x2D4(0), field_0x2D5(0), field_0x2FC(0), field_0x2FD(0), field_0x2FE(0), field_0x2FF(0) {}
    virtual ~dLytCommonIconItemPart1_c() {}
    dLytSub lyt;
    d2d::AnmGroup_c mAnm[7];
    u32 unk[0x3];
    d2d::dLytStructD structD;
    u8 field_0x2D4;
    u8 field_0x2D5;
    u8 field_0x2D6[0x2FC - 0x2D6];
    u8 field_0x2FC;
    u8 field_0x2FD;
    u8 field_0x2FE;
    u8 field_0x2FF;
};

class dLytCommonIconItemPart2_c {
public:
    dLytCommonIconItemPart2_c()
        : field_0x64C(0), field_0x64D(0), field_0x674(0), field_0x675(0), field_0x676(0), field_0x677(0),
          field_0x678(1), field_0x67C(0.0f) {}
    virtual ~dLytCommonIconItemPart2_c() {}
    dLytSub lyt;
    d2d::AnmGroup_c mAnm[10];
    u32 unk[0x3];
    d2d::dLytStructD structD;
    u8 field_0x64C;
    u8 field_0x64D;
    u8 field_0x64E[0x674 - 0x64E];
    u8 field_0x674;
    u8 field_0x675;
    u8 field_0x676;
    u8 field_0x677;
    u8 field_0x678;
    f32 field_0x67C;
};

class dLytCommonIconItemPart3_c {
public:
    dLytCommonIconItemPart3_c() : field_0x804(0), field_0x805(0), field_0x806(0) {}
    virtual ~dLytCommonIconItemPart3_c() {}
    dLytSub lyt;
    d2d::AnmGroup_c mAnm[3];
    u32 unk;
    d2d::dLytStructD structD;
    u8 field_0x804;
    u8 field_0x805;
    u8 field_0x806;
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
    dLytCommonIconItemPart2_c part2;
    dLytCommonIconItemPart3_c part3;
};

#endif
