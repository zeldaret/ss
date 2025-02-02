#ifndef NW4R_G3D_SCN_ROOT_H
#define NW4R_G3D_SCN_ROOT_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_camera.h>
#include <nw4r/g3d/g3d_fog.h>
#include <nw4r/g3d/g3d_light.h>
#include <nw4r/g3d/g3d_scnobj.h>
#include <nw4r/g3d/g3d_state.h>

namespace nw4r {
namespace g3d {

// Forward declarations
class AnmScn;

/******************************************************************************
 *
 * ScnRoot
 *
 ******************************************************************************/
class ScnRoot : public ScnGroup {
public:
    enum ScnRootFlag {
        SCNROOTFLAG_FORCE_RESMDLDRAWMODE = (1 << 0)
    };

public:
    static ScnRoot *
    Construct(MEMAllocator *pAllocator, u32 *pSize, u32 maxChildren, u32 maxScnObj, u32 numLightObj, u32 numLightSet);

    static ScnRoot *Construct(MEMAllocator *pAllocator, u32 *pSize, u32 maxChildren, u32 maxScnObj) {
        return Construct(pAllocator, pSize, maxChildren, maxScnObj, G3DState::NUM_LIGHT, G3DState::NUM_LIGHT_SET);
    }

    ScnRoot(
        MEMAllocator *pAllocator, IScnObjGather *pGather, ScnObj **ppChildrenBuf, u32 maxChildren, u32 numLight,
        u32 numLightSet, LightObj *pLightObjBuf, AmbLightObj *pAmbObjBuf, LightSetData *pLightSetBuf
    );

    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~ScnRoot();                                     // at 0x10

    Camera GetCamera(int idx);
    Camera GetCurrentCamera();
    void SetCurrentCamera(int idx);

    Fog GetFog(int idx);
    LightSet GetLightSet(int idx);

    void UpdateFrame();
    void SetGlbSettings();

    void CalcAnmScn();
    void CalcWorld();
    void CalcMaterial();
    void CalcVtx();
    void CalcView();

    void GatherDrawScnObj();
    void ZSort();
    void DrawOpa();
    void DrawXlu();

    u32 TestScnRootFlag(ScnRootFlag flag) const {
        return (mScnRootFlags & flag) != 0;
    }
    void SetScnRootFlag(ScnRootFlag flag, u32 on) {
        if (on) {
            mScnRootFlags |= flag;
        } else {
            mScnRootFlags &= ~flag;
        }
    }

    int GetCurrentCameraID() const {
        return mCurrentCameraID;
    }

    LightSetting &GetLightSetting() {
        return mLightSetting;
    }

private:
    IScnObjGather *mpCollection;              // at 0xE8
    ResMdlDrawMode mDrawMode;                 // at 0xEC
    u32 mScnRootFlags;                        // at 0xF0
    u8 mCurrentCameraID;                      // at 0xF4
    u8 PADDING_0xF5;                          // at 0xF5
    u8 PADDING_0xF6;                          // at 0xF6
    u8 PADDING_0xF7;                          // at 0xF7
    CameraData mCamera[G3DState::NUM_CAMERA]; // at 0xF8
    FogData mFog[G3DState::NUM_FOG];          // at 0x2278
    LightSetting mLightSetting;               // at 0x2878
    AnmScn *mpAnmScn;                         // at 0x2888

    NW4R_G3D_RTTI_DECL_DERIVED(ScnRoot, ScnGroup);
};

/******************************************************************************
 *
 * ScnObjGather
 *
 ******************************************************************************/
class ScnObjGather : public IScnObjGather {
public:
    ScnObjGather(ScnObj **ppBufOpa, ScnObj **ppBufXlu, u32 capacity);

    virtual ~ScnObjGather() {} // at 0x8

    virtual CullingStatus Add(ScnObj *pObj, bool opa, bool xlu); // at 0xC
    virtual void Clear() {
        mNumScnObjOpa = mNumScnObjXlu = 0;
    } // at 0x10

    virtual void ZSort(); // at 0x14

    virtual void Sort(); // at 0x18
    virtual void Sort(LessThanFunc pOpaFunc,
                      LessThanFunc pXluFunc); // at 0x1C

    virtual void DrawOpa(ResMdlDrawMode *pForceMode); // at 0x20
    virtual void DrawXlu(ResMdlDrawMode *pForceMode); // at 0x24

protected:
    ScnObj **mpArrayOpa; // at 0x4
    ScnObj **mpArrayXlu; // at 0x8
    u32 mSizeScnObj;     // at 0xC
    u32 mNumScnObjOpa;   // at 0x10
    u32 mNumScnObjXlu;   // at 0x14
};

} // namespace g3d
} // namespace nw4r

#endif
