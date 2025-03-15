#ifndef NW4R_G3D_RES_RES_PLTT_H
#define NW4R_G3D_RES_RES_PLTT_H
#include <nw4r/types_nw4r.h>

/** NOTICE: Revision change from 1->??. Structures, Enums, and/or Functions may have changed and not yet done so
 * (Zeldex72, Feb 1, 2025) */

#include "nw4r/g3d/res/g3d_rescommon.h"
#include "rvl/GX.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {

struct ResPlttData {
    ResBlockHeaderData header; // at 0x0
    u32 revision;              // at 0x8
    s32 toResFileData;         // at 0xC
    s32 toPlttData;            // at 0x10
    s32 name;                  // at 0x14
    GXTlutFmt fmt;             // at 0x18
    u16 numEntries;            // at 0x1C
    u16 PADDING_0x1E;          // at 0x1E
    s32 original_path;         // at 0x20
    s32 toResUserData;         // at 0x24
};

class ResPltt : public ResCommon<ResPlttData> {
public:
    static const u32 SIGNATURE = 'PLT0';
    static const int REVISION = 1;

public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResPltt);

    void Init() {
        DCStore(false);
    }

    u32 GetRevision() const {
        return ref().revision;
    }

    bool CheckRevision() const {
        return GetRevision() == REVISION;
    }

    void DCStore(bool sync);

    u16 *GetPlttData() {
        ResPlttData &r = ref();

        // clang-format off
        return r.toPlttData != 0
            ? reinterpret_cast<u16*>(reinterpret_cast<u8*>(&r) + r.toPlttData)
            : NULL;
        // clang-format on
    }

    const u16 *GetPlttData() const {
        const ResPlttData &r = ref();

        // clang-format off
        return r.toPlttData != 0
            ? reinterpret_cast<const u16*>(reinterpret_cast<const u8*>(&r) + r.toPlttData)
            : NULL;
        // clang-format on
    }

    GXTlutFmt GetFmt() const {
        return ref().fmt;
    }

    u32 GetNumEntries() const {
        return ref().numEntries;
    }
};

} // namespace g3d
} // namespace nw4r

#endif
