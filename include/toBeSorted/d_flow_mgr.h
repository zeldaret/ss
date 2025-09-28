#ifndef D_FLOW_MGR_H
#define D_FLOW_MGR_H

#include "common.h"

class dFlow_c;

class dFlowMgr_c {
public:
    dFlowMgr_c(dFlow_c *flow);
    virtual ~dFlowMgr_c() {}

private:
    /* 0x04 */ dFlow_c *mpFlow;
    /* 0x08 */ u8 field_0x05;
};

#endif
