#ifndef NW4R_DB_MAPFILE_H
#define NW4R_DB_MAPFILE_H

#include <rvl/OS.h>

namespace nw4r {
namespace db {

typedef struct MapFile {
    u8 *mapBuf;
    const OSModuleInfo *moduleInfo;
    s32 fileEntry;
    MapFile *next;
} MapFile;

typedef MapFile *MapFileHandle;

MapFileHandle MapFile_RegistOnDvd(void *, const char *, const OSModuleInfo *);
void MapFile_Unregist(MapFileHandle);
void MapFile_UnregistAll();
bool MapFile_QuerySymbol(u32 address, u8 *strBuf, u32 strBufSize);

} // namespace db
} // namespace nw4r

#endif
