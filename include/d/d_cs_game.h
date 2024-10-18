#ifndef D_CS_GAME_H
#define D_CS_GAME_H

#include "d/d_cs.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_structd.h"
#include "m/m2d.h"
#include "s/s_State.hpp"
#include "s/s_StateID.hpp"
#include "toBeSorted/effects_struct.h"

class UnkMaybeEmpty {
public:
    virtual ~UnkMaybeEmpty() {}
};

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

class dCsGameLytBase_c {
public:
    dCsGameLytBase_c() {}
    virtual ~dCsGameLytBase_c() {}
    virtual void dCsGameLytBase_0x0C() {} // not overridden
    virtual void dCsGameLytBase_0x10() {} // overridden by both
    virtual void dCsGameLytBase_0x14() = 0;
    virtual void dCsGameLytBase_0x18() {} // not overridden
    virtual void dCsGameLytBase_0x1C() {} // not overridden
    virtual void dCsGameLytBase_0x20() = 0;
    virtual void dCsGameLytBase_0x24() = 0;
    virtual void dCsGameLytBase_0x28() = 0;

protected:
    /* 0x04 */ u8 field_0x04[0x0C - 0x04];
};

class dCsGameLyt1_c : public dCsGameLytBase_c {
public:
    dCsGameLyt1_c() : mpResAcc(nullptr) {}
    virtual ~dCsGameLyt1_c() {}
    virtual void dCsGameLytBase_0x10() override;
    virtual void dCsGameLytBase_0x14() override;
    virtual void dCsGameLytBase_0x20() override;
    virtual void dCsGameLytBase_0x24() override;
    virtual void dCsGameLytBase_0x28() override;

    void setResAcc(m2d::ResAccIf_c *resAcc) {
        mpResAcc = resAcc;
    }

private:
    /* 0x0C */ m2d::ResAccIf_c *mpResAcc;
    /* 0x10 */ d2d::LytBase_c mLyt;
};

class dCsGameLyt2_c : public dCsGameLytBase_c {
public:
    dCsGameLyt2_c() : mpResAcc(nullptr) {}
    virtual ~dCsGameLyt2_c() {}
    virtual void dCsGameLytBase_0x10() override;
    virtual void dCsGameLytBase_0x14() override;
    virtual void dCsGameLytBase_0x20() override;
    virtual void dCsGameLytBase_0x24() override;
    virtual void dCsGameLytBase_0x28() override;

    void setResAcc(m2d::ResAccIf_c *resAcc) {
        mpResAcc = resAcc;
    }

private:
    /* 0x0C */ m2d::ResAccIf_c *mpResAcc;
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

class dCsGameBorrowedAnmGroups_c {
public:
    dCsGameBorrowedAnmGroups_c(dCsGameAnmGroups_c &g) : mpAnmGroups(g.tmp.mAnmGroups), mNumAnmGroups(0x17) {}
    virtual ~dCsGameBorrowedAnmGroups_c() {}
    d2d::AnmGroup_c *mpAnmGroups;
    s32 mNumAnmGroups;
};

class dCsGame_c : public dCs_c {
public:
    dCsGame_c();
    virtual ~dCsGame_c();

    virtual int create() override;
    virtual int doDelete() override;

    void setSomething(int);

    static dCsGame_c *sInstance;

    class lytItemCursor_c {
        friend class dCsGame_c;

    public:
        class lytBowCsr_c {
        public:
            lytBowCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g), mpLyt(lyt), mStateMgr(*this, sStateID::null) {}
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
            dCsGameBorrowedAnmGroups_c mAnm;
            d2d::LytBase_c *mpLyt;
        };

        class lytDowsingCsr_c {
        public:
            lytDowsingCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g), mpLyt(lyt), mStateMgr(*this, sStateID::null) {}
            virtual ~lytDowsingCsr_c() {}

        private:
            STATE_FUNC_DECLARE(lytDowsingCsr_c, NotFind);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, ToFind);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, Find);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, FindMinimum);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, ToNotFind);

            UI_STATE_MGR_DECLARE(lytDowsingCsr_c);
            dCsGameBorrowedAnmGroups_c mAnm;
            d2d::LytBase_c *mpLyt;
            EffectsStruct mEffects1;
            EffectsStruct mEffects2;
        };

        class lytPachinkoCsr_c {
        public:
            lytPachinkoCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g), mpLyt(lyt), mStateMgr(*this, sStateID::null) {}
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
            dCsGameBorrowedAnmGroups_c mAnm;
            d2d::LytBase_c *mpLyt;
        };

        class lytCrawShotCsr_c {
        public:
            lytCrawShotCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g), mpLyt(lyt), mStateMgr(*this, sStateID::null) {}
            virtual ~lytCrawShotCsr_c() {}

        private:
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, Normal);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, ToLock);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, Lock);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, ToNormal);

            UI_STATE_MGR_DECLARE(lytCrawShotCsr_c);
            dCsGameBorrowedAnmGroups_c mAnm;
            d2d::LytBase_c *mpLyt;
        };

        class lytVacuumCsr_c {
        public:
            lytVacuumCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g), mpLyt(lyt), mStateMgr(*this, sStateID::null) {}
            virtual ~lytVacuumCsr_c() {}

        private:
            STATE_FUNC_DECLARE(lytVacuumCsr_c, Normal);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, ToLock);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, Lock);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, ToNormal);

            UI_STATE_MGR_DECLARE(lytVacuumCsr_c);
            dCsGameBorrowedAnmGroups_c mAnm;
            d2d::LytBase_c *mpLyt;
        };

    public:
        lytItemCursor_c()
            : mStateMgr(*this, sStateID::null), mAnm(mAnmGroups), mBow(mAnmGroups, &mLyt), mDowsing(mAnmGroups, &mLyt),
              mPachinko(mAnmGroups, &mLyt), mCrawShot(mAnmGroups, &mLyt), mVacuum(mAnmGroups, &mLyt) {}
        virtual ~lytItemCursor_c() {}
        virtual void lytItemCursor0x0C();
        virtual bool init();
        virtual bool remove();

        void setResAcc(m2d::ResAccIf_c *resAcc) {
            mpResAcc = resAcc;
        }

        void setField0x9A0(u8 val) {
            field_0x9A0 = val;
        }

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
        /* 0x0C8 */ m2d::ResAccIf_c *mpResAcc;
        d2d::LytBase_c mLyt;
        dCsGameAnmGroups_c mAnmGroups;
        dCsGameBorrowedAnmGroups_c mAnm;

        /* 0x72C */ lytBowCsr_c mBow;
        /* 0x794 */ lytDowsingCsr_c mDowsing;
        /* 0x86C */ lytPachinkoCsr_c mPachinko;
        /* 0x8C0 */ lytCrawShotCsr_c mCrawShot;
        /* 0x934 */ lytVacuumCsr_c mVacuum;
        /* 0x9A0 */ u8 field_0x9A0;
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
