#pragma once

// This file was ported from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_list_mg_ptmf.hpp

#include <types.h>

#include <f/f_list_mg.h>
#include <f/f_list_nd_prio.h>

class fBase_c;

/// @brief A list of fLiNdPrio_c nodes with a reference to a process function.
/// @note Unofficial name.
class fLiMgPTMF_c : public fLiMgBa_c {
public:
    /// @brief Constructs a new list node.
    /// @param procFunc The process function.
    fLiMgPTMF_c(int (fBase_c::*procFunc)()) : mpProcFunc(procFunc) {}

    /**
     * @brief Inserts a node in the correct place according to its priority.
     *
     * @param node The node to insert.
     * @return If the operation was successful.
     */
    bool addNode(fLiNdPrio_c *node);

    /// @brief Calls the process function ::mpProcFunc on the owner of each node in the list.
    /// @return If the operation was successful.
    bool walkPack();

    fLiNdPrio_c *getFirst() const {
        return (fLiNdPrio_c *) fLiMgBa_c::getFirst();
    }

    fLiNdPrio_c *getLast() const {
        return (fLiNdPrio_c *) fLiMgBa_c::getLast();
    }

private:
    int (fBase_c::*mpProcFunc)(); ///< The process function for the list.
};