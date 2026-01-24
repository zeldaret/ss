#ifndef TO_BE_SORTED_D_CAMERA_MAP_H
#define TO_BE_SORTED_D_CAMERA_MAP_H

#include "common.h"
#include "m/m_vec.h"
#include "toBeSorted/d_camera_base.h"

class dAcObjBase_c;

class dCameraMap_c : public dCameraBase_c {
public:
    dCameraMap_c() : field_0xC4(0.0f) {}
    /* vt 0x08 */ virtual void onBecomeActive() override;
    /* vt 0x0C */ virtual void onBecomeInactive() override;
    /* vt 0x10 */ virtual bool create() override;
    /* vt 0x14 */ virtual bool remove() override;
    /* vt 0x18 */ virtual bool execute() override;
    /* vt 0x1C */ virtual bool draw() override;
    /* vt 0x20 */ virtual ~dCameraMap_c() {}
    /* vt 0x30 */ virtual void setView(const CamView &view) override;
    // Not sure what this is...
    /* vt 0x34 */ virtual bool vt_0x34();

    void fn_80093340();
    void setNoCameraTransition();
    void setCameraTransition();
    bool isActiveOrAnimating() const;
    void startOut();

private:
    /* 0x2C */ mVec3_c field_0x2C;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3C;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ dAcObjBase_c *field_0x44;
    /* 0x48 */ CamView mView1;
    /* 0x68 */ CamView mSourceView;
    /* 0x88 */ CamView mTargetView;
    /* 0xA8 */ u8 _0xA8[0xAC - 0xA8];
    /* 0xAC */ bool mIsMovingOut;
    /* 0xB0 */ UNKWORD field_0xB0;
    /* 0xB4 */ bool mIsActiveOrAnimating;
    /* 0xB8 */ s32 mTimer;
    /* 0xBC */ u8 _0xBC[0xC4 - 0xBC];
    /* 0xC4 */ f32 field_0xC4;
};

#endif
