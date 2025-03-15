#ifndef NW4R_G3D_SCN_MDL_H
#define NW4R_G3D_SCN_MDL_H

#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/g3d_draw.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"
#include "nw4r/g3d/res/g3d_resmat.h"

namespace nw4r {
namespace g3d {

// Forward declarations
class AnmObjShp;

class ScnMdl : public ScnMdlSimple {
public:
    class CopiedMatAccess {
    public:
        CopiedMatAccess(ScnMdl *pScnMdl, u32 id);

        ResTexObj GetResTexObj(bool markDirty);
        ResTexSrt GetResTexSrt(bool markDirty);
        ResMatChan GetResMatChan(bool markDirty);
        ResGenMode GetResGenMode(bool markDirty);
        ResMatMisc GetResMatMisc(bool markDirty);
        ResMatPix GetResMatPix(bool markDirty);
        ResMatTevColor GetResMatTevColor(bool markDirty);
        ResTev GetResTev(bool markDirty);

        ResTexObj GetResTexObjEx();
        ResTexSrt GetResTexSrtEx();
        ResMatChan GetResMatChanEx();

    private:
        ScnMdl *mpScnMdl;                     // at 0x0
        u32 mMatID;                           // at 0x4
        ResTexObj mTexObj;                    // at 0x8
        ResTlutObj mTlutObj;                  // at 0xC
        ResTexSrt mTexSrt;                    // at 0x10
        ResMatChan mChan;                     // at 0x14
        ResGenMode mGenMode;                  // at 0x18
        ResMatMisc mMatMisc;                  // at 0x1C
        ResMatPix mPix;                       // at 0x20
        ResMatTevColor mTevColor;             // at 0x24
        ResMatIndMtxAndScale mIndMtxAndScale; // at 0x28
        ResMatTexCoordGen mTexCoordGen;       // at 0x2C
        ResTev mTev;                          // at 0x30
    };

    class CopiedVisAccess {
    public:
        CopiedVisAccess(ScnMdl *pScnMdl, u32 id);

        bool IsVisible() const;
        bool SetVisibility(bool visible);
        bool SetVisibilityEx(bool visible);

    private:
        ScnMdl *mpScnMdl; // at 0x0
        u32 mNodeID;      // at 0x4
        u8 *mpVis;        // at 0x8
    };

    class CopiedVtxAccess {
    public:
        ResVtxPos GetResVtxPos(u32 id);
        ResVtxNrm GetResVtxNrm(u32 id);
        ResVtxClr GetResVtxClr(u32 id);

        ResVtxPos GetResVtxPosEx(u32 id);
        ResVtxNrm GetResVtxNrmEx(u32 id);
        ResVtxClr GetResVtxClrEx(u32 id);

        // Needed an Inline
        ResVtxPos GetPos(u32 id) {
            if (mpScnMdl != NULL) {
                if (id < mpScnMdl->GetResMdl().GetResVtxPosNumEntries()) {
                    return ResVtxPos(mpScnMdl->mReplacement.vtxPosTable[id]);
                }
            }
            return ResVtxPos(NULL);
        }

        // Needed an Inline
        ResVtxNrm GetNrm(u32 id) {
            if (mpScnMdl != NULL) {
                if (id < mpScnMdl->GetResMdl().GetResVtxNrmNumEntries()) {
                    return ResVtxNrm(mpScnMdl->mReplacement.vtxNrmTable[id]);
                }
            }
            return ResVtxNrm(NULL);
        }

    private:
        // Idk
        ScnMdl *mpScnMdl; // at 0x0
    };

#define OPT(KEY, VALUE) OPTION_##KEY = (0x30000 | (VALUE))
    enum ScnMdlOption {
        OPT(NONE, 0),
        OPT(VISBUFFER_REFRESH_NEEDED, 1),
    };
#undef OPT

public:
    static ScnMdl *Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl, u32 bufferOption, int numView);

