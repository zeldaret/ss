#pragma once

// This file was ported from
// https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_list_nd_prio.hpp

#include "f/f_list_nd.h"
#include "f/f_profile.h"
#include <common.h>

/// @brief A list node with priority fields for an order in a list.
/// @note Unofficial name.
class fLiNdPrio_c : public fLiNdBa_c {
public:
    /// @brief Constructs a new list node.
    /// @param owner The node's owner.
    fLiNdPrio_c(fBase_c *owner) : fLiNdBa_c(owner), m_order(0), m_new_order(0) {}

    fLiNdPrio_c *getPrev() const {
        return (fLiNdPrio_c *)fLiNdBa_c::getPrev();
    }

    fLiNdPrio_c *getNext() const {
        return (fLiNdPrio_c *)fLiNdBa_c::getNext();
    }

    u16 getOrder() const {
        return m_order;
    }

    u16 getNewOrder() const {
        return m_new_order;
    }

    bool isPriorityChange() {
        return m_new_order != m_order;
    }

    void updatePriority() {
        m_order = m_new_order;
    }

    void setOrder(u16 order) {
        m_order = order;
        m_new_order = order;
    }

    u16 m_order;     ///< The priority of this node.
    u16 m_new_order; ///< The priority the node should change to if it differs from ::mOrder.
};
