#ifndef M3D_M_ANMMDL_H
#define M3D_M_ANMMDL_H

#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_mdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

namespace m3d {

// Not in NSMBW, name pulled from City Folk
class mdlAnmChr {
public:
    mdlAnmChr() {}
    virtual ~mdlAnmChr(); // 0x08

    virtual void play();                                         // 0x0C
    virtual void setFrame(f32);                                  // 0x10
    virtual bool setAnm(const char *name, playMode_e mode, f32); // 0x14
    virtual bool setAnm(const char *name, playMode_e mode);      // 0x18
    virtual void setRate(f32);                                   // 0x1C

    bool create(
        void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, mAllocator_c *alloc, u32 bufferOption,
        int nView, u32 *pSize
    );
    // Unknown overload
    bool create2(
        void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, mAllocator_c *alloc, u32 bufferOption,
        int nView, u32 *pSize
    );
    bool create(
        void *resFile, const char *mdlName, const char *anmName, mAllocator_c *alloc, u32 bufferOption, int nView,
        u32 *pSize
    );
    bool create(
        void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, mdl_c::mdlCallback_c *callback,
        mAllocator_c *alloc, u32 bufferOption, int nView, u32 *pSize
    );

    inline bool create(void *resFile, const char *mdlName, const char *anmName, mAllocator_c *alloc, u32 bufferOption) {
        return create(resFile, mdlName, anmName, alloc, bufferOption, 1, nullptr);
    }

    inline mdl_c &getModel() {
        return mMdl;
    }

    inline const mdl_c &getModel() const {
        return mMdl;
    }

    inline anmChr_c &getAnm() {
        return mAnm;
    }

    inline const anmChr_c &getAnm() const {
        return mAnm;
    }

protected:
    nw4r::g3d::ResFile mMdlFile;
    nw4r::g3d::ResFile mAnmFile;

    mdl_c mMdl;
    anmChr_c mAnm;
};

} // namespace m3d

#endif
