#ifndef D_LYT_METER_BASE_H
#define D_LYT_METER_BASE_H

#include "nw4r/lyt/lyt_pane.h"

namespace d2d {
class ResAccIf_c;
}

class dLytMeterBase {
public:
    dLytMeterBase() : field_0x04(false), field_0x05(0) {}
    virtual ~dLytMeterBase() {}
    virtual bool build(d2d::ResAccIf_c *resAcc) = 0;
    virtual bool LytMeter0x10() = 0;
    virtual bool LytMeter0x14() = 0;
    virtual nw4r::lyt::Pane *getPane() = 0;
    virtual void *LytMeter0x1C() = 0;
    virtual const char *getName() const = 0;
    virtual bool LytMeter0x24() const {
        return field_0x04;
    }
    virtual void LytMeter0x28(bool arg) {
        field_0x04 = arg;
    }
    virtual u8 LytMeter0x2C() const {
        return field_0x05;
    }

    virtual void LytMeter0x30(u8 arg) {
        field_0x05 = arg;
    }

    bool field_0x04;
    u8 field_0x05;
};

struct LytMeterListNode {
    nw4r::ut::LinkListNode mNode;
    dLytMeterBase *mpMeter;
    nw4r::lyt::Pane *mpPane;
};

typedef nw4r::ut::LinkList<LytMeterListNode, 0> LytMeterGroup;

#endif
