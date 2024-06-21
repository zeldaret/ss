#ifndef M3D_M_ANMCHRBLEND_H
#define M3D_M_ANMCHRBLEND_H

#include <m/m3d/m_anmchr.h>
#include <m/m3d/m_banm.h>
#include <m/m3d/m_bmdl.h>
#include <nw4r/g3d/g3d_anmchr.h>

namespace m3d {

class anmChrBlend_c : public banm_c {
public:
    virtual ~anmChrBlend_c();

    virtual int getType() override;

    bool create(nw4r::g3d::ResMdl, int, mAllocator_c *, u32 *);
    bool attach(int, nw4r::g3d::AnmObjChrRes *, f32);
    bool attach(int, anmChr_c *, f32);
    void detach(int);
    // Not in NSMBW
    void setWeight(int, f32);
    f32 getWeight(int) const;
};

} // namespace m3d

#endif