    ScnMdl(
        MEMAllocator *pAllocator, ResMdl mdl, math::MTX34 *pWorldMtxArray, u32 *pWorldMtxAttribArray,
        math::MTX34 *pViewPosMtxArray, math::MTX33 *pViewNrmMtxArray, math::MTX34 *pViewTexMtxArray, int numView,
        int numViewMtx, DrawResMdlReplacement *pReplacement, u32 *pMatBufferDirtyFlag, u32 replacementFlag
    );

    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~ScnMdl();                                      // at 0x10

    virtual bool SetScnObjOption(u32 option, u32 value);         // at 0x20
    virtual bool GetScnObjOption(u32 option, u32 *pValue) const; // at 0x24

    virtual bool SetAnmObj(AnmObj *pObj, AnmObjType type);  // at 0x34
    virtual bool RemoveAnmObj(AnmObj *pObj);                // at 0x38
    virtual AnmObj *RemoveAnmObj(AnmObjType type);          // at 0x3C
    virtual AnmObj *GetAnmObj(AnmObjType type);             // at 0x40
    virtual const AnmObj *GetAnmObj(AnmObjType type) const; // at 0x44

    void InitBuffer();
    void CleanMatBuffer(u32 idx, u32 option);
    void CleanVisBuffer();

    AnmObjShp *GetAnmObjShp() {
        return mpAnmObjShp;
    }

    DrawResMdlReplacement &GetDrawResMdlReplacement() {
        return mReplacement;
    }

protected:
    void ScnMdl_G3DPROC_CALC_WORLD(u32 param, const math::MTX34 *pParent);
    void ScnMdl_G3DPROC_CALC_MAT(u32 param, void *pInfo);
    void ScnMdl_G3DPROC_CALC_VTX(u32 param, void *pInfo);
    void ScnMdl_G3DPROC_DRAW_OPA(u32 param, void *pInfo);
    void ScnMdl_G3DPROC_DRAW_XLU(u32 param, void *pInfo);

private:
    enum VisBufferFlag {
        VISBUFFER_DIRTY = (1 << 0),
        VISBUFFER_NOT_REFRESH_NEEDED = (1 << 1),
    };

    enum MatBufferOption {
        BUFOPTION_TEXOBJ = (1 << 0),
        BUFOPTION_TLUTOBJ = (1 << 1),
        BUFOPTION_TEXSRT = (1 << 2),
        BUFOPTION_MATCHAN = (1 << 3),
        BUFOPTION_GENMODE = (1 << 4),
        BUFOPTION_MATMISC = (1 << 5),
        BUFOPTION_VIS = (1 << 6),
        BUFOPTION_MATPIX = (1 << 7),
        BUFOPTION_MATTEVCOLOR = (1 << 8),
        BUFOPTION_MATINDMTXSCALE = (1 << 9),
        BUFOPTION_MATTEXCOORDGEN = (1 << 10),
        BUFOPTION_TEV = (1 << 11),
        BUFOPTION_VTXPOS = (1 << 12),
        BUFOPTION_VTXNRM = (1 << 13),
        BUFOPTION_VTXCLR = (1 << 14),

        BUFOPTION_0x1000000 = (1 << 24),
    };

private:
    bool IsVisBufferDirty() const {
        return mFlagVisBuffer & VISBUFFER_DIRTY;
    }
    bool IsVisBufferRefreshNeeded() const {
        return !(mFlagVisBuffer & VISBUFFER_NOT_REFRESH_NEEDED);
    }
    void VisBufferDirty() {
        mFlagVisBuffer |= VISBUFFER_DIRTY;
    }

    bool IsMatBufferDirty(u32 idx, u32 option) const {
        return option & mpMatBufferDirtyFlag[idx];
    }
    void MatBufferDirty(u32 idx, u32 option) {
        mpMatBufferDirtyFlag[idx] |= option;
    }

private:
    AnmObjShp *mpAnmObjShp;             // at 0x138
    u32 mFlagVisBuffer;                 // at 0x13C
    u32 *mpMatBufferDirtyFlag;          // at 0x140
    DrawResMdlReplacement mReplacement; // at 0x144
    u32 mReplacementFlag;               // at 0x184

    NW4R_G3D_RTTI_DECL_DERIVED(ScnMdl, ScnMdlSimple);
};

} // namespace g3d
} // namespace nw4r

#endif
