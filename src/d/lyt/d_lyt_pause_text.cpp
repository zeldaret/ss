#include "d/lyt/d_lyt_pause_text.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/d_message.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_pause.h"
#include "d/lyt/d_lyt_util_items.h"
#include "d/lyt/d_textbox.h"
#include "nw4r/lyt/lyt_group.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

STATE_DEFINE(dLytPauseText_c, None);
STATE_DEFINE(dLytPauseText_c, In);
STATE_DEFINE(dLytPauseText_c, Wait);
STATE_DEFINE(dLytPauseText_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "iconText_00_in.brlan", "G_inOut_00"},
    {"iconText_00_loop.brlan",  "G_loop_00"},
    { "iconText_00_out.brlan", "G_inOut_00"},
};

#define PAUSE_TEXT_ANIM_IN 0
#define PAUSE_TEXT_ANIM_LOOP 1
#define PAUSE_TEXT_ANIM_OUT 2

#define PAUSE_TEXT_NUM_ANIMS 3

static const char *sGroupName = "G_ref_00";

dLytPauseText_c::dLytPauseText_c() : mStateMgr(*this) {}

bool dLytPauseText_c::build() {
    d2d::ResAccIf_c *resAcc = dLytPauseMgr_c::GetInstance()->getResAcc1();
    mLyt.setResAcc(resAcc);
    mLyt.build("iconText_00.brlyt", nullptr);
    mLyt.setPriority(0x86);

    for (int i = 0; i < PAUSE_TEXT_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
    }

    void *data = LayoutArcManager::GetInstance()->getLoadedData("System2D");
    mResAcc.attach(data, "");

    mSubpanes[0].mpLytPane = &mABtn;
    for (int i = 0; i < 1; i++) {
        mSubpanes[i].mpLytPane->build(&mResAcc);
        mSubpaneList.PushBack(&mSubpanes[i]);
    }

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mSubpaneList);
        }
    }

    mLineSpace = mLyt.getTextBox("T_text_00")->GetLineSpace();
    mStateMgr.changeState(StateID_None);

    return true;
}

bool dLytPauseText_c::remove() {
    mLyt.unbindAnims();

    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < 1; i++) {
        mSubpanes[i].mpLytPane->remove();
    }
    for (int i = 0; i < PAUSE_TEXT_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }

    mResAcc.detach();

    return true;
}

bool dLytPauseText_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < 1; i++) {
        mSubpanes[i].mpLytPane->execute();
    }
    mLyt.calc();
    return true;
}

bool dLytPauseText_c::draw() {
    if (field_0x6D8 == true) {
        mLyt.addToDrawList();
    }
    return true;
}

void dLytPauseText_c::requestIn() {
    mInRequest = true;
}

void dLytPauseText_c::requestOut() {
    mOutRequest = true;
}

void dLytPauseText_c::initializeState_None() {
    mLyt.unbindAnims();
    field_0x6D7 = false;
    mInRequest = false;
    mOutRequest = false;
    field_0x6D8 = false;
    mStep = 0;
    mABtn.resetToInvisble();
    updateText();
}
void dLytPauseText_c::executeState_None() {
    if (mInRequest == true) {
        mInRequest = false;
        mStateMgr.changeState(StateID_In);
    }
}
void dLytPauseText_c::finalizeState_None() {}

void dLytPauseText_c::initializeState_In() {
    updateText();
    startAnim(PAUSE_TEXT_ANIM_IN);
    startAnim(PAUSE_TEXT_ANIM_LOOP);
    mABtn.requestIn();
    field_0x6D8 = true;
}
void dLytPauseText_c::executeState_In() {
    d2d::AnmGroup_c &anm = mAnm[PAUSE_TEXT_ANIM_IN];
    if (anm.isEndReached() == true) {
        mStateMgr.changeState(StateID_Wait);
        mAnm[PAUSE_TEXT_ANIM_LOOP].play();
    } else {
        anm.play();
        mAnm[PAUSE_TEXT_ANIM_LOOP].play();
    }
}
void dLytPauseText_c::finalizeState_In() {}

