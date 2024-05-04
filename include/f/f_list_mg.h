#ifndef F_LIST_MANAGER_H
#define F_LIST_MANAGER_H

// This file was ported from
// https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_list_mg.hpp

#include "c/c_list.h"
#include "f/f_base_id.h"
#include "f/f_list_nd.h"
#include "f/f_profile.h"
#include <common.h>

class fBase_c;

/// @brief A list of fLiNdBa_c nodes.
class fLiMgBa_c : public cListMg_c {
public:
    ~fLiMgBa_c() {}
    /**
     * @brief Counts the number of nodes of a given profile in this list.
     *
     * @param profName The profile name.
     * @return How many nodes were found.
     */
    int countNodeByProfName(ProfileName profName) const;

    /* 802e2760 */ const fLiNdBa_c *searchNodeByID(fBaseID_e id) const;
    /* 802e2790 */ const fLiNdBa_c *searchNodeByProfName(ProfileName name, fLiNdBa_c *start) const;

    inline fLiNdBa_c *getFirst() const {
        return (fLiNdBa_c *)cListMg_c::getFirst();
    }

    inline fLiNdBa_c *getLast() const {
        return (fLiNdBa_c *)cListMg_c::getLast();
    }
};

#endif
