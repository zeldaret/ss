#ifndef UNK_WITH_WATER_H
#define UNK_WITH_WATER_H

#include "common.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/tlist.h"

struct UnkWithWaterNode {
public:
    /* 0x00 */ TListNode<UnkWithWaterNode> mNode;
    /* 0x08 */ UNKWORD field_0x08;
    /* 0x0C */ UNKWORD field_0x0C;
};

class UnkWithWater {
public:
    typedef TList<UnkWithWaterNode, offsetof(UnkWithWaterNode, mNode)> NodeList;

    UnkWithWater() : field_0x04(0), field_0x08(0) {}
    virtual ~UnkWithWater() {
        remove();
    }

    static bool linkMdl(nw4r::g3d::ResMdl &mdl, UnkWithWater *thing);
    bool hasLinks() const {
        return mList.mCount != 0;
    }

private:
    void remove();

    /* 0x04 */ UNKWORD field_0x04;
    /* 0x08 */ UNKWORD field_0x08;
    /* 0x0C */ NodeList mList;
};

#endif