void dLytPauseText_c::initializeState_Wait() {
    field_0x6D7 = true;
}
void dLytPauseText_c::executeState_Wait() {
    mAnm[PAUSE_TEXT_ANIM_LOOP].play();
    field_0x6D7 = false;
    if (mOutRequest == true && mABtn.requestOut(false)) {
        mOutRequest = false;
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytPauseText_c::finalizeState_Wait() {}

void dLytPauseText_c::initializeState_Out() {
    stopAnim(PAUSE_TEXT_ANIM_IN);
    startAnim(PAUSE_TEXT_ANIM_OUT);
    mStep = 0;
}
void dLytPauseText_c::executeState_Out() {
    d2d::AnmGroup_c &anm = mAnm[PAUSE_TEXT_ANIM_OUT];
    switch (mStep) {
        case 0: {
            if (mABtn.isDoneOut()) {
                mStep = 1;
            }
            return;
        }
        case 1: {
            if (anm.isEndReached() == true) {
                mStep = 2;
                field_0x6D7 = true;
            }
            break;
        }
        case 2: {
            mStateMgr.changeState(StateID_None);
            return;
        }
    }
    anm.play();
    mAnm[PAUSE_TEXT_ANIM_LOOP].play();
}
void dLytPauseText_c::finalizeState_Out() {}

void dLytPauseText_c::startAnim(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.bind(false);
    anm.setFrame(0.0f);
}

void dLytPauseText_c::stopAnim(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.unbind();
}

void dLytPauseText_c::updateText() {
// "planned implementation"
#define TODO_TEXT (L"実装予定")
    dTextBox_c *text0 = mLyt.getTextBox("T_text_00");
    dTextBox_c *text0S = mLyt.getTextBox("T_textS_00");
    dTextBox_c *name0 = mLyt.getTextBox("T_name_00");
    dTextBox_c *name0S = mLyt.getTextBox("T_nameS_00");

    const wchar_t *text = getCurrentPausePopupName();
    if (text != nullptr) {
        name0->setTextWithGlobalTextProcessor(text);
        name0S->setTextWithGlobalTextProcessor(text);
    } else {
        name0->setTextWithGlobalTextProcessor(TODO_TEXT);
        name0S->setTextWithGlobalTextProcessor(TODO_TEXT);
    }

    if (isCurrentItemWallet() && dMessage_c::getGlobalTagProcessor() != nullptr) {
        dMessage_c::getGlobalTagProcessor()->setNumericArg0(dAcItem_c::getCurrentWalletCapacity());
    }

    text = getCurrentPausePopupCaption();
    calcTextLines(text0, text);
    if (text != nullptr) {
        if (mNumTextLines == 1) {
            text0->SetLineSpace(0.0f);
            text0S->SetLineSpace(0.0f);
        } else {
            text0->SetLineSpace(mLineSpace);
            text0S->SetLineSpace(mLineSpace);
        }
        text0->setTextWithGlobalTextProcessor(text);
        text0S->setTextWithGlobalTextProcessor(text);
    } else {
        text0->setTextWithGlobalTextProcessor(TODO_TEXT);
        text0S->setTextWithGlobalTextProcessor(TODO_TEXT);
    }

#undef TODO_TEXT
}

const char *dLytPauseText_c::getCurrentPauseLabel(SizedString<32> &buf) {
    // This is for the title - it's used by a different component (dLytPauseInfo_c)
    const char *result = nullptr;
    dLytPauseMgr_c::SelectionType_e selectionType = dLytPauseMgr_c::GetInstance()->getCurrentSelectionType();
    s32 id = dLytPauseMgr_c::GetInstance()->getCurrentSelectionId();

    switch (selectionType) {
        case dLytPauseMgr_c::SELECT_DOWSING: {
            getDowsingLabel(id, buf);
            if (buf.len() != 0) {
                result = buf;
            }
            break;
        }
        case dLytPauseMgr_c::SELECT_CATEGORY: {
            getPauseCategoryLabel(id, buf);
            if (buf.len() != 0) {
                result = buf;
            }
            break;
        }
        case dLytPauseMgr_c::SELECT_ITEM: {
            switch (id) {
                case ITEM_TRI_COURAGE:
                case ITEM_TRI_POWER:
                case ITEM_TRI_WISDOM:  id = ITEM_COMPLETE_TRIFORCE; break;
            }
            getItemLabel(id, buf);
            if (buf.len() != 0) {
                result = buf;
            }
            break;
        }
        case dLytPauseMgr_c::SELECT_FIRE: {
            buf.empty();
            buf.sprintf("NAME_FIRE_%03d", id);
            result = buf;
            break;
        }
        case dLytPauseMgr_c::SELECT_RING: {
            buf.empty();
            buf.sprintf("SUB_RING_%02d", id);
            result = buf;
            break;
        }
        default:
            getItemLabel(id, buf);
            if (buf.len() != 0) {
                result = buf;
            }
            break;
    }

    return result;
}

const wchar_t *dLytPauseText_c::getCurrentPausePopupName() {
    dLytPauseMgr_c::SelectionType_e selectionType = dLytPauseMgr_c::GetInstance()->getCurrentSelectionType();
    s32 id = dLytPauseMgr_c::GetInstance()->getCurrentSelectionId();

    switch (selectionType) {
        case dLytPauseMgr_c::SELECT_DOWSING: return getDowsingText(id);
        case dLytPauseMgr_c::SELECT_ITEM:
            switch (id) {
                case ITEM_TRI_COURAGE:
                case ITEM_TRI_POWER:
                case ITEM_TRI_WISDOM:  return dMessage_c::getTextMessageByLabel("NAME_ITEM_005", true, nullptr, 0);
                default:               return getItemText(id);
            }
        case dLytPauseMgr_c::SELECT_CATEGORY: return getPauseCategoryText(id);
        case dLytPauseMgr_c::SELECT_FIRE:     {
            SizedString<32> buf;
            buf.sprintf("NAME_FIRE_%03d", id);
            return dMessage_c::getTextMessageByLabel(buf, true, nullptr, 0);
        }
        default: return getItemText(id);
    }
}

const wchar_t *dLytPauseText_c::getCurrentPausePopupCaption() {
    // TODO - bunch of unresolved IDs here
    dLytPauseMgr_c::SelectionType_e selectionType = dLytPauseMgr_c::GetInstance()->getCurrentSelectionType();

    if (dLytPauseMgr_c::GetInstance()->getField_0x0841()) {
        // "It has been stolen by monsters! You've got to get it back."
        return dMessage_c::getTextMessageByLabel("CAPTION_ITEM_STOLEN", true, nullptr, 0);
    }

    s32 id = dLytPauseMgr_c::GetInstance()->getCurrentSelectionId();

    switch (selectionType) {
        case dLytPauseMgr_c::SELECT_DOWSING: {
            if (id == 12) {
                if (ItemflagManager::sInstance->getFlagDirect(502) == 80) {
                    // "You've collected all the Gratitude Crystals. There are none left for your sword to react to."
                    return dMessage_c::getTextMessageByLabel("CAPTION_DOWSING_012_02", true, nullptr, 0);
                } else {
                    // "Use your dowsing ability to detect the Gratitude Crystals that the demon Batreaux wants to see."
                    return dMessage_c::getTextMessageByLabel("CAPTION_DOWSING_012", true, nullptr, 0);
                }
            } else if (id == 14) {
                if (StoryflagManager::sInstance->getCounterOrFlag(1087) == 27) {
                    // "You've activated all of the Goddess Cubes. There are none left for your sword to react to."
                    return dMessage_c::getTextMessageByLabel("CAPTION_DOWSING_014_02", true, nullptr, 0);
                } else {
                    // "Use your dowsing ability to detect Goddess Cubes and their corresponding treasure chests in the
                    // sky above."
                    return dMessage_c::getTextMessageByLabel("CAPTION_DOWSING_014", true, nullptr, 0);
                }
            } else {
                SizedString<32> buf;
                buf.sprintf("CAPTION_DOWSING_%03d", id);
                return dMessage_c::getTextMessageByLabel(buf, true, nullptr, 0);
            }
        }
        case dLytPauseMgr_c::SELECT_ITEM: {
            if (id == ITEM_TRI_COURAGE || id == ITEM_TRI_POWER || id == ITEM_TRI_WISDOM) {
                if (id == ITEM_TRI_COURAGE) {
                    return dMessage_c::getTextMessageByLabel("CAPTION_ITEM_005", true, nullptr, 0);
                } else if (id == ITEM_TRI_POWER) {
                    return dMessage_c::getTextMessageByLabel("CAPTION_ITEM_005_02", true, nullptr, 0);
                } else {
                    return dMessage_c::getTextMessageByLabel("CAPTION_ITEM_005_03", true, nullptr, 0);
                }
            } else if (id == ITEM_HEART_PIECE) {
                if (dAcItem_c::getHeartContainerHealthCount() == 24) {
                    // "You've collected all the Pieces of Heart."
                    return dMessage_c::getTextMessageByLabel("CAPTION_ITEM_094_02", true, nullptr, 0);
                } else {
                    return getCaptionItemText(id);
                }
            } else {
                return getCaptionItemText(id);
            }
        }
        case dLytPauseMgr_c::SELECT_FIRE: {
            SizedString<32> buf;
            buf.sprintf("CAPTION_FIRE_%03d", id);
            return dMessage_c::getTextMessageByLabel(buf, true, nullptr, 0);
        }
        default: {
            if (id == ITEM_CAWLIN_LETTER) {
                if (StoryflagManager::sInstance->getFlag(552) || StoryflagManager::sInstance->getFlag(555)) {
                    // "A heartfelt letter from Cawlin that you were able to somewhat successfully deliver..."
                    return dMessage_c::getTextMessageByLabel("CAPTION_ITEM_158_02", true, nullptr, 0);
                } else {
                    return getCaptionItemText(id);
                }
            } else if (id == ITEM_BEEDLE_INSECT_CAGE) {
                if (StoryflagManager::sInstance->getFlag(477)) {
                    // "Formerly contained Beedle's beloved insect. You successfully returned it to its owner."
                    return dMessage_c::getTextMessageByLabel("CAPTION_ITEM_159_02", true, nullptr, 0);
                } else {
                    return getCaptionItemText(id);
                }
            } else if (id == ITEM_RATTLE) {
                if (StoryflagManager::sInstance->getFlag(387)) {
                    // "This toy belongs to the baby of Bertie,  who runs the Potion Shop. You've successfully returned it to its owner."
                    return dMessage_c::getTextMessageByLabel("CAPTION_ITEM_160_02", true, nullptr, 0);
                } else {
                    return getCaptionItemText(id);
                }
            } else if (id == ITEM_1_CRYSTAL) {
                if (StoryflagManager::sInstance->getFlag(360)) {
                    // "Crystallized gratitude from the people of the land. This power of thankfulness turned Batreaux into a human."
                    return dMessage_c::getTextMessageByLabel("CAPTION_ITEM_048_02", true, nullptr, 0);
                } else {
                    return getCaptionItemText(id);
                }
            } else {
                return getCaptionItemText(id);
            }
        }
    }
}

void dLytPauseText_c::calcTextLines(dTextBox_c *textBox, const wchar_t *text) {
    if (text != nullptr) {
        mNumTextLines = textBox->calcTextLines(text, dMessage_c::getGlobalTagProcessor());
    } else {
        mNumTextLines = 2;
    }
}

bool dLytPauseText_c::isCurrentItemWallet() const {
    dLytPauseMgr_c::SelectionType_e selectionType = dLytPauseMgr_c::GetInstance()->getCurrentSelectionType();
    if (dLytPauseMgr_c::GetInstance()->getField_0x0841()) {
        return false;
    }

    s32 id = dLytPauseMgr_c::GetInstance()->getCurrentSelectionId();

    if (selectionType != dLytPauseMgr_c::SELECT_ITEM) {
        return false;
    }

    switch (id) {
        case ITEM_UNK_107:
        case ITEM_MEDIUM_WALLET:
        case ITEM_BIG_WALLET:
        case ITEM_GIANT_WALLET:
        case ITEM_TYCOON_WALLET: return true;
        default:                 return false;
    }
}
