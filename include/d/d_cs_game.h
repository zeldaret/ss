#ifndef D_CS_GAME_H
#define D_CS_GAME_H

#include "common.h"
#include "d/d_cs.h"
#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "m/m2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_FStateID.hpp"
#include "s/s_State.hpp"
#include "s/s_StateID.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_emitter_callbacks.h"

/** A variant of the cursor position getter for dCsGame_c */
class dCursorInterfaceGame_c : public dCursorInterface_c {
public:
    dCursorInterfaceGame_c() {}
    virtual ~dCursorInterfaceGame_c() {}
    virtual mVec2_c &getCursorPos();

private:
    /* 0x14 */ mVec2_c field_0x14;
};

// TODO - these could also be in the dCsGame_c namespace
// TODO - rename these
class dCsGameLytBase1_c {
public:
    virtual ~dCsGameLytBase1_c() {}
    /* 0x0C */ virtual void setPosition(const mVec2_c &pos) {
        mPosition = pos;
    }

    /* 0x10 */ virtual bool init() = 0;
    /* 0x14 */ virtual bool remove() = 0;
    /* 0x18 */ virtual bool execute() = 0;
    /* 0x1C */ virtual bool draw() = 0;

protected:
    /* 0x04 */ mVec2_c mPosition;
};

class dCsGameLytBase2_c : public dCsGameLytBase1_c {
public:
    virtual ~dCsGameLytBase2_c() {}

    /* 0x10 */ virtual bool init();
    /* 0x14 */ virtual bool remove() = 0;
    /* 0x18 */ virtual bool execute();
    /* 0x1C */ virtual bool draw();

    /* 0x20 */ virtual void loadResAcc() = 0;
    /* 0x24 */ virtual void build() = 0;
    /* 0x28 */ virtual d2d::LytBase_c *getLyt() = 0;

protected:
};

class dCsGameLyt1_c : public dCsGameLytBase2_c {
public:
    dCsGameLyt1_c() : mpResAcc(nullptr) {}
    virtual ~dCsGameLyt1_c() {}
    virtual bool init() override;
    virtual bool remove() override;
    virtual void loadResAcc() override;
    virtual void build() override;
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }

    void setResAcc(m2d::ResAccIf_c *resAcc) {
        mpResAcc = resAcc;
    }

    void setPriorityDraw(u8 priority) {
        mLyt.setPriority(priority);
    }

private:
    /* 0x0C */ m2d::ResAccIf_c *mpResAcc;
    /* 0x10 */ d2d::LytBase_c mLyt;
};

class dCsGameLyt2_c : public dCsGameLytBase2_c {
public:
    dCsGameLyt2_c() : mpResAcc(nullptr) {}
    virtual ~dCsGameLyt2_c() {}
    virtual bool init() override;
    virtual bool remove() override;
    virtual void loadResAcc() override;
    virtual void build() override;
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }

    void setResAcc(m2d::ResAccIf_c *resAcc) {
        mpResAcc = resAcc;
    }

    void setPriorityDraw(u8 priority) {
        mLyt.setPriority(priority);
    }

private:
    /* 0x0C */ m2d::ResAccIf_c *mpResAcc;
    /* 0x10 */ d2d::LytBase_c mLyt;
};

/// @brief Game cursor.
class dCsGame_c : public dCs_c {
public:
    enum CursorType_e {
        CS_NONE = 0,
        /** Default pointer */
        CS_POINTER_DEF = 1,
        /** Grab pointer */
        CS_POINTER_CAT = 2,
        CS_BOW = 6,
        CS_DOWSING = 7,
        CS_VACUUM = 8,
        CS_PACHINKO = 9,
        CS_HOOKSHOT = 10,
        CS_PLAYERCAM = 14,
    };

    dCsGame_c();
    virtual ~dCsGame_c();

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

    static dCsGame_c *GetInstance() {
        return sInstance;
    }

    bool shouldDraw() const;

    /// @brief Item cursor.
    class lytItemCursor_c : public dCsGameLytBase2_c {
        friend class dCsGame_c;

