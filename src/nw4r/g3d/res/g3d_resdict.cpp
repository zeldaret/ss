#include <nw4r/g3d.h>

#include <cstring>

namespace nw4r {
namespace g3d {

ResDicNodeData* ResDic::Get(const ResName name) const {
    u32 len = name.GetLength();
    const char* pName = name.GetName();
    const ResDicData& r = ref();

    const ResDicNodeData* c = &r.data[0];
    const ResDicNodeData* x = &r.data[c->idxLeft];

    while (c->ref > x->ref) {
        c = x;

        u32 wd = x->ref >> 3;
        u32 pos = x->ref & 7;

        if (wd < len && (pName[wd] >> pos) & 1) {
            x = &r.data[x->idxRight];
        } else {
            x = &r.data[x->idxLeft];
        }
    }

    if (name == NW4R_G3D_OFS_TO_RESNAME(&r, x->ofsString)) {
        return const_cast<ResDicNodeData*>(x);
    }

    return NULL;
}

ResDicNodeData* ResDic::Get(const char* pName, u32 len) const {
    const ResDicData& r = ref();

    const ResDicNodeData* c = &r.data[0];
    const ResDicNodeData* x = &r.data[c->idxLeft];

    while (c->ref > x->ref) {
        c = x;

        u32 wd = x->ref >> 3;
        u32 pos = x->ref & 7;

        if (wd < len && (pName[wd] >> pos) & 1) {
            x = &r.data[x->idxRight];
        } else {
            x = &r.data[x->idxLeft];
        }
    }

    if (x->ofsString != 0 &&
        std::strcmp(pName, ofs_to_ptr<char>(x->ofsString)) == 0) {
        return const_cast<ResDicNodeData*>(x);
    }

    return NULL;
}

void* ResDic::operator[](const char* pName) const {
    if (IsValid() && pName != NULL) {
        ResDicNodeData* pNode = Get(pName, std::strlen(pName));

        if (pNode != NULL) {
            return const_cast<void*>(ofs_to_ptr_raw<void>(pNode->ofsData));
        }
    }

    return NULL;
}

void* ResDic::operator[](const ResName name) const {
    if (IsValid() && name.IsValid()) {
        ResDicNodeData* pNode = Get(name);

        if (pNode != NULL) {
            return const_cast<void*>(ofs_to_ptr_raw<void>(pNode->ofsData));
        }
    }

    return NULL;
}

s32 ResDic::GetIndex(const ResName name) const {
    if (IsValid() && name.IsValid()) {
        ResDicNodeData* pNode = Get(name);

        if (pNode != NULL) {
            return pNode - (ref().data + 1);
        }
    }

    return NOT_FOUND;
}

} // namespace g3d
} // namespace nw4r
