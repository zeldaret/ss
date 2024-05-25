#include <MSL_C/string.h>
#include <nw4r/db/db_mapFile.h>
#include <rvl/DVD.h>
#include <rvl/OS.h>

namespace nw4r {
namespace db {

static u32 sFileLength;
static MapFile *sMapFileList = nullptr;
static u8 (*GetCharPtr_)(const u8 *) = nullptr;

static u8 dvdReadBuf[512] ALIGN(32);
static DVDFileInfo sFileInfo;

static s32 sMapBufMaxSize = 0x200;
static u8 *sMapBuf = dvdReadBuf;
static s32 sMapBufOffset = -1;

static void MapFile_Append_(MapFile *file) {
    if (sMapFileList == nullptr) {
        sMapFileList = file;
        return;
    }

    if (file->moduleInfo != nullptr) {
        file->next = sMapFileList->next;
        sMapFileList->next = file;
    } else {
        file->next = sMapFileList;
        sMapFileList = file;
    }
}

/** 80436a50 */
MapFileHandle MapFile_RegistOnDvd(void *buf, const char *path, const OSModuleInfo *info) {
    MapFile *file = (MapFile *)buf;
    file->mapBuf = nullptr;
    file->moduleInfo = info;
    file->fileEntry = DVDConvertPathToEntrynum(path);
    file->next = nullptr;

    MapFile_Append_(file);
    return file;
}

/** 80436ae0 */
void MapFile_Unregist(MapFileHandle file) {
    if (file == sMapFileList) {
        sMapFileList = sMapFileList->next;
        return;
    }

    MapFile *cursor = sMapFileList;
    for (; cursor != nullptr; cursor = cursor->next) {
        if (cursor->next == file) {
            cursor->next = file->next;
            return;
        }
    }
}

/** 80436b30 */
void MapFile_UnregistAll() {
    sMapFileList = nullptr;
}

/** 80436b40 */
static u8 GetCharOnMem_(const u8 *arg) {
    return *arg;
}

/** 80436b50 */
static u8 GetCharOnDvd_(const u8 *buf) {
    s32 address = (u32)buf & 0x7fffffff;
    s32 offset = address - sMapBufOffset;
    if (address >= sFileLength) {
        return 0;
    }

    if (sMapBufOffset < 0 || offset < 0 || offset >= sMapBufMaxSize) {
        sMapBufOffset = ROUND_DOWN(address, 32);
        offset = address - sMapBufOffset;
        address = sMapBufMaxSize;
        if (sMapBufOffset + sMapBufMaxSize >= sFileLength) {
            address = ROUND_UP(sFileLength - sMapBufOffset, 32);
        }
        BOOL enabled = OSEnableInterrupts();
        BOOL read = DVDReadAsyncPrio(&sFileInfo, sMapBuf, address, sMapBufOffset, nullptr, 2);
        while (DVDGetCommandBlockStatus(&sFileInfo.block)) {}

        OSRestoreInterrupts(enabled);
        if (read <= 0) {
            return 0;
        }
    }
    return *(sMapBuf + offset);
}

static u8 *SearchNextLine_(u8 *buf, s32 lines) {
    u8 c;

    if (buf == nullptr) {
        return nullptr;
    }

    while ((c = (GetCharPtr_)(buf)) != '\0') {
        if (c == '\n') {
            if (--lines <= 0) {
                return buf + 1;
            }
        }
        buf++;
    }

    return nullptr;
}

static u8 *SearchNextSection_(u8 *buf) {
    do {
        buf = SearchNextLine_(buf, 1);
        if (buf == nullptr) {
            return nullptr;
        }
    } while ((GetCharPtr_)(buf) != '.');
    return buf;
}

static u8 *SearchParam_(u8 *lineTop, u32 argNum, u8 splitter) {
    bool inArg = false;
    u8 *buf = lineTop;

    if (buf == nullptr) {
        return nullptr;
    }
    while (true) {
        u8 c = (GetCharPtr_)(buf);
        if (c == '\0' || c == '\n') {
            return 0;
        }
        if (inArg) {
            if (c == splitter) {
                inArg = false;
            }
        } else if (c != splitter) {
            if (argNum-- == 0) {
                return buf;
            }
            inArg = true;
        }
        buf++;
    }
}

static u32 XStrToU32_(const u8 *str) {
    u32 val = 0;

    while (true) {
        u32 num;
        u8 c = (GetCharPtr_)(str);
        if ('0' <= c && c <= '9') {
            num = c - '0';
        } else if ('a' <= c && c <= 'z') {
            num = c - 'a' + 10;
        } else if ('A' <= c && c <= 'Z') {
            num = c - 'A' + 10;
        } else {
            return val;
        }

        if (val >= 0x10000000) {
            return 0;
        }
        val = num + val * 16;
        str++;
    }
}

static u32 CopySymbol_(const u8 *buf, u8 *str, u32 strLenMax, u8 splitter) {
    u32 cnt = 0;

    while (true) {
        u8 c = (GetCharPtr_)(buf++);
        if (c == splitter || c == '\0' || c == '\n') {
            *str = '\0';
            return cnt;
        }
        *str++ = c;
        if (++cnt >= strLenMax - 1) {
            *str = '\0';
            return cnt;
        }
    }
}

bool QuerySymbolToMapFile_(u8 *buf, const OSModuleInfo *moduleInfo, u32 address, u8 *strBuf, u32 strBufSize) {
    OSSectionInfo *sectionInfo = nullptr;
    u32 sectionCnt;

    if (moduleInfo != nullptr) {
        sectionInfo = OSGetSectionInfo(moduleInfo);
        sectionCnt = moduleInfo->numSections;
    }

    while (true) {
        u32 offset = 0;
        buf = SearchNextSection_(buf);
        buf = SearchNextLine_(buf, 3);
        if (sectionInfo != nullptr) {
            offset = ROUND_DOWN(sectionInfo->offset, 2);
            if (address < offset || address >= offset + sectionInfo->size) {
                goto next;
            }
        }

        while (true) {
            u8 *param;
            u32 startAddr;
            u32 size;
            buf = SearchNextLine_(buf, 1);
            if (buf == nullptr) {
                return false;
            }
            param = SearchParam_(buf, 1, ' ');
            if (param == nullptr) {
                goto next;
            }
            size = XStrToU32_(param);
            param = SearchParam_(buf, 2, ' ');
            if (param == nullptr) {
                goto next;
            }
            startAddr = XStrToU32_(param);
            if (startAddr == 0) {
                continue;
            }
            startAddr += offset;
            if (address < startAddr || startAddr + size <= address) {
                continue;
            }

            param = SearchParam_(buf, 5, ' ');
            if (param == nullptr) {
                strBuf[0] = '\0';
                return true;
            }
            if ((GetCharPtr_)(param) == '.') {
                continue;
            }

            CopySymbol_(param, strBuf, strBufSize, '\t');
            return true;
        }
    next:
        if (sectionInfo != nullptr) {
            if (--sectionCnt == 0) {
                return 0;
            }
            sectionInfo++;
        }
    }
}

// No idea, doesn't appear in the DWARF
/** 804370d0 */
bool UnkFunction_(const OSModuleInfo *moduleInfo, u32 address, u8 *strBuf, u32 strBufSize) {
    if (moduleInfo == nullptr) {
        if (address < (u32)_stack_end) {
            snprintf((char *)strBuf, strBufSize, "[%p]", address);
            return true;
        } else {
            return false;
        }
    } else {
        u32 sectionIdx = 0;
        OSSectionInfo *sectionInfo = OSGetSectionInfo(moduleInfo);
        for (u32 sectionCnt = 0; sectionCnt < moduleInfo->numSections; sectionCnt++) {
            u32 offset = ROUND_DOWN(sectionInfo->offset, 2);
            if (offset <= address && address < offset + sectionInfo->size) {
                snprintf((char *)strBuf, strBufSize, "[%d:%d:%06x]", moduleInfo->id, sectionIdx, address - offset);
                return true;
            }
            sectionInfo++;
            sectionIdx++;
        }
        return false;
    }
}

bool QuerySymbolToSingleMapFile_(MapFileHandle pMapFile, u32 address, u8 *strBuf, u32 strBufSize) {
    if (pMapFile->mapBuf != nullptr) {
        GetCharPtr_ = GetCharOnMem_;
        return QuerySymbolToMapFile_(pMapFile->mapBuf, pMapFile->moduleInfo, address, strBuf, strBufSize);
    } else if (pMapFile->fileEntry >= 0) {
        u8 *buf = (u8 *)0x80000000;
        bool ret;
        if (!DVDFastOpen(pMapFile->fileEntry, &sFileInfo)) {
            goto err;
        }
        sMapBufOffset = -1;
        sFileLength = sFileInfo.size;
        GetCharPtr_ = GetCharOnDvd_;
        ret = QuerySymbolToMapFile_(buf, pMapFile->moduleInfo, address, strBuf, strBufSize);
        DVDClose(&sFileInfo);
        return ret;
    } else {
        return UnkFunction_(pMapFile->moduleInfo, address, strBuf, strBufSize);
    }

err:
    strBuf[0] = '\0';
    return false;
}

bool MapFile_QuerySymbol(u32 address, u8 *strBuf, u32 strBufSize) {
    if (sMapFileList == nullptr) {
        if (address < (u32)_stack_end) {
            snprintf((char *)strBuf, strBufSize, "[%p]", address);
            return true;
        } else {
            u32 offset;
            u32 section;
            OSModuleInfo *info = OSSearchModule((void *)address, &section, &offset);
            if (info) {
                snprintf((char *)strBuf, strBufSize, "[%d:%d:%06x]", info->id, section, offset);
                return true;
            }
            return false;
        }
    } else {
        for (MapFile *pMap = sMapFileList; pMap != nullptr; pMap = pMap->next) {
            if (QuerySymbolToSingleMapFile_(pMap, address, strBuf, strBufSize)) {
                return true;
            }
        }

        return false;
    }
}

} // namespace db
} // namespace nw4r
