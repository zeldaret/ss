#ifndef D_T_TIME_AREA_CHECK_H
#define D_T_TIME_AREA_CHECK_H

#include <d/tg/d_tg.h>

class dTgTimeAreaCheck_c : public dTg_c {
public:
    dTgTimeAreaCheck_c() {}
    virtual ~dTgTimeAreaCheck_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    u8 mPastSceneFlag;
    u8 mPresentSceneFlag;
};

#endif
