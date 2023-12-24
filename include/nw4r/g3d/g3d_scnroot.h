#ifndef NW4R_G3D_SCNROOT_H
#define NW4R_G3D_SCNROOT_H
#include "types_nw4r.h"
#include "g3d_scnobj.h"
#include "g3d_camera.h"
#include "g3d_fog.h"

namespace nw4r
{
    namespace g3d
    {
        class ScnRoot : public ScnGroup
        {
        public:
            Camera GetCamera(int);
            Camera GetCurrentCamera();
            void SetCurrentCamera(int);
            Fog GetFog(int);

            void UpdateFrame();
            void CalcWorld();
            void CalcMaterial();
            void CalcVtx();
            void CalcView();
            void GatherDrawScnObj();
            void ZSort();
            void DrawOpa();
            void DrawXlu();

            u8 GetCurrentCameraID() const { return mCameraId; }

        private:
            UNKWORD WORD_0xE8;
            UNKWORD WORD_0xEC;
            UNKWORD WORD_0xF0;
            u8 mCameraId;
        };
    }
}

#endif