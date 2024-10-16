#ifndef NW4R_UT_LINK_LIST_H
#define NW4R_UT_LINK_LIST_H
#include "nw4r/types_nw4r.h"
#include "nw4r/ut/ut_NonCopyable.h"

/**
 * Declare typedef for linked-list specialization.
 */
#define NW4R_UT_LIST_TYPEDEF_DECL(T) typedef nw4r::ut::LinkList<T, offsetof(T, node)> T##List;

/**
 * Declare typedef for linked-list specialization.
 *
 * Use the specified link node (name suffix) for classes with multiple nodes.
 */
#define NW4R_UT_LIST_TYPEDEF_DECL_EX(T, SUFFIX)                                                                        \
    typedef nw4r::ut::LinkList<T, offsetof(T, node##SUFFIX)> T##SUFFIX##List;

/**
 * Declare a member LinkListNode for use with the typedef.
 */
#define NW4R_UT_LIST_NODE_DECL() nw4r::ut::LinkListNode node

/**
 * Declare a member LinkListNode for use with the typedef.
 *
 * Use the specified link node (name suffix) for classes with multiple nodes.
 */
#define NW4R_UT_LIST_NODE_DECL_EX(SUFFIX) nw4r::ut::LinkListNode node##SUFFIX

/**
 * Explicitly instantiate a linked list specialization.
 * (RESERVED FOR MATCHING DECOMP HACKS)
 */
#ifndef NON_MATCHING
#define NW4R_UT_LIST_TYPEDEF_INST(T) template struct nw4r::ut::LinkList<T, offsetof(T, node)>
#else
#define NW4R_UT_LIST_TYPEDEF_INST(T)
#endif

namespace nw4r {
namespace ut {

// Forward declaration
namespace detail {
class LinkListImpl;
}

class LinkListNode : private NonCopyable {
    friend class detail::LinkListImpl;

public:
    LinkListNode() : mNext(NULL), mPrev(NULL) {}

    LinkListNode *GetNext() const {
        return mNext;
    }
    LinkListNode *GetPrev() const {
        return mPrev;
    }

private:
    LinkListNode *mNext; // at 0x0
    LinkListNode *mPrev; // at 0x4
};

namespace detail {

class LinkListImpl : private NonCopyable {
public:
    // Forward declaration
    class ConstIterator;

    class Iterator {
        friend class LinkListImpl;
        friend class ConstIterator;

    public:
        Iterator() : mNode(NULL) {}
        Iterator(LinkListNode *node) : mNode(node) {}

        Iterator &operator++() {
            mNode = mNode->GetNext();
            return *this;
        }

        Iterator &operator--() {
            mNode = mNode->GetPrev();
            return *this;
        }

        LinkListNode *operator->() const {
            return mNode;
        }

        friend bool operator==(LinkListImpl::Iterator lhs, LinkListImpl::Iterator rhs) {
            return lhs.mNode == rhs.mNode;
        }

    private:
        LinkListNode *mNode; // at 0x0
    };

    class ConstIterator {
        friend class LinkListImpl;

    public:
        ConstIterator(Iterator it) : mNode(it.mNode) {}

        ConstIterator &operator++() {
            mNode = mNode->GetNext();
            return *this;
        }

        ConstIterator &operator--() {
            mNode = mNode->GetPrev();
            return *this;
        }

        const LinkListNode *operator->() const {
            return mNode;
        }

        friend bool operator==(LinkListImpl::ConstIterator lhs, LinkListImpl::ConstIterator rhs) {
            return lhs.mNode == rhs.mNode;
        }

    private:
        LinkListNode *mNode; // at 0x0
    };

protected:
    static Iterator GetIteratorFromPointer(LinkListNode *node) {
        return Iterator(node);
    }

    LinkListImpl() {
        Initialize_();
    }
    ~LinkListImpl();

    Iterator GetBeginIter() {
        return Iterator(mNode.GetNext());
    }
    Iterator GetEndIter() {
        return Iterator(&mNode);
    }

    Iterator Insert(Iterator it, LinkListNode *node);

    Iterator Erase(Iterator it);
    Iterator Erase(LinkListNode *node);
    Iterator Erase(Iterator begin, Iterator end);

public:
    u32 GetSize() const {
        return mSize;
    }
    bool IsEmpty() const {
        return mSize == 0;
    }

    void PopFront() {
        Erase(GetBeginIter());
    }
    void PopBack() {
        Erase(GetEndIter());
    }

    void Clear();

private:
    void Initialize_() {
        mSize = 0;
        mNode.mNext = &mNode;
        mNode.mPrev = &mNode;
    }

private:
    u32 mSize;          // at 0x0
    LinkListNode mNode; // at 0x4
};

template <class TIter>
class ReverseIterator {
public:
    ReverseIterator(TIter it) : mCurrent(it) {}

    TIter GetBase() const {
        return mCurrent;
    }

    ReverseIterator &operator++() {
        --mCurrent;
        return *this;
    }

    const typename TIter::TElem *operator->() const {
        return &this->operator*();
    }

    typename TIter::TElem &operator*() const {
        TIter it = mCurrent;
        --it;
        return *it;
    }

    friend bool operator==(const ReverseIterator &lhs, const ReverseIterator &rhs) {
        return lhs.mCurrent == rhs.mCurrent;
    }

    friend bool operator!=(const ReverseIterator &lhs, const ReverseIterator &rhs) {
        return !(lhs.mCurrent == rhs.mCurrent);
    }

private:
    TIter mCurrent; // at 0x0
};

} // namespace detail

template <typename T, int Ofs>
class LinkList : public detail::LinkListImpl {
public:
    class Iterator {
        friend class LinkList;

    public:
        // Element type must be visible to ReverseIterator
        typedef T TElem;

    public:
        Iterator() : mIterator(NULL) {}
        Iterator(LinkListImpl::Iterator it) : mIterator(it) {}

        Iterator &operator++() {
            ++mIterator;
            return *this;
        }

        Iterator &operator--() {
            --mIterator;
            return *this;
        }

        Iterator operator++(int) {
            Iterator ret = *this;
            ++*this;
            return ret;
        }

        T *operator->() const {
            return GetPointerFromNode(mIterator.operator->());
        }

        T &operator*() const {
            return *this->operator->();
        }

        friend bool operator==(Iterator lhs, Iterator rhs) {
            return lhs.mIterator == rhs.mIterator;
        }

        friend bool operator!=(Iterator lhs, Iterator rhs) {
            return !(lhs == rhs);
        }

    private:
        LinkListImpl::Iterator mIterator; // at 0x0
    };

    class ConstIterator {
        friend class LinkList;

    public:
        // Element type must be visible to ReverseIterator
        typedef T TElem;

    public:
        ConstIterator(LinkListImpl::Iterator it) : mIterator(it) {}

        ConstIterator &operator++() {
            ++mIterator;
            return *this;
        }

        ConstIterator &operator--() {
            --mIterator;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator ret = *this;
            ++*this;
            return ret;
        }

        const T *operator->() const {
            return GetPointerFromNode(mIterator.operator->());
        }

        const T &operator*() const {
            return *this->operator->();
        }

        friend bool operator==(ConstIterator lhs, ConstIterator rhs) {
            return lhs.mIterator == rhs.mIterator;
        }

        friend bool operator!=(ConstIterator lhs, ConstIterator rhs) {
            return !(lhs == rhs);
        }

    private:
        LinkListImpl::ConstIterator mIterator; // at 0x0
    };

public:
    // Shorthand names for reverse iterator types
    typedef detail::ReverseIterator<Iterator> RevIterator;
    typedef detail::ReverseIterator<ConstIterator> RevConstIterator;

public:
    LinkList() {}
    ~LinkList() {}

    Iterator GetBeginIter() {
        return Iterator(LinkListImpl::GetBeginIter());
    }
    ConstIterator GetBeginIter() const {
        return ConstIterator(GetBeginIter());
    }
    detail::ReverseIterator<Iterator> GetBeginReverseIter() {
        return detail::ReverseIterator<Iterator>(GetBeginIter());
    }

    Iterator GetEndIter() {
        return Iterator(LinkListImpl::GetEndIter());
    }
    ConstIterator GetEndIter() const {
        return ConstIterator(GetEndIter());
    }
    detail::ReverseIterator<Iterator> GetEndReverseIter() {
        return detail::ReverseIterator<Iterator>(GetEndIter());
    }

    Iterator Insert(Iterator it, T *p) {
        return Iterator(LinkListImpl::Insert(it.mIterator, GetNodeFromPointer(p)));
    }

    Iterator Erase(T *p) {
        return Iterator(LinkListImpl::Erase(GetNodeFromPointer(p)));
    }
    Iterator Erase(Iterator it) {
        return Iterator(LinkListImpl::Erase(it.mIterator));
    }

    void PushBack(T *p) {
        Insert(GetEndIter(), p);
    }

    T &GetFront() {
        return *GetBeginIter();
    }
    const T &GetFront() const {
        return *GetBeginIter();
    }

    T &GetBack() {
        return *--GetEndIter();
    }
    const T &GetBack() const {
        return *--GetEndIter();
    }

    static Iterator GetIteratorFromPointer(T *p) {
        return GetIteratorFromPointer(GetNodeFromPointer(p));
    }

    static Iterator GetIteratorFromPointer(LinkListNode *node) {
        return Iterator(LinkListImpl::GetIteratorFromPointer(node));
    }

    static LinkListNode *GetNodeFromPointer(T *p) {
        return reinterpret_cast<LinkListNode *>(reinterpret_cast<char *>(p) + Ofs);
    }

    static T *GetPointerFromNode(LinkListNode *node) {
        return reinterpret_cast<T *>(reinterpret_cast<char *>(node) - Ofs);
    }

    static const T *GetPointerFromNode(const LinkListNode *node) {
        return reinterpret_cast<const T *>(reinterpret_cast<const char *>(node) - Ofs);
    }
};

} // namespace ut
} // namespace nw4r

#endif
