#ifndef NW4R_G3D_SCN_MDL_SIMPLE_H
#define NW4R_G3D_SCN_MDL_SIMPLE_H
#include "types_nw4r.h"
#include "g3d_scnobj.h"
#include "g3d_resmdl.h"

namespace nw4r
{
    namespace g3d
    {
        class ScnMdlSimple : public ScnLeaf
        {
        public:
            enum ByteCodeType
            {
                BYTE_CODE_CALC,
                BYTE_CODE_MIX,
                BYTE_CODE_DRAW_OPA,
                BYTE_CODE_DRAW_XLU,
            };

        public:
            ScnMdlSimple(MEMAllocator *, ResMdl, math::MTX34 *, u32 *,
                math::MTX34 *, math::MTX33 *, math::MTX34 *, int, int);

            virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
            {
                return (other == GetTypeObjStatic()) ? true
                    : ScnLeaf::IsDerivedFrom(other);
            }
            virtual void G3dProc(u32, u32, void *); // at 0xC
            virtual ~ScnMdlSimple(); // at 0x10
            virtual const TypeObj GetTypeObj() const // at 0x14
            {
                return TypeObj(TYPE_NAME);
            }
            virtual const char * GetTypeName() const // at 0x18
            {
                return GetTypeObj().GetTypeName();
            }

            const u8 * GetByteCode(ByteCodeType) const;

            const ResMdl GetResMdl() const { return mResMdl; }
            ResMdl GetResMdl() { return mResMdl; }

            math::MTX34 * GetWldMtxArray() { return mWldMatrixArray; }
            u32 * GetWldMtxAttribArray() { return mWldMtxAttribArray; }

            u16 GetNumViewMtx() const { return mNumViewMtx; }

        private:
            ResMdl mResMdl; // at 0xE8
            math::MTX34 *mWldMatrixArray; // at 0xEC
            u32 *mWldMtxAttribArray; // at 0xF0
            math::MTX34 *PTR_0xF4;
            math::MTX33 *PTR_0xF8;
            math::MTX34 *PTR_0xFC;
            u8 BYTE_0x100;
            u8 BYTE_0x101;
            u16 mNumViewMtx; // at 0x102
            UNKWORD WORD_0x104;
            void *mByteCodeCalc; // at 0x108
            void *mByteCodeMix; // at 0x10C
            void *mByteCodeDrawOpa; // at 0x110
            void *mByteCodeDrawXlu; // at 0x114

            NW4R_G3D_TYPE_OBJ_DECL(ScnMdlSimple);
        };
    }
}

#endif