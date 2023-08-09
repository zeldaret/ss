#pragma once

// This file was ported from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_tree_mg_ptmf.hpp

#include <types.h>
#include <c/c_tree.h>
#include <f/f_tree_mg.h>

class fBase_c;

/// @brief A container for a tree of fTrNdBa_c nodes with a reference to a process function.
/// @note Unofficial name.
class fTrMgPTMF_c : public fTrMgBa_c {
public:
    /// @brief Constructs a new tree container.
    /// @param procFunc The process function.
    fTrMgPTMF_c(int (fBase_c::*procFunc)()) : mpProcFunc(procFunc) {}

    /// @brief Calls the process function ::mpProcFunc on the owner of each node in the tree.
    /// @return If the operation was successful.
    bool walkPack();

private:
    int (fBase_c::*mpProcFunc)(); ///< The process function for the tree.
};