#include "d/snd/d_snd_source_mgr.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source.h"

// clang-format off
#include "d/snd/d_snd_source_e_spark.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_source_equipment.h"
#include "d/snd/d_snd_source_group.h"
#include "d/snd/d_snd_source_py_bird.h"
#include "d/snd/d_snd_source_enemy.h"
#include "d/snd/d_snd_source_obj_clef.h"
#include "d/snd/d_snd_source_obj.h"
#include "d/snd/d_snd_source_npc.h"
#include "d/snd/d_snd_source_harp_related.h"

#include "d/snd/d_snd_source_demo.h"
#include "d/snd/d_snd_source_player.h"
#include "d/snd/d_snd_source_player_head.h"
#include "d/snd/d_snd_source_npc_head.h"
#include "d/snd/d_snd_source_npc_special.h"
#include "d/snd/d_snd_source_tg_sound.h"
// clang-format on

#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/ut/ut_list.h"
#include "sized_string.h"

#include <cmath>

// TODO move
struct ActorBaseNamePair {
    const char *variant;
    const char *base;
};
extern "C" const ActorBaseNamePair Actor_BaseActorName_Pairs[];
extern "C" const s32 lbl_8057E394;

s32 dSndSourceMgr_c::getSourceCategoryForSourceType(s32 sourceType, const char *name) {
    // This might be a full-on switch statement but I don't want to write out
    // all the unknown entries yet and this matches anyway

    if (sourceType >= SND_SOURCE_PLAYER && sourceType <= SND_SOURCE_PLAYER_HEAD) {
        return SND_SOURCE_CATEGORY_PLAYER;
    }

    if (sourceType >= SND_SOURCE_NET && sourceType <= SND_SOURCE_HOOKSHOT) {
        return SND_SOURCE_CATEGORY_EQUIPMENT;
    }

    if (sourceType >= SND_SOURCE_ENEMY_10 && sourceType <= SND_SOURCE_ENEMY_31) {
        return SND_SOURCE_CATEGORY_ENEMY;
    }

    if (sourceType >= SND_SOURCE_OBJECT && sourceType <= SND_SOURCE_OBJECT_42) {
        return SND_SOURCE_CATEGORY_OBJECT;
    }

    if (sourceType >= SND_SOURCE_NPC_43 && sourceType <= SND_SOURCE_NPC_DRAGON) {
        return SND_SOURCE_CATEGORY_NPC;
    }

    if (sourceType == SND_SOURCE_TG_SOUND) {
        return SND_SOURCE_CATEGORY_TG_SOUND;
    }

    if (sourceType >= SND_SOURCE_54 && sourceType <= SND_SOURCE_TG_HARP) {
        return SND_SOURCE_CATEGORY_HARP_RELATED;
    }

    switch (sourceType) {
        case SND_SOURCE_58: return SND_SOURCE_CATEGORY_7;
        case SND_SOURCE_59: return SND_SOURCE_CATEGORY_9;
        default:            return -1;
    }
}

