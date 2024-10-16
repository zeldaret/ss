#include "d/d_dylink.h"

#include "DynamicLink.h"
#include "m/m_dvd.h"
#include "m/m_heap.h"
#include "rvl/DVD.h"  // IWYU pragma: export
#include "rvl/NAND.h" // IWYU pragma: export

#include <common.h>
#include <string.h>

struct RelNamePtr {
    u16 relId;
    const char *name;
};

extern const RelNamePtr DYNAMIC_NAME_TABLE[];

static const RelNamePtr *pDynamicNameTable;
static int nDynamicNameTable;

DynamicModuleControl g_DMC("d_profile", nullptr);

namespace dDyl {

DynamicModuleControl **pDMC;
int nDMC;
EGG::Heap *cCc_frmHeap;
u32 Initialized;
mDvd_callback_c *DVD;

enum NandChunkStatus {
    Success = 1,
    DVDPending = 2,
    NANDPending = 4,
    Error = 8,

    FinalizedMask = Success | Error,
};

NandChunkStatus status_1 = Success;
NandChunkStatus status_2 = Success;
char relsDir[] = "/rels";

/** 80052E00 */
int initDylinkHeap(int maxRelId, const RelNamePtr *dynNameTable, int dynNameTableNum, EGG::Heap *parentHeap) {
    cCc_frmHeap = mHeap::createFrmHeap(
        maxRelId * 0x10 + dynNameTableNum * 0x48, parentHeap, "ダイナミックリンク制御用ヒープ(dDyl::cCc_frmHeap)", 0x20,
        0
    );

    mHeap _guard(cCc_frmHeap);

    nDMC = maxRelId;
    pDMC = new DynamicModuleControl *[maxRelId]();
    memset(pDMC, 0, sizeof(void *) * nDMC);

    pDynamicNameTable = dynNameTable;
    nDynamicNameTable = dynNameTableNum;

    for (int i = 0; i < nDynamicNameTable; i++) {
        const RelNamePtr *rel = &pDynamicNameTable[i];
        if (rel->name != nullptr) {
            for (int j = 0; j < nDMC; j++) {
                DynamicModuleControl **dmc = &pDMC[j];
                if (*dmc != nullptr) {
                    const char *nm = (*dmc)->getModuleName();
                    if (!strcmp(rel->name, nm)) {
                        pDMC[rel->relId] = pDMC[j];
                        break;
                    }
                }
            }
            if (pDMC[rel->relId] == nullptr) {
                pDMC[rel->relId] = new DynamicModuleControl(rel->name, nullptr);
            }
        }
    }
    cCc_frmHeap->adjust();
    return 1;
}

/** 80052FA0 */
bool isLinked(u16 relId) {
    return pDMC[relId] != nullptr ? pDMC[relId]->isLinked() : true;
}

BOOL Unlink(u16 relId) {
    DynamicModuleControl *dmc = pDMC[relId];
    if (dmc != nullptr) {
        return dmc->unlink();
    }

    return false;
}

int tryLink(u16 relId) {
    if (!Initialized) {
        return 0;
    }

    DynamicModuleControl *dmc = pDMC[relId];
    if (dmc != nullptr) {
        if (dmc->load_async()) {
            return dmc->link() ? 1 : 2;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

// TODO prevent moves
struct UnkNandStruct {
    /* 0x00 */ NandChunkStatus status;
    /* 0x04 */ OSThreadQueue queue;
    /* 0x0C */ DVDFileInfo dvdFileInfo;
    /* 0x48 */ NANDFileInfo *nandFileInfo;
    /* 0x4C */ NANDCommandBlock nandBlock;

    UnkNandStruct() {
        status = status_2;
        OSInitThreadQueue(&queue);
        nandFileInfo = nullptr;
        updateStatus(status_1);
        nandFileInfo = nullptr;
        dvdFileInfo.block.userData = this;
        NANDSetUserData(&nandBlock, this);
    }

    ~UnkNandStruct() {
        waitForFinalization();
    }

    void waitForFinalization() {
        if (!isFinalized()) {
            BOOL enabled = OSDisableInterrupts();
            while (!isFinalized()) {
                OSSleepThread(&queue);
            }
            OSRestoreInterrupts(enabled);
        }
    }

    bool isFinalized() {
        return (status & FinalizedMask) != 0;
    }

    void updateStatus(NandChunkStatus newStatus) {
        if (status != newStatus) {
            status = newStatus;
            OSWakeupThread(&queue);
        }
    }

    void open(s32 entryNum, NANDFileInfo *fInfo) {
        DVDFastOpen(entryNum, &dvdFileInfo);
        nandFileInfo = fInfo;
    }

    bool isError() {
        return (status & Error) != 0;
    }
};

void nandCallback(s32 result, NANDCommandBlock *block) {
    UnkNandStruct *s = reinterpret_cast<UnkNandStruct *>(NANDGetUserData(block));
    NandChunkStatus status = result >= 0 ? Success : Error;
    s->updateStatus(status);
}

void dvdCallback(s32 result, DVDFileInfo *dvdInfo) {
    NandChunkStatus status = Error;
    UnkNandStruct *s = reinterpret_cast<UnkNandStruct *>(dvdInfo->block.userData);
    u32 length = dvdInfo->block.length;
    if (result == length) {
        if (!NANDWriteAsync(s->nandFileInfo, dvdInfo->block.addr, length, nandCallback, &s->nandBlock)) {
            status = NANDPending;
        }
    }
    s->updateStatus(status);
}

bool startDvdRead(UnkNandStruct *s, void *dst, s32 size, s32 offset) {
    s->waitForFinalization();

    if (s->isError()) {
        return false;
    }

    BOOL dvdResult = DVDReadAsyncPrio(&s->dvdFileInfo, dst, size, offset, dvdCallback, 2);
    NandChunkStatus status = dvdResult != 0 ? DVDPending : Error;
    s->updateStatus(status);
    return dvdResult;
}

extern "C" void fn_802DEFE0(const char *fmt, ...);

extern "C" bool fn_80053240(char *relPath, char *tmpRelPath, EGG::Heap *heap) {
    DVDFileInfo dvdFileInfo;
    NANDFileInfo nandFileInfo;

    s32 entryNum = DVDConvertPathToEntrynum(relPath);
    if (entryNum < 0 || !DVDFastOpen(entryNum, &dvdFileInfo)) {
        goto err;
    }

    u32 sizeRead = 0;
    u32 fileSize = ROUND_UP(dvdFileInfo.size, 0x20);
    NANDResult deleteResult = NANDDelete(tmpRelPath);
    if (deleteResult != NAND_RESULT_OK && deleteResult != NAND_RESULT_NOEXISTS) {
        goto end;
    }

    if (NANDCreate(tmpRelPath, NAND_PERM_RUSR | NAND_PERM_WUSR, 0) != NAND_RESULT_OK ||
        NANDOpen(tmpRelPath, &nandFileInfo, 2) != NAND_RESULT_OK) {
        goto end;
    }

    fn_802DEFE0("BufferSize %u KB\n", 0x200);

    void *buf = heap->alloc(0x80000, 0x20);
    if (buf != nullptr) {
        // Weird part start (everything above this is regshuffles)

        // TODO the initialization of this value is a bit
        // weird because it essentially computes the signed
        // division 0x80000 / 2
        //   (which adds +1 to the dividend if the result is negative to
        //   ensure rounding to 0 behavior)
        // and the optimizer fails to constant propagate these through
        // to realize that 0x80000 is actually always positive
        int totalBufSize = 0x80000;
        int chunkSize = 0x80000 / 2;

        // TODO this initalization happens slightly differently
        UnkNandStruct nandStructs[2];

        // Weird part end (most below this is regshuffles)

        nandStructs[0].open(entryNum, &nandFileInfo);
        nandStructs[1].open(entryNum, &nandFileInfo);

        // dispatch multiple requests to load parts of the profile REL in parallel
        int whichThread = 0;
        for (; sizeRead < fileSize; sizeRead += chunkSize) {
            if (chunkSize > fileSize - sizeRead) {
                chunkSize = fileSize - sizeRead;
            }
            // NB the whichThread * totalBufSize / 2 match relies on whichThread * totalBufSize being able to overflow
            if (!startDvdRead(
                    &nandStructs[whichThread], ((u8 *)buf) + whichThread * totalBufSize / 2, chunkSize,
                    /* offset */ sizeRead
                )) {
                break;
            }

            whichThread ^= 1;
        }

        nandStructs[0].waitForFinalization();
        nandStructs[1].waitForFinalization();

        if (nandStructs[0].isError() || nandStructs[1].isError()) {
            sizeRead = 0;
        }

        // TODO here are the inline dtors that mostly match, but their addressing is weird
        // and there's a check that &nandStructs[1] != nullptr before destructing it,
        // which is a bit weird for an inline dtor for a stack local
    }

    heap->free(buf);
    if (NANDClose(&nandFileInfo) != NAND_RESULT_OK) {
        sizeRead = 0;
    }

end:
    DVDClose(&dvdFileInfo);
    return sizeRead >= fileSize;

err:
    return false;
}

u32 loadRelsArcCallback(void *arg) {
    EGG::Archive *arc = nullptr;
    EGG::Heap *heap = DynamicModuleControl::sDylinkHeap;

    char rels[] = "/rels.arc";
    char tmpRels[] = "/tmp/rels.arc";

    if (fn_80053240(rels, tmpRels, heap)) {
        arc = EGG::Archive::loadFST(tmpRels, heap, 0x20);
    }
    if (arc != nullptr) {
        DynamicModuleControl::sArchive = arc;
    }
    return (u32)arc;
}

u32 dvdCallback(void *arg) {
    DynamicModuleControl::initialize(mHeap::g_dylinkHeap);
    DynamicModuleControl::sRelsDir = relsDir;
    DVDFileInfo info;

    if (DVDOpen("/RVZELDANP.str", &info)) {
        if (info.size != 0) {
            u32 size = ROUND_UP(info.size, 0x20);
            void *destination = DynamicModuleControl::sDylinkHeap->alloc(size, 0x20);
            DVDReadPrio(&info, destination, size, 0, 2);
            OSSetStringTable(destination);
        }
        DVDClose(&info);
    }

    BOOL result = g_DMC.link();
    do {
    } while (!result);
    Initialized = 1;
    return true;
}

void initModule() {
    initDylinkHeap(0x2bf, DYNAMIC_NAME_TABLE, 0x27c, mHeap::g_dylinkHeap);
    DVD = mDvd_callback_c::createOrFail(dvdCallback, mHeap::g_dylinkHeap);
}

void initRelsArc() {
    DVD = mDvd_callback_c::createOrFail(loadRelsArcCallback, nullptr);
}

bool destroy() {
    if (DVD == nullptr) {
        return true;
    }

    if (DVD->mStatus != 0) {
        DVD->do_delete();
        DVD = nullptr;
        return true;
    }

    return false;
}

} // namespace dDyl

dDynamicModuleControl::~dDynamicModuleControl() {
    if (mPtr != nullptr) {
        do_unlink();
    }
}

void dDynamicModuleControl::set(u16 *ptr, int count) {
    mPtr = ptr;
    mCount = count;
}

BOOL dDynamicModuleControl::do_link() const {
    if (mPtr == nullptr) {
        return true;
    }
    bool result = true;
    u16 *rel = mPtr;
    for (int i = 0; i < mCount; i++) {
        bool linked = dDyl::isLinked(*rel);
        if (!linked && dDyl::tryLink(*rel) != 1) {
            result = false;
            goto end;
        }
        rel++;
    }
end:
    return result;
}

BOOL dDynamicModuleControl::do_unlink() {
    u16 *rel = mPtr;
    if (rel == nullptr) {
        return true;
    }
    for (int i = 0; i < mCount; i++) {
        bool linked = dDyl::isLinked(*rel);
        if (linked && dDyl::Unlink(*rel) == 0) {
            return false;
        }
        rel++;
    }
    mPtr = nullptr;
    return true;
}
