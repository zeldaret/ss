#ifndef D_CAMERA_H
#define D_CAMERA_H

#include "common.h"
#include "d/d_base.h"
#include "egg/gfx/eggCamera.h"
#include "m/m_angle.h"
#include "m/m_frustum.h"
#include "m/m_vec.h"
#include "toBeSorted/d_camera_base.h"
#include "toBeSorted/d_camera_event.h"
#include "toBeSorted/d_camera_game.h"
#include "toBeSorted/d_camera_map.h"

class dCamera_c : public dBase_c {
private:
    class substruct_1 {
    public:
        substruct_1() : mActive(false), mFSStickAngle(0), field_0x04(0) {}

        void setCamera(dCamera_c *cam);
        void fn_8019E890();
        mAng fn_8019E930();
        void fn_8019E940();

    private:
        void fn_8019E8D0(const mAng &ang);

        /* 0x00 */ bool mActive;
        /* 0x02 */ mAng mFSStickAngle;
        /* 0x04 */ mAng field_0x04;
        /* 0x08 */ dCamera_c *mpCamera;
    };

    enum CameraIdx_e {
        CAM_GAME_0,
        CAM_GAME_1,
        CAM_EVENT,
        CAM_MAP,

        CAM_MAX,
    };

    enum Flags_e {
        CAM_FLAGS_0x40 = (1 << 6),
        CAM_FLAGS_0x80 = (1 << 7),
        CAM_FLAGS_0x100 = (1 << 8),
        CAM_FLAGS_0x200 = (1 << 9),
    };

public:
    dCamera_c();
    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

    void setFrustum(f32 fov, f32 near, f32 far);

    dCameraGame_c *getGameCam1() {
        return static_cast<dCameraGame_c *>(mpCameras[CAM_GAME_0]);
    }

    dCameraGame_c *getGameCam2() {
        return static_cast<dCameraGame_c *>(mpCameras[CAM_GAME_1]);
    }

    dCameraEvent_c *getEventCam() {
        return static_cast<dCameraEvent_c *>(mpCameras[CAM_EVENT]);
    }

    dCameraMap_c *getMapCam() {
        return static_cast<dCameraMap_c *>(mpCameras[CAM_MAP]);
    }

    const mVec3_c &getPositionMaybe() const {
        return mView.field_0x00;
    }
    const mVec3_c &getField_0x78() const {
        return mView.field_0x0C;
    }
    f32 getWaterHeight() const {
        return mWaterHeight;
    }
    mAng getYRot() const;

    void setScreenShakeIntensity(f32 val) {
        mScreenShakeIntensity = val;
    }

    UNKWORD getField_0xDA8() const {
        return mActiveCameraIdx;
    }

    bool isUnderwater() const;
    f32 getUnderwaterDepth() const;

    void fn_8019EA00(const mVec3_c &, const mVec3_c &, f32, f32);
    void fn_8019E430();
    void fn_8019E410();
    mAng fn_8019E3B0() const;

private:
    void fn_8019DB80();
    void fn_8019DCE0();
    void fn_8019DE70();
    f32 fn_8019E1F0();

    void updateUnderwaterDepth(const mVec3_c &pos);
    bool isUnderwater_() const;

    /* 0x068 */ s32 mMyCameraIndex;
    /* 0x06C */ CamView mView;
    /* 0x08C */ mVec3_c field_0x08C;
    /* 0x098 */ mAng field_0x098;
    /* 0x09A */ mAng field_0x09A;
    /* 0x09C */ mFrustum_c mFrustum;

    /* 0x18C */ u8 _0x18C[0x1F8 - 0x18C];

    /* 0x1F8 */ UNKWORD field_0x1F8;
    /* 0x1FC */ u8 field_0x1FC;
    /* 0x200 */ EGG::Screen *mpScreen;
    /* 0x204 */ EGG::LookAtCamera mLookAtCamera;

    /* 0x28C */ f32 mLetterboxAmount;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ f32 mGlobalAlpha;
    /* 0x298 */ u8 field_0x298;
    /* 0x299 */ u8 field_0x299;
    /* 0x29C */ f32 field_0x29C;
    /* 0x2A0 */ u8 field_0x2A0;
    /* 0x2A1 */ u8 field_0x2A1;
    /* 0x2A4 */ f32 mUnderwaterDepth;
    /* 0x2A8 */ f32 mWaterHeight;
    /* 0x2AC */ bool mIsUnderwater;
    /* 0x2B0 */ u32 mFlags;
    /* 0x2B4 */ dCameraGame_c mGameCam1;
    /* 0x728 */ dCameraGame_c mGameCam2;
    /* 0xB9C */ dCameraEvent_c mEventCam;
    /* 0xCDC */ dCameraMap_c mMapCam;

    /* 0xD8C */ u8 _0xD8C[0xD94 - 0xD8C];

    /* 0xD94 */ f32 field_0xD94;
    /* 0xD98 */ dCameraBase_c *mpCameras[CAM_MAX];
    /* 0xDA8 */ s32 mActiveCameraIdx;
    /* 0xDAC */ void *field_0xDAC;
    /* 0xDB0 */ f32 mScreenShakeIntensity;
    /* 0xDB4 */ mVec3_c field_0xDB4;
    /* 0xDC0 */ substruct_1 field_0xDC0;
    /* 0xDCC */ u8 field_0xDCC;
    /* 0xDD0 */ CamView mView1;
};

#endif
