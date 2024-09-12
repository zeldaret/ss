#ifndef NW4R_G3D_SCNROOT_H
#define NW4R_G3D_SCNROOT_H
#include "common.h"
#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/g3d/g3d_fog.h"
#include "nw4r/g3d/g3d_light.h"
#include "nw4r/g3d/g3d_scnobj.h"

namespace nw4r {
namespace g3d {
class ScnRoot : public ScnGroup {
public:
    Camera GetCamera(int);
    Camera GetCurrentCamera();
    void SetCurrentCamera(int);
    Fog GetFog(int);

    static ScnRoot *Construct(MEMAllocator *pHeap, u32 *pSize, u32 maxNumChildren, u32 maxNumScnObj, u32 numLightObj,
            u32 numLightSet);

    void UpdateFrame();
    void CalcWorld();
    void CalcMaterial();
    void CalcVtx();
    void CalcView();
    void GatherDrawScnObj();
    void ZSort();
    void DrawOpa();
    void DrawXlu();

    u8 GetCurrentCameraID() const {
        return mCameraId;
    }

    inline LightSetting *getLightSetting() {
        return &mLightSetting;
    }

private:
    UNKWORD WORD_0xE8;
    UNKWORD WORD_0xEC;
    UNKWORD WORD_0xF0;
    /* 0x00F4 */ u8 mCameraId;
    u8 unk1[0x2878 - 0x00F8];
    /* 0x2878 */ LightSetting mLightSetting;
};
} // namespace g3d
} // namespace nw4r

#endif
