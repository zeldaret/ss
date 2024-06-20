#ifndef M3D_M_BMDL_H
#define M3D_M_BMDL_H

#include <m/m3d/m_banm.h>
#include <m/m3d/m_scnleaf.h>
#include <nw4r/g3d/g3d_scnmdlsmpl.h>


namespace m3d {

class bmdl_c : public scnLeaf_c {
public:
    bmdl_c() : mpCurrentAnm(nullptr) {}
    virtual ~bmdl_c();

    virtual int getType() const override;
    virtual void remove() override;
    virtual void setAnm(banm_c &anm);
    virtual void play();

    void getNodeWorldMtx(u32 p1, nw4r::math::MTX34 *out) const;

    nw4r::g3d::ResMdl getResMdl() const;
    nw4r::g3d::ResMat getResMat(u32 index) const;

    void removeAnm(nw4r::g3d::ScnMdlSimple::AnmObjType);

private:
    banm_c *mpCurrentAnm;
};

} // namespace m3d

#endif
