#pragma once

// This file was ported from
// https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_list_nd.hpp

#include "c/c_list.h"
#include <common.h>


class fBase_c;

class fLiNdBa_c : public cListNd_c {
public:
    fLiNdBa_c() : p_owner(nullptr) {}
    fLiNdBa_c(fBase_c *owner) : p_owner(owner) {}

    inline fLiNdBa_c *getPrev() const {
        return (fLiNdBa_c *)cListNd_c::getPrev();
    }

    inline fLiNdBa_c *getNext() const {
        return (fLiNdBa_c *)cListNd_c::getNext();
    }

    fBase_c *p_owner;
};
