#include <d/lyt/meter/d_lyt_meter_item_select.h>

dLytMeterItemSelect_c::dLytMeterItemSelect_c() : mStateMgr(*this, sStateID::null) {
    field_0x574C = 11;
    field_0x5750 = 11;
    field_0x5754 = 0;
    field_0x5758 = -1;
    field_0x575C = 0;
    field_0x5760 = 0;
    field_0x5764 = -1;
    field_0x5768 = 2;
}
