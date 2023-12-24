#ifndef NW4R_G3D_SCN_MDL_H
#define NW4R_G3D_SCN_MDL_H
#include "types_nw4r.h"
#include "g3d_scnmdlsmpl.h"
#include "g3d_resmat.h"
#include "g3d_draw.h"

namespace nw4r
{
    namespace g3d
    {
        class ScnMdl : public ScnMdlSimple
        {
        public:
            class CopiedMatAccess
            {
            public:
                CopiedMatAccess(ScnMdl *, u32);
                ResTexSrt GetResTexSrtEx();

            private:
                char UNK_0x0[0x2C];
            };

        public:
            static const G3dObj::TypeObj GetTypeObjStatic()
            {
                return TypeObj(TYPE_NAME);
            }

            DrawResMdlReplacement * GetDrawResMdlReplacement() { return &mDrawMdlReplace; }

        private:
            char UNK_0x118[0x144 - 0x118];
            DrawResMdlReplacement mDrawMdlReplace; // at 0x144

            NW4R_G3D_TYPE_OBJ_DECL(ScnMdl);
        };
    }
}

#endif