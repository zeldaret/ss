#include "nw4r/ut/ut_LinkList.h"

namespace nw4r {
namespace ut {
namespace detail {

LinkListImpl::~LinkListImpl() {
    Clear();
}

LinkListImpl::Iterator LinkListImpl::Erase(LinkListImpl::Iterator it) {
    Iterator copy(it);
    return Erase(it, ++copy);
}

void LinkListImpl::Clear() {
    Erase(GetBeginIter(), GetEndIter());
}

LinkListImpl::Iterator LinkListImpl::Insert(Iterator it, LinkListNode *p) {
    LinkListNode *next = it.mNode;
    LinkListNode *prev = next->mPrev;

    // prev <- p -> next
    p->mNext = next;
    p->mPrev = prev;
    // prev <-> p <-> next
    next->mPrev = p;
    prev->mNext = p;

    mSize++;

    return Iterator(p);
}

LinkListImpl::Iterator LinkListImpl::Erase(LinkListNode *p) {
    LinkListNode *next = p->mNext;
    LinkListNode *prev = p->mPrev;

    // Remove connections to node
    next->mPrev = prev;
    prev->mNext = next;

    mSize--;

    // Isolate node
    p->mNext = NULL;
    p->mPrev = NULL;

    return Iterator(next);
}

/* Not in SS */
LinkListImpl::Iterator LinkListImpl::Erase(Iterator begin, Iterator end) {
    LinkListNode *pCur = begin.mNode;
    LinkListNode *pEnd = end.mNode;

    while (pCur != pEnd) {
        // Preserve next node before erasing pointers
        LinkListNode *pNext = pCur->mNext;
        // Erase current node
        Erase(pCur);
        pCur = pNext;
    }

    return Iterator(pEnd);
}

} // namespace detail
} // namespace ut
} // namespace nw4r
