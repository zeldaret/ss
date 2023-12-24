#ifndef NW4R_G3D_RESANMFOG_H
#define NW4R_G3D_RESANMFOG_H
#include "types_nw4r.h"
#include "g3d_rescommon.h"
#include "g3d_anmfog.h"

namespace nw4r
{
    namespace g3d
    {
        struct ResAnmFogData
        {
            char UNK_0x0[0xC];
            u32 mID; // at 0xC
        };

        struct ResAnmFog
        {            
            ResCommon<ResAnmFogData> mAnmFog;
            
            inline ResAnmFog(void * vptr) : mAnmFog(vptr) {}

            void GetAnmResult(FogAnmResult *, f32) const;

            bool IsValid() const { return mAnmFog.IsValid(); }
        };
    }
}

#endif