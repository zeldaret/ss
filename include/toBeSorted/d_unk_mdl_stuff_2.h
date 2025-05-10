#ifndef D_UNK_MDL_STUFF_2_H
#define D_UNK_MDL_STUFF_2_H

#include "common.h"
#include "m/m3d/m_proc.h"
#include "m/m_allocator.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resshp.h"

/** a process for drawing shapes directly */
class dShpProcBase_c : public m3d::proc_c {
public:
    dShpProcBase_c(): mLightSetId(1) {}
    virtual ~dShpProcBase_c() {}

    void setResMat(nw4r::g3d::ResMat mat);
    void setResShp(nw4r::g3d::ResShp shp) {
        mShp = shp;
    }

    void setupLight();
    void draw(mVec3_c *pos);
    void setAllFlags();
    void doEntry(s32 lightSetId);

    bool init(nw4r::g3d::ResMat mat, nw4r::g3d::ResShp shp, s32 count, bool xlu, u32 *pSize);

    bool isDrawDisabled(s32 idx) const {
        return mpFlags[idx];
    }

    void setDrawDisabled(s32 idx, bool val) {
        mpFlags[idx] = val;
    }

protected:
    /* 0x18 */ nw4r::g3d::ResMat mMat;
    /* 0x1C */ nw4r::g3d::ResShp mShp;
    /* 0x20 */ bool *mpFlags;
    /* 0x24 */ s32 mCount;
    /* 0x28 */ s32 mLightSetId;
    /* 0x2C */ u8 field_0x2C;
};

class dShpProc1_c : public dShpProcBase_c {
public:
    virtual ~dShpProc1_c() {}
    /* vt 0x18 */ virtual void drawOpa() override {
        draw();
    }
    /* vt 0x1C */ virtual void drawXlu() override {
        draw();
    };

    void clearTransforms() {
        mpTransforms = nullptr;
    }

    bool create(nw4r::g3d::ResMat mat, nw4r::g3d::ResShp shp, s32 count, mHeapAllocator_c *alloc, bool xlu, u32 *pSize);
    void draw();

    void setTransform(s32 idx, const mMtx_c &transform) {
        mpTransforms[idx].copyFrom(transform);
    }

protected:
    /* 0x30 */ mMtx_c *mpTransforms;
};

#endif
