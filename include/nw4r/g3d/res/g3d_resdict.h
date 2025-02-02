#ifndef NW4R_G3D_RES_RES_DICT_H
#define NW4R_G3D_RES_RES_DICT_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_rescommon.h>

namespace nw4r {
namespace g3d {

struct ResDicNodeData {
    u16 ref;       // at 0x0
    u16 flag;      // at 0x2
    u16 idxLeft;   // at 0x4
    u16 idxRight;  // at 0x6
    s32 ofsString; // at 0x8
    s32 ofsData;   // at 0xC
};

struct ResDicData {
    u32 size;               // at 0x0
    u32 numData;            // at 0x4
    ResDicNodeData data[1]; // at 0x8
};

class ResDic : public ResCommon<ResDicData> {
public:
    static const s32 NOT_FOUND = -1;

public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResDic);

    void *operator[](const char *pName) const;
    void *operator[](const ResName name) const;
    void *operator[](int idx) const {
        if (IsValid()) {
            return const_cast<void *>(ofs_to_ptr<void>(ref().data[idx + 1].ofsData));
        }

        return NULL;
    }

    s32 GetIndex(const ResName name) const;

    u32 GetNumData() const {
        if (IsValid()) {
            return ptr()->numData;
        }

        return 0;
    }

private:
    ResDicNodeData *Get(const ResName name) const;
    ResDicNodeData *Get(const char *pName, u32 len) const;
};

} // namespace g3d
} // namespace nw4r

#endif
