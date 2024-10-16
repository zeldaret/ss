#include "c/c_list.h"

/* 0x802E08C0 */
void cListMg_c::insertAfter(cListNd_c *node, cListNd_c *prevNode) {
    if (prevNode == nullptr) {
        return this->prepend(node);
    }
    if (node == nullptr) {
        return;
    }
    node->mpNext = prevNode->mpNext;
    node->mpPrev = prevNode;
    prevNode->mpNext = node;
    if (node->mpNext != nullptr) {
        node->mpNext->mpPrev = node;
    } else {
        this->mpLast = node;
    }
}

/* 0x802E0900 */
void cListMg_c::remove(cListNd_c *node) {
    if (node == nullptr) {
        return;
    }

    if (node->mpPrev == nullptr && node->mpNext == nullptr) {
        if (this->mpFirst == node && this->mpLast == node) {
            this->mpFirst = nullptr;
            this->mpLast = nullptr;
        }
    } else {
        if (node->mpPrev != nullptr) {
            node->mpPrev->mpNext = node->mpNext;
        } else if (node == this->mpFirst) {
            this->mpFirst = node->mpNext;
        }

        if (node->mpNext != nullptr) {
            node->mpNext->mpPrev = node->mpPrev;
        } else if (node == this->mpLast) {
            this->mpLast = node->mpPrev;
        }

        node->mpPrev = nullptr;
        node->mpNext = nullptr;
    }
}

/* 0x802E09B0 */
void cListMg_c::append(cListNd_c *node) {
    if (node == nullptr) {
        return;
    }
    if (this->mpLast != nullptr) {
        this->mpLast->mpNext = node;
        node->mpPrev = this->mpLast;
    } else {
        this->mpFirst = node;
    }
    this->mpLast = node;
}

/* 0x802E09E0 */
void cListMg_c::prepend(cListNd_c *node) {
    if (node == nullptr) {
        return;
    }
    if (this->mpFirst != nullptr) {
        this->mpFirst->mpPrev = node;
        node->mpNext = this->mpFirst;
    } else {
        this->mpLast = node;
    }
    this->mpFirst = node;
}
