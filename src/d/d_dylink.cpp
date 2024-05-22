#include <common.h>
#include <DynamicLink.h>
#include <d/d_dylink.h>
#include <m/m_dvd.h>
#include <m/m_heap.h>
// clang-format off
#include <MSL_C/string.h>
// clang-format on

struct RelNamePtr {
    u16 relId;
    const char *name;
};

static RelNamePtr *pDynamicNameTable;
static int nDynamicNameTable;

namespace dDyl {

DynamicModuleControl **pDMC;
int nDMC;
EGG::Heap *cCc_frmHeap;
u32 Initialized;
mDvd_callback_c *DVD;

// initDylinkHeap
extern "C" int fn_80052E00(int maxRelId, RelNamePtr *dynNameTable, int dynNameTableNum, EGG::Heap *parentHeap) {
    cCc_frmHeap = mHeap::createFrmHeap(maxRelId * 0x10 + dynNameTableNum * 0x48, parentHeap,
            "ダイナミックリンク制御用ヒープ(dDyl::cCc_frmHeap)", 0x20, 0);

    mHeap _guard(cCc_frmHeap);

    nDMC = maxRelId;
    pDMC = new DynamicModuleControl *[maxRelId]();
    memset(pDMC, 0, sizeof(void *) * nDMC);

    pDynamicNameTable = dynNameTable;
    nDynamicNameTable = dynNameTableNum;

    for (int i = 0; i < nDynamicNameTable; i++) {
        RelNamePtr *rel = &pDynamicNameTable[i];
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

// isLoaded?
extern "C" bool fn_80052FA0(u16 relId) {
    return pDMC[relId] != nullptr ? pDMC[relId]->isLinked() : true;
}

bool dDyl::Unlink(u16 relId) {
    DynamicModuleControl *dmc = pDMC[relId];
    if (dmc != nullptr) {
        return dmc->unlink();
    }

    return false;
}

extern "C" int fn_80052FF0(u16 relId) {
    if (!Initialized) {
        return 0;
    }

    DynamicModuleControl *dmc = pDMC[relId];
    if (dmc != nullptr) {
        if (dmc->load_async()) {
            // what is going on here?
            return (bool)dmc->link() + 2;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

} // namespace dDyl
