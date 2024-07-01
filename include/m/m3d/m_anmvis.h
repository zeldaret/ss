#ifndef M3D_M_ANMVIS_H
#define M3D_M_ANMVIS_H

#include <m/m3d/m_bmdl.h>
#include <m/m3d/m_fanm.h>


namespace m3d {

class anmVis_c : public fanm_c {
public:
    virtual ~anmVis_c();

    virtual int getType() const override;

    bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmVis, mAllocator_c *, u32 *);
    void setAnm(m3d::bmdl_c &, nw4r::g3d::ResAnmVis, m3d::playMode_e);
    void setFrmCtrlDefault(nw4r::g3d::ResAnmVis &, m3d::playMode_e);
};

} // namespace m3d

#endif
