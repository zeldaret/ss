#include "d/lyt/d_lyt_map_capture.h"
#include "common.h"
#include "d/d_stage.h"

STATE_DEFINE(dLytMapCapture_c, RenderingWait);
STATE_DEFINE(dLytMapCapture_c, RenderingWaitStep2);

void dLytMapCapture_c::initializeState_RenderingWait() {}
void dLytMapCapture_c::executeState_RenderingWait() {
    if (mRenderRequest && !mIsBusyRendering) {
        fn_8012D6F0();
        mRenderRequest = false;
        mIsBusyRendering = true;
        mStateMgr.changeState(StateID_RenderingWaitStep2);
    }
}
void dLytMapCapture_c::finalizeState_RenderingWait() {}

void dLytMapCapture_c::initializeState_RenderingWaitStep2() {}
void dLytMapCapture_c::executeState_RenderingWaitStep2() {
    if (mIsBusyRendering && dStage_c::GetInstance() != nullptr && !dStage_c::GetInstance()->getMapRelated()->getField_0x1EE()) {
        mIsBusyRendering = false;
        if (mRenderRequest) {
            fn_8012D6F0();
            mRenderRequest = false;
            mIsBusyRendering = true;
        } else {
            mStateMgr.changeState(StateID_RenderingWait);
        }
    }
}
void dLytMapCapture_c::finalizeState_RenderingWaitStep2() {}
