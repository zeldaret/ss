#include <f/f_base.h>
#include <f/f_list_mg.h>
#include <f/f_list_mg_ptmf.h>

/* 802e2680 */
void fLiMgPTMF_c::addNode(fLiNdPrio_c *add) {
    fLiNdPrio_c *node = getFirst();

    if (add == nullptr) {
        return;
    }

    if (node == nullptr) {
        append(add);
        return;
    }

    if (node->getOrder() > add->getOrder()) {
        insertAfter(add, nullptr);
        return;
    }

    while (node->getNext() != nullptr && node->getNext()->getOrder() <= add->getOrder()) {
        node = node->getNext();
    }
    insertAfter(add, node);
}

/* 802e26e0 */
bool fLiMgPTMF_c::walkPack() {
    if (mpProcFunc == nullptr) {
        return true;
    }

    fLiNdBa_c *node = getFirst();
    while (node != nullptr) {
        fLiNdBa_c *next_node = node->getNext();
        (node->p_owner->*mpProcFunc)();
        node = next_node;
    }

    return true;
}

/* 802e2760 */
const fLiNdBa_c *fLiMgBa_c::searchNodeByID(fBaseID_e id) const {
    fLiNdBa_c *node = getFirst();
    while (node != nullptr) {
        if (node->p_owner->unique_ID == id) {
            return node;
        }
        node = node->getNext();
    }
    return nullptr;
}

/* 802e2790 */
const fLiNdBa_c *fLiMgBa_c::searchNodeByProfName(ProfileName name, fLiNdBa_c *start) const {
    fLiNdBa_c *node = start != nullptr ? start->getNext() : getFirst();
    while (node != nullptr) {
        if (node->p_owner->profile_name == name) {
            return node;
        }
        node = node->getNext();
    }
    return nullptr;
}

/* 802e27d0 */
void fLiNdBa_c::link(fBase_c *link) {
    if (p_owner != nullptr) {
        unlink();
    }

    if (link != nullptr) {
        p_owner = link;
        link->actor_list.append(this);
    }
}

/* 802e2830 */
void fLiNdBa_c::unlink() {
    if (p_owner != nullptr) {
        reinterpret_cast<fBase_c *>(p_owner)->actor_list.remove(this);
        p_owner = nullptr;
    }
}
