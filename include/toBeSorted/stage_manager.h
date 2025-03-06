#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include "d/d_base.h"

class dStageMgr_c : dBase_c {
public:
    bool isAreaTypeDungeon() const;
    bool isAreaTypeSky() const;
    s32 getAreaType() const;

    static dStageMgr_c *GetInstance() {
        return sInstance;
    }

private:
    static dStageMgr_c *sInstance;
};

#endif
