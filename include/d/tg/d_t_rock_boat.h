#ifndef D_T_ROCK_BOAT
#define D_T_ROCK_BOAT

#include <d/a/d_a_base.h>
#include <f/f_list_nd.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgAbs_c : public dAcBase_c {
public:
    virtual ~dTgAbs_c() {}
};

class dTgRockBoat_c : public dTgAbs_c {
public:
    dTgRockBoat_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgRockBoat_c() {}
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dTgRockBoat_c, Wait);

private:
    STATE_MGR_DECLARE(dTgRockBoat_c);
    int cooldown;
    int boatNum;
};

#endif
