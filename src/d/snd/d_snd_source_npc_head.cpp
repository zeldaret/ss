#include "d/snd/d_snd_source_npc_head.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_rng_mgr.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_group.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"
#include "sized_string.h"

dSndSourceNpcHead_c::dSndSourceNpcHead_c(
    s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup
)
    : dSoundSource_c(sourceType, ac, name, pOwnerGroup), field_0x15C(0) {
    if (dSndStateMgr_c::isInStage("F001r") && streq(name, "NpcCbFdHead") && ac->mRoomID == 1) {
        // Fledge in Knight's Academy
        pOwnerGroup->getAmbientParam()->field_0x00 = 100.0f;
        pOwnerGroup->getAmbientParam()->field_0x04 = 300.0f;
    }
}

bool dSndSourceNpcHead_c::npcSpeak(u32 id) {
    if (id == -1) {
        return false;
    }

    if (id < (LABEL_NV_END - LABEL_NV_START)) {
        // turn SE_NV_123 into SE_NV_123_NpcZld
        SizedString<64> nvVariantLabel;
        const char *baseLabel = dSndMgr_c::getSoundLabelString(id + LABEL_NV_START);
        nvVariantLabel.sprintf("%s_%s", baseLabel, mpMainName);
        id = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(nvVariantLabel);
        if (id == -1) {
            return false;
        }
    } else if (id < LABEL_NV_START || id >= LABEL_SE_AMBIENT_START) {
        // This condition allows all the SE_TgSound sounds...
        return false;
    }

    // if there are _R1, _R2 variants, pick a random one
    SizedString<64> label;
    if (checkUserParamMask(id, 0x20)) {
        label.sprintf("%s_%s", dSndMgr_c::getSoundLabelString(id), "R1");

        if (label == dSndMgr_c::getSoundLabelString(id + 1)) {
            // _R1 variant directly follows base variant
            u32 randRange = 2;

            label.sprintf("%s_%s", dSndMgr_c::getSoundLabelString(id), "R2");
            if (label == dSndMgr_c::getSoundLabelString(id + 2)) {
                // base, base_R1, base_R2
                randRange = 3;
            }
            id += dSndRngMgr_c::GetInstance()->rndInt(randRange);
        }
    }

    return startSound(id, nullptr);
}

bool dSndSourceNpcHead_c::startVoiceLine(u32 id) {
    a_field_0x7D = false;
    return npcSpeak(id);
}

bool dSndSourceNpcHead_c::vt_0xDC(u32 id) {
    a_field_0x7D = true;
    return npcSpeak(id);
}

void dSndSourceNpcHead_c::setOrigName(const char *arg) {
    mpOrigName = arg;
    mpMainName = arg;
}
