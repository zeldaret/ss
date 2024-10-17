#include "d/lyt/d_structd.h"

namespace d2d {

dLytStructDList::dLytStructDList() {
    field_0x18 = true;
    sInstance = this;
}

dLytStructDList *dLytStructDList::create(EGG::Heap *heap) {
    return new (heap) dLytStructDList();
}

void dLytStructDList::appendToList1(dLytStructD *other) {
    mList1.insert(other);
}

void dLytStructDList::removeFromList1(dLytStructD *other) {
    mList1.remove(other);
}

void dLytStructDList::appendToList2(dLytStructD *other) {
    mList2.insert(other);
}

void dLytStructDList::removeFromList2(dLytStructD *other) {
    mList2.remove(other);
}

dLytStructD::~dLytStructD() {
    delete[] field_0x1C;
    field_0x1C = nullptr;
}

} // namespace d2d
