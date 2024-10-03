#ifndef D_LYT_METER_H
#define D_LYT_METER_H

#include <nw4r/lyt/lyt_pane.h>

namespace d2d {
    class ResAccIf_c;
}

class dLytMeterBase {
public:
    dLytMeterBase() : field_0x04(false) {}
    virtual ~dLytMeterBase() {}
    virtual bool build(d2d::ResAccIf_c *resAcc);
    virtual bool LytMeter0x10();
    virtual bool LytMeter0x14();
    virtual nw4r::lyt::Pane *LytMeter0x18();
    virtual void *LytMeter0x1C();
    virtual const char *LytMeter0x20() const;
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

struct LytMeterGroup {
    nw4r::ut::LinkList<LytMeterListNode, 0> list;
};


#endif
