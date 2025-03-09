#ifndef D_STAGE_PARSE_H
#define D_STAGE_PARSE_H

void parseRoomStageBzs(int roomid, const void *bzs);
void parseBzsStageRoom(int roomid, const void *bzs);
void parseStageBzs(int roomid, const void *bzs);
void parseRoomBzs(int roomid, const void *bzs);
void parseRoomReactivateBzs(int roomid, const void *bzs);
void parseRoomActivateBzs(int roomid, const void *bzs);

#endif
