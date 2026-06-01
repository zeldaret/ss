#include "d/lyt/d_lyt_map_capture.h"
#include "common.h"
#include "d/d_stage.h"
#include "d/lyt/d_lyt_map_global.h"

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

bool dLytMapCapture_c::isBusyRendering() const{
    return mIsBusyRendering;
}

void dLytMapCapture_c::execute() {
    mStateMgr.executeState();
}

void fn_8012D6D0(mVec3_c *ignored, mVec3_c *out) {
    *out = dStage_c::GetInstance()->getMapRelated()->field_0x0FC;
}

void fn_8012D610(mVec3_c *ignored, mVec3_c *out){
    dStage_c::GetInstance()->getMapRelated()->fn_801B50C0(0);
    const mVec3_c &v = dStage_c::GetInstance()->getMapRelated()->fn_801B4CB0();
    
    float z = v.z;
    float y = v.y; 
    float x = v.x;    
    out->y = y;
    out->x = x;    
    out->z = z;
}

extern int fn_801B5970(MapRelated*, const GXTexObj*);

void dLytMapCapture_c::fn_8012D800(nw4r::lyt::Pane *param_2, const GXTexObj *param_3) {
    if (param_3 != nullptr && dStage_c::GetInstance() != nullptr && fn_801B5970(dStage_c::GetInstance()->getMapRelated(), param_3)) {
        field_0x070.width = GXGetTexObjWidth(param_3);
        field_0x070.height = GXGetTexObjHeight(param_3);
        param_2->SetSize(field_0x070);
        param_2->GetMaterial()->GetTexMapAry()->ReplaceImage(*param_3);
    }
}


void dLytMapCapture_c::fn_8012D6F0(){
 
    dStage_c * pStage = dStage_c::GetInstance();
    
    if (pStage != nullptr) {
        dLytMapGlobal_c* pLytMap = dLytMapGlobal_c::GetInstance();

        f32 zoom = pLytMap->getZoomFrame();
        f32 inv = 1.0f / pLytMap->getField_0x44();
        s32 floor = pLytMap->getFloor();

        mVec3_c v(field_0x064 * inv, 1, field_0x06C * inv);

        pStage->getMapRelated()->fn_801B50C0(floor);
 
        pStage->getMapRelated()->field_0x108 = pLytMap->getMapScroll();
        pStage->getMapRelated()->field_0x114 = v;
         
        pStage->getMapRelated()->field_0x120 =  pLytMap->getMapRotationCenter(); 
        
        
        pStage->getMapRelated()->field_0x1E6 = (u16)(-pLytMap->getMapRotation());
        pStage->getMapRelated()->field_0x12C = zoom;
        pStage->getMapRelated()->field_0x1EE = 1;
    }
    return;
} 
