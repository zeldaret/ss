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

class LinkedColliderUnk : public dCcD_Unk {
public:
    LinkedColliderUnk() : mNode(this) {}
    virtual ~LinkedColliderUnk() {
        mNode.remove();
    }
    /* 0x210 */ LinkedColliderNode mNode;
};

class LinkedColliderCyl : public dCcD_Cyl {
public:
    LinkedColliderCyl() : mNode(this) {}
    virtual ~LinkedColliderCyl() {
        mNode.remove();
    }
    /* 0x150 */ LinkedColliderNode mNode;
};

class LinkedColliderSph : public dCcD_Sph {
public:
    LinkedColliderSph() : mNode(this) {}
    virtual ~LinkedColliderSph() {
        mNode.remove();
    }
    /* 0x150 */ LinkedColliderNode mNode;
};

class LinkedColliderCps : public dCcD_Cps {
public:
    LinkedColliderCps() : mNode(this) {}
    virtual ~LinkedColliderCps() {
        mNode.remove();
    }
    /* 0x170 */ LinkedColliderNode mNode;
};

class ColliderLinkedList {
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
    virtual ~ColliderLinkedList();

    void postInit(cCcD_Obj &ccD);
    void addCc(LinkedColliderCyl &ccD, const dCcD_SrcCyl &src);
    void addCc(LinkedColliderSph &ccD, const dCcD_SrcSph &src);
    void addCc(LinkedColliderCps &ccD, const dCcD_SrcCps &src);
    void addCc(LinkedColliderUnk &ccD, const dCcD_SrcUnk &src);
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
