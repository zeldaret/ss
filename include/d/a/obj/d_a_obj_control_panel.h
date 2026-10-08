#ifndef D_A_OBJ_CONTROL_PANEL_H
#define D_A_OBJ_CONTROL_PANEL_H

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_D3_dummy.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/d_message.h"
#include "d/flag/sceneflag_manager.h"
#include "m/m3d/m_mdl.h"
#include "m/m3d/m_smdl.h"
#include "m/m_color.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_flow_mgr.h"

class dAcOCtrlPnl_c : public dAcObjBase_c {
    class callback_c : public m3d::callback_c {
        public:
            void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;
            void setPosAndScale(int i, const mVec3_c& vec, f32 f) {
                field_0x0[i] = vec;
                field_0x20[i] = f;
            }
            void setNodeID(int i, s32 nodeID) {
                mNodeIDs[i] = nodeID;
            }
        private:
            /* 0x00 */ mVec3_c field_0x0[8];
            /* 0x20 */ f32 field_0x20[8];
            /* 0x40 */ s32 mNodeIDs[8];
        };
    friend class callback_c;
    struct Spot {
        /* 0x00 */ Spot* mNeighbors[4];
        /* 0x10 */ u8 mSpot;
        /* 0x11 */ u8 mCurrentTile;
    };
public:
    dAcOCtrlPnl_c() : mStateMgr(*this), mEvent(*this, nullptr), mFlowMgr(&mFlow) {}
    virtual ~dAcOCtrlPnl_c() {}
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual void doInteraction(s32 /* InteractionType */) override;

    void updatePuzzleFromFile();
    void savePuzzleState();
    bool handleSlidingTile();
    void setupNeighbors();
    void updateTileScales();
    bool hasAdjacentEmptySpot(s32);
    bool getAdjacentEmptySpot(s32, s32*);
    void handleEvent();
    void handleControl();
    void updateDoorSceneflags();
    u8 getSpotForCursor(mVec2_c&);
    u8 getMoveDirection(s32, s32);
    void setTileID(u32, u32);
    u8 getTileID(u32);
    void resetPuzzleState();
    void updateTilePositions();
    void updateSpotTilesFromPuzzle();
    void getScreenCenter(mVec3_c&);
    void handleLinkObjProp0x200(bool);
    void updateTileColors();
    void afterControlPanel();
    void afterControlPanelFirst();
    void enterMove();
    void finishMove();
    bool changedPuzzle();
    bool isFirstTime();

    inline bool doChaseTilePosToTarget() {
        return cLib::chasePos(mTilePositions[mMovingTile], mGrabTargetPosition, 10.f) > 0;
    }

    void setSceneflag(u8 flag) {
        SceneflagManager::sInstance->setFlag(mRoomID, flag);
    }

    void unsetSceneflag(u8 flag) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, flag);
    }

    STATE_FUNC_DECLARE(dAcOCtrlPnl_c, Wait);
    STATE_FUNC_DECLARE(dAcOCtrlPnl_c, Control);
    STATE_FUNC_DECLARE(dAcOCtrlPnl_c, EndEvent);

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::mdl_c mModel;
    /* 0x358 */ mVec3_c mTilePositions[8];
    /* 0x3B8 */ STATE_MGR_DECLARE(dAcOCtrlPnl_c);
    /* 0x3F4 */ ActorEventRelated mEvent;
    /* 0x444 */ mVec3_c mGrabTargetPosition;
    /* 0x450 */ s32 mMovingTile;
    /* 0x454 */ dBgW mBgW;
    /* 0x664 */ s32 mCurrentlyHoveredTile;
    /* 0x668 */ s32 mCurrentlyHoveredSpot;
    /** The spots where tiles can be, in the following order:
     * TODO check
     *  0 | 1 | 2
     * ---+---+---
     *  3 | 4 | 5
     * ---+---+---
     *  6 | 7 | 8
     */
    /* 0x66C */ Spot mSpots[9];
    /* 0x720 */ dAcRef_c<dAcOD3Dummy_c> mD3DummyRef;
    /* 0x72C */ bool mIsGrabbing;
    /* 0x72D */ u8 mGrabTargetMoveDirection;
    /* 0x72E */ u8 mGrabTargetSpot;
    /* 0x730 */ mVec2_c mGrabStartCursorPos;
    /* 0x738 */ bool mHasMovedATile;
    /* 0x739 */ bool mLeavePuzzle;
    /* 0x73C */ dFlow_c mFlow;
    /* 0x7A0 */ dFlowMgrBase_c mFlowMgr;
    /* 0x7F8 */ u8 mSkykeepPuzzle[9];
    /* 0x801 */ u8 mSkykeepPuzzleOriginal[9];
    /* 0x80C */ callback_c mCallback;
    /* 0x8B0 */ f32 mTileScales[8];
    /* 0x8D0 */ s32 mTilePlateMatIDs[8];
    /* 0x8F0 */ s32 mTriforceMatIDs[3];
    /* 0x8FC */ mColor mNormalPlateColor;
    /* 0x900 */ s32 mTimer;
};

#endif
