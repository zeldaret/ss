#ifndef T_LIST_H
#define T_LIST_H

// A templated list, similar to nw4r::ut::LinkList.
// Notable differences are:
// * No dtor
// * Entirely in headers
// * Swapped node and count members
// Used in DowsingTarget, maybe also in dAcBase_c.soundStuff

// It's not immediately clear whether this stores pointers to nodes
// or pointers to objects. DowsingTarget's node offset is 0, so there's
// no observable difference, but the SoundSource stuff makes it
// look like it stores object pointers

class TListNode {
public:
    TListNode() : mpPrev(nullptr), mpNext(nullptr) {}

    void *GetNext() const {
        return mpNext;
    }
    void *GetPrev() const {
        return mpPrev;
    }
    void *mpPrev;
    void *mpNext;
};

template <typename T, int offset>
class TList {
public:
    class Iterator {
    public:
        Iterator() : mNode(NULL) {}
        Iterator(TListNode *node) : mNode(node) {}

        Iterator &operator++() {
            mNode = GetNodeFromPointer(mNode->GetNext());
            return *this;
        }

        Iterator &operator--() {
            mNode = mNode->GetPrev();
            return *this;
        }

        T *operator->() const {
            return GetPointerFromNode(mNode);
        }

        T &operator*() {
            return *this->operator->();
        }

        friend bool operator==(Iterator lhs, Iterator rhs) {
            return lhs.mNode == rhs.mNode;
        }

        friend bool operator!=(Iterator lhs, Iterator rhs) {
            return !(lhs.mNode == rhs.mNode);
        }

    private:
        TListNode *mNode; // at 0x0
    };

    TList() {
        this->mNode.mpNext = GetPointerFromNode(&mNode);
        this->mNode.mpPrev = GetPointerFromNode(&mNode);
        mCount = 0;
    }

    Iterator GetBeginIter() {
        return GetNodeFromPointer(&mNode) != mNode.GetNext() ? Iterator(GetNodeFromPointer(mNode.GetNext())) :
                                                               Iterator(&mNode);
    }
    Iterator GetEndIter() {
        return Iterator(&mNode);
    }

    static TListNode *GetNodeFromPointer(void *p) {
        return reinterpret_cast<TListNode *>(reinterpret_cast<char *>(p) + offset);
    }

    static T *GetPointerFromNode(TListNode *node) {
        return reinterpret_cast<T *>(reinterpret_cast<char *>(node) - offset);
    }

    void insert(T *value) {
        TListNode *node = GetNodeFromPointer(value);
        if (GetPointerFromNode(&mNode) == this->mNode.mpNext) {
            node->mpNext = GetPointerFromNode(&mNode);
            node->mpPrev = GetPointerFromNode(&mNode);
            this->mNode.mpNext = value;
            this->mNode.mpPrev = value;
            mCount++;
        } else {
            node->mpPrev = this->mNode.mpPrev;
            node->mpNext = GetPointerFromNode(&mNode);
            GetNodeFromPointer(this->mNode.mpPrev)->mpNext = value;
            this->mNode.mpPrev = value;
            mCount++;
        }
    }

    void remove(T *value) {
        TListNode *node = GetNodeFromPointer(value);
        T *next = reinterpret_cast<T *>(node->mpNext);
        if (GetPointerFromNode(&mNode) == node->mpPrev) {
            this->mNode.mpNext = next;
        } else {
            GetNodeFromPointer(node->mpPrev)->mpNext = next;
        }

        if (GetPointerFromNode(&mNode) == node->mpNext) {
            this->mNode.mpPrev = node->mpPrev;
        } else {
            GetNodeFromPointer(node->mpNext)->mpPrev = node->mpPrev;
        }
        node->mpPrev = nullptr;
        node->mpNext = nullptr;
        mCount--;
    }

    TListNode mNode;
    s32 mCount;
};

#endif
