#ifndef D_T_SOUND_AREA_MGR_H
#define D_T_SOUND_AREA_MGR_H

#include "d/t/d_tg.h"

class dTgSndMg_c : public dTg_c {
public:
    dTgSndMg_c() {
        sInstance = this;
    }
    virtual ~dTgSndMg_c() {
        sInstance = nullptr;
    }
    virtual int draw() override;
    virtual int actorExecute() override;
    virtual int doDelete() override;
    virtual int create() override;

    void setBgmFlag(int flag) {
        mBgmFlags |= 1 << flag;
    }

    static dTgSndMg_c *GetInstance() {
        return sInstance;
    }

private:
    static dTgSndMg_c *sInstance;
    u32 mBgmFlags;
};

#endif
