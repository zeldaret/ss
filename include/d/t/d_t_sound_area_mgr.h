#ifndef D_T_SOUND_AREA_MGR_H
#define D_T_SOUND_AREA_MGR_H

#include <d/t/d_tg.h>

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


    static dTgSndMg_c *sInstance;

    void setBgmFlag(int flag) {
        mBgmFlags |= 1 << flag;
    }

private:
    u32 mBgmFlags;
};

#endif
