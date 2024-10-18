#ifndef D_CS_GAME_H
#define D_CS_GAME_H

#include "d/d_cs.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_structd.h"
#include "m/m2d.h"
#include "s/s_State.hpp"
#include "s/s_StateID.hpp"
#include "toBeSorted/effects_struct.h"

class EffectRelatedTmp {
public:
    EffectRelatedTmp();
    virtual ~EffectRelatedTmp();
};

class EffectRelated : public EffectRelatedTmp {
public:
    EffectRelated(u32 x = 0x28, f32 y = 3.5f);
    virtual ~EffectRelated() {}
};

class dCsGameLyt1_c {
public:
    dCsGameLyt1_c() : field_0x0C(nullptr) {}
    virtual ~dCsGameLyt1_c() {}

private:
    /* 0x04 */ u8 field_0x04[0x0C - 0x04];
    /* 0x0C */ void *field_0x0C;
    /* 0x10 */ d2d::LytBase_c mLyt;
};

class dCsGameLyt2_c {
public:
    dCsGameLyt2_c() : field_0x0C(nullptr) {}
    virtual ~dCsGameLyt2_c() {}

private:
    /* 0x04 */ u8 field_0x04[0x0C - 0x04];
    /* 0x0C */ void *field_0x0C;
    /* 0x10 */ d2d::LytBase_c mLyt;
};

class Tmp {
public:
    d2d::AnmGroup_c mAnmGroups[0x17];
};

class dCsGameAnmGroups_c {
public:
    dCsGameAnmGroups_c() {}
    virtual ~dCsGameAnmGroups_c() {}

    Tmp tmp;
};

class dCsGame_c : public dCs_c {
public:
    dCsGame_c();
    virtual ~dCsGame_c();

    static dCsGame_c *sInstance;

    class lytItemCursor_c {
    public:
        class lytDowsingCsr_c {
        public:
            lytDowsingCsr_c(d2d::AnmGroup_c *groups, s32 num)
                : mpAnmGroups(groups), mNumAnmGroups(num), mStateMgr(*this, sStateID::null) {}
            virtual ~lytDowsingCsr_c() {}

