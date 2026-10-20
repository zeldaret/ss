#ifndef NW4R_G3D_RES_RES_USER_H
#define NW4R_G3D_RES_RES_USER_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_rescommon.h"
#include "nw4r/g3d/res/g3d_resdict.h"

// DISCLAIMER:
// We only know ResUserData as a name.
// ResUserDataEntry and ResUserDataEntryData were made up from Brawlcrates info.
namespace nw4r {
namespace g3d {

// Example UserData Binary as refernce
// (ObjectPack -> Bc -> KeySmall mdl -> m_KeySmallNormal material)
// Help from referencing https://github.com/soopercool101/BrawlCrate/blob/master/BrawlLib/SSBB/UserData.cs
//               [00 00 00 44] [00 00 00 28   00 00 00 01
// (FF FF 00 00   00 01 00 00   00 00 00 00   00 00 00 00)
// (00 06 00 00   00 00 00 01   00 00 01 DC   00 00 00 28)]  (1DC -> 00 00 00 01  45 00 00 00) "E"
// [00 00 00 18   00 00 01 AC   00 00 00 01   00 00 00 02    (1AC -> 00 00 00 01  30 00 00 00) "0"
//  00 00 01 B4   00 00 00 00]                               (1B4 -> 00 00 00 01  45 00 00 00) "E" // Same entry as 1DC

// Types 0 -> int
//       1 -> float
//       2 -> string (used)
struct ResUserDataEntryData {
    u32 size;        // at 0x00 (BrawlCrate: _totalLen)
    u32 toEntryData; // at 0x04 (BrawlCrate: _dataOffset)
    u32 entryCount;  // at 0x08 (BrawlCrate: _entryCount)
    u32 entryType;   // at 0x0C (BrawlCrate: _type)
    u32 toName;      // at 0x10 (BrawlCrate: _stringOffset)
    u32 id;          // at 0x14 (BrawlCrate: _id)
};

struct ResUserDataData {
    u32 size;       // at 0x00 (BrawlCrate: _totalLen)
    ResDicData dic; // at 0x04 (BrawlCrate: ResourceGroup)
};

class ResUserDataEntry : public ResCommon<ResUserDataEntryData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResUserDataEntry);

    const char *GetName() const {
        return ofs_to_ptr<char>(ref().toName);
    }
};

class ResUserData : public ResCommon<ResUserDataData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResUserData);

    // Null Check doesnt work here when copying from g3d_resfile.cpp as template.
    ResUserDataEntry GetResUserDataEntry(u32 idx) const {
        void *pResUserDataEntryData = ResDic(const_cast<ResDicData *>(&ref().dic))[idx];

        // if (pResUserDataEntryData != NULL) {
        //     return ResUserDataEntry(pResUserDataEntryData);
        // }
        // return ResUserDataEntry(NULL);

        return ResUserDataEntry(pResUserDataEntryData);
    }

    u32 GetNumEntries() const {
        ResDic dic = ResDic(const_cast<ResDicData *>(&ref().dic));
        return dic.GetNumData();
    }
};

} // namespace g3d
} // namespace nw4r

#endif
