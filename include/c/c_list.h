#pragma once

// This file was ported from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/cLib/c_list.hpp

#include <types.h>

/// @brief A doubly-linked list node. See cListMg_c.
/// @note Unofficial name.
class cListNd_c {
public:
    /// @brief Constructs a new list node.
    cListNd_c() : mpPrev(nullptr), mpNext(nullptr) {}

    cListNd_c *getPrev() const { return mpPrev; }
    cListNd_c *getNext() const { return mpNext; }

protected:
    cListNd_c *mpPrev; ///< The previous node.
    cListNd_c *mpNext; ///< The next node.

    friend class cListMg_c;
};

/// @brief A doubly-linked list container. See cListNd_c.
/// @note Unofficial name.
class cListMg_c {
public:
    /// @brief Constructs a new list container.
    cListMg_c() : mpFirst(nullptr), mpLast(nullptr) {}

    /**
     * @brief Inserts a node after the given previous node.
     *
     * @param node The node to insert.
     * @param prevNode The node to insert it after, or @p nullptr to insert it at the beginning.
     * @return If the operation was successful.
     */
    bool insertAfter(cListNd_c *node, cListNd_c *prevNode);

    /**
     * @brief Removes a node from the list.
     *
     * @param node The node to remove.
     * @return If the operation was successful.
     */
    bool remove(cListNd_c *node);

    /**
     * @brief Adds a node to the end of the list.
     *
     * @param node The node to append.
     * @return If the operation was successful.
     */
    bool append(cListNd_c *node);

    /**
     * @brief Adds a node to the beginning of the list.
     *
     * @param node The node to prepend.
     * @return If the operation was successful.
     */
    bool prepend(cListNd_c *node);

    cListNd_c *getFirst() const { return mpFirst; }
    cListNd_c *getLast() const { return mpLast; }

protected:
    cListNd_c *mpFirst; ///< The first node in the list.
    cListNd_c *mpLast; ///< The last node in the list.
};