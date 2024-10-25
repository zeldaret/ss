#ifndef D_LYT_METER_CONFIGURATION
#define D_LYT_METER_CONFIGURATION

#include "common.h"

class dLytMeterConfiguration_c {
public:
    dLytMeterConfiguration_c();
    virtual ~dLytMeterConfiguration_c();


    static dLytMeterConfiguration_c *sInstance;
    f32 field_0x04[0x7A - 0x01];
};

#endif
