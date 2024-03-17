#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "d/d_base.h"
#include <common.h>

#define MAX_ROOM_NUMBER 64

class dRoom;
class RoomTable {
    RoomTable();
    virtual ~RoomTable();
    dRoom *rooms[MAX_ROOM_NUMBER - 1];
};

// OBJ NAME: STAGE
// Ghidra: RoomManager
//   size: 0x239c
// vtable: 8052dff4
class RoomManager : public dBase_c {
public:
    /* 0068 */ char field_0x68[0x7C - 0x68];
    /* 007C */ RoomTable rooms;
    /* 017c */ char fader[0x1a0 - 0x17c];      // size unk
    /* 01a0 */ char mapRelated[0x39c - 0x1a0]; // size unk
    /* 039c */ u32 loaded_entities[2047];
    /* 2398 */ u8 curr_room_id;

public:
    /* 80575760 */ static RoomManager *m_Instance;

public:
    /* 801b42b0 */ static dBase_c *getRoom(int roomid);
};

#endif
