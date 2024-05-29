#ifndef NW4R_LYT_COMMON_H
#define NW4R_LYT_COMMON_H
#include "common.h"
#include "nw4r/math/math_types.h"
#include <nw4r/lyt/lyt_resources.h>
#include <nw4r/lyt/lyt_types.h>

namespace nw4r {
namespace lyt {

namespace detail {

inline s32 GetSignatureInt(const char *sig) {
    return *((s32 *)sig);
}
inline bool TestFileVersion(const res::BinaryFileHeader &fileHeader) {
    u32 majorVer = (fileHeader.version >> 8) & 0xFF;
    u32 minorVer = fileHeader.version & 0xFF;
    bool ret = majorVer == 0 && (minorVer > 7 && minorVer <= 10);
    return ret;
}

inline u8 GetVtxColorElement(const ut::Color *cols, u32 idx) {
    return ((u8 *)cols)[(idx & ~3) + (idx & 3)];
}

inline void SetVtxColElement(ut::Color *cols, u32 idx, u8 value) {
    ((u8 *)cols)[(idx & ~3) + (idx & 3)] = value;
}

inline u8 GetHorizontalPosition(u8 var) {
    return var % 3;
}
inline u8 GetVerticalPosition(u8 var) {
    return var / 3;
}
inline void SetHorizontalPosition(u8 *pVar, u8 newVal) {
    *pVar = newVal + GetVerticalPosition(*pVar) * 3;
}
inline void SetVerticalPosition(u8 *pVar, u8 newVal) {
    *pVar = newVal * 3 + GetHorizontalPosition(*pVar);
}

typedef math::VEC2 TexCoordData[TEXCOORD_VTX_COUNT];

class TexCoordAry {
public:
    TexCoordAry();
    void Free();
    void Reserve(u8);
    void SetSize(u8);
    void Copy(const void *, u8);
    bool IsEmpty() const {
        return mCap == 0;
    }
    u8 GetSize() const {
        return mNum;
    }
    TexCoordData *GetArray() const {
        return mpData;
    }

private:
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
void DrawQuad(const math::VEC2 &basePt, const Size &size, u8 texCoordNum, const TexCoordData *texCoords,
        const ut::Color *vtxColors, u8 alpha);
void DrawLine(const math::VEC2 &pos, const Size &size, ut::Color color);

} // namespace detail

struct ResBlockSet {
    const res::TextureList *pTextureList;   // at 0x00
    const res::FontList *pFontList;         // at 0x04
    const res::MaterialList *pMaterialList; // at 0x08
    ResourceAccessor *pResAccessor;         // at 0x0C
};

} // namespace lyt
} // namespace nw4r

#endif
