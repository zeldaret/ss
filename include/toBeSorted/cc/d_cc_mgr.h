#ifndef TOSORT_D_CC_MGR_H
#define TOSORT_D_CC_MGR_H

#include <toBeSorted/cc/d_cc_d.h>

class ColliderManager {
public:
    static ColliderManager *getColliderManager();

    void addCollider(dCcD_GObjInf *collider);
};

#endif
