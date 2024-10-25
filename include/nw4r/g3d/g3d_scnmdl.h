#ifndef NW4R_G3D_SCN_MDL_H
#define NW4R_G3D_SCN_MDL_H
#include "common.h"
#include "nw4r/g3d/g3d_draw.h"
#include "nw4r/g3d/g3d_resmat.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"

namespace nw4r {
namespace g3d {
class ScnMdl : public ScnMdlSimple {
public:
    class CopiedMatAccess {
    public:
        CopiedMatAccess(ScnMdl *, u32);
        ResTexSrt GetResTexSrtEx(bool);
        ResMatPix GetResMatPix(bool);
        ResMatTevColor GetResMatTevColor(bool);
        ResGenMode GetResGenMode(bool);

    private:
        char UNK_0x0[0x34];
    };

    class CopiedVisAccess {
    public:
        CopiedVisAccess(ScnMdl *, u32);
        void SetVisibility(bool);
    private:
        // TODO
        char UNK_0x0[0x10];
    };

public:
    virtual bool SetAnmObj(AnmObj *p, AnmObjType type) override;

    static ScnMdl *Construct(MEMAllocator *, unsigned long *, nw4r::g3d::ResMdl, u32 bufferOption, int);

    static const G3dObj::TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    DrawResMdlReplacement *GetDrawResMdlReplacement() {
        return &mDrawMdlReplace;
    }

private:
    char UNK_0x120[0x144 - 0x120];
    DrawResMdlReplacement mDrawMdlReplace; // at 0x144

    NW4R_G3D_TYPE_OBJ_DECL(ScnMdl);
};
} // namespace g3d
} // namespace nw4r

#endif
