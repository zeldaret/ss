#ifndef D_LYT_STRUCTD_H
#define D_LYT_STRUCTD_H

#include <egg/core/eggHeap.h>
#include <nw4r/ut/ut_LinkList.h>
#include <toBeSorted/tlist.h>

namespace d2d {

struct dLytStructD_Base {
    dLytStructD_Base() : field_0x0C(0), field_0x10(0), field_0x14(0), field_0x015(0), field_0x016(0) {}
    virtual ~dLytStructD_Base();
    TLIST_NODE_DEF(dLytStructD_Base);
    u32 field_0x0C;
    u32 field_0x10;
    u16 field_0x14;
    u8 field_0x015;
    u8 field_0x016;
};

struct dLytStructD : dLytStructD_Base {
    dLytStructD() : field_0x18(0), field_0x1C(0), field_0x20(0), field_0x22(0), field_0x23(0), field_0x24(0) {}

    virtual ~dLytStructD();

    void init(nw4r::lyt::Pane *, u16, u8, u8);
    void fn_80065E70(nw4r::lyt::Pane *, s32, s32, s32);
    void fn_80065F70();

    u32 field_0x18;
    u32 field_0x1C;
    u16 field_0x20;
    u8 field_0x22;
    u8 field_0x23;
    u32 field_0x24;
};

class dLytStructDList {
public:
    dLytStructDList();
    static dLytStructDList *create(EGG::Heap *heap);

    static dLytStructDList *sInstance;

    void appendToList1(dLytStructD *other);
    void removeFromList1(dLytStructD *other);

    void appendToList2(dLytStructD *other);
    void removeFromList2(dLytStructD *other);

private:
    TList<dLytStructD> mList2;
    TList<dLytStructD> mList1;
    bool field_0x18;
};

} // namespace d2d

#endif
