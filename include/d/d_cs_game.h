#ifndef D_CS_GAME_H
#define D_CS_GAME_H

#include "common.h"
#include "d/d_cs.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_structd.h"
#include "m/m2d.h"
#include "s/s_FStateID.hpp"
#include "s/s_State.hpp"
#include "s/s_StateID.hpp"
#include "toBeSorted/effects_struct.h"

struct CsGameConfigMaybe {
    CsGameConfigMaybe();
    virtual ~CsGameConfigMaybe() {}

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3C;
    /* 0x40 */ u8 field_0x40;
    /* 0x41 */ u8 field_0x41;
    /* 0x42 */ u8 field_0x42;
};

class EffectRelatedTmp {
public:
    EffectRelatedTmp();
    virtual ~EffectRelatedTmp();

    void doSomething(EffectsStruct *s);
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

/// @brief Game cursor.
class dCsGame_c : public dCs_c {
public:
    dCsGame_c();
    virtual ~dCsGame_c();

    virtual int create() override;
    virtual int doDelete() override;

    void setSomething(int);

    static dCsGame_c *sInstance;

    /// @brief Item cursor.
    class lytItemCursor_c {
        friend class dCsGame_c;

    public:

        enum CursorType_e {
            BOW = 6,
        };

        /// @brief Bow cursor.
        class lytBowCsr_c {
        public:
            lytBowCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g.tmp.mAnmGroups, 0x17), mpLyt(lyt), mStateMgr(*this, sStateID::null) {}
            virtual ~lytBowCsr_c() {}

            void init();

            void enter();
            inline void select() {
                mStateMgr.changeState(StateID_Select);
            }
            void execute();

        private:
            STATE_FUNC_DECLARE(lytBowCsr_c, Invisible);
            STATE_FUNC_DECLARE(lytBowCsr_c, Select);
            STATE_FUNC_DECLARE(lytBowCsr_c, Ready);
            STATE_FUNC_DECLARE(lytBowCsr_c, ToAimStart);
            STATE_FUNC_DECLARE(lytBowCsr_c, AimStart);
            STATE_FUNC_DECLARE(lytBowCsr_c, Draw);
            STATE_FUNC_DECLARE(lytBowCsr_c, Charge);

            /* 0x04 */ UI_STATE_MGR_DECLARE(lytBowCsr_c);
            /* 0x40 */ d2d::AnmGroups mAnm;
            /* 0x4C */ d2d::LytBase_c *mpLyt;
            /* 0x50 */ f32 field_0x50;
            /* 0x54 */ f32 field_0x54;
        };

        /// @brief Dowsing cursor.
        class lytDowsingCsr_c {
        public:
            lytDowsingCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g.tmp.mAnmGroups, 0x17), mpLyt(lyt), mStateMgr(*this, sStateID::null) {}
            virtual ~lytDowsingCsr_c() {}

            void init();

            void enter();
            void execute();

