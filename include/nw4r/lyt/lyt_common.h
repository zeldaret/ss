#ifndef NW4R_LYT_COMMON_H
#define NW4R_LYT_COMMON_H
#include "types_nw4r.h"
#include "math_types.h"
#include "ut_binaryFileFormat.h"

#define NW4R_RES_NAME_SIZE 16
#define NW4R_MAT_NAME_SIZE 20

#define TEXCOORD_VTX_COUNT 4

namespace nw4r
{
    namespace lyt
    {
        namespace res
        {
            struct BinaryFileHeader : ut::BinaryFileHeader
            {

            };

            struct DataBlockHeader {
                u32 kind; // at 0x0
                u32 size; // at 0x4
            };
        }

        namespace detail
        {
            typedef math::VEC2 TexCoordData[TEXCOORD_VTX_COUNT];
             
            struct TexCoordAry
            {
                TexCoordAry();
                void Free();
                void Reserve(u8);
                void SetSize(u8);
                void Copy(const void *, u8);

                u8 mCap; // at 0x0
                u8 mSize; // at 0x1
                TexCoordData *mTexCoords; // at 0x4
            };

            bool EqualsResName(const char *, const char *);
            bool EqualsMaterialName(const char *, const char *);
            bool TestFileHeader(const res::BinaryFileHeader&);
            bool TestFileHeader(const res::BinaryFileHeader&, u32);
            bool IsModulateVertexColor(ut::Color *, u8);
            void MultipleAlpha(ut::Color *, const ut::Color *, u8); // Inlined
            ut::Color MultipleAlpha(ut::Color, u8);
            void SetVertexFormat(bool, u8);
            void DrawQuad(const math::VEC2&, const Size&, u8, const TexCoordData *, const ut::Color *);
            void DrawQuad(const math::VEC2&, const Size&, u8, const TexCoordData *, const ut::Color *, u8);
        }
    }
}

#endif