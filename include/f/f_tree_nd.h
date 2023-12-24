#pragma once

// This file was ported from
// https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_tree_nd.hpp

#include "c/c_tree.h"
#include "f/f_profile.h"
#include <common.h>


class fBase_c;

class fTrNdBa_c : public cTreeNd_c {
public:
    fTrNdBa_c(fBase_c *owner) : p_owner(owner) {}

    fTrNdBa_c *getTreeNext() const {
        return (fTrNdBa_c *)cTreeNd_c::getTreeNext();
    }

    fTrNdBa_c *getTreeNextNotChild() const {
        return (fTrNdBa_c *)cTreeNd_c::getTreeNextNotChild();
    }

    fTrNdBa_c *getParent() const {
        return (fTrNdBa_c *)cTreeNd_c::getParent();
    }

    fTrNdBa_c *getChild() const {
        return (fTrNdBa_c *)cTreeNd_c::getChild();
    }

    fTrNdBa_c *getBrPrev() const {
        return (fTrNdBa_c *)cTreeNd_c::getBrPrev();
    }

    fTrNdBa_c *getBrNext() const {
        return (fTrNdBa_c *)cTreeNd_c::getBrNext();
    }

    fBase_c *p_owner;
};
