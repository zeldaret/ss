#ifndef T_LIST_H
#define T_LIST_H

// A templated list, similar to nw4r::ut::LinkList.
// Notable differences are:
// * No dtor
// * Entirely in headers
// * Swapped node and count members
// Used in DowsingTarget, maybe also in dAcBase_c.soundStuff

#define TLIST_NODE_DEF(class) TListNode<class> mLink

template <typename T>
class TListNode {
public:
    TListNode() : mpPrev(nullptr), mpNext(nullptr) {}

    T *GetNext() const {
        return mpNext;
    }
    T *GetPrev() const {
        return mpPrev;
    }

    static TListNode<T> *GetNodeFromPtr(T *pT) {
        return &(pT->mLink);
    }
    T *GetPtr() {
        return (T *)((u8 *)this - offsetof(T, mLink));
    }

    T *mpPrev;
    T *mpNext;
};

template <typename T>
class TList {
public:
    typedef TListNode<T> TNode;
    class Iterator {
    public:
        Iterator() : pVal(NULL) {}
        Iterator(T *p_val) : pVal(p_val) {}

        Iterator &operator++() {
            pVal = TNode::GetNodeFromPtr(pVal)->GetNext();
            return *this;
        }

        Iterator &operator--() {
            pVal = TNode::GetNodeFromPtr(pVal)->GetPrev();
            return *this;
        }

        T *operator->() const {
            return pVal;
        }

        T &operator*() {
            return *this->operator->();
        }

        friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
            return lhs.pVal == rhs.pVal;
        }

        friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
            return !(lhs.pVal == rhs.pVal);
        }

    private:
        T *pVal; // at 0x0
    };

    TList() {
        mStartEnd.mpNext = mStartEnd.GetPtr();
        mStartEnd.mpPrev = mStartEnd.GetPtr();
        mCount = 0;
    }

    Iterator GetBeginIter() {
        if (GetEndIter() != Iterator(mStartEnd.GetNext())) {
            return Iterator(mStartEnd.GetNext());
        } else {
            return GetEndIter();
        }
    }

    Iterator GetEndIter() {
        return Iterator(mStartEnd.GetPtr());
    }

    void insert(T *value) {
        TNode *node = TNode::GetNodeFromPtr(value);
        if (mStartEnd.GetPtr() == mStartEnd.mpNext) {
            node->mpNext = mStartEnd.GetPtr();
            node->mpPrev = mStartEnd.GetPtr();
            mStartEnd.mpNext = value;
            mStartEnd.mpPrev = value;
            mCount++;
        } else {
            node->mpPrev = mStartEnd.mpPrev;
            node->mpNext = mStartEnd.GetPtr();
            TNode::GetNodeFromPtr(mStartEnd.mpPrev)->mpNext = value;
            mStartEnd.mpPrev = value;
            mCount++;
        }
    }

    void remove(T *value) {
        TNode *node = TNode::GetNodeFromPtr(value);
        T *next = reinterpret_cast<T *>(node->mpNext);
        if (mStartEnd.GetPtr() == node->mpPrev) {
            this->mStartEnd.mpNext = next;
        } else {
            TNode::GetNodeFromPtr(node->mpPrev)->mpNext = next;
        }

        if (mStartEnd.GetPtr() == node->mpNext) {
            this->mStartEnd.mpPrev = node->mpPrev;
        } else {
            TNode::GetNodeFromPtr(node->mpNext)->mpPrev = node->mpPrev;
        }
        node->mpPrev = nullptr;
        node->mpNext = nullptr;
        mCount--;
    }

    TNode mStartEnd;
    s32 mCount;
};

#endif
