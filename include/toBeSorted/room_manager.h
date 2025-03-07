#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "common.h"
#include "d/d_base.h"
#include "d/d_room.h"
#include "m/m_allocator.h"
#include "m/types_m.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

#define MAX_ROOM_NUMBER 64

class dRoomTable {
    /* 801b4670 */ dRoomTable(dRoomTable *roomTable);
    virtual ~dRoomTable();
    dRoom_c *rooms[MAX_ROOM_NUMBER - 1];
};

class MapRelated {
public:
    mAllocator_c allocator;
    char unk[472];

public:
    /* 801b4780 */ void init(MapRelated *);
    /* 801b4900 */ void fn_801b4900();
};

// OBJ NAME: STAGE
// Ghidra: RoomManager
//   size: 0x239c
// vtable: 8052dff4
class RoomManager : public dBase_c {
public:
    /* 0068 */ char field_0x68[0x7C - 0x68];
    /* 007C */ dRoomTable rooms;
    /* 017c */ char fader[0x1a0 - 0x174]; // size unk
    /* 01a0 */ MapRelated mapRelated;
    /* 039c */ u32 loaded_entities[2047];
    /* 2398 */ u8 curr_room_id;

public:
    /* 80575760 */ static RoomManager *m_Instance;

public:
    /* 801b3970 */ dRoom_c *GetRoomByIndex(int roomid);
    /* 801b42b0 */ static dRoom_c *getRoom(int roomid);
    /* 801b42d0 */ static void bindStageResToFile(nw4r::g3d::ResFile *);
    /* 801b4320 */ static void bindSkyCmnToResFile(nw4r::g3d::ResFile *);
    /* 801b4380 */ static bool getMA0AnmTexSrt(nw4r::g3d::ResAnmTexSrt *, char *);
    /* 801b4420 */ static bool getMA0IndirectSrt(nw4r::g3d::ResAnmTexSrt *, const nw4r::g3d::ResMdl &mdl);
    /* 801b44c0 */ static void changeLoadedEntities(RoomManager *mgr, u32 index_shift, bool set);
    /* 801b4510 */ static u32 checkRoomFlag(RoomManager *mgr, u32 flag);
    /* 801b4550 */ static bool checkEnemyDefeatFlag(RoomManager *mgr, u32 flag);
    /* 801b45c0 */ static bool fn_801B45C0(RoomManager *mgr /* plus others */);
    /* 801b6c60 */ bool hasVisitedRoomId(s32 roomid) const;

    void setRoom(int roomid, dRoom_c *room);
};

#endif
