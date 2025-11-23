#ifndef D_T_BARREL_POS_H
#define D_T_BARREL_POS_H

#include "d/t/d_tg.h"
#include "m/m_vec.h"

class dAcOstageSink_c;

class dTgBarrelPos_c : public dTg_c {
public:
    dTgBarrelPos_c() {}
    virtual ~dTgBarrelPos_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

    u8 getLinkIndex() const {
        return mLinkIndex;
    }
    u8 getLinkId() const {
        return mLinkId;
    }

private:
    /* 0x0FC */ dAcRef_c<dAcOstageSink_c> mStageRef;
    /* 0x108 */ mVec3_c mSpawnPosition;
    /* 0x114 */ u8 mLinkIndex; ///< Link Index used in dTgBarrel_c array
    /* 0x115 */ u8 mLinkId;    ///< Link Id used to find correct dTgBarrel_c
};

#endif
