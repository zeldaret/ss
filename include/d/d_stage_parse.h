#ifndef D_STAGE_PARSE_H
#define D_STAGE_PARSE_H

#include "common.h"

typedef s32 parseCallbackArg;

void parseStageBzs(parseCallbackArg arg, void *bzs);
void parseRoomStageBzs(parseCallbackArg arg, void *bzs);
void parseBzsStageRoom(parseCallbackArg arg, void *bzs);

#endif
