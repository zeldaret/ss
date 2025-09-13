#include "d/snd/d_snd_id_mappers.h"

#include "common.h"
#include "d/col/bg/d_bg_pc.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_source_group.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_id_mappers_data.h"
#include "sized_string.h"

const char *getBaseVariant(const char *name) {
    const ActorBaseNamePair *pair = sActorBaseNamePairs;
    for (int i = 0; i < sNumActorBaseNamePairs; i++) {
        if (streq(name, sActorBaseNamePairs[i].variant)) {
            return sActorBaseNamePairs[i].base;
        }
    }
    return nullptr;
}

u32 getGrpId(s32 sourceType, const char *name) {
    if (name == nullptr) {
        return -1;
    }
    if (sourceType < 0) {
        return -1;
    }

    if (sourceType >= SND_SOURCE_59 + 1) {
        return -1;
    }

    SizedString<64> label;
    label.sprintf("GRP_%s", name);
    u32 id = dSndMgr_c::GetInstance()->getArchive()->ConvertLabelStringToGroupId(label);
    if (id != -1) {
        return id;
    }
    const char *base = getBaseVariant(name);
    if (base != nullptr) {
        label.sprintf("GRP_%s", base);
        id = dSndMgr_c::GetInstance()->getArchive()->ConvertLabelStringToGroupId(label);
        return id;
    }
    return -1;
}

u32 getGrpId(dSndSourceGroup_c *pGroup) {
    if (pGroup == nullptr) {
        return -1;
    }
    u32 id = getGrpId(pGroup->getSourceType(), pGroup->getName());
    if (id == -1 && pGroup->getOrigName() != nullptr) {
        id = getGrpId(pGroup->getSourceType(), pGroup->getOrigName());
    }
    return id;
}

u32 getBnkSeId(s32 sourceType, const char *name) {
    if (name == nullptr) {
        return -1;
    }
    if (sourceType < 0) {
        return -1;
    }

    if (sourceType >= SND_SOURCE_59 + 1) {
        return -1;
    }

    SizedString<64> label;
    label.sprintf("BNK_SE_%s", name);
    u32 id = dSndMgr_c::GetInstance()->getArchive()->ConvertLabelStringToBankId(label);
    if (id != -1) {
        return id;
    }
    const char *base = getBaseVariant(name);
    if (base != nullptr) {
        label.sprintf("BNK_SE_%s", base);
        id = dSndMgr_c::GetInstance()->getArchive()->ConvertLabelStringToBankId(label);
        return id;
    }
    return -1;
}

u32 getBnkSeId(dSndSourceGroup_c *pGroup) {
    if (pGroup == nullptr) {
        return -1;
    }
    u32 id = getBnkSeId(pGroup->getSourceType(), pGroup->getName());
    if (id == -1 && pGroup->getOrigName() != nullptr) {
        id = getBnkSeId(pGroup->getSourceType(), pGroup->getOrigName());
    }
    return id;
}

u32 getSeId(s32 sourceType, const char *name) {
    if (name == nullptr) {
        return -1;
    }
    if (sourceType < 0) {
        return -1;
    }

    if (sourceType >= SND_SOURCE_59 + 1) {
        return -1;
    }

    SizedString<64> label;
    label.sprintf("SE_%s", name);
    u32 id = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
    if (id != -1) {
        return id;
    }
    const char *base = getBaseVariant(name);
    if (base != nullptr) {
        label.sprintf("SE_%s", base);
        id = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
        return id;
    }
    return -1;
}

u32 getSeId(dSndSourceGroup_c *pGroup) {
    if (pGroup == nullptr) {
        return -1;
    }
    u32 id = getSeId(pGroup->getSourceType(), pGroup->getName());
    if (id == -1 && pGroup->getOrigName() != nullptr) {
        id = getSeId(pGroup->getSourceType(), pGroup->getOrigName());
    }
    return id;
}

#pragma push
#pragma readonly_strings on
// TODO could be yet another file
const char *sSndHitEffects[] = {
    "TUTI",  "ROCK", "SAND", "GRASS", "TREE",   "LAVA",  "WATER", "STONE", "LOTUS",
    "METAL", "NUMA", "TUTA", "LIFE",  "CARPET", "QSAND", "WOOD",  "DEATH",
};

#pragma pop

const char *getHitEffectName(u32 polyAttr0) {
    if (polyAttr0 >= 1 && polyAttr0 < POLY_ATT_0_MAX) {
        polyAttr0 -= 1;
        return sSndHitEffects[polyAttr0];
    }
    return "";
}
