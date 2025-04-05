

#include "d/d_textwindow_unk.h"

#include "common.h"
#include "d/d_message.h"
#include "d/d_tag_processor.h"
#include "d/d_textunk.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "m/m_vec.h"
#include "toBeSorted/lyt_related_floats.h"
#include "toBeSorted/other_sound_stuff.h"


TextWindowUnk::TextWindowUnk(dTagProcessor_c *c) {
    mpTagProcessor = c;
    reset();
}

bool TextWindowUnk::fn_800B2130(const char *label, dTextBox_c *textBox, dLytMsgWindowCharData *charData, bool b) {
    if (!checkEndReached()) {
        return false;
    }
    reset();

    field_0x1402 = label;
    mpCharacterData = charData;
    mpTextBox = textBox;

    const wchar_t *msg = dMessage_c::getTextMessageByLabel(label, mpTagProcessor, true, nullptr, 0);
    if (b) {
        dTagProcessor_c *p = mpTagProcessor;
        p->setTextbox(mpTextBox);
        p->setField_0xEE0(1);
    }

    // Oh no
    mpTagProcessor->format(
        mpTextBox, msg, mRawTextBuffer, ARRAY_LENGTH(mRawTextBuffer), (u32 *)&mRawTextEndPos, nullptr
    );
    mLineWidths = mpTagProcessor->getLineData();

    if (mpCharacterData != nullptr) {
        mpTagProcessor->fn_800B4290(
            mpTextBox, mRawTextBuffer, mUnkBuffer, field_0x1458, &field_0x1478, mpCharacterData
        );
        if (mpTagProcessor->getMsgWindowSubtype() == 2) {
            fn_803996B0(lbl_80575DE0, field_0x1402, field_0x1478, mpTagProcessor->getField_0x824());
        }
    }

    UnkTextThing::getInstance()->setStr1(label);
    if (dLytMsgWindow_c::getInstance()->fn_80117390(true) != nullptr) {
        // Regswap
        UnkTextThing::getInstance()->setStr2(dLytMsgWindow_c::getInstance()->fn_80117390(true));
        s32 idx = dMessage_c::getMsbtIndexForLabel(label);
        if (idx >= 0) {
            UnkTextThing::getInstance()->setStr3(dMessage_c::getMsbtFileName(idx));
        }
    }
    return true;
}

bool TextWindowUnk::checkLastLineReached() {
    return mCurrentLine >= mpTagProcessor->getMaxNumLines(mpTagProcessor->getMsgWindowSubtype());
}

bool TextWindowUnk::checkEndReached() const {
    return mRawTextPos >= mRawTextEndPos;
}

void MsgWindowBlurRelated::build() {
    for (int i = 0; i < 1023; i++) {
        mData[i].reset();
    }
}

void MsgWindowBlurRelated::drawTextBlur(dLytTextLight *thing, LytVec2f size, u8 alpha) {
    s8 t = -1;
    for (int i = 0; i < 1023; i++) {
        if (mData[i].displayTimerMaybe >= 0) {
            if (t == -1) {
                mData[i].displayTimerMaybe++;

                if (mData[i].displayTimerMaybe >= thing->fn_80121050()) {
                    mData[i].displayTimerMaybe = 0;
                }
                t = mData[i].displayTimerMaybe;
            } else {
                mData[i].displayTimerMaybe = t;
            }
            nw4r::math::VEC3 v((size.x + mData[i].posX) / get_805751A4(), size.y + mData[i].posY, 0.0f);
            thing->draw2(mData[i].displayTimerMaybe, v, alpha, mData[i].field_0x08);
        }
    }
}
