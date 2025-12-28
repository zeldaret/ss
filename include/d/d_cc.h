#ifndef D_CC_H
#define D_CC_H

#include "c/c_list.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"

class LinkedColliderNode : public cListNd_c {
public:
    LinkedColliderNode(cCcD_Obj *obj) : mpList(nullptr), mpCcDObj(obj) {}
    void remove() {
        if (mpList != nullptr) {
            mpList->remove(this);
            mpList = nullptr;
        }
    }
    /* 0x08 */ cCcD_Obj *mpCcDObj;
    /* 0x0C */ cListMg_c *mpList;
};

class dCcD_Linked_Unk : public dCcD_Unk {
public:
    dCcD_Linked_Unk() : mNode(this) {}
    virtual ~dCcD_Linked_Unk() {
        mNode.remove();
    }
    /* 0x210 */ LinkedColliderNode mNode;
};

class dCcD_Linked_Cps : public dCcD_Cps {
public:
    dCcD_Linked_Cps() : mNode(this) {}
    virtual ~dCcD_Linked_Cps() {
        mNode.remove();
    }
    /* 0x170 */ LinkedColliderNode mNode;
};
class dCcD_Linked_Sph : public dCcD_Sph {
public:
    dCcD_Linked_Sph() : mNode(this) {}
    virtual ~dCcD_Linked_Sph() {
        mNode.remove();
    }
    /* 0x150 */ LinkedColliderNode mNode;
};
class dCcD_Linked_Cyl : public dCcD_Cyl {
public:
    dCcD_Linked_Cyl() : mNode(this) {}
    virtual ~dCcD_Linked_Cyl() {
        mNode.remove();
    }
    /* 0x150 */ LinkedColliderNode mNode;
};

class dColliderLinkedList {
private:
    cListMg_c mList;
    static void Set(cCcD_Obj *obj, void *cbArg);
    static void SetStts(cCcD_Obj *obj, void *cbArg);
    static void AtSet(cCcD_Obj *obj, void *cbArg);
    static void ClrAt(cCcD_Obj *obj, void *cbArg);
    static void TgSet(cCcD_Obj *obj, void *cbArg);
    static void ClrTg(cCcD_Obj *obj, void *cbArg);
    static void CoSet(cCcD_Obj *obj, void *cbArg);
    static void ClrCo(cCcD_Obj *obj, void *cbArg);

public:
    virtual ~dColliderLinkedList();

    void postInit(cCcD_Obj &ccD);
    void addCc(dCcD_Linked_Cyl &ccD, const dCcD_SrcCyl &src);
    void addCc(dCcD_Linked_Sph &ccD, const dCcD_SrcSph &src);
    void addCc(dCcD_Linked_Cps &ccD, const dCcD_SrcCps &src);
    void addCc(dCcD_Linked_Unk &ccD, const dCcD_SrcUnk &src);
    void SetStts(cCcD_Stts &stts);
    void registerColliders();

    typedef void (*ccCbFunc)(cCcD_Obj *obj, void *cbArg);
    void foreachCc(ccCbFunc f, void *cbArg) const;
    void foreachCc2(ccCbFunc f, void *cbArg) const;

    // This find function is used by a lot of other
    // TUs, all of which own the function passed as
    // a ptmf. It's not immediately clear how this works,
    // does every actor subclass the LinkedCollider___,
    // or are the functions all added here for the other
    // files to implement?
    typedef bool (cCcD_Obj::*ccPtmf)();
    cCcD_Obj *find(ccPtmf f) const;

    void AtSet();
    void ClrAt();
    void TgSet();
    void ClrTg();
    void CoSet();
    void ClrCo();
};

#endif
