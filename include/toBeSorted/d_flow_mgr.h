#ifndef D_FLOW_MGR_H
#define D_FLOW_MGR_H

#include "common.h"
#include "d/d_message.h"
#include "sized_string.h"

class dFlowMgrBase_c {
public:
    dFlowMgrBase_c(dFlow_c *flow);
    virtual ~dFlowMgrBase_c() {}

    void triggerEntryPoint(s32 labelPart1, s32 labelPart2, u8 p4, u8 p5);
    bool checkFinished();

private:
    /* 0x04 */ dFlow_c *mpFlow;
    /* 0x08 */ u8 _0x08[0x14 - 0x08];
    /* 0x14 */ SizedString<0x40> field_0x14;
    /* 0x54 */ u8 field_0x54;
};

class dFlowMgr_c : public dFlowMgrBase_c {
public:
    dFlowMgr_c();

private:
    /* 0x58  */ dFlow_c mFlow;
};

#endif
