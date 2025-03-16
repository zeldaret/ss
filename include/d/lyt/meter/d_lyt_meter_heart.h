#ifndef D_LYT_METER_HEART_H
#define D_LYT_METER_HEART_H

#include "d/lyt/d2d.h"

class dLytMeterHeart_c : public d2d::dSubPane {
public:
    dLytMeterHeart_c() {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterHeart_c() {}
private:

    d2d::dLytSub mLyt;
    /* 0xD8 */ d2d::AnmGroup_c mAnm[26];
    /* 0x??? */ u8 padding[0x12DA4 - 0x12D24];
};

#endif
