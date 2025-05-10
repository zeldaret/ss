#ifndef M3D_M_BMDL_H
#define M3D_M_BMDL_H

#include "m/m3d/m_banm.h"
#include "m/m3d/m_scnleaf.h"
#include "m/m_math.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"

namespace m3d {

class bmdl_c : public scnLeaf_c {
public:
    bmdl_c() : mpCurrentAnm(nullptr) {}
    virtual ~bmdl_c();

    virtual int getType() const override;
    virtual void remove() override;
    virtual bool setAnm(banm_c &anm);
    virtual void play();

    int getMatID(const char *name) const;
    int getNodeID(const char *name) const;
    bool getNodeWorldMtx(u32 p1, nw4r::math::MTX34 *out) const;
    bool getNodeWorldMtxMultVecZero(u32 p1, nw4r::math::VEC3 &out) const;
    bool getNodeWorldMtxMultVec(u32, nw4r::math::VEC3 &, nw4r::math::VEC3 &) const;
    nw4r::g3d::ResMdl getResMdl() const;
    nw4r::g3d::ResMat getResMat(u32 index) const;

    void removeAnm(nw4r::g3d::ScnMdlSimple::AnmObjType);
    nw4r::g3d::AnmObj *getAnmObj(nw4r::g3d::ScnMdlSimple::AnmObjType) const;
    void setTevColor(u32, GXTevRegID, GXColor, bool);
    void setTevColorAll(GXTevRegID, GXColor, bool);
    void setTevKColor(u32, GXTevKColorID, GXColor, bool);
    void setTevKColorAll(GXTevKColorID, GXColor, bool);
    void setBlendModeAll(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp, bool bMarkDirty);
    void setCullMode(u32 matId, GXCullMode cullMode, bool bMarkDirty);
    void setCullModeAll(GXCullMode cullMode, bool bMarkDirty);
    void setMatVisible(u32 matId, bool bVisble);
    int setMatTexture(char *name, GXTexObj *texObj, bool copy, void *unk, int, int);
    bool getBounds(mVec3_c *min, mVec3_c *max) const;

private:
    banm_c *mpCurrentAnm;
};

} // namespace m3d

#endif