        private:
            STATE_FUNC_DECLARE(lytDowsingCsr_c, NotFind);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, ToFind);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, Find);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, FindMinimum);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, ToNotFind);

            /* 0x04 */ UI_STATE_MGR_DECLARE(lytDowsingCsr_c);
            /* 0x40 */ d2d::AnmGroups mAnm;
            /* 0x4C */ d2d::LytBase_c *mpLyt;
            /* 0x50 */ EffectsStruct mEffects1;
            /* 0x84 */ EffectsStruct mEffects2;
            /* 0xB8 */ f32 field_0xB8;
            /* 0xBC */ f32 field_0xBC;
            /* 0xC0 */ f32 field_0xC0;
        };

        /// @brief Slingshot cursor.
        class lytPachinkoCsr_c {
        public:
            lytPachinkoCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g.tmp.mAnmGroups, 0x17), mpLyt(lyt), mStateMgr(*this, sStateID::null), field_0x50(0),
                  field_0x54(0.0f), field_0x58(0.0f) {}
            virtual ~lytPachinkoCsr_c() {}

            void init();

            // void enter();
            void execute();

        private:
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Invisible);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Select);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, ToDraw);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Draw);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Out);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, Charge);
            STATE_FUNC_DECLARE(lytPachinkoCsr_c, ChargeFull);

            /* 0x04 */ UI_STATE_MGR_DECLARE(lytPachinkoCsr_c);
            /* 0x40 */ d2d::AnmGroups mAnm;
            /* 0x4C */ d2d::LytBase_c *mpLyt;
            /* 0x50 */ u8 field_0x50;
            /* 0x54 */ f32 field_0x54;
            /* 0x58 */ f32 field_0x58;
        };

        /// @brief Clawshots cursor.
        class lytCrawShotCsr_c {
        public:
            lytCrawShotCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g.tmp.mAnmGroups, 0x17), mpLyt(lyt), mStateMgr(*this, sStateID::null) {}
            virtual ~lytCrawShotCsr_c() {}

            void init();

            void enter();
            void execute();

        private:
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, Normal);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, ToLock);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, Lock);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, ToNormal);

            /* 0x04 */ UI_STATE_MGR_DECLARE(lytCrawShotCsr_c);
            /* 0x40 */ d2d::AnmGroups mAnm;
            /* 0x4C */ d2d::LytBase_c *mpLyt;
        };

        /// @brief Gust Bellows cursor.
        class lytVacuumCsr_c {
        public:
            lytVacuumCsr_c(dCsGameAnmGroups_c &g, d2d::LytBase_c *lyt)
                : mAnm(g.tmp.mAnmGroups, 0x17), mpLyt(lyt), mStateMgr(*this, sStateID::null) {}
            virtual ~lytVacuumCsr_c() {}

            void init();

            void enter();
            void execute();

        private:
            STATE_FUNC_DECLARE(lytVacuumCsr_c, Normal);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, ToLock);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, Lock);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, ToNormal);

            /* 0x04 */ UI_STATE_MGR_DECLARE(lytVacuumCsr_c);
            /* 0x40 */ d2d::AnmGroups mAnm;
            /* 0x4C */ d2d::LytBase_c *mpLyt;
            /* 0x5C */ u8 field_0x5C;
        };

    public:
        lytItemCursor_c()
            : mStateMgr(*this, sStateID::null), mAnm(mAnmGroups.tmp.mAnmGroups, 0x17), mBow(mAnmGroups, &mLyt),
              mDowsing(mAnmGroups, &mLyt), mPachinko(mAnmGroups, &mLyt), mCrawShot(mAnmGroups, &mLyt),
              mVacuum(mAnmGroups, &mLyt) {}
        virtual ~lytItemCursor_c() {}
        virtual void lytItemCursor0x0C();
        virtual bool init();
        virtual bool remove();

        bool preInit();
        bool doInit();

        void setResAcc(m2d::ResAccIf_c *resAcc) {
            mpResAcc = resAcc;
        }

        void setField0x9A0(u8 val) {
            field_0x9A0 = val;
        }

        void setNextCursorType(CursorType_e cs) {
            field_0x9A8 = 1;
            mNextCursorType = cs;
        }

        void changeState(const sFStateID_c<lytItemCursor_c> &newState);

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

        /* 0x048 */ EffectsStruct mEffects;
        /* 0x07C */ EffectRelated mEffectRelated;
        /* 0x0C8 */ m2d::ResAccIf_c *mpResAcc;
        /* 0x0CC */ d2d::LytBase_c mLyt;
        /* 0x15C */ dCsGameAnmGroups_c mAnmGroups;
        /* 0x720 */ d2d::AnmGroups mAnm;
        /* 0x72C */ lytBowCsr_c mBow;
        /* 0x794 */ lytDowsingCsr_c mDowsing;
        /* 0x86C */ lytPachinkoCsr_c mPachinko;
        /* 0x8C0 */ lytCrawShotCsr_c mCrawShot;
        /* 0x934 */ lytVacuumCsr_c mVacuum;
        /* 0x990 */ u8 field_0x990;
        /* 0x99C */ u8 field_0x99C;
        /* 0x9A0 */ u8 field_0x9A0;
        /* 0x9A8 */ u8 field_0x9A8;
        /* 0x9AC */ CursorType_e mNextCursorType;
    };

    void setNextCursorType(lytItemCursor_c::CursorType_e);

    /* 0x068 */ m2d::ResAccIf_c mCursorResAcc;
    /* 0x11C */ m2d::ResAccIf_c mMain2DResAcc;
    /* 0x1D0 */ d2d::dLytStructCSub mStructC;
    /* 0x1EC */ s32 mCursorType;
    /* 0x1F0 */ dCsGameLyt1_c mLyt1;
    /* 0x290 */ dCsGameLyt2_c mLyt2;
    /* 0x330 */ lytItemCursor_c mCursor;
};

#endif
