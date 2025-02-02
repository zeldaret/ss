#ifndef NW4R_G3D_SCN_MDL_SMPL_H
#define NW4R_G3D_SCN_MDL_SMPL_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_calcview.h>
#include <nw4r/g3d/g3d_scnobj.h>
#include <nw4r/g3d/res/g3d_resmdl.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {

// Forward declarations
class AnmObj;
class AnmObjChr;
class AnmObjVis;
class AnmObjMatClr;
class AnmObjTexPat;
class AnmObjTexSrt;
class ICalcWorldCallback;

class ScnMdlSimple : public ScnLeaf {
public:
    enum AnmObjType {
        ANMOBJTYPE_CHR,
        ANMOBJTYPE_VIS,
        ANMOBJTYPE_MATCLR,
        ANMOBJTYPE_TEXPAT,
        ANMOBJTYPE_TEXSRT,
        ANMOBJTYPE_SHP,
        ANMOBJTYPE_NOT_SPECIFIED,

        ANMOBJTYPE_VTX = ANMOBJTYPE_SHP
    };

    enum ByteCodeType {
        BYTE_CODE_CALC,
        BYTE_CODE_MIX,
        BYTE_CODE_DRAW_OPA,
        BYTE_CODE_DRAW_XLU,
    };

#define OPT(KEY, VALUE) OPTION_##KEY = (0x20000 | (VALUE))
    enum ScnMdlSimpleOption {
        OPT(NONE, 0),
        OPT(IGNORE_ANMCHR_TRANS, 1),
    };
#undef OPT

public:
    static ScnMdlSimple *Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl, int numView);

    ScnMdlSimple(
        MEMAllocator *pAllocator, ResMdl mdl, math::MTX34 *pWorldMtxArray, u32 *pWorldMtxAttribArray,
        math::MTX34 *pViewPosMtxArray, math::MTX33 *pViewNrmMtxArray, math::MTX34 *pViewTexMtxArray, int numView,
        int numViewMtx
    );

    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~ScnMdlSimple();                                // at 0x10

    virtual bool SetScnObjOption(u32 option, u32 value);         // at 0x20
    virtual bool GetScnObjOption(u32 option, u32 *pValue) const; // at 0x24

    virtual bool SetAnmObj(AnmObj *pObj, AnmObjType type);  // at 0x34
    virtual bool RemoveAnmObj(AnmObj *pObj);                // at 0x38
    virtual AnmObj *RemoveAnmObj(AnmObjType type);          // at 0x3C
    virtual AnmObj *GetAnmObj(AnmObjType type);             // at 0x40
    virtual const AnmObj *GetAnmObj(AnmObjType type) const; // at 0x44

    bool GetScnMtxPos(math::MTX34 *pMtx, ScnObjMtxType type, u32 idx) const;

    void UpdateFrame();

    void EnableScnMdlCallbackTiming(Timing timing);
    void DisableScnMdlCallbackTiming(Timing timing);

    math::MTX34 *GetViewPosMtxArray();
    const math::MTX34 *GetViewPosMtxArray() const;

    math::MTX33 *GetViewNrmMtxArray();
    const math::MTX33 *GetViewNrmMtxArray() const;

    math::MTX34 *GetViewTexMtxArray();
    const math::MTX34 *GetViewTexMtxArray() const;

    const u8 *GetByteCode(ByteCodeType type) const;

    ResMdl GetResMdl() {
        return mResMdl;
    }
    const ResMdl GetResMdl() const {
        return mResMdl;
    }

    math::MTX34 *GetWldMtxArray() {
        return mpWorldMtxArray;
    }
    u32 *GetWldMtxAttribArray() {
        return mpWorldMtxAttribArray;
    }

    u32 GetNumViewMtx() const {
        return mNumViewMtx;
    }

    const u8 *GetByteCodeCalc() {
        return mpByteCodeCalc;
    }
    const u8 *GetByteCodeMix() {
        return mpByteCodeMix;
    }
    const u8 *GetByteCodeDrawOpa() {
        return mpByteCodeDrawOpa;
    }
    const u8 *GetByteCodeDrawXlu() {
        return mpByteCodeDrawXlu;
    }

    ResMdlDrawMode GetDrawMode() const {
        return mDrawMode;
    }

    void SetScnMdlCallback(ICalcWorldCallback *pCallback) {
        mpCalcWorldCallback = pCallback;
    }
    ICalcWorldCallback *GetScnMdlCallback() {
        return mpCalcWorldCallback;
    }

    u8 GetScnMdlCallbackTiming() const {
        return mCwcbTiming;
    }

    void SetScnMdlCallbackNodeID(u32 id) {
        mCwcbNodeID = id;
    }
    u32 GetScnMdlCallbackNodeID() const {
        return mCwcbNodeID;
    }

    AnmObjChr *GetAnmObjChr() {
        return mpAnmObjChr;
    }
    AnmObjVis *GetAnmObjVis() {
        return mpAnmObjVis;
    }
    AnmObjMatClr *GetAnmObjMatClr() {
        return mpAnmObjMatClr;
    }
    AnmObjTexPat *GetAnmObjTexPat() {
        return mpAnmObjTexPat;
    }
    AnmObjTexSrt *GetAnmObjTexSrt() {
        return mpAnmObjTexSrt;
    }

