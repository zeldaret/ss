#ifndef M3D_M_ANMMDL_H
#define M3D_M_ANMMDL_H

#include <m/m3d/m_anmchr.h>
#include <m/m3d/m_mdl.h>
#include <nw4r/g3d/g3d_resfile.h>

namespace m3d {

// Not in NSMBW, name pulled from City Folk
class mdlAnmChr {
public:
    mdlAnmChr() {}
    virtual ~mdlAnmChr();

    virtual void play();
    virtual void setFrame(f32);
    virtual void setAnm(const char *name, playMode_e mode, f32);
    virtual void setAnm(const char *name, playMode_e mode);
    virtual void setRate(f32);

    bool create(void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, mAllocator_c *alloc,
            u32 bufferOption, int nView, u32 *pSize);
    // Unknown overload
    bool create2(void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, mAllocator_c *alloc,
            u32 bufferOption, int nView, u32 *pSize);
    bool create(void *resFile, const char *mdlName, const char *anmName, mAllocator_c *alloc, u32 bufferOption,
            int nView, u32 *pSize);
    bool create(void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, mdl_c::mdlCallback_c *callback,
            mAllocator_c *alloc, u32 bufferOption, int nView, u32 *pSize);

private:
    nw4r::g3d::ResFile mMdlFile;
    nw4r::g3d::ResFile mAnmFile;

    mdl_c mMdl;
    anmChr_c mAnm;
};

} // namespace m3d

#endif
