#include <c/c_tree.h>

/* 0x802E0E70 */
cTreeNd_c::cTreeNd_c() {
    this->forcedClear();
}

/* 0x802E0EA0 */
void cTreeNd_c::forcedClear() {
    this->mpParent = nullptr;
    this->mpChild = nullptr;
    this->mpPrev = nullptr;
    this->mpNext = nullptr;
}

/* 0x802E0EC0 */
bool cTreeMg_c::addTreeNode(cTreeNd_c *node, cTreeNd_c *parent) {
    if (node != nullptr) {
        if (parent != nullptr) {
            node->mpParent = parent;
            if (parent->mpChild == nullptr) {
                parent->mpChild = node;
            } else {
                cTreeNd_c *cursor;
                for (cursor = parent->mpChild; cursor->mpNext != nullptr; cursor = cursor->mpNext) {}
                cursor->mpNext = node;
                node->mpPrev = cursor;
            }
        } else {
            cTreeNd_c *cursor = this->mpRootNode;
            if (cursor != nullptr) {
                for (; cursor->mpNext != nullptr; cursor = cursor->mpNext) {}
                cursor->mpNext = node;
                node->mpPrev = cursor;
            } else {
                this->mpRootNode = node;
            }
        }
    } else {
        return false;
    }
    return true;
}

/* 0x802E0F60*/
bool cTreeMg_c::removeTreeNode(cTreeNd_c *node) {
    if (node != nullptr) {
        if (node->mpChild != nullptr) {
            return false;
        }
        if (node->mpPrev != nullptr) {
            node->mpPrev->mpNext = node->mpNext;
        } else if (node->mpParent != nullptr) {
            node->mpParent->mpChild = node->mpNext;
        } else if (node == this->mpRootNode) {
            this->mpRootNode = node->mpNext;
        }

        if (node->mpNext != nullptr) {
            node->mpNext->mpPrev = node->mpPrev;
        }

        node->mpPrev = nullptr;
        node->mpNext = nullptr;
        node->mpParent = nullptr;
    } else {
        return false;
    }
    return true;
}

/* 0x802E1000 */
bool cTreeMg_c::insertTreeNode(cTreeNd_c *node, cTreeNd_c *parent) {
    cTreeNd_c *cursor;

    for (cursor = parent; cursor != nullptr; cursor = cursor->mpParent) {
        if (cursor == node) {
            return false;
        }
    }

    if (node != nullptr) {
        cTreeNd_c *child = node->mpChild;
        node->mpChild = nullptr;
        if (!this->removeTreeNode(node)) {
            node->mpChild = child;
            return false;
        } else {
            node->mpChild = child;
            return this->addTreeNode(node, parent);
        }
    }

    return false;
}

/* 0x802E10C0 */
cTreeNd_c *cTreeNd_c::getTreeNext() const {
    cTreeNd_c *child = this->mpChild;
    if (child != nullptr) {
        return child;
    } else {
        return this->getTreeNextNotChild();
    }
}

/* 0x802E1100 */
cTreeNd_c *cTreeNd_c::getTreeNextNotChild() const {
    if (this->mpNext != nullptr) {
        return this->mpNext;
    }

    cTreeNd_c *parent;

    for (parent = this->mpParent; parent != nullptr; parent = parent->mpParent) {
        if (parent->mpNext != nullptr) {
            return parent->mpNext;
        }
    }
    return nullptr;
}
