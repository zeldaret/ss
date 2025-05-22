#ifndef D_A_PLAYER_BASE_H
#define D_A_PLAYER_BASE_H

#include "d/a/obj/d_a_obj_base.h"

/*
 * The inheritance chain goes something like:
 *     dAcObjBase_c          straightfoward
 *          v
 *     daPlBase_c            only has a dtor, vtable (80533128) pooled into d_a_player at the end
 *          v
 *     daPlayerActBase_c     has a few strong and many weak functions, vtable stripped
 *          v
 *     daPlayerModelBase_c   has the actual implementation of many functions, vtable at 8050da00, size 0x310
 *          v
 *     dAcPy_c               "ActorLink", vtable at 80532cb0
 */

// Vtable at 80533128, no differences to dAcObjBase_c?
// Name unknown, though this name exists in NSMBW
class daPlBase_c : public dAcObjBase_c {
public:
    virtual ~daPlBase_c() {}
};

#endif
