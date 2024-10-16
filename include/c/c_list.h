#ifndef C_LIST_H
#define C_LIST_H

// This file was ported from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/cLib/c_list.hpp

#include "common.h"

/// @brief A doubly-linked list node. See cListMg_c.
/// @note Unofficial name.
class cListNd_c {
public:
    /// @brief Constructs a new list node.
    cListNd_c() : mpPrev(nullptr), mpNext(nullptr) {}

    cListNd_c *getPrev() const {
        return mpPrev;
    }
    cListNd_c *getNext() const {
        return mpNext;
    }

protected:
    cListNd_c *mpPrev; ///< The previous node.
    cListNd_c *mpNext; ///< The next node.

    friend class cListMg_c;
};

class cListMg_c {
public:
    /* 802e2be0 */ cListMg_c() : mpFirst(nullptr), mpLast(nullptr) {}
    // /* 802e2880 */ ~cListMg_c();

    void insertAfter(cListNd_c *node, cListNd_c *prevNode);

    /**
     * @brief Removes a node from the list.
     *
     * @param node The node to remove.
     * @return If the operation was successful.
     */
    void remove(cListNd_c *node);

    /**
     * @brief Adds a node to the end of the list.
     *
     * @param node The node to append.
     * @return If the operation was successful.
     */
    void append(cListNd_c *node);

    /**
     * @brief Adds a node to the beginning of the list.
     *
     * @param node The node to prepend.
     * @return If the operation was successful.
     */
    void prepend(cListNd_c *node);

    cListNd_c *getFirst() const {
        return mpFirst;
    }
    cListNd_c *getLast() const {
        return mpLast;
    }

protected:
    cListNd_c *mpFirst; ///< The first node in the list.
    cListNd_c *mpLast;  ///< The last node in the list.
};

#endif
