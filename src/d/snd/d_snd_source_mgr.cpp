#include "d/snd/d_snd_source_mgr.h"

#include "common.h"
#include "d/snd/d_snd_source.h"
#include "nw4r/ut/ut_list.h"

#include <cmath>

SND_DISPOSER_DEFINE(dSndSourceMgr_c);

dSndSourceMgr_c::dSndSourceMgr_c()
    : field_0x0010(0),
      field_0x0011(0),
      field_0x0012(0),
      field_0x0013(0),
      field_0x3860(0),
      field_0x3864(0),
      field_0x3868(0),
      field_0x386C(INFINITY),
      mpPlayerSource(nullptr),
      mpKenseiSource(nullptr),
      mpBoomerangSource(nullptr),
      mpTBoatSource(nullptr),
      field_0x3880(nullptr),
      field_0x3884(nullptr) {
    // TODO offsetof
    nw4r::ut::List_Init(&mGroupList1, 0);
    nw4r::ut::List_Init(&mGroupList2, 0);

    // TODO figure out what these are for
    nw4r::ut::List_Init(&mGroupList3, 8);
    nw4r::ut::List_Init(&mAllSources, 0xE8);
    nw4r::ut::List_Init(&field_0x3848, 0x15C);
    nw4r::ut::List_Init(&field_0x3854, 0x160);

    mpDefaultGroup = new dSndSourceGroup_c(-1, "Default", 0, 0);

    for (dSndSourceGroup_c *group = &mGroups[0]; group < &mGroups[NUM_GROUPS]; group++) {
        nw4r::ut::List_Append(&mGroupList2, group);
    }
}

void dSndSourceMgr_c::registerSource(dSoundSource_c *source) {
    if (source != nullptr) {
        nw4r::ut::List_Append(&mAllSources, source);
        // TODO enums
        switch (source->getCategory()) {
            case 0: {
                if (source->getActorType() == 0 && mpPlayerSource == nullptr) {
                    mpPlayerSource = source;
                }
                break;
            }
            case 1: {
                if (source->getActorType() == 6) {
                    mpBoomerangSource = source;
                }
                break;
            }
            case 2: {
                if (isCertainEnemyType(source)) {
                    nw4r::ut::List_Append(&field_0x3848, source);
                }
                break;
            }
            case 6: {
                nw4r::ut::List_Append(&field_0x3854, source);
                break;
            }
            case 3: {
                if (source->isName("TBoat") && mpTBoatSource == nullptr) {
                    mpTBoatSource = source;
                }
                break;
            }
            case 4: {
                if (source->getActorType() == 44) {
                    mpKenseiSource = source;
                }
                break;
            }
        }
    }
}

void dSndSourceMgr_c::unregisterSource(dSoundSource_c *source) {
    if (source != nullptr) {
        removeSourceFromList(source, &mAllSources);
        removeSourceFromList(source, &field_0x3848);
        removeSourceFromList(source, &field_0x3854);
        if (source == mpPlayerSource) {
            mpPlayerSource = nullptr;
        } else if (source == mpKenseiSource) {
            mpKenseiSource = nullptr;
        } else if (source == mpBoomerangSource) {
            mpBoomerangSource = nullptr;
        }

        if (mpTBoatSource == source) {
            mpTBoatSource = nullptr;
        }
    }
}

void dSndSourceMgr_c::removeSourceFromList(dSoundSource_c *source, nw4r::ut::List *list) {
    if (source != nullptr && list != nullptr) {
        // This removal code appears to be needlessly defensive
        dSoundSource_c *sourceIter = static_cast<dSoundSource_c *>(nw4r::ut::List_GetFirst(list));
        while (sourceIter != nullptr) {
            if (field_0x3880 == source) {
                // why in here????
                field_0x3880 = nullptr;
            }

            if (sourceIter == source) {
                nw4r::ut::List_Remove(list, sourceIter);
                sourceIter = nullptr;
            } else {
                sourceIter = static_cast<dSoundSource_c *>(nw4r::ut::List_GetNext(list, sourceIter));
            }
        }
    }
}

void dSndSourceMgr_c::clearSourceLists() {
    clearSourceList(&mAllSources);
    clearSourceList(&field_0x3848);
    clearSourceList(&field_0x3854);
    mpPlayerSource = nullptr;
}

void dSndSourceMgr_c::clearSourceList(nw4r::ut::List *list) {
    if (list != nullptr) {
        dSoundSource_c *sourceIter = static_cast<dSoundSource_c *>(nw4r::ut::List_GetFirst(list));
        while (sourceIter != nullptr) {
            nw4r::ut::List_Remove(list, sourceIter);
            sourceIter = static_cast<dSoundSource_c *>(nw4r::ut::List_GetFirst(list));
        }
    }
}
