#ifndef M_M3D_ANMSHP_H
#define M_M3D_ANMSHP_H

#include <common.h>
#include <m/m3d/m_bmdl.h>
#include <m/m3d/m_fanm.h>
#include <nw4r/g3d/g3d_resanmshp.h>

namespace m3d {

class anmShp_c : public fanm_c {
public:
    virtual ~anmShp_c();

    virtual int getType() const override;

    bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmShp, mAllocator_c *, u32 *);
    void setAnm(m3d::bmdl_c &, nw4r::g3d::ResAnmShp, m3d::playMode_e);
    void setFrmCtrlDefault(nw4r::g3d::ResAnmShp &, m3d::playMode_e);
};

} // namespace m3d

#endif
