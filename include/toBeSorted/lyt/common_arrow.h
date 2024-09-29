#ifndef D_LYT_COMMON_ARROW_H
#define D_LYT_COMMON_ARROW_H

#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>
#include <toBeSorted/lyt/d2d.h>


class dLytCommonArrow_c {
public:
    dLytCommonArrow_c();
    virtual ~dLytCommonArrow_c() {}

    bool init();

    STATE_FUNC_DECLARE(dLytCommonArrow_c, None);
    STATE_FUNC_DECLARE(dLytCommonArrow_c, In);
    STATE_FUNC_DECLARE(dLytCommonArrow_c, Wait);
    STATE_FUNC_DECLARE(dLytCommonArrow_c, Out);

    void setState(s32 state);
    bool addToDrawList();

    bool fn_80168490();
    bool fn_80168500();
    void fn_80168640(f32 *);
    bool fn_80168680();
    bool fn_801686F0();
    bool fn_80168760();
    void fn_80168790(s32, f32);
    void fn_80168800(s32);
    void fn_80168810(d2d::AnmGroup_c *);
    void fn_80168880();

private:

    STATE_MGR_DECLARE(dLytCommonArrow_c);
    /* 0x40 */ d2d::LytBase_c mLytBase;
    d2d::ResAccIf_c mResAcc;
    d2d::AnmGroup_c field_0x440[0x9];
    d2d::dLytStructD field_0x680;
    void *field_0x6A8;
    void *field_0x6AC;
    s32 field_0x6B0;
    s32 field_0x6B4;
    s32 field_0x6B8;
    s32 field_0x6BC;
    s32 field_0x6C0;
    s32 field_0x6C4;
    u8 field_0x6C8;
    u8 field_0x6C9;
    u8 field_0x6CA;
    u8 field_0x6CB;
    u8 field_0x6CC;
};


#endif
