#ifndef NW4R_G3D_SCN_PROC_H
#define NW4R_G3D_SCN_PROC_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_scnobj.h>

namespace nw4r {
namespace g3d {

class ScnProc : public ScnLeaf {
public:
    typedef void (*DrawProc)(ScnProc *pProc, bool opa);

public:
    static ScnProc *Construct(MEMAllocator *pAllocator, u32 *pSize, DrawProc pProc, bool opa, bool xlu, u32 userData);

    ScnProc(MEMAllocator *pAllocator, DrawProc pProc, void *pUserData, bool opa, bool xlu)
        : ScnLeaf(pAllocator), mFlag(0), mpDrawProc(pProc), mpUserData(pUserData) {
        if (opa) {
            mFlag |= SCNPROCFLAG_DRAW_OPA;
        }

        if (xlu) {
            mFlag |= SCNPROCFLAG_DRAW_XLU;
        }
    }

    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~ScnProc() {}                                   // at 0x10

    void SetDrawProc(DrawProc pProc, bool opa, bool xlu) {
        mpDrawProc = pProc;

        if (opa) {
            mFlag |= SCNPROCFLAG_DRAW_OPA;
        } else {
            mFlag &= ~SCNPROCFLAG_DRAW_OPA;
        }

        if (xlu) {
            mFlag |= SCNPROCFLAG_DRAW_XLU;
        } else {
            mFlag &= ~SCNPROCFLAG_DRAW_XLU;
        }
    }

    void SetUserData(void *pData) {
        mpUserData = pData;
    }
    void *GetUserData() {
        return mpUserData;
    }

private:
    enum ScnProcFlag {
        SCNPROCFLAG_DRAW_OPA = (1 << 0),
        SCNPROCFLAG_DRAW_XLU = (1 << 1)
    };

private:
    u32 mFlag;           // at 0xE8
    DrawProc mpDrawProc; // at 0xEC
    void *mpUserData;    // at 0xF0

    NW4R_G3D_RTTI_DECL_DERIVED(ScnProc, ScnLeaf);
};

} // namespace g3d
} // namespace nw4r

#endif
