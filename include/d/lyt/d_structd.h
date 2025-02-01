#ifndef D_LYT_STRUCTD_H
#define D_LYT_STRUCTD_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "toBeSorted/tlist.h"

namespace d2d {

class dLytStructD;

class dLytStructC {
public:
    dLytStructC() : field_0x10(0), field_0x14(nullptr) {}
    virtual ~dLytStructC() {}
    virtual void *dLytStructC0x0C();

    TListNode<dLytStructC> mLink;
    u16 field_0x10;
    dLytStructD *field_0x14;
};

class dLytStructCSub : public dLytStructC {
private:
public:
    dLytStructCSub() {}
    virtual ~dLytStructCSub() {}
    virtual void *dLytStructC0x0C();

    u8 field_0x18[8];
};

struct dLytStructD_Base {
    dLytStructD_Base() : field_0x0C(0), field_0x10(0), field_0x14(0), field_0x015(0), field_0x016(0) {}
    virtual u32 getType() = 0;
    virtual void doSomething() = 0;
    TListNode<dLytStructD_Base> mLink;
    u32 field_0x0C;
    u32 field_0x10;
    u16 field_0x14;
    u8 field_0x015;
    u8 field_0x016;
};

struct dLytStructDInArray {
    dLytStructDInArray() {
        field_0x04 = 0;
        field_0x08 = 0;
        field_0x0A = 0;
        field_0x0C = 0;
        field_0x0E = 0;
    }
    virtual ~dLytStructDInArray() {}
    u32 field_0x04;
    u16 field_0x08;
    u16 field_0x0A;
    u16 field_0x0C;
    u16 field_0x0E;
};

struct dLytStructD : dLytStructD_Base {
    dLytStructD() : field_0x18(0), field_0x1C(nullptr), field_0x20(0), field_0x22(0), field_0x23(0), field_0x24(nullptr) {}

    virtual u32 getType() override;
    virtual void doSomething() override;
    virtual ~dLytStructD();

    void init(nw4r::lyt::Pane *, u16, u8, u8);
    void fn_80065E70(nw4r::lyt::Pane *, s32, s32, s32);
    void fn_80065F70();

    u32 field_0x18;
    dLytStructDInArray *field_0x1C;
    u16 field_0x20;
    u8 field_0x22;
    u8 field_0x23;
    void *field_0x24;
};

class dLytStructDList {
public:
    dLytStructDList();
    static dLytStructDList *create(EGG::Heap *heap);

    static dLytStructDList *sInstance;

    void appendToList1(dLytStructC *other);
    void removeFromList1(dLytStructC *other);

    void appendToList2(dLytStructD *other);
    void removeFromList2(dLytStructD *other);

    bool fn_80065A30(dLytStructD *other);

private:
    TList<dLytStructC, offsetof(dLytStructC, mLink)> mList1;
    TList<dLytStructD, offsetof(dLytStructD, mLink)> mList2;
    bool field_0x18;
};

} // namespace d2d

#endif
