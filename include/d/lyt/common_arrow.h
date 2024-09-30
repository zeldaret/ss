#ifndef D_LYT_COMMON_ARROW_H
#define D_LYT_COMMON_ARROW_H

#include <d/lyt/d2d.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

extern "C" u8 lbl_80572D10;
struct SomeVtableThing {
    virtual u32 getType() {
        return 'lyt ';
    }

    u8 field_0x04[0x24 - 0x04];
    void *field_0x24;
};

struct CsBaseTmp {
    u8 field_0x000[0x250 - 0x000];
    SomeVtableThing *field_0x250;

    SomeVtableThing *getThing() const {
        return field_0x250;
    }
};

extern "C" CsBaseTmp *C_BASE;

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
    void fn_80168640(const mVec3_c &arg);
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
    d2d::AnmGroup_c mAnmGroups[0x9];
    d2d::dLytStructD field_0x680;
    void *field_0x6A8;
    void *field_0x6AC;
    s32 mType;
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
