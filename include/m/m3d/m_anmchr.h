#ifndef M3D_M_ANMCHR_H
#define M3D_M_ANMCHR_H

#include "common.h"
#include "m/m3d/m_bmdl.h"
#include "m/m3d/m_fanm.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"

namespace m3d {

class anmChr_c : public fanm_c {
public:
    virtual ~anmChr_c();

    virtual int getType() const override;

    bool create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmChr anm, mAllocator_c *alloc) {
        return create(mdl, anm, alloc, nullptr);
    }
    // Not sure about this overload but it fixes stack alloc in some cases and other files
    // definitely have similar overloads with seemingly identical argument types
    bool create2(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmChr anm, mAllocator_c *alloc) {
        return create(mdl, anm, alloc);
    }
    bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmChr, mAllocator_c *, u32 *);
    void setAnm(bmdl_c &, nw4r::g3d::ResAnmChr, playMode_e);
    void setAnmAfter(bmdl_c &, nw4r::g3d::ResAnmChr, playMode_e);
    void setFrmCtrlDefault(nw4r::g3d::ResAnmChr &, m3d::playMode_e);
};

} // namespace m3d

#endif