    public:
        /// @brief Bow cursor.
        class lytBowCsr_c {
        public:
            lytBowCsr_c(d2d::AnmGroups g, d2d::LytBase_c *lyt)
                : mAnm(g), mpLyt(lyt), mStateMgr(*this, sStateID::null), field_0x50(0.0f), field_0x54(0.0f) {}
            virtual ~lytBowCsr_c() {}

            void init();

            void enter();
            inline void select() {
                // TODO this function might be breaking vtable order
                mStateMgr.changeState(StateID_Select);
            }
            void execute();

            void startDrawOrCharge(f32 f1, f32 f2);
            void aimStart();
            void ready();

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
            /* 0x58 */ nw4r::lyt::Pane *mpPanesArrowRing[4];
        };

        /// @brief Dowsing cursor.
        class lytDowsingCsr_c {
        public:
            lytDowsingCsr_c(d2d::AnmGroups g, d2d::LytBase_c *lyt)
                : mAnm(g),
                  mpLyt(lyt),
                  mStateMgr(*this, sStateID::null),
                  mpRingAllPane(nullptr),
                  mpDowsingPane(nullptr),
                  field_0xC0(0.0f),
                  field_0xC4(0),
                  mRotZ(0),
                  field_0xCC(0.0f),
                  field_0xD0(0.0f),
                  mAlpha(0) {}
            virtual ~lytDowsingCsr_c() {}

            void init();

            void enter();
            void execute();

            void setParams(const mAng &rot, f32 v1, f32 v2);
            void setUnkWord(UNKWORD v);

            void moveEffectsIn();
            void moveEffectsOut();
            void updateEffects();

