#ifndef D_T_SOUND_AREA_H
#define D_T_SOUND_AREA_H

#include <d/tg/d_tg.h>
#include <m/m_vec.h>
#include <m/m_mtx.h>
#include <toBeSorted/actor_on_rail.h>

class dTgSndAr_c : public dTg_c {
public:
    dTgSndAr_c() {}
    virtual ~dTgSndAr_c() {}
    virtual int draw() override;
    virtual int actorExecute() override;
    virtual int doDelete() override;
    virtual int create() override;

private:
    int getTypeFromParams() {
        return params >> 0x1C;
    }
    bool checkPosInArea(mVec3_c &pos);

    bool checkAlg0(mVec3_c &pos);
    bool checkAlg1(mVec3_c &pos);
    bool checkAlg2(mVec3_c &pos);
    bool checkAlg3(mVec3_c &pos);

    mMtx_c mtx;
    ActorOnRail mRail;
};

#endif
