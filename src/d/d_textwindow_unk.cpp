

#include "d/d_textwindow_unk.h"

#include "common.h"
#include "d/d_gfx.h"
#include "d/d_message.h"
#include "d/d_tag_processor.h"
#include "d/d_lyt_hio.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/snd/d_snd_fi_vocal_mgr.h"
#include "m/m_vec.h"

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
        mpTagProcessor->computeCharacterPlacement(
            mpTextBox, mRawTextBuffer, mUnkBuffer, field_0x1458, &field_0x1478, mpCharacterData
        );
        if (mpTagProcessor->getMsgWindowSubtype() == dLytMsgWindow_c::MSG_WINDOW_SWORD_FI) {
            dSndFiVocalMgr_c::GetInstance()->fiSpeak(field_0x1402, field_0x1478, mpTagProcessor->getField_0x824());
        }
    }

    dLyt_HIO_c::getInstance()->setStr1(label);
    if (dLytMsgWindow_c::getInstance()->fn_80117390(true) != nullptr) {
        // Regswap
        dLyt_HIO_c::getInstance()->setStr2(dLytMsgWindow_c::getInstance()->fn_80117390(true));
        s32 idx = dMessage_c::getMsbtIndexForLabel(label);
        if (idx >= 0) {
            dLyt_HIO_c::getInstance()->setStr3(dMessage_c::getMsbtFileName(idx));
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
    for (int i = 0; i < TextWindowUnk::BUF_SIZE; i++) {
        mData[i].reset();
    }
}

void MsgWindowBlurRelated::drawTextBlur(dLytTextLight *thing, mVec2_c size, u8 alpha) {
    s8 t = -1;
    for (int i = 0; i < TextWindowUnk::BUF_SIZE; i++) {
        if (mData[i].displayTimerMaybe >= 0) {
            if (t == -1) {
                mData[i].displayTimerMaybe++;

                if (mData[i].displayTimerMaybe >= thing->getAnimDuration()) {
                    mData[i].displayTimerMaybe = 0;
                }
                t = mData[i].displayTimerMaybe;
            } else {
                mData[i].displayTimerMaybe = t;
            }
            mVec3_c v((size.x + mData[i].posX) / dGfx_c::get16x9to4x3WidthScaleF(), size.y + mData[i].posY, 0.0f);
            thing->draw(mData[i].displayTimerMaybe, v, alpha, mData[i].field_0x08);
        }
    }
}
