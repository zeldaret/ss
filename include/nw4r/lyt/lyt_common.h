#ifndef NW4R_LYT_COMMON_H
#define NW4R_LYT_COMMON_H
#include "common.h"
#include "nw4r/math/math_types.h"
#include "nw4r/ut/ut_binaryFileFormat.h"

#define NW4R_RES_NAME_SIZE 16
#define NW4R_MAT_NAME_SIZE 20

#define TEXCOORD_VTX_COUNT 4

namespace nw4r {
namespace lyt {
namespace res {
struct BinaryFileHeader {
    char signature[4]; // at 0x00
    u16 byteOrder;     // at 0x04;
    u16 version;       // at 0x06
    u32 fileSize;      // at 0x08
    u16 headerSize;    // at 0x0C
    u16 dataBlocks;    // at 0x0E
};

struct DataBlockHeader {
    u32 kind; // at 0x0
    u32 size; // at 0x4
};
} // namespace res

namespace detail {
typedef math::VEC2 TexCoordData[TEXCOORD_VTX_COUNT];

struct TexCoordAry {
    TexCoordAry();
    void Free();
    void Reserve(u8);
    void SetSize(u8);
    void Copy(const void *, u8);

    u8 mCap;              // at 0x0
    u8 mNum;              // at 0x1
    TexCoordData *mpData; // at 0x4
};

bool EqualsResName(const char *, const char *);
bool EqualsMaterialName(const char *, const char *);
bool TestFileHeader(const res::BinaryFileHeader &);
bool TestFileHeader(const res::BinaryFileHeader &, u32);
bool IsModulateVertexColor(ut::Color *, u8);
void MultipleAlpha(ut::Color *, const ut::Color *, u8); // Inlined
ut::Color MultipleAlpha(ut::Color, u8);
void SetVertexFormat(bool, u8);
void DrawQuad(const math::VEC2 &, const Size &, u8, const TexCoordData *, const ut::Color *);
void DrawQuad(const math::VEC2 &, const Size &, u8, const TexCoordData *, const ut::Color *, u8);
void DrawLine(const math::VEC2 &pos, const Size &size, ut::Color color);

inline s32 GetSignatureInt(const char *sig) {
    return *((s32 *)sig);
}
inline bool TestFileVersion(const res::BinaryFileHeader &fileHeader) {
    u32 majorVer = (fileHeader.version >> 8) & 0xFF;
    u32 minorVer = fileHeader.version & 0xFF;
    bool ret = majorVer == 0 && (minorVer > 7 && minorVer <= 10);
    return ret;
}

} // namespace detail
} // namespace lyt
} // namespace nw4r

#endif
