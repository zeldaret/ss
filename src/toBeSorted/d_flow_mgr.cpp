#include "toBeSorted/d_flow_mgr.h"

dFlowMgrBase_c::dFlowMgrBase_c(dFlow_c *flow): mpFlow(flow) {}

dFlowMgr_c::dFlowMgr_c() : dFlowMgrBase_c(&mFlow) {}
