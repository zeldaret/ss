#include "d/d_cc.h"

#include "common.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"


void ColliderLinkedList::postInit(cCcD_Obj &ccD) {
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

void ColliderLinkedList::addCc(LinkedColliderCyl &ccD, const dCcD_SrcCyl &src) {
    ccD.Set(src);
    mList.append(&ccD.mNode);
    ccD.mNode.mpList = &mList;
    postInit(ccD);
}

void ColliderLinkedList::addCc(LinkedColliderSph &ccD, const dCcD_SrcSph &src) {
    ccD.Set(src);
    mList.append(&ccD.mNode);
    ccD.mNode.mpList = &mList;
    postInit(ccD);
}

void ColliderLinkedList::addCc(LinkedColliderCps &ccD, const dCcD_SrcCps &src) {
    ccD.Set(src);
    mList.append(&ccD.mNode);
    ccD.mNode.mpList = &mList;
    postInit(ccD);
}

void ColliderLinkedList::addCc(LinkedColliderUnk &ccD, const dCcD_SrcUnk &src) {
    ccD.Set(src);
    mList.append(&ccD.mNode);
    ccD.mNode.mpList = &mList;
    postInit(ccD);
}

void ColliderLinkedList::foreachCc(ccCbFunc f, void *cbArg) const {
    LinkedColliderNode *nd = static_cast<LinkedColliderNode *>(mList.getFirst());
    while (nd != nullptr) {
        f(nd->mpCcDObj, cbArg);
        nd = static_cast<LinkedColliderNode *>(nd->getNext());
    }
}

// What's the difference between these two?
void ColliderLinkedList::foreachCc2(ccCbFunc f, void *cbArg) const {
    LinkedColliderNode *nd = static_cast<LinkedColliderNode *>(mList.getFirst());
    while (nd != nullptr) {
        f(nd->mpCcDObj, cbArg);
        nd = static_cast<LinkedColliderNode *>(nd->getNext());
    }
}

void ColliderLinkedList::Set(cCcD_Obj *obj, void *cbArg) {
    dCcS::GetInstance()->Set(obj);
}

void ColliderLinkedList::registerColliders() {
    foreachCc(Set, nullptr);
}

void ColliderLinkedList::SetStts(cCcD_Obj *obj, void *cbArg) {
    obj->SetStts(*static_cast<cCcD_Stts *>(cbArg));
}

void ColliderLinkedList::SetStts(cCcD_Stts &stts) {
    foreachCc(SetStts, &stts);
}

cCcD_Obj *ColliderLinkedList::find(ccPtmf f) const {
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

void ColliderLinkedList::AtSet(cCcD_Obj *obj, void *cbArg) {
    if (!obj->ChkAt_0x40000000()) {
        return;
    }
    obj->OnAtSet();
}

void ColliderLinkedList::AtSet() {
    foreachCc(AtSet, nullptr);
}

void ColliderLinkedList::ClrAt(cCcD_Obj *obj, void *cbArg) {
    obj->ClrAtSet();
    obj->ClrAtHit();
}

void ColliderLinkedList::ClrAt() {
    foreachCc(ClrAt, nullptr);
}

void ColliderLinkedList::TgSet(cCcD_Obj *obj, void *cbArg) {
    if (!obj->ChkTg_0x40000000()) {
        return;
    }
    obj->OnTgSet();
}

void ColliderLinkedList::TgSet() {
    foreachCc(TgSet, nullptr);
}

void ColliderLinkedList::ClrTg(cCcD_Obj *obj, void *cbArg) {
    obj->ClrTgSet();
    obj->ClrTgHit();
}

void ColliderLinkedList::ClrTg() {
    foreachCc(ClrTg, nullptr);
}

void ColliderLinkedList::CoSet(cCcD_Obj *obj, void *cbArg) {
    if (!obj->ChkCo_0x40000000()) {
        return;
    }
    obj->OnCoSet();
}

void ColliderLinkedList::CoSet() {
    foreachCc(CoSet, nullptr);
}

void ColliderLinkedList::ClrCo(cCcD_Obj *obj, void *cbArg) {
    obj->ClrCoSet();
    obj->ClrCoHit();
}

void ColliderLinkedList::ClrCo() {
    foreachCc(ClrCo, nullptr);
}

ColliderLinkedList::~ColliderLinkedList() {
    mList.clear();
}
