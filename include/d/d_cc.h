#ifndef D_CC_H
#define D_CC_H

#include "c/c_list.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"

// Somewhere in d_cc
class LinkedCollider : public dCcD_Unk {
public:
    LinkedCollider() : mpList(nullptr), field_0x210(this) {}
    virtual ~LinkedCollider() {
        if (mpList != nullptr) {
            mpList->remove(&mNode);
            mpList = nullptr;
        }
    }
    /* 0x208 */ cListNd_c mNode;
    /* 0x210 */ dCcD_Unk *field_0x210;
    /* 0x214 */ cListMg_c *mpList;
};

struct ColliderLinkedList {
    cListMg_c mList;
    virtual ~ColliderLinkedList();

    void addCc(LinkedCollider &ccD, const dCcD_SrcUnk &src);
    void SetStts(cCcD_Stts &stts);
    void registerColliders();
};

#endif
