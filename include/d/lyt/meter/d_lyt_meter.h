#ifndef D_LYT_METER_H
#define D_LYT_METER_H

#include <d/lyt/d2d.h>
#include <d/lyt/meter/d_lyt_meter.h>
#include <d/lyt/meter/d_lyt_meter_remocon_bg.h>

class dLytMeter_c {
public:
    dLytMeter_c();
    virtual ~dLytMeter_c() {}

    d2d::ResAccIf_c mResAcc;
    d2d::LytBase_c mLyt;
    d2d::AnmGroup_c mAnmGroups[34];
    // Meters...
    dLytMeterRemoconBg_c mRemoCon;
    LytMeterGroup mMeters;

};


#endif