protected:
    enum ScnMdlSmplFlag {
        SCNMDLSMPLFLAG_LC_DMA = (1 << 0),
    };

    static const int VIEW_MAX = 16;

    static const int MTX_CACHE_MIN = 8;
    static const int MTX_CACHE_MAX = sizeof(detail::MtxCacheMap) / sizeof(math::MTX34) + 1;

protected:
    void ScnMdlSmpl_CalcPosture(u32 param, const math::MTX34 *pParent);

    void ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ(u32 param, IScnObjGather *pCollection);
    void ScnMdlSmpl_G3DPROC_CALC_WORLD(u32 param, const math::MTX34 *pParent);
    void ScnMdlSmpl_G3DPROC_CALC_MAT(u32 param, void *pInfo);
    void ScnMdlSmpl_G3DPROC_CALC_VIEW(u32 param, const math::MTX34 *pCamera);
    void ScnMdlSmpl_G3DPROC_DRAW_OPA(u32 param, void *pInfo);
    void ScnMdlSmpl_G3DPROC_DRAW_XLU(u32 param, void *pInfo);

    void ScnMdlSmpl_G3DPROC_UPDATEFRAME(u32 /* param */, void * /* pInfo */) {
        UpdateFrame();
    }

private:
    ResMdl mResMdl;                 // at 0xE8
    math::MTX34 *mpWorldMtxArray;   // at 0xEC
    u32 *mpWorldMtxAttribArray;     // at 0xF0
    math::MTX34 *mpViewPosMtxArray; // at 0xF4
    math::MTX33 *mpViewNrmMtxArray; // at 0xF8
    math::MTX34 *mpViewTexMtxArray; // at 0xFC
    u8 mNumView;                    // at 0x100
    u8 mCurView;                    // at 0x101
    u16 mNumViewMtx;                // at 0x102
    u32 mFlagScnMdlSimple;          // at 0x104

    const u8 *mpByteCodeCalc;    // at 0x108
    const u8 *mpByteCodeMix;     // at 0x10C
    const u8 *mpByteCodeDrawOpa; // at 0x110
    const u8 *mpByteCodeDrawXlu; // at 0x114
    ResMdlDrawMode mDrawMode;    // at 0x118

    ICalcWorldCallback *mpCalcWorldCallback; // at 0x11C
    u8 mCwcbTiming;                          // at 0x120
    u8 mCwcbDeleteOption;                    // at 0x121
    u16 mCwcbNodeID;                         // at 0x122

    AnmObjChr *mpAnmObjChr;       // at 0x124
    AnmObjVis *mpAnmObjVis;       // at 0x128
    AnmObjMatClr *mpAnmObjMatClr; // at 0x12C
    AnmObjTexPat *mpAnmObjTexPat; // at 0x130
    AnmObjTexSrt *mpAnmObjTexSrt; // at 0x134

    NW4R_G3D_RTTI_DECL_DERIVED(ScnMdlSimple, ScnLeaf);
};

} // namespace g3d
} // namespace nw4r

#endif
