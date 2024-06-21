#ifndef M3D_M_ANMCHR_H
#define M3D_M_ANMCHR_H

#include <m/m3d/m_bmdl.h>
#include <m/m3d/m_fanm.h>
#include <nw4r/g3d/g3d_resanmchr.h>


namespace m3d {

class anmChr_c : public fanm_c {
public:
    virtual ~anmChr_c();

    virtual int getType() override;

    bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmChr, mAllocator_c*, u32*);
    void setAnm(bmdl_c &, nw4r::g3d::ResAnmChr, playMode_e);
    void setAnmAfter(bmdl_c &, nw4r::g3d::ResAnmChr, playMode_e);
    void setFrmCtrlDefault(nw4r::g3d::ResAnmChr &, m3d::playMode_e);
};

} // namespace m3d

#endif
