#pragma once

// This file was ported from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/cLib/c_tree.hpp

#include <types.h>

/// @brief A tree node. See cTreeMg_c.
/// @details The tree is represented as a doubly-linked LCRS tree.
class cTreeNd_c {
public:
    /// @brief Constructs a new tree node.
    cTreeNd_c();

    /// @brief Gets the next node in preorder traversal order.
    cTreeNd_c *getTreeNext() const;

    /// @brief Gets the next node in preorder traversal order, excluding the node's children.
    cTreeNd_c *getTreeNextNotChild() const;

    cTreeNd_c *getParent() const { return mpParent; }
    cTreeNd_c *getChild() const { return mpChild; }
    cTreeNd_c *getBrPrev() const { return mpPrev; }
    cTreeNd_c *getBrNext() const { return mpNext; }

protected:
    /// @brief Clears all fields.
    void forcedClear();

    cTreeNd_c *mpParent; ///< The parent node.
    cTreeNd_c *mpChild; ///< The child node.
    cTreeNd_c *mpPrev; ///< The previous sibling node.
    cTreeNd_c *mpNext; ///< The next sibling node.

    friend class cTreeMg_c;
};

/// @brief A tree container. See cTreeNd_c.
class cTreeMg_c {
public:
    /// @brief Constructs a new tree container.
    cTreeMg_c() : mpRootNode(nullptr) {}

    /**
     * @brief Adds a node to the tree, either to the root node or to a specified parent node.
     *
     * @param node The node to add.
     * @param parent The parent node to attach it to, or @p nullptr to attach it to the tree root.
     * @return If the operation was successful.
     */
    bool addTreeNode(cTreeNd_c *node, cTreeNd_c *parent);

    /**
     * @brief Removes a node from the tree.
     *
     * @param node The node to remove.
     * @return If the operation was successful.
     */
    bool removeTreeNode(cTreeNd_c *node);

protected:
    cTreeNd_c *mpRootNode; ///< The root node of the tree.
};