        private:
            STATE_FUNC_DECLARE(lytDowsingCsr_c, NotFind);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, ToFind);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, Find);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, FindMinimum);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, ToNotFind);

            UI_STATE_MGR_DECLARE(lytDowsingCsr_c);
            d2d::AnmGroup_c *mpAnmGroups;
            s32 mNumAnmGroups;
            EffectsStruct mEffects1;
            EffectsStruct mEffects2;
        };

        class lytBowCsr_c {
        public:
            lytBowCsr_c(d2d::AnmGroup_c *groups, s32 num)
                : mpAnmGroups(groups), mNumAnmGroups(num), mStateMgr(*this, sStateID::null) {}
            virtual ~lytBowCsr_c() {}

        private:
            STATE_FUNC_DECLARE(lytBowCsr_c, Invisible);
            STATE_FUNC_DECLARE(lytBowCsr_c, Select);
            STATE_FUNC_DECLARE(lytBowCsr_c, Ready);
            STATE_FUNC_DECLARE(lytBowCsr_c, ToAimStart);
            STATE_FUNC_DECLARE(lytBowCsr_c, AimStart);
            STATE_FUNC_DECLARE(lytBowCsr_c, Draw);
            STATE_FUNC_DECLARE(lytBowCsr_c, Charge);

            UI_STATE_MGR_DECLARE(lytBowCsr_c);
            d2d::AnmGroup_c *mpAnmGroups;
            s32 mNumAnmGroups;
        };

        class lytPachinkoCsr_c {
        public:
            lytPachinkoCsr_c(d2d::AnmGroup_c *groups, s32 num)
                : mpAnmGroups(groups), mNumAnmGroups(num), mStateMgr(*this, sStateID::null) {}
            virtual ~lytPachinkoCsr_c() {}

        private:
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Invisible);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Select);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, ToDraw);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Draw);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Out);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Charge);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, ChargeFull);

            UI_STATE_MGR_DECLARE(lytPachinkoCsr_c);
            d2d::AnmGroup_c *mpAnmGroups;
            s32 mNumAnmGroups;
        };

        class lytCrawShotCsr_c {
        public:
            lytCrawShotCsr_c(d2d::AnmGroup_c *groups, s32 num)
                : mpAnmGroups(groups), mNumAnmGroups(num), mStateMgr(*this, sStateID::null) {}
            virtual ~lytCrawShotCsr_c() {}

        private:
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, Normal);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, ToLock);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, Lock);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, ToNormal);

            UI_STATE_MGR_DECLARE(lytCrawShotCsr_c);
            d2d::AnmGroup_c *mpAnmGroups;
            s32 mNumAnmGroups;
        };

        class lytVacuumCsr_c {
        public:
            lytVacuumCsr_c(d2d::AnmGroup_c *groups, s32 num)
                : mpAnmGroups(groups), mNumAnmGroups(num), mStateMgr(*this, sStateID::null) {}
            virtual ~lytVacuumCsr_c() {}

        private:
            STATE_FUNC_DECLARE(lytVacuumCsr_c, Normal);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, ToLock);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, Lock);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, ToNormal);

            UI_STATE_MGR_DECLARE(lytVacuumCsr_c);
            d2d::AnmGroup_c *mpAnmGroups;
            s32 mNumAnmGroups;
        };

    public:
        lytItemCursor_c()
            : mStateMgr(*this, sStateID::null), mBow(mAnmGroups.tmp.mAnmGroups, 0x17),
              mDowsing(mAnmGroups.tmp.mAnmGroups, 0x17), mPachinko(mAnmGroups.tmp.mAnmGroups, 0x17),
              mCrawShot(mAnmGroups.tmp.mAnmGroups, 0x17), mVacuum(mAnmGroups.tmp.mAnmGroups, 0x17) {}
        virtual ~lytItemCursor_c() {}

    private:
        STATE_FUNC_DECLARE(lytItemCursor_c, Invisible);
        STATE_FUNC_DECLARE(lytItemCursor_c, Bow);
        STATE_FUNC_DECLARE(lytItemCursor_c, Dowsing);
        STATE_FUNC_DECLARE(lytItemCursor_c, Vacuum);
        STATE_FUNC_DECLARE(lytItemCursor_c, Pachinko);
        STATE_FUNC_DECLARE(lytItemCursor_c, HookShot);
        STATE_FUNC_DECLARE(lytItemCursor_c, PlayerCam);

        /* 0x004 */ f32 field_0x004;
        /* 0x008 */ f32 field_0x008;
        /* 0x00C */ UI_STATE_MGR_DECLARE(lytItemCursor_c);

        EffectsStruct mEffects;
        EffectRelated mEffectRelated;
        d2d::LytBase_c mLyt;
        dCsGameAnmGroups_c mAnmGroups;

        /* 0x72C */ lytBowCsr_c mBow;
        /* 0x794 */ lytDowsingCsr_c mDowsing;
        /* 0x86C */ lytPachinkoCsr_c mPachinko;
        /* 0x8C0 */ lytCrawShotCsr_c mCrawShot;
        /* 0x934 */ lytVacuumCsr_c mVacuum;
    };

    /* 0x068 */ m2d::ResAccIf_c mCursorResAcc;
    /* 0x11C */ m2d::ResAccIf_c mMain2DResAcc;
    /* 0x1D0 */ d2d::dLytStructCSub mStructC;
    /* 0x1EC */ s32 mCursorType;
    /* 0x1F0 */ dCsGameLyt1_c mLyt1;
    /* 0x290 */ dCsGameLyt2_c mLyt2;
    /* 0x330 */ lytItemCursor_c mCursor;
};

#endif
