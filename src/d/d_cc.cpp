#include "d/d_cc.h"

#include "common.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"


void dColliderLinkedList::postInit(cCcD_Obj &ccD) {
    if (ccD.ChkAtSet()) {
        ccD.SetAt_0x40000000();
    }
    if (ccD.ChkTgSet()) {
        ccD.SetTg_0x40000000();
    }
    if (ccD.ChkCoSet()) {
        ccD.SetCo_0x40000000();
    }
}

void dColliderLinkedList::addCc(dCcD_Linked_Cyl &ccD, const dCcD_SrcCyl &src) {
    ccD.Set(src);
    mList.append(&ccD.mNode);
    ccD.mNode.mpList = &mList;
    postInit(ccD);
}

void dColliderLinkedList::addCc(dCcD_Linked_Sph &ccD, const dCcD_SrcSph &src) {
    ccD.Set(src);
    mList.append(&ccD.mNode);
    ccD.mNode.mpList = &mList;
    postInit(ccD);
}

void dColliderLinkedList::addCc(dCcD_Linked_Cps &ccD, const dCcD_SrcCps &src) {
    ccD.Set(src);
    mList.append(&ccD.mNode);
    ccD.mNode.mpList = &mList;
    postInit(ccD);
}

void dColliderLinkedList::addCc(dCcD_Linked_Unk &ccD, const dCcD_SrcUnk &src) {
    ccD.Set(src);
    mList.append(&ccD.mNode);
    ccD.mNode.mpList = &mList;
    postInit(ccD);
}

void dColliderLinkedList::foreachCc(ccCbFunc f, void *cbArg) const {
    LinkedColliderNode *nd = static_cast<LinkedColliderNode *>(mList.getFirst());
    while (nd != nullptr) {
        f(nd->mpCcDObj, cbArg);
        nd = static_cast<LinkedColliderNode *>(nd->getNext());
    }
}

// What's the difference between these two?
void dColliderLinkedList::foreachCc2(ccCbFunc f, void *cbArg) const {
    LinkedColliderNode *nd = static_cast<LinkedColliderNode *>(mList.getFirst());
    while (nd != nullptr) {
        f(nd->mpCcDObj, cbArg);
        nd = static_cast<LinkedColliderNode *>(nd->getNext());
    }
}

void dColliderLinkedList::Set(cCcD_Obj *obj, void *cbArg) {
    dCcS::GetInstance()->Set(obj);
}

void dColliderLinkedList::registerColliders() {
    foreachCc(Set, nullptr);
}

void dColliderLinkedList::SetStts(cCcD_Obj *obj, void *cbArg) {
    obj->SetStts(*static_cast<cCcD_Stts *>(cbArg));
}

void dColliderLinkedList::SetStts(cCcD_Stts &stts) {
    foreachCc(SetStts, &stts);
}

cCcD_Obj *dColliderLinkedList::find(ccPtmf f) const {
    cCcD_Obj *o;
    LinkedColliderNode *nd = static_cast<LinkedColliderNode *>(mList.getFirst());
    while (nd != nullptr) {
        o = nd->mpCcDObj;
        if ((o->*f)()) {
            return o;
        }
        nd = static_cast<LinkedColliderNode *>(nd->getNext());
    }
    return nullptr;
}

void dColliderLinkedList::AtSet(cCcD_Obj *obj, void *cbArg) {
    if (!obj->ChkAt_0x40000000()) {
        return;
    }
    obj->OnAtSet();
}

void dColliderLinkedList::AtSet() {
    foreachCc(AtSet, nullptr);
}

void dColliderLinkedList::ClrAt(cCcD_Obj *obj, void *cbArg) {
    obj->ClrAtSet();
    obj->ClrAtHit();
}

void dColliderLinkedList::ClrAt() {
    foreachCc(ClrAt, nullptr);
}

void dColliderLinkedList::TgSet(cCcD_Obj *obj, void *cbArg) {
    if (!obj->ChkTg_0x40000000()) {
        return;
    }
    obj->OnTgSet();
}

void dColliderLinkedList::TgSet() {
    foreachCc(TgSet, nullptr);
}

void dColliderLinkedList::ClrTg(cCcD_Obj *obj, void *cbArg) {
    obj->ClrTgSet();
    obj->ClrTgHit();
}

void dColliderLinkedList::ClrTg() {
    foreachCc(ClrTg, nullptr);
}

void dColliderLinkedList::CoSet(cCcD_Obj *obj, void *cbArg) {
    if (!obj->ChkCo_0x40000000()) {
        return;
    }
    obj->OnCoSet();
}

void dColliderLinkedList::CoSet() {
    foreachCc(CoSet, nullptr);
}

void dColliderLinkedList::ClrCo(cCcD_Obj *obj, void *cbArg) {
    obj->ClrCoSet();
    obj->ClrCoHit();
}

void dColliderLinkedList::ClrCo() {
    foreachCc(ClrCo, nullptr);
}

dColliderLinkedList::~dColliderLinkedList() {
    mList.clear();
}
