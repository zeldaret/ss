#ifndef D_C_BASE_H
#define D_C_BASE_H

#include "d/d_base.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_cursor_stick.h"
#include "d/lyt/d_structd.h"
#include "m/m2d.h"


class dCs_c : public dBase_c {};

class dCsBase_c : public dCs_c {
public:
    dCsBase_c();
    virtual ~dCsBase_c();
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int execute() override;

    static dCsBase_c *sInstance;

    int update();
    int similarToDraw();

private:
    static int sfield0x700Init;
    void setCurrentLyt(int lyt);

    /* 0x068 */ m2d::ResAccIf_c mResAcc;
    /* 0x11C */ d2d::LytBase_c mLyts[2];
    /* 0x23C */ d2d::LytBase_c *mpCurrLyt;
    /* 0x240 */ d2d::dLytStructC mStructC;
    /* 0x254 */ dLytCursorStick_c mCursorStick;
    /* 0x6F0 */ f32 field_0x6F0;
    /* 0x6F4 */ f32 field_0x6F4;
    /* 0x6F8 */ f32 field_0x6F8;
    /* 0x6FC */ f32 field_0x6FC;
    /* 0x700 */ u16 field_0x700;
    /* 0x702 */ u8 field_0x702;
    /* 0x703 */ u8 field_0x703;
    /* 0x704 */ u8 field_0x704;
    /* 0x705 */ u8 field_0x705;
};

#endif
