#ifndef D_STAGE_H
#define D_STAGE_H

#include "common.h"
#include "d/d_base.h"
#include "d/d_fader.h"
#include "d/d_room.h"
#include "egg/gfx/eggPostEffectBlur.h"
#include "egg/gfx/eggScreenEffectBlur.h"
#include "m/m_allocator.h"
#include "m/m_fader_base.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "s/s_FPhase.h"

#define MAX_ROOM_NUMBER 64

class dRoomTable_c {
public:
    dRoomTable_c();
    virtual ~dRoomTable_c() {}

    void setRoom(int roomid, dRoom_c *room) {
        mRooms[roomid] = room;
    }

    dRoom_c *getRoom(s32 idx) {
        return mRooms[idx];
    }

private:
    dRoom_c *mRooms[MAX_ROOM_NUMBER - 1];
};

class MapRelated {
    friend class dStage_c;
    struct Child {
        Child() : a(mVec3_c::Zero), b(mVec3_c::Zero), c(false) {}
        ~Child() {}
        mVec3_c a;
        mVec3_c b;
        bool c;
    };

public:
    MapRelated();
    ~MapRelated();
    /* 0x000 */ mHeapAllocator_c mAllocator;
    /* 0x01C */ Child mChildren[8];
    /* 0x0FC */ mVec3_c field_0x0FC;
    /* 0x108 */ f32 field_0x108;
    /* 0x10C */ f32 field_0x10C;
    /* 0x110 */ f32 field_0x110;
    /* 0x114 */ f32 field_0x114;
    /* 0x118 */ f32 field_0x118;
    /* 0x11C */ f32 field_0x11C;
    /* 0x120 */ f32 field_0x120;
    /* 0x124 */ f32 field_0x124;
    /* 0x128 */ f32 field_0x128;
    /* 0x12C */ f32 field_0x12C;
    /* 0x130 */ EGG::ScreenEffectBlur mScreenEffect;
    /* 0x168 */ EGG::PostEffectBlur mPostEffect;
    /* 0x1E0 */ u32 field_0x1E0;
    /* 0x1E4 */ u16 field_0x1E4;
    /* 0x1E6 */ u16 field_0x1E6;
    /* 0x1E8 */ u8 field_0x1E8;
    /* 0x1E9 */ u8 field_0x1E9;
    /* 0x1EA */ s8 field_0x1EA;
    /* 0x1EB */ u8 field_0x1EB;
    /* 0x1EC */ u8 field_0x1EC;
    /* 0x1ED */ u8 field_0x1ED;
    /* 0x1EE */ u8 field_0x1EE;
    /* 0x1EF */ u8 field_0x1EF;
    /* 0x1F0 */ u8 field_0x1F0;

public:
    u8 getField_0x1EE() const {
        return field_0x1EE;
    }

    void setField_0x1E8(u8 v) {
        field_0x1E8 = v;
    }

    void setField_0x1EF(u8 v) {
        field_0x1EF = v;
    }

    void init(MapRelated *);
    void fn_801b4900();
    void fn_801B4B80(u32 mapParams, const mVec3_c &center, const mVec3_c &size);
    void fn_801B4C70(const mVec3_c &);
    void fn_801B50C0(s32);
    const mVec3_c &fn_801B4CB0() const;
    s32 fn_801B4F10(s32 roomid, const mVec3_c &position) const;
};

// OBJ NAME: STAGE
// Ghidra: RoomManager
//   size: 0x239c
// vtable: 8052dff4
class dStage_c : public dBase_c {
public:
    dStage_c();
    virtual ~dStage_c() {
        sInstance = nullptr;
    }

    int create() override;
    int doDelete() override;
    int execute() override;
    int draw() override;
    void deleteReady() override;

    void drawMap(mMtx_c *mtx, int);
    dRoom_c *getRoom(s32 idx);
    void setRoom(int roomid, dRoom_c *room);
    void handleRoomChange(int roomid);

    bool checkEnemyDefeatFlag(u16 flag);

    bool fadeIn(s32 fadeType, u16 frames);
    bool fadeOut(s32 fadeType, u16 frames);

    void forceFadeIn();
    void forceFadeOut();

    bool hasVisitedRoom(int roomid) const;
    void setRoomVisited(int roomid);

    static void bindStageResToFile(nw4r::g3d::ResFile *file);
    static void bindSkyCmnToResFile(nw4r::g3d::ResFile *file);
    static bool getMA0AnmTexSrt(nw4r::g3d::ResAnmTexSrt *, const char *matName);
    static bool getMA0IndirectSrt(nw4r::g3d::ResAnmTexSrt *, const nw4r::g3d::ResMdl &mdl);

    /** Returns stage if idx == -1, otherwise room */
    static dBase_c *getParentForRoom(s32 idx);

    void changeLoadedEntities(u32 index_shift, bool set);

    static dStage_c *GetInstance() {
        return sInstance;
    }

    MapRelated *getMapRelated() {
        return &mMapRelated;
    }

    bool isFadedOut() const {
        return mFader.isStatus(mFaderBase_c::FADED_OUT);
    }

    bool isFadedIn() const {
        return mFader.isStatus(mFaderBase_c::FADED_IN);
    }

    s8 getCurrRoomId() const {
        return curr_room_id;
    }

    bool fn_801B3EE0();
    void fn_801B3F20();

private:
    dRoom_c *createRoom(int roomid, bool flag0x40);
    void createLytSeekerStone();

    sFPhaseBase::sFPhaseState createRooms();
    sFPhaseBase::sFPhaseState waitForRooms();

    static sFPhase<dStage_c>::phaseCallback sCallbacks[];

    /* 0x0068 */ sFPhase<dStage_c> mPhase;
    /* 0x007C */ dRoomTable_c rooms;
    /* 0x017C */ dFader_c mFader;
    /* 0x01A0 */ MapRelated mMapRelated;
    /* 0x0394 */ u8 _0x394[0x39C - 0x394];
    /* 0x039C */ u32 loaded_entities[2047];
    /* 0x2398 */ s8 curr_room_id;

    static dStage_c *sInstance;
};

#endif
