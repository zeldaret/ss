#ifndef T_LIST_H
#define T_LIST_H

// A templated list, similar to nw4r::ut::LinkList.
// Notable differences are:
// * No dtor
// * Entirely in headers
// * Swapped node and count members
// Used in DowsingTarget, maybe also in dAcBase_c.soundStuff

#define TLIST_NODE_DEF(class) TListNode<class> mLink

#define TLIST_LIST_DEF(class, name) TList<class, offsetof(class, mLink)> name

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

    T *mpPrev;
    T *mpNext;
};

template <typename T, int offset>
class TList {
public:
    typedef TListNode<T> TNode;
    class Iterator {
    public:
        Iterator() : pVal(NULL) {}
        Iterator(T *p_val) : pVal(p_val) {}

        Iterator &operator++() {
            pVal = GetNodeFromPtr(pVal)->GetNext();
            return *this;
        }

        Iterator &operator--() {
            pVal = GetNodeFromPtr(pVal)->GetPrev();
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
        mStartEnd.mpNext = GetPtrFromNode(&mStartEnd);
        mStartEnd.mpPrev = GetPtrFromNode(&mStartEnd);
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
        return Iterator(GetPtrFromNode(&mStartEnd));
    }

    static TNode *GetNodeFromPtr(T *pT) {
        return (TNode *)((u8 *)pT + offset);
    }
    static T *GetPtrFromNode(TNode *pN) {
        return (T *)((u8 *)pN - offset);
    }
    void insert(T *value) {
        TNode *node = GetNodeFromPtr(value);
        if (GetPtrFromNode(&mStartEnd) == mStartEnd.mpNext) {
            node->mpNext = GetPtrFromNode(&mStartEnd);
            node->mpPrev = GetPtrFromNode(&mStartEnd);
            mStartEnd.mpNext = value;
            mStartEnd.mpPrev = value;
            mCount++;
        } else {
            node->mpPrev = mStartEnd.mpPrev;
            node->mpNext = GetPtrFromNode(&mStartEnd);
            GetNodeFromPtr(mStartEnd.mpPrev)->mpNext = value;
            mStartEnd.mpPrev = value;
            mCount++;
        }
    }

    void remove(T *value) {
        TNode *node = GetNodeFromPtr(value);
        T *next = reinterpret_cast<T *>(node->mpNext);
        if (GetPtrFromNode(&mStartEnd) == node->mpPrev) {
            this->mStartEnd.mpNext = next;
        } else {
            GetNodeFromPtr(node->mpPrev)->mpNext = next;
        }

        if (GetPtrFromNode(&mStartEnd) == node->mpNext) {
            this->mStartEnd.mpPrev = node->mpPrev;
        } else {
            GetNodeFromPtr(node->mpNext)->mpPrev = node->mpPrev;
        }
        node->mpPrev = nullptr;
        node->mpNext = nullptr;
        mCount--;
    }

    TNode mStartEnd;
    s32 mCount;
};

#endif
