#ifndef NW4R_G3D_RESANMCAMERA_H
#define NW4R_G3D_RESANMCAMERA_H
#include "types_nw4r.h"
#include "g3d_rescommon.h"
#include "g3d_anmcamera.h"

namespace nw4r
{
    namespace g3d
    {
        struct ResAnmCameraData
        {
            char UNK_0x0[0xC];
            u32 mID; // at 0xC
        };

        struct ResAnmCamera
        {            
            ResCommon<ResAnmCameraData> mAnmCamera;
            
            inline ResAnmCamera(void * vptr) : mAnmCamera(vptr) {}

            bool IsValid() const { return mAnmCamera.IsValid(); }

            void GetAnmResult(CameraAnmResult *, f32) const;
        };
    }
}

#endif