dSoundSource_c *dSndSourceMgr_c::createSource(s32 sourceType, dAcBase_c *actor, const char *name, u8 _subtype) {
    if (actor == nullptr) {
        return nullptr;
    }

    u8 subtype = actor->subtype;
    SizedString<64> nameStr;
    nameStr.sprintf("%s", name);

    bool isModified = false;

    if (dSndStateMgr_c::GetInstance()->isInDemo() != nullptr && strneq(name, "$act", 4)) {
        nameStr.sprintf("%s_%s", name + 1, dSndStateMgr_c::GetInstance()->getCurrentStageMusicDemoName());
        isModified = true;
    }

    dSoundSource_c *existingSource = static_cast<dSoundSource_c *>(actor->getSoundSource());
    if (!isModified) {
        bool allowSubtype = true;
        switch (sourceType) {
            case SND_SOURCE_BIGBOSS:
                nameStr = "BBigBos";
                isModified = true;
                break;
            case SND_SOURCE_GIRAHUMU_3:
                nameStr = "BGh3";
                isModified = true;
                break;
            case SND_SOURCE_NPC_HEAD:
                if (existingSource != nullptr) {
                    name = existingSource->getName();
                    nameStr.sprintf("%sHead", name);
                    allowSubtype = false;
                }
                break;
        }

        if (strneq(name, "NpcMole", 7)) {
            if (subtype != 0) {
                nameStr = "NpcMoT";
                subtype = 0;
                isModified = true;
            }
        } else if (strneq(name, "NpcMoT2", 7)) {
            nameStr = "NpcMoT";
            isModified = true;
        } else if (strneq(name, "NpcMoN2", 7)) {
            nameStr = "NpcMoN";
            isModified = true;
        } else if (strneq(name, "NpcMoEN", 7)) {
            nameStr = "NpcMole";
            subtype = 0;
            isModified = true;
        } else if (strneq(name, "NpcMoS", 6)) {
            nameStr = "NpcMoE1";
            isModified = true;
        }

        if (allowSubtype && subtype != 0) {
            nameStr.sprintf("%s_A%d", &nameStr, subtype);
            isModified = true;
        }

        if (sourceType == SND_SOURCE_NPC_NRM) {
            const ActorBaseNamePair *pair = Actor_BaseActorName_Pairs;
            for (int i = 0; i < lbl_8057E394; i++) {
                if (streq(nameStr, Actor_BaseActorName_Pairs[i].variant)) {
                    nameStr = Actor_BaseActorName_Pairs[i].base;
                    isModified = true;
                    break;
                }
            }
        }
    }

    const char *actualName = nameStr;
    s32 category = getSourceCategoryForSourceType(sourceType, actualName);
    dSndSourceGroup_c *group = nullptr;

    if (category != SND_SOURCE_CATEGORY_9) {
        if (isModified) {
            group = GetInstance()->findGroup(sourceType, actor, actualName, name, subtype);
        } else {
            group = GetInstance()->findGroup(sourceType, actor, actualName, nullptr, subtype);
        }
        actualName = group->getName();
    }

    s32 sourceCategory = getSourceCategoryForSourceType(sourceType, actualName);
    dSoundSource_c *newSource = nullptr;
    bool isAnimSource = isAnimSoundSource(sourceType, actualName);
    bool isMultiSource = isMultiSoundSource(sourceType, actualName);
    bool isDemo = dSndPlayerMgr_c::GetInstance()->canUseThisPlayer(sourceType);

    if (isDemo) {
        newSource = new dSndSourceDemo_c(sourceType, actor, actualName, group);
    }

    if (newSource == nullptr) {
        switch (sourceCategory) {
            case SND_SOURCE_CATEGORY_PLAYER:
                if (sourceType == SND_SOURCE_PLAYER_HEAD) {
                    newSource = new dSndSourcePlayerHead_c(sourceType, actor, actualName, group);
                } else {
                    newSource = new dSndSourcePlayer_c(sourceType, actor, actualName, group);
                }
                break;
            case SND_SOURCE_CATEGORY_ENEMY:
                if (sourceType == SND_SOURCE_SPARK) {
                    newSource = new dSndSourceESpark_c(sourceType, actor, actualName, group);
                } else if (isAnimSource) {
                    if (isMultiSource) {
                        newSource = new dSndSourceEnemyMulti_c(sourceType, actor, actualName, group);
                    } else {
                        newSource = new dSndSourceEnemyAnim_c(sourceType, actor, actualName, group);
                    }
                } else {
                    newSource = new dSndSourceEnemy_c(sourceType, actor, actualName, group);
                }
                break;
            case SND_SOURCE_CATEGORY_OBJECT:
                if (sourceType == SND_SOURCE_OBJECT_33) {
                    return nullptr;
                }
                if (sourceType == SND_SOURCE_LIGHT_SHAFT) {
                    newSource = new dSndSourceObjLightShaft_c(sourceType, actor, actualName, group);
                } else if (sourceType == SND_SOURCE_CLEF) {
                    newSource = new dSndSourceObjClef_c(sourceType, actor, actualName, group);
                } else if (isAnimSource) {
                    newSource = new dSndSourceObjAnim_c(sourceType, actor, actualName, group);
                } else {
                    newSource = new dSndSourceObj_c(sourceType, actor, actualName, group);
                }
                break;
            case SND_SOURCE_CATEGORY_EQUIPMENT:
                if (sourceType == SND_SOURCE_WHIP) {
                    newSource = new dSndSourceEquipmentWhip_c(sourceType, actor, actualName, group);
                } else {
                    newSource = new dSndSourceEquipment_c(sourceType, actor, actualName, group);
                }
                break;
            case SND_SOURCE_CATEGORY_NPC:
                if (sourceType == SND_SOURCE_NPC_HEAD) {
                    newSource = new dSndSourceNpcHead_c(sourceType, actor, actualName, group);
                } else if (sourceType == SND_SOURCE_NPC_DRAGON) {
                    newSource = new dSndSourceNpcDr_c(sourceType, actor, actualName, group);
                } else if (sourceType >= SND_SOURCE_NPC_50) {
                    newSource = new dSndSourceNpcSpecial_c(sourceType, actor, actualName, group);
                } else if (sourceType == SND_SOURCE_PLAYER_BIRD) {
                    newSource = new dSndSourcePyBird_c(sourceType, actor, actualName, group);
                } else if (isAnimSource) {
                    newSource = new dSndSourceNpcAnim_c(sourceType, actor, actualName, group);
                } else {
                    newSource = new dSndSourceNpc_c(sourceType, actor, actualName, group);
                }
                break;
            case SND_SOURCE_CATEGORY_TG_SOUND:
                newSource = new dSndSourceTgSound_c(sourceType, actor, actualName, group);
                break;
            case SND_SOURCE_CATEGORY_HARP_RELATED:
                // TODO
                new dSndSourceHarpTg_c(0, actor, name, nullptr);
                break;
            case SND_SOURCE_CATEGORY_7: newSource = new dSndSourceDemo_c(sourceType, actor, actualName, group); break;
            default:
                if (sourceType < SND_SOURCE_59 + 1) {
                    GetInstance()->fn_803846D0(sourceType, actualName, subtype);
                }
                return nullptr;
        }
    }

    if (newSource == nullptr) {
        return nullptr;
    }

    newSource->setSubtype(subtype);
    newSource->setup();
    if (!streq(name, actualName)) {
        newSource->setOrigName(name);
    }

    if (sourceType == SND_SOURCE_NPC_HEAD) {
        if (existingSource != nullptr && strneq(existingSource->getName(), "NpcMoT", 6)) {
            static_cast<dSndSourceNpcHead_c *>(newSource)->setMainName(existingSource->getName());
        } else if (existingSource != nullptr && streq(name, "NpcGrd")) {
            static_cast<dSndSourceNpcHead_c *>(newSource)->setMainName("NpcGra");
        }
    }

    if (existingSource != nullptr && existingSource != newSource && sourceType != SND_SOURCE_NPC_HEAD && existingSource->isMultiSource()) {
        existingSource->registerAdditionalSource(newSource);
    }

    return newSource;
}

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
        switch (source->getCategory()) {
            case SND_SOURCE_CATEGORY_PLAYER: {
                if (source->getSourceType() == SND_SOURCE_PLAYER && mpPlayerSource == nullptr) {
                    mpPlayerSource = source;
                }
                break;
            }
            case SND_SOURCE_CATEGORY_EQUIPMENT: {
                if (source->getSourceType() == SND_SOURCE_BOOMERANG) {
                    mpBoomerangSource = source;
                }
                break;
            }
            case SND_SOURCE_CATEGORY_ENEMY: {
                if (isCertainEnemyType(source)) {
                    nw4r::ut::List_Append(&field_0x3848, source);
                }
                break;
            }
            case SND_SOURCE_CATEGORY_HARP_RELATED: {
                nw4r::ut::List_Append(&field_0x3854, source);
                break;
            }
            case SND_SOURCE_CATEGORY_OBJECT: {
                if (streq(source->getName(), "TBoat") && mpTBoatSource == nullptr) {
                    mpTBoatSource = source;
                }
                break;
            }
            case SND_SOURCE_CATEGORY_NPC: {
                if (source->getSourceType() == SND_SOURCE_KENSEI) {
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

void dSndSourceMgr_c::onShutdownSource(dSoundSource_c *source) {
    if (source == nullptr) {
        return;
    }
    removeSourceFromList(source, &field_0x3848);
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

s32 dSndSourceMgr_c::getPlayerSourceRoomId() const {
    if (mpPlayerSource == nullptr) {
        return -1;
    }
    if (getBoomerangSource() != nullptr) {
        return getBoomerangSource()->getRoomId();
    }
    return mpPlayerSource->getRoomId();
}

struct dSndSourceMgrEmptySinit {
    dSndSourceMgrEmptySinit() {}
};

dSndSourceMgrEmptySinit emptySinit;