        private:
            STATE_FUNC_DECLARE(lytDowsingCsr_c, NotFind);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, ToFind);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, Find);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, FindMinimum);
            STATE_FUNC_DECLARE(lytDowsingCsr_c, ToNotFind);

            /* 0x04 */ UI_STATE_MGR_DECLARE(lytDowsingCsr_c);
            /* 0x40 */ d2d::AnmGroups mAnm;
            /* 0x4C */ d2d::LytBase_c *mpLyt;
            /* 0x50 */ dEmitter_c mEffects1;
            /* 0x84 */ dEmitter_c mEffects2;
            /* 0xB8 */ nw4r::lyt::Pane *mpRingAllPane;
            /* 0xBC */ nw4r::lyt::Pane *mpDowsingPane;
            /* 0xC0 */ f32 field_0xC0;
            /* 0xC4 */ UNKWORD field_0xC4;
            /* 0xC8 */ mAng mRotZ;
            /* 0xCC */ f32 field_0xCC;
            /* 0xD0 */ f32 field_0xD0;
            /* 0xD4 */ u32 mAlpha;
        };

        /// @brief Slingshot cursor.
        class lytPachinkoCsr_c {
        public:
            lytPachinkoCsr_c(d2d::AnmGroups g, d2d::LytBase_c *lyt)
                : mAnm(g),
                  mpLyt(lyt),
                  mStateMgr(*this, sStateID::null),
                  mIsCharging(false),
                  mDrawProgress(0.0f),
                  mSavedOnProgress(0.0f) {}
            virtual ~lytPachinkoCsr_c() {}

            void init();
            // void enter();
            void execute();

            void setCharging(bool charging, f32 progress);

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
            /* 0x50 */ bool mIsCharging;
            /* 0x54 */ f32 mDrawProgress;
            /* 0x58 */ f32 mSavedOnProgress;
        };

        /// @brief Clawshots cursor.
        class lytCrawShotCsr_c {
        public:
            lytCrawShotCsr_c(d2d::AnmGroups g, d2d::LytBase_c *lyt)
                : mAnm(g), mpLyt(lyt), mStateMgr(*this, sStateID::null), mLocked(false) {}
            virtual ~lytCrawShotCsr_c() {}

            void init();

            void enter();
            void execute();

            void setLocked(bool locked);
            void setRotate(f32 rot);

        private:
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, Normal);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, ToLock);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, Lock);
            STATE_FUNC_DECLARE(lytCrawShotCsr_c, ToNormal);

            /* 0x04 */ UI_STATE_MGR_DECLARE(lytCrawShotCsr_c);
            /* 0x40 */ d2d::AnmGroups mAnm;
            /* 0x4C */ d2d::LytBase_c *mpLyt;
            /* 0x50 */ u8 field_0x50[0x54 - 0x50];
            /* 0x54 */ bool mLocked;
            /* 0x58 */ nw4r::lyt::Pane *mpPaneCrawFix;
            /* 0x5C */ nw4r::lyt::Pane *mpPaneCraws[3];
            /* 0x68 */ f32 field_0x68;
        };

        /// @brief Gust Bellows cursor.
        class lytVacuumCsr_c {
        public:
            lytVacuumCsr_c(d2d::AnmGroups g, d2d::LytBase_c *lyt)
                : mAnm(g), mpLyt(lyt), mStateMgr(*this, sStateID::null), mLocked(false) {}
            virtual ~lytVacuumCsr_c() {}

            void init();

            void enter();
            void execute();

            void setUnkFloat(f32 f);
            void setLocked(bool locked);

        private:
            STATE_FUNC_DECLARE(lytVacuumCsr_c, Normal);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, ToLock);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, Lock);
            STATE_FUNC_DECLARE(lytVacuumCsr_c, ToNormal);

            /* 0x04 */ UI_STATE_MGR_DECLARE(lytVacuumCsr_c);
            /* 0x40 */ d2d::AnmGroups mAnm;
            /* 0x4C */ d2d::LytBase_c *mpLyt;
            /* 0x50 */ u8 field_0x50[0x54 - 0x50];
            /* 0x54 */ f32 field_0x54;
            /* 0x58 */ u8 field_0x58[0x5C - 0x58];
            /* 0x5C */ bool mLocked;
        };

    public:
        lytItemCursor_c()
            : mStateMgr(*this, sStateID::null),
              mAnm(mAnmGroups.tmp.mAnmGroups, 0x17),
              mBow(mAnm, &mLyt),
              mDowsing(mAnm, &mLyt),
              mPachinko(mAnm, &mLyt),
              mCrawShot(mAnm, &mLyt),
              mVacuum(mAnm, &mLyt) {}
        virtual ~lytItemCursor_c() {}
        virtual bool init() override;
        virtual bool remove() override;
        virtual bool execute() override;
        virtual void loadResAcc() override;
        virtual void build() override;
        virtual d2d::LytBase_c *getLyt() override {
            return &mLyt;
        }

        void setResAcc(m2d::ResAccIf_c *resAcc) {
            mpResAcc = resAcc;
        }

        void setPriorityDraw(u8 priority) {
            mLyt.setPriority(priority);
        }

        void changeState(const sFStateID_c<lytItemCursor_c> &newState);

        void pachinkoSetCharging(bool charging, f32 progress);
        void dowsingSetParams(const mAng &rot, f32 v1, f32 v2);
        void dowsingSetUnkWord(UNKWORD v);
        void vacuumSetUnkFloat(f32 v);
        void vacuumSetLocked(bool locked);
        void clawshotsSetLocked(bool locked);
        void bowStartDrawOrCharge(f32 f1, f32 f2);
        void bowAimStart();
        void bowReady();


    private:
        STATE_FUNC_DECLARE(lytItemCursor_c, Invisible);
        STATE_FUNC_DECLARE(lytItemCursor_c, Bow);
        STATE_FUNC_DECLARE(lytItemCursor_c, Dowsing);
        STATE_FUNC_DECLARE(lytItemCursor_c, Vacuum);
        STATE_FUNC_DECLARE(lytItemCursor_c, Pachinko);
        STATE_FUNC_DECLARE(lytItemCursor_c, HookShot);
        STATE_FUNC_DECLARE(lytItemCursor_c, PlayerCam);

        /* 0x00C */ UI_STATE_MGR_DECLARE(lytItemCursor_c);

        template <int N>
        class Tmp {
        public:
            d2d::AnmGroup_c mAnmGroups[N];
        };

        class dCsGameAnmGroups_c {
        public:
            dCsGameAnmGroups_c() {}
            virtual ~dCsGameAnmGroups_c() {}

            d2d::AnmGroup_c &operator[](int idx) {
                return tmp.mAnmGroups[idx];
            }

            Tmp<0x17> tmp;
        };

        /* 0x048 */ dEmitter_c mEffects;
        /* 0x07C */ dEmitterCallbackCursorTrail_c mTrailCb;
        /* 0x0B0 */ mVec3_c mPositionThisFrame;
        /* 0x0BC */ mVec3_c mPositionLastFrame;
        /* 0x0C8 */ m2d::ResAccIf_c *mpResAcc;
        /* 0x0CC */ d2d::LytBase_c mLyt;
        /* 0x15C */ dCsGameAnmGroups_c mAnmGroups;
        /* 0x720 */ d2d::AnmGroups mAnm;
        /* 0x72C */ lytBowCsr_c mBow;
        /* 0x794 */ lytDowsingCsr_c mDowsing;
        /* 0x86C */ lytPachinkoCsr_c mPachinko;
        /* 0x8C8 */ lytCrawShotCsr_c mCrawShot;
        /* 0x934 */ lytVacuumCsr_c mVacuum;
        /* 0x994 */ u8 field_0x994[0x99C - 0x994]; // seemingly not used
        /* 0x99C */ bool mDrawCursorTrailThisFrame;
        /* 0x99D */ u8 field_0x99D[0x9A0 - 0x99D]; // Havent seen this range be set
    };

    void setNextCursorType(CursorType_e);
    void noneSet();
    void pointerDefSet();
    void pointerCatSet();

    void pachinkoSetCharging(bool charging, f32 progress);
    void dowsingSetParams(const mAng &rot, f32 v1, f32 v2);
    void dowsingSetUnkWord(UNKWORD v);
    void vacuumSetNotLocked(f32 v);
    void vacuumSetLocked();
    void clawshotsSetLocked(bool locked);
    void bowStartDrawOrCharge(f32 f1, f32 f2);
    void bowAimStart();
    void bowReady();

    void setCursorTypePlayerCam();

    bool isCursorActive() const {
        return mCursorActive;
    }

    void setField_0x9A2(bool value) {
        field_0x9A2 = value;
    }

    void offNextCursor() {
        mNextCursorActive = false;
    }

    dCursorHitCheck_c *getCursorHit() const {
        return mCursorIf.getHit();
    }

    dCursorInterfaceGame_c *getCursorIf() {
        return &mCursorIf;
    }

    void setPriorityDraw(u8 priority) {
        mLyt1.setPriorityDraw(priority);
        mLyt2.setPriorityDraw(priority);
        mCursor.setPriorityDraw(priority);
    }

private:
    static dCsGame_c *sInstance;
    bool isForcedHidden() const;

    /* 0x068 */ m2d::ResAccIf_c mCursorResAcc;
    /* 0x11C */ m2d::ResAccIf_c mMain2DResAcc;
    /* 0x1D0 */ dCursorInterfaceGame_c mCursorIf;
    /* 0x1EC */ CursorType_e mCursorType;
    /* 0x1F0 */ dCsGameLyt1_c mLyt1;
    /* 0x290 */ dCsGameLyt2_c mLyt2;
    /* 0x330 */ lytItemCursor_c mCursor;
    /* 0xCD0 */ bool mCursorActive;
    /* 0xCD1 */ bool field_0x9A1; // Compares to mCursorActive to change state
    /* 0xCD2 */ bool field_0x9A2; // A way of signalling Lyt non-normal state.
    /* 0xCD4 */ CursorType_e mActiveCursorType;
    /* 0xCD8 */ bool mNextCursorActive;
    /* 0xCDC */ CursorType_e mNextCursorType;
};

#endif
