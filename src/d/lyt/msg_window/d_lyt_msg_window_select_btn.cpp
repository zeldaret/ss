#include "d/lyt/msg_window/d_lyt_msg_window_select_btn.h"

#include "common.h"
#include "d/d_gfx.h"
#include "d/d_lyt_hio.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_cursor_stick.h"
#include "d/lyt/d_lyt_system_window.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "m/m_angle.h"
#include "m/m_pad.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"

STATE_DEFINE(dLytMsgWindowSelectBtnParts_c, Wait);
STATE_DEFINE(dLytMsgWindowSelectBtnParts_c, On);
STATE_DEFINE(dLytMsgWindowSelectBtnParts_c, Select);
STATE_DEFINE(dLytMsgWindowSelectBtnParts_c, Off);

STATE_DEFINE(dLytMsgWindowSelectBtn_c, Wait);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, In);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, WaitSelect);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, CursorInOut);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, WaitDecide);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, WaitCancel);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, Out);

SelectBtnHelper::SelectBtnHelper() {
    field_0x4C = -1;
    mSelectedBtnIdx = -1;
    mButtonMode = 0;
    mCursorSelectTimer = 0;
    mAngX = mAngXCenter = 0.0f;
    mAngY = mAngYCenter = 0.0f;
    mIsNavEnabled = false;
    mIsCursorActive = true;

    panes[0] = nullptr;
    panes[1] = nullptr;
    panes[2] = nullptr;
    panes[3] = nullptr;
    panes[4] = nullptr;
    panes[5] = nullptr;
    panes[6] = nullptr;
    panes[7] = nullptr;
    panes[8] = nullptr;
}
SelectBtnHelper::~SelectBtnHelper() {}

void SelectBtnHelper::init() {
    field_0x24 = dPad::getDpdPosScreen();
    fn_8011E110(field_0x24);
    field_0x2C = dPad::getDpdPosScreen();
    resetCursor();
}

void SelectBtnHelper::remove() {
    // no-op
}

void SelectBtnHelper::resetCursor() {
    mAngX = mAngXCenter = mAng::ang2deg_c(dPad::ex_c::getInstance()->mMPLS.getHorizontalAngle());
    mAngY = mAngYCenter = mAng::ang2deg_c(dPad::ex_c::getInstance()->mMPLS.getVerticalAngle());
}

u8 SelectBtnHelper::execute() {
    u8 ret = EXECUTE_NONE;
    if (mIsCursorActive && hasNewFSStickButtonSelect() && !mIsNavEnabled) {
        mIsNavEnabled = true;
        dPadNav::setNavEnabled(true, false);
        dPadNav::hidePointer();
    }

    if (mIsCursorActive != dPadNav::isPointerVisible()) {
        mIsCursorActive = dPadNav::isPointerVisible();
        if (mIsCursorActive == true) {
            if (mIsNavEnabled) {
                mIsNavEnabled = false;
                dPadNav::setNavEnabled(false, false);
            }
            resetCursor();
        }
        ret = EXECUTE_SWITCH_CURSOR;
    }

    s8 btn = mSelectedBtnIdx;
    if (mIsCursorActive) {
        if (handleCursorInput()) {
            return EXECUTE_RESET_CURSOR;
        } else if (btn == -1 && mSelectedBtnIdx != -1) {
            ret = EXECUTE_SWITCH_CURSOR;
        }
    } else {
        handleButtonInput();
        if (btn == -1 && mSelectedBtnIdx != -1) {
            ret = EXECUTE_SWITCH_STICK;
        }
    }

    return ret;
}

bool SelectBtnHelper::handleCursorInput() {
    if (dPad::getDownTrigDown()) {
        resetCursor();
        return true;
    }

    mAngX = mAng::ang2deg_c(dPad::ex_c::getInstance()->mMPLS.getHorizontalAngle());
    mAngY = mAng::ang2deg_c(dPad::ex_c::getInstance()->mMPLS.getVerticalAngle());

    mVec2_c screenPos = dPad::getDpdPosScreen();
    if (mCursorSelectTimer > 0) {
        mCursorSelectTimer--;
        if (mCursorSelectTimer == 0) {
            fn_8011E110(screenPos);
            field_0x2C = dPad::getDpdPosScreen();
            mAngXCenter = mAngX;
            mAngYCenter = mAngY;
        }
    } else {
        s32 btn = calculatePointedAtButton(mAngX - mAngXCenter, mAngY - mAngYCenter);
        if (mSelectedBtnIdx != btn) {
            mSelectedBtnIdx = btn;
            mCursorSelectTimer = dLyt_HIO_c::getInstance()->getField0x796();
            if (mSelectedBtnIdx != -1) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR);
                mPad::getCore()->startPatternRumble("**-*----", 0, false);
            }
            fn_8011E110(screenPos);
            // TODO additional stack stores
            field_0x2C = dPad::getDpdPosScreen();
            mAngXCenter = mAngX;
            mAngYCenter = mAngY;
        }

        f32 length = 0.0f;
        f32 angle = 0.0f;
        s32 selected = mSelectedBtnIdx;
        mVec2_c v(
            -(mAngX - mAngXCenter) / dLyt_HIO_c::getInstance()->getField0x778(),
            (mAngY - mAngYCenter) / dLyt_HIO_c::getInstance()->getField0x780()
        );

        if (v.x > 1.0f) {
            v.x = 1.0f;
        }
        if (v.x < -1.0f) {
            v.x = -1.0f;
        }
        if (v.y > 1.0f) {
            v.y = 1.0f;
        }
        if (v.y < -1.0f) {
            v.y = -1.0f;
        }

        if (selected != -1) {
            nw4r::math::MTX34 mtx0 = getRootPane()->GetGlobalMtx();
            mVec2_c pos0 = mVec2_c(mtx0._03, mtx0._13);
            nw4r::math::MTX34 mtxi = getPosPane(selected)->GetGlobalMtx();
            mVec2_c posi = mVec2_c(mtxi._03, mtxi._13);
            mVec2_c d1 = posi - pos0;
            s16 ang = d1.ang();

            mVec2_c adj;
            adj.x = v.x * dLyt_HIO_c::getInstance()->getField0x770();
            adj.y = v.y * dLyt_HIO_c::getInstance()->getField0x774();
            mVec2_c d2 = posi + adj - pos0;
            s16 ang2 = d2.ang();

            angle = mAng::ang2deg_c(ang2) - mAng::ang2deg_c(ang);
            length = d2.length() / d1.length();
        }
        if (length > 1.0f) {
            length = 1.0f;
        }

        if ((mButtonMode == MODE_3_UP && selected == 0) || (mButtonMode == MODE_3_DOWN && selected == 1)) {
            // "middle" button
            length *= 0.53f;
        }

        dLytMeter_c::setSelectBtn(angle + getAngleForButtonIdx(selected), length);
        dLytSystemWindow_c::setSelectBtn(angle + getAngleForButtonIdx(selected), length);
    }
    return false;
}

bool SelectBtnHelper::handleButtonInput() {
    if (dPad::getDownTrigDown()) {
        resetCursor();
    }

    s8 btn = mSelectedBtnIdx;
    if (btn != -1) {
#define IS(dir) (dPadNav::getFSStickNavDirection() == dPadNav::FS_STICK_##dir)

        switch (mButtonMode) {
            case MODE_2:
                if (btn == 0) {
                    if (IS(LEFT) || IS(UP_LEFT) || IS(DOWN_LEFT)) {
                        btn = 1;
                    }
                } else if (btn == 1) {
                    if (IS(RIGHT) || IS(UP_RIGHT) || IS(DOWN_RIGHT)) {
                        btn = 0;
                    }
                }
                break;
            case MODE_3_UP:
                if (btn == 0) {
                    if (IS(RIGHT) || IS(DOWN_RIGHT)) {
                        btn = 1;
                    } else if (IS(LEFT) || IS(DOWN_LEFT)) {
                        btn = 2;
                    }
                } else if (btn == 1) {
                    if (IS(UP) || IS(UP_LEFT)) {
                        btn = 0;
                    } else if (IS(DOWN_LEFT) || IS(LEFT)) {
                        btn = 2;
                    }
                } else if (btn == 2) {
                    if (IS(UP) || IS(UP_RIGHT)) {
                        btn = 0;
                    } else if (IS(DOWN_RIGHT) || IS(RIGHT)) {
                        btn = 1;
                    }
                }
                break;
            case MODE_4:
                if (btn == 0) {
                    if (IS(DOWN) || IS(DOWN_RIGHT)) {
                        btn = 1;
                    } else if (IS(LEFT) || IS(UP_LEFT)) {
                        btn = 2;
                    } else if (IS(DOWN_LEFT)) {
                        btn = 3;
                    }
                } else if (btn == 1) {
                    if (IS(UP) || IS(UP_RIGHT)) {
                        btn = 0;
                    } else if (IS(UP_LEFT)) {
                        btn = 2;
                    } else if (IS(LEFT) || IS(DOWN_LEFT)) {
                        btn = 3;
                    }
                } else if (btn == 2) {
                    if (IS(RIGHT) || IS(UP_RIGHT)) {
                        btn = 0;
                    } else if (IS(DOWN_RIGHT)) {
                        btn = 1;
                    } else if (IS(DOWN) || IS(DOWN_LEFT)) {
                        btn = 3;
                    }
                } else if (btn == 3) {
                    if (IS(UP_RIGHT)) {
                        btn = 0;
                    } else if (IS(RIGHT) || IS(DOWN_RIGHT)) {
                        btn = 1;
                    } else if (IS(UP) || IS(UP_LEFT)) {
                        btn = 2;
                    }
                }
                break;
            case MODE_3_DOWN:
                if (btn == 1) {
                    if (IS(RIGHT) || IS(UP_RIGHT)) {
                        btn = 0;
                    } else if (IS(LEFT) || IS(UP_LEFT)) {
                        btn = 2;
                    }
                } else if (btn == 0) {
                    if (IS(DOWN) || IS(DOWN_LEFT)) {
                        btn = 1;
                    } else if (IS(UP_LEFT) || IS(LEFT)) {
                        btn = 2;
                    }
                } else if (btn == 2) {
                    if (IS(DOWN) || IS(DOWN_RIGHT)) {
                        btn = 1;
                    } else if (IS(UP_RIGHT) || IS(RIGHT)) {
                        btn = 0;
                    }
                }
                break;
        }

#undef IS
    } else {
#define IS(dir) (dPadNav::getFSStickDirectionTrig() == dPadNav::FS_STICK_##dir)

        switch (mButtonMode) {
            case MODE_2:
                if (IS(UP_RIGHT) || IS(RIGHT) || IS(DOWN_RIGHT)) {
                    btn = 0;
                } else if (IS(UP_LEFT) || IS(LEFT) || IS(DOWN_LEFT)) {
                    btn = 1;
                }
                break;
            case MODE_3_UP:
                if (IS(UP_RIGHT) || IS(UP) || IS(UP_LEFT)) {
                    btn = 0;
                } else if (IS(LEFT) || IS(DOWN_LEFT)) {
                    btn = 2;
                } else if (IS(RIGHT) || IS(DOWN_RIGHT)) {
                    btn = 1;
                }
                break;
            case MODE_4:
                if (IS(UP_RIGHT) || IS(RIGHT)) {
                    btn = 0;
                } else if (IS(DOWN_RIGHT)) {
                    btn = 1;
                } else if (IS(DOWN_LEFT) || IS(LEFT)) {
                    btn = 3;
                } else if (IS(UP_LEFT)) {
                    btn = 2;
                }
                break;
            case MODE_3_DOWN:
                if (IS(DOWN_RIGHT) || IS(DOWN) || IS(DOWN_LEFT)) {
                    btn = 1;
                } else if (IS(LEFT) || IS(UP_LEFT)) {
                    btn = 2;
                } else if (IS(RIGHT) || IS(UP_RIGHT)) {
                    btn = 0;
                }
                break;
        }

#undef IS

        if (btn == -1) {
            // TODO
            btn = field_0x4C != -1 ? field_0x4C : 0;
        }
    }

    if (mSelectedBtnIdx != btn) {
        mSelectedBtnIdx = btn;
        if (mSelectedBtnIdx != -1) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR);
            mPad::getCore()->startPatternRumble("**-*----", 0, false);
            // TODO - pane array
            dLytCursorStick_c::GetInstance()->setTargetPane(getBounding(btn));
        }
    }

    return false;
}

f32 SelectBtnHelper::getAngleForButtonIdx(s32 idx) const {
    f32 ret = 0.0f;
    switch (mButtonMode) {
        case MODE_2:
            if (idx == 0) {
                ret = -90.0f;
            } else {
                ret = 90.0f;
            }
            break;
        case MODE_3_UP:
            if (idx == 0) {
                return 0.0f;
            } else if (idx == 1) {
                return -90.0f;
            } else {
                ret = 90.0f;
            }
            break;
        case MODE_4:
            if (idx == 0) {
                return -75.0f;
            } else if (idx == 1) {
                return -105.0f;
            } else if (idx == 2) {
                return 75.0f;
            } else {
                ret = 105.0f;
            }
            break;
        case MODE_3_DOWN:
            if (idx == 1) {
                return -180.0f;
            } else if (idx == 0) {
                return -90.0f;
            } else {
                ret = 90.0f;
            }
            break;
    }

    return ret;
}

bool SelectBtnHelper::hasNewFSStickButtonSelect() const {
    if (dPadNav::getFSStickDirectionTrig() == dPadNav::FS_STICK_NONE) {
        return false;
    }

#define TRIG(dir) (dPadNav::getFSStickDirectionTrig() == dPadNav::FS_STICK_##dir)

    switch (mButtonMode) {
        case MODE_2:
            if (TRIG(UP) || TRIG(DOWN) ||
                ((TRIG(RIGHT) || TRIG(UP_RIGHT) || TRIG(DOWN_RIGHT)) && mSelectedBtnIdx == 0) ||
                ((TRIG(LEFT) || TRIG(UP_LEFT) || TRIG(DOWN_LEFT)) && mSelectedBtnIdx == 1)) {
                return false;
            }
            break;
        case MODE_3_UP:
            if (((TRIG(UP) || TRIG(UP_RIGHT) || TRIG(UP_LEFT)) && mSelectedBtnIdx == 0) ||
                ((TRIG(DOWN) || TRIG(DOWN_RIGHT) || TRIG(DOWN_LEFT)) && (mSelectedBtnIdx == 1 || mSelectedBtnIdx == 2)
                ) ||
                ((TRIG(RIGHT) || TRIG(UP_RIGHT) || TRIG(DOWN_RIGHT)) && mSelectedBtnIdx == 1) ||
                ((TRIG(LEFT) || TRIG(UP_LEFT) || TRIG(DOWN_LEFT)) && mSelectedBtnIdx == 2)) {
                return false;
            }
            break;
        case MODE_3_DOWN:
            if (((TRIG(UP) || TRIG(UP_LEFT) || TRIG(UP_RIGHT)) && (mSelectedBtnIdx == 0 || mSelectedBtnIdx == 2)) ||
                ((TRIG(DOWN) || TRIG(DOWN_RIGHT) || TRIG(DOWN_LEFT)) && mSelectedBtnIdx == 1) ||
                ((TRIG(RIGHT) || TRIG(UP_RIGHT) || TRIG(DOWN_RIGHT)) && mSelectedBtnIdx == 0) ||
                ((TRIG(LEFT) || TRIG(UP_LEFT) || TRIG(DOWN_LEFT)) && mSelectedBtnIdx == 2)) {
                return false;
            }
            break;
        case MODE_4:
            if ((TRIG(UP) && (mSelectedBtnIdx == 0 || mSelectedBtnIdx == 2)) ||
                (TRIG(DOWN) && (mSelectedBtnIdx == 1 || mSelectedBtnIdx == 3)) ||
                (TRIG(RIGHT) && (mSelectedBtnIdx == 0 || mSelectedBtnIdx == 1)) ||
                (TRIG(LEFT) && (mSelectedBtnIdx == 2 || mSelectedBtnIdx == 3)) ||
                (TRIG(UP_RIGHT) && mSelectedBtnIdx == 0) || (TRIG(UP_LEFT) && mSelectedBtnIdx == 2) ||
                (TRIG(DOWN_RIGHT) && mSelectedBtnIdx == 1) || (TRIG(DOWN_LEFT) && mSelectedBtnIdx == 3)) {
                return false;
            }
            break;
    }

#undef TRIG

    return true;
}

s8 SelectBtnHelper::calculatePointedAtButton(f32 x, f32 y) {
    s32 ret = mSelectedBtnIdx;
    switch (mButtonMode) {
        case MODE_2:
            if (x <= -dLyt_HIO_c::getInstance()->getField0x778()) {
                ret = 0;
            } else if (x >= dLyt_HIO_c::getInstance()->getField0x778()) {
                ret = 1;
            }
            if (mSelectedBtnIdx == 0 && x < 0) {
                mAngXCenter = mAngX;
            } else if (mSelectedBtnIdx == 1 && x > 0) {
                mAngXCenter = mAngX;
            }
            break;
        case MODE_3_UP:
            if (y >= dLyt_HIO_c::getInstance()->getField0x780()) {
                ret = 0;
            } else if (x <= -dLyt_HIO_c::getInstance()->getField0x778()) {
                ret = 1;
            } else if (x >= dLyt_HIO_c::getInstance()->getField0x778()) {
                ret = 2;
            } else if (y <= -dLyt_HIO_c::getInstance()->getField0x780()) {
                if (x < 0.0f) {
                    ret = 1;
                } else {
                    ret = 2;
                }
            }
            if (mSelectedBtnIdx == 0 && y > 0) {
                mAngYCenter = mAngY;
            } else if (mSelectedBtnIdx == 1 && x < 0) {
                mAngXCenter = mAngX;
            } else if (mSelectedBtnIdx == 2 && x > 0) {
                mAngXCenter = mAngX;
            }
            break;
        case MODE_4:
            if (mSelectedBtnIdx == -1) {
                if (y >= dLyt_HIO_c::getInstance()->getField0x780() &&
                    x <= -dLyt_HIO_c::getInstance()->getField0x778()) {
                    ret = 0;
                } else if (y <= -dLyt_HIO_c::getInstance()->getField0x780() &&
                           x <= -dLyt_HIO_c::getInstance()->getField0x778()) {
                    ret = 1;
                } else if (y >= dLyt_HIO_c::getInstance()->getField0x780() &&
                           x >= dLyt_HIO_c::getInstance()->getField0x778()) {
                    ret = 2;
                } else if (y <= -dLyt_HIO_c::getInstance()->getField0x780() &&
                           x >= dLyt_HIO_c::getInstance()->getField0x778()) {
                    ret = 3;
                }
            } else if (mSelectedBtnIdx == 0) {
                if (y <= -dLyt_HIO_c::getInstance()->getField0x784() &&
                    x >= dLyt_HIO_c::getInstance()->getField0x77C()) {
                    ret = 3;
                } else if (y <= -dLyt_HIO_c::getInstance()->getField0x780()) {
                    ret = 1;
                } else if (x >= dLyt_HIO_c::getInstance()->getField0x778()) {
                    ret = 2;
                }
            } else if (mSelectedBtnIdx == 1) {
                if (y >= dLyt_HIO_c::getInstance()->getField0x784() &&
                    x >= dLyt_HIO_c::getInstance()->getField0x77C()) {
                    ret = 2;
                } else if (y >= dLyt_HIO_c::getInstance()->getField0x780()) {
                    ret = 0;
                } else if (x >= dLyt_HIO_c::getInstance()->getField0x778()) {
                    ret = 3;
                }
            } else if (mSelectedBtnIdx == 2) {
                if (y <= -dLyt_HIO_c::getInstance()->getField0x784() &&
                    x <= -dLyt_HIO_c::getInstance()->getField0x77C()) {
                    ret = 1;
                } else if (y <= -dLyt_HIO_c::getInstance()->getField0x780()) {
                    ret = 3;
                } else if (x <= -dLyt_HIO_c::getInstance()->getField0x778()) {
                    ret = 0;
                }
            } else if (mSelectedBtnIdx == 3) {
                if (y >= dLyt_HIO_c::getInstance()->getField0x784() &&
                    x <= -dLyt_HIO_c::getInstance()->getField0x77C()) {
                    ret = 0;
                } else if (y >= dLyt_HIO_c::getInstance()->getField0x780()) {
                    ret = 2;
                } else if (x <= -dLyt_HIO_c::getInstance()->getField0x778()) {
                    ret = 1;
                }
            }

            if (mSelectedBtnIdx == 0 && y > 0.0f) {
                mAngYCenter = mAngY;
            } else if (mSelectedBtnIdx == 0 && x < 0.0f) {
                mAngXCenter = mAngX;
            } else if (mSelectedBtnIdx == 1 && y < 0.0f) {
                mAngYCenter = mAngY;
            } else if (mSelectedBtnIdx == 1 && x < 0.0f) {
                mAngXCenter = mAngX;
            } else if (mSelectedBtnIdx == 2 && y > 0.0f) {
                mAngYCenter = mAngY;
            } else if (mSelectedBtnIdx == 2 && x > 0.0f) {
                mAngXCenter = mAngX;
            } else if (mSelectedBtnIdx == 3 && y < 0.0f) {
                mAngYCenter = mAngY;
            } else if (mSelectedBtnIdx == 3 && x > 0.0f) {
                mAngXCenter = mAngX;
            }
            break;
        case MODE_3_DOWN:
            if (y <= -dLyt_HIO_c::getInstance()->getField0x780()) {
                ret = 1;
            } else if (x <= -dLyt_HIO_c::getInstance()->getField0x778()) {
                ret = 0;
            } else if (x >= dLyt_HIO_c::getInstance()->getField0x778()) {
                ret = 2;
            } else if (y >= dLyt_HIO_c::getInstance()->getField0x780()) {
                if (x < 0.0f) {
                    ret = 0;
                } else {
                    ret = 2;
                }
            }
            if (mSelectedBtnIdx == 1 && y < 0) {
                mAngYCenter = mAngY;
            } else if (mSelectedBtnIdx == 0 && x < 0) {
                mAngXCenter = mAngX;
            } else if (mSelectedBtnIdx == 2 && x > 0) {
                mAngXCenter = mAngX;
            }
            break;
    }

    return ret;
}

void SelectBtnHelper::convertScreenPosToDpdPos(mVec2_c *pOutDpd, mVec2_c *screenPos) const {
    pOutDpd->x = (screenPos->x - dGfx_c::getWidth4x3LeftF()) / (dGfx_c::getWidth4x3F() * 0.5f) - 1.0f;
    pOutDpd->y = (screenPos->y - dGfx_c::getCurrentScreenTopF()) / (dGfx_c::getCurrentScreenHeightF() * -0.5f) - 1.0f;
}

void SelectBtnHelper::fn_8011E110(mVec2_c screenPos) const {
    mVec2_c dpdPos;
    convertScreenPosToDpdPos(&dpdPos, &screenPos);
    dPad::ex_c::getInstance()->fn_80056580(mPad::getCurrentCoreID(), dpdPos);
}

void dLytMsgWindowSelectBtnParts_c::initializeState_Wait() {}
void dLytMsgWindowSelectBtnParts_c::executeState_Wait() {
    if (mShouldBeOn != 0) {
        for (int i = 0; i <= 1; i++) {
            mpAnms[i]->setForwardOnce();
            mpAnms[i]->setFrame(0.0f);
            mpAnms[i]->setAnimEnable(true);
        }
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMsgWindowSelectBtnParts_c::finalizeState_Wait() {}

void dLytMsgWindowSelectBtnParts_c::initializeState_On() {}
void dLytMsgWindowSelectBtnParts_c::executeState_On() {
    if (mpAnms[0]->isEndReached() && mpAnms[1]->isEndReached()) {
        mpAnms[0]->setAnimEnable(false);
        mpAnms[1]->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }
    for (int i = 0; i <= 1; i++) {
        if (mpAnms[i]->isEnabled()) {
            mpAnms[i]->play();
        }
    }
}
void dLytMsgWindowSelectBtnParts_c::finalizeState_On() {}

void dLytMsgWindowSelectBtnParts_c::initializeState_Select() {}
void dLytMsgWindowSelectBtnParts_c::executeState_Select() {
    if (mShouldBeOn == 0) {
        for (int i = 0; i <= 1; i++) {
            mpAnms[i]->setBackwardsOnce();
            mpAnms[i]->setToStart();
            mpAnms[i]->setAnimEnable(true);
        }
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMsgWindowSelectBtnParts_c::finalizeState_Select() {}

void dLytMsgWindowSelectBtnParts_c::initializeState_Off() {}
void dLytMsgWindowSelectBtnParts_c::executeState_Off() {
    if (mpAnms[0]->isStop2() && mpAnms[1]->isStop2()) {
        mpAnms[0]->setAnimEnable(false);
        mpAnms[1]->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
    for (int i = 0; i <= 1; i++) {
        if (mpAnms[i]->isEnabled()) {
            mpAnms[i]->play();
        }
    }
}
void dLytMsgWindowSelectBtnParts_c::finalizeState_Off() {}

void dLytMsgWindowSelectBtnParts_c::init() {
    mStateMgr.changeState(StateID_Wait);
    mShouldBeOn = 0;
    field_0x50 = 0;
    field_0x4C = 3;
}

void dLytMsgWindowSelectBtnParts_c::execute() {
    mStateMgr.executeState();
    mpAnms[2]->play();
}

const char *d_lyt_msg_window_select_btn_string_order_1() {
    return "N_allBtn_00";
}

const char *d_lyt_msg_window_select_btn_string_order_2() {
    return "N_arrowIn_00";
}

#define SELECT_BTN_ANIM_IN 0
#define SELECT_BTN_ANIM_LOOP_REMOCON 1
#define SELECT_BTN_ANIM_INOUT_CURSOR 2
#define SELECT_BTN_ANIM_MESSAGE_BTN 3
#define SELECT_BTN_ANIM_ITEM_ARROW_0 4
#define SELECT_BTN_ANIM_ITEM_ARROW_1 5
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_0 6
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_1 7
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_2 8
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_3 9
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0 10
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_1 11
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_2 12
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_3 13
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_0 14
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_1 15
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_2 16
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_3 17
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_0 18
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_1 19
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_2 20
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_3 21
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_0 22
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_1 23
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_2 24
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_3 25
#define SELECT_BTN_ANIM_OUT 26

#define SELECT_BTN_NUM_ANIMS 27
#define SELECT_BTN_NUM_BTNS 4
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_0
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_0
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_0
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_0

static const d2d::LytBrlanMapping brlanMap[] = {
    {         "messageBtn_00_in.brlan",       "G_inOut_00"},
    {"messageBtn_00_loopRemocon.brlan", "G_loopRemocon_00"},
    {"messageBtn_00_inOutCursor.brlan",  "G_inOutAlpha_00"},
    { "messageBtn_00_messageBtn.brlan",  "G_messageBtn_00"},
    {  "messageBtn_00_itemArrow.brlan",   "G_itemArrow_00"},
    {  "messageBtn_00_itemArrow.brlan",   "G_itemArrow_01"},
    {       "messageBtn_00_loop.brlan",         "G_btn_00"},
    {       "messageBtn_00_loop.brlan",         "G_btn_01"},
    {       "messageBtn_00_loop.brlan",         "G_btn_02"},
    {       "messageBtn_00_loop.brlan",         "G_btn_03"},
    {     "messageBtn_00_cancel.brlan",      "G_cancel_00"},
    {     "messageBtn_00_cancel.brlan",      "G_cancel_01"},
    {     "messageBtn_00_cancel.brlan",      "G_cancel_02"},
    {     "messageBtn_00_cancel.brlan",      "G_cancel_03"},
    {     "messageBtn_00_onOffB.brlan",      "G_onOffB_00"},
    {     "messageBtn_00_onOffB.brlan",      "G_onOffB_01"},
    {     "messageBtn_00_onOffB.brlan",      "G_onOffB_02"},
    {     "messageBtn_00_onOffB.brlan",      "G_onOffB_03"},
    {     "messageBtn_00_onOffA.brlan",         "G_btn_00"},
    {     "messageBtn_00_onOffA.brlan",         "G_btn_01"},
    {     "messageBtn_00_onOffA.brlan",         "G_btn_02"},
    {     "messageBtn_00_onOffA.brlan",         "G_btn_03"},
    {     "messageBtn_00_decide.brlan",         "G_btn_00"},
    {     "messageBtn_00_decide.brlan",         "G_btn_01"},
    {     "messageBtn_00_decide.brlan",         "G_btn_02"},
    {     "messageBtn_00_decide.brlan",         "G_btn_03"},
    {        "messageBtn_00_out.brlan",       "G_inOut_00"}
};

static const char *sSelectTextBoxes[SELECT_BTN_NUM_BTNS][2] = {
    {"T_selectS_00", "T_select_00"},
    {"T_selectS_01", "T_select_01"},
    {"T_selectS_02", "T_select_02"},
    {"T_selectS_03", "T_select_03"},
};

static const char *sDecideTextBoxes[2] = {
    "T_decide_00",
    "T_decideS_00",
};

static const char *sWindowNames[] = {"W_bgP_01"};

static const char *sBoundings[SELECT_BTN_NUM_BTNS] = {
    "B_btn_00",
    "B_btn_01",
    "B_btn_02",
    "B_btn_03",
};

static const char *sSelectPanes[7] = {
    "N_itemArrow_00",  "N_arrowHand_00",  "N_aBtn_00",       "N_messageBtn_00",
    "N_messageBtn_01", "N_messageBtn_02", "N_messageBtn_03",
};

#define SELECT_BTN_PANE_ITEM_ARROW_0 0
#define SELECT_BTN_PANE_ARROW_HAND 1
#define SELECT_BTN_PANE_BTN_0 2

void dLytMsgWindowSelectBtn_c::requestIn(s32 numBtns, bool playSound) {
    mPlayInSound = playSound;
    mBtnHelper.mSelectedBtnIdx = -1;

    if (numBtns < 2) {
        numBtns = 2;
    } else if (numBtns > 4) {
        numBtns = 4;
    }

    mNumBtns = numBtns;
    switch (numBtns) {
        case 2: field_0x9AC = 2; break;
        case 3: field_0x9AC = 1; break;
        case 4: field_0x9AC = 0; break;
    }

    mIsVisible = true;
    if (mFlipBtnLayout == 1 && mNumBtns == 3) {
        mAnm[SELECT_BTN_ANIM_MESSAGE_BTN].setFrame(3.0f);
    } else {
        mAnm[SELECT_BTN_ANIM_MESSAGE_BTN].setFrame(mNumBtns - 2);
    }
    mAnm[SELECT_BTN_ANIM_MESSAGE_BTN].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_IN].setAnimEnable(false);
    mConfirmedBtnIdx = -1;
    mStateMgr.changeState(StateID_In);
}

void dLytMsgWindowSelectBtn_c::initializeState_Wait() {
    mIsVisible = false;
    field_0x9B8 = -1;
    mCancelBtnIdx = -1;

    mCanceledViaBBtnIdx = 0xFF;
    mWaitCancelTimer = 10;
    field_0x9CD = 1;
}
void dLytMsgWindowSelectBtn_c::executeState_Wait() {}
void dLytMsgWindowSelectBtn_c::finalizeState_Wait() {}

void dLytMsgWindowSelectBtn_c::initializeState_In() {
    mIsVisible = true;
    mDecidedBtnIdx = -1;
    for (int i = 0; i < 2; i++) {
        // "Select"
        mLyt.loadTextVariant(mpDecideTextBoxes[i], 1);
    }

    mpWindow[0]->UpdateSize(mpSizeBox[0], 32.0f);

    for (int i = SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0; i <= SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_3; i++) {
        if (mCancelBtnIdx == i - SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0) {
            mAnm[i].setFrame(1.0f);
        } else {
            mAnm[i].setFrame(0.0f);
        }
    }

    mBtnHelper.mSelectedBtnIdx = -1;
    mBtnHelper.mIsCursorActive = true;
    if (mBtnHelper.getSelectedtBtnIdx() != -1) {
        // unreachable but the call isn't eliminated
        dLytCursorStick_c::GetInstance()->setTargetPane(mBtnHelper.getBounding(mBtnHelper.getSelectedtBtnIdx()));
    }
    dLytMeter_c::setSelectBtn(mBtnHelper.getAngleForButtonIdx(mBtnHelper.getSelectedtBtnIdx()), 0.0f);

    mLyt.findPane("N_allBtn_00")->SetVisible(true);
    mLyt.findPane("N_arrowIn_00")->SetVisible(true);

    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setForwardOnce();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setToStart();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(true);

    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setFrame(0.0f);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setFrame(0.0f);

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mParts[i].init();
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET].setAnimEnable(true);
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET].setFrame(0.0f);
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET].setAnimEnable(true);
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET].setFrame(0.0f);
    }

    mLyt.calc();

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET].setAnimEnable(false);
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET].setAnimEnable(false);
    }

    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(false);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setAnimEnable(false);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setAnimEnable(false);

    mLyt.findPane("N_allBtn_00")->SetVisible(false);
    mLyt.findPane("N_arrowIn_00")->SetVisible(false);

    mAnm[SELECT_BTN_ANIM_IN].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_IN].setForwardOnce();

    if (field_0x9D1 != 0) {
        mAnm[SELECT_BTN_ANIM_IN].setToEnd();
        field_0x9D1 = false;
    } else {
        if (mPlayInSound) {
            if (mInSound == 1) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CHOICE_START);
            } else if (mInSound == 2) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CHOICE_START_GAMEOVER);
            } else {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CHOICE_START);
            }
        }
        mAnm[SELECT_BTN_ANIM_IN].setFrame(0.0f);
    }
}
void dLytMsgWindowSelectBtn_c::executeState_In() {
    mAnm[SELECT_BTN_ANIM_IN].play();
    if (mAnm[SELECT_BTN_ANIM_IN].isEndReached()) {
        mStateMgr.changeState(StateID_WaitSelect);
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_In() {
    mAnm[SELECT_BTN_ANIM_IN].setAnimEnable(false);
}

void dLytMsgWindowSelectBtn_c::initializeState_WaitSelect() {
    if (!field_0x9D0) {
        field_0x9D0 = true;
        dSndPlayerMgr_c::GetInstance()->enterMsgWait();
    }

    mBtnHelper.init();
    mBtnHelper.mButtonMode = getSelectBtnMode();
}

void dLytMsgWindowSelectBtn_c::executeState_WaitSelect() {
    u8 v = mBtnHelper.execute();
    if (v == SelectBtnHelper::EXECUTE_SWITCH_CURSOR) {
        mStateMgr.changeState(StateID_CursorInOut);
        return;
    } else if (v == SelectBtnHelper::EXECUTE_SWITCH_STICK) {
        dLytMeter_c::setSelectBtn(mBtnHelper.getAngleForButtonIdx(mBtnHelper.getSelectedtBtnIdx()), 1.0f);
        for (int i = 0; i < 2; i++) {
            // "Confirm"
            mLyt.loadTextVariant(mpDecideTextBoxes[i], 0);
        }

        mpWindow[0]->UpdateSize(mpSizeBox[0], 32.0f);
    } else if (v == SelectBtnHelper::EXECUTE_RESET_CURSOR) {
        field_0x9D1 = 1;
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_POINTER_RESET);
        mStateMgr.changeState(StateID_In);
        return;
    }

    if (mBtnHelper.getSelectedtBtnIdx() >= 0 && dPad::getDownTrigA()) {
        mConfirmedBtnIdx = mBtnHelper.getSelectedtBtnIdx();
        field_0x9D0 = false;
        mStateMgr.changeState(StateID_WaitDecide);
        if (mpTagProcessor != nullptr) {
            if (mpTagProcessor->getOptionSound(mBtnHelper.getSelectedtBtnIdx()) == 0) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR_CANCEL);
            } else {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR_OK);
            }
        }
        dSndPlayerMgr_c::GetInstance()->leaveMsgWait();
    } else if (mCancelBtnIdx >= 0 && dPad::getDownTrigB()) {
        dLytMeter_c::setSelectBtn(mBtnHelper.getAngleForButtonIdx(mCancelBtnIdx), 1.0f);
        mCanceledViaBBtnIdx = mCancelBtnIdx;
        field_0x9D0 = false;
        mStateMgr.changeState(StateID_WaitCancel);
        if (mpTagProcessor != nullptr) {
            if (mpTagProcessor->getOptionSound(mCanceledViaBBtnIdx) == 0) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR_CANCEL);
            } else {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR_OK);
            }
        }
        dSndPlayerMgr_c::GetInstance()->leaveMsgWait();
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_WaitSelect() {
    mBtnHelper.remove();
    if (!field_0x9D0 && mBtnHelper.mIsNavEnabled) {
        mBtnHelper.mIsNavEnabled = false;
        dPadNav::setNavEnabled(false, false);
    }
}

void dLytMsgWindowSelectBtn_c::initializeState_CursorInOut() {
    for (int i = 0; i < 2; i++) {
        // "Confirm"
        mLyt.loadTextVariant(mpDecideTextBoxes[i], 0);
    }
    mpWindow[0]->UpdateSize(mpSizeBox[0], 32.0f);

    if (mBtnHelper.mIsCursorActive) {
        mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setForwardOnce();
        if (mBtnHelper.getSelectedtBtnIdx() != -1) {
            f32 length = 1.0f;
            if ((mBtnHelper.mButtonMode == SelectBtnHelper::MODE_3_UP && mBtnHelper.getSelectedtBtnIdx() == 0) ||
                (mBtnHelper.mButtonMode == SelectBtnHelper::MODE_3_DOWN && mBtnHelper.getSelectedtBtnIdx() == 1)) {
                // "middle" button
                length *= 0.53f;
            }
            dLytMeter_c::setSelectBtn(mBtnHelper.getAngleForButtonIdx(mBtnHelper.getSelectedtBtnIdx()), length);
        } else {
            dLytMeter_c::setSelectBtn(mBtnHelper.getAngleForButtonIdx(mBtnHelper.getSelectedtBtnIdx()), 0.0f);
        }
    } else {
        f32 length = 1.0f;
        mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setBackwardsOnce();
        if (mBtnHelper.getSelectedtBtnIdx() != -1) {
            dLytCursorStick_c::GetInstance()->setTargetPane(mpBoundings[mBtnHelper.getSelectedtBtnIdx()]);
            if ((mBtnHelper.mButtonMode == SelectBtnHelper::MODE_3_UP && mBtnHelper.getSelectedtBtnIdx() == 0) ||
                (mBtnHelper.mButtonMode == SelectBtnHelper::MODE_3_DOWN && mBtnHelper.getSelectedtBtnIdx() == 1)) {
                // "middle" button
                length *= 0.53f;
            }
        }
        dLytMeter_c::setSelectBtn(mBtnHelper.getAngleForButtonIdx(mBtnHelper.getSelectedtBtnIdx()), length);
    }
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setToStart();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(true);
}
void dLytMsgWindowSelectBtn_c::executeState_CursorInOut() {
    if (mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].isStop2()) {
        mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(false);
        mStateMgr.changeState(StateID_WaitSelect);
    }
    if (mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].isEnabled()) {
        mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].play();
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_CursorInOut() {}

void dLytMsgWindowSelectBtn_c::initializeState_WaitDecide() {
    s32 decideAnm = mConfirmedBtnIdx + SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_OFFSET;
    mAnm[decideAnm].setAnimEnable(true);
    mAnm[decideAnm].setFrame(0.0f);
    mAnm[decideAnm].setForwardOnce();
}
void dLytMsgWindowSelectBtn_c::executeState_WaitDecide() {
    s32 decideAnm = mConfirmedBtnIdx + SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_OFFSET;
    mAnm[decideAnm].play();
    if (mAnm[decideAnm].isStop2()) {
        mAnm[decideAnm].setAnimEnable(false);
        mDecidedBtnIdx = mConfirmedBtnIdx;
        if (mSkipOutAnim == 1) {
            mStateMgr.changeState(StateID_Wait);
        } else {
            mStateMgr.changeState(StateID_Out);
        }
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_WaitDecide() {}

void dLytMsgWindowSelectBtn_c::initializeState_WaitCancel() {
    mWaitCancelTimer = 10;
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setForwardOnce();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setToEnd2();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(true);
    mLyt.calc();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(false);
    mpSelectPanes[0]->SetAlpha(0xFF);
}
void dLytMsgWindowSelectBtn_c::executeState_WaitCancel() {
    if (--mWaitCancelTimer <= 0) {
        mConfirmedBtnIdx = mCanceledViaBBtnIdx;
        mStateMgr.changeState(StateID_WaitDecide);
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_WaitCancel() {}

void dLytMsgWindowSelectBtn_c::initializeState_Out() {
    mAnm[SELECT_BTN_ANIM_OUT].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_OUT].setFrame(0.0f);
    mAnm[SELECT_BTN_ANIM_OUT].setForwardOnce();
    mStep = 0;
}
void dLytMsgWindowSelectBtn_c::executeState_Out() {
    switch (mStep) {
        case 0:
            mAnm[SELECT_BTN_ANIM_OUT].play();
            if (mAnm[SELECT_BTN_ANIM_OUT].isStop2()) {
                mIsVisible = false;
                mStep = 1;
            }
            break;
        case 1:
            mAnm[SELECT_BTN_ANIM_OUT].setAnimEnable(false);
            mDecidedBtnIdx = mConfirmedBtnIdx;
            mStateMgr.changeState(StateID_Wait);
            break;
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_Out() {
    mStep = 0;
}

bool dLytMsgWindowSelectBtn_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("messageBtn_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < SELECT_BTN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0; i <= SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_3; i++) {
        mAnm[i].setAnimEnable(true);
    }

    mAnm[SELECT_BTN_ANIM_LOOP_REMOCON].setAnimEnable(true);

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mParts[i].mpAnms[0] = &mAnm[SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET + i];
        mParts[i].mpAnms[1] = &mAnm[SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET + i];
        mParts[i].mpAnms[2] = &mAnm[SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_OFFSET + i];
        mParts[i].init();
        mAnm[SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_OFFSET + i].setAnimEnable(true);
    }

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        for (int j = 0; j < 2; j++) {
            mpSelectTextBoxes[i][j] = mLyt.getTextBox(sSelectTextBoxes[i][j]);
        }
    }

    for (int i = 0; i < 2; i++) {
        mpDecideTextBoxes[i] = mLyt.getTextBox(sDecideTextBoxes[i]);
    }

    for (int i = 0; i < 1; i++) {
        mpWindow[i] = mLyt.getWindow(sWindowNames[i]);
        mpSizeBox[i] = mLyt.getSizeBoxInWindow(sWindowNames[i]);
        mpWindow[i]->UpdateSize(mpSizeBox[i], 32.0f);
    }

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mpBoundings[i] = mLyt.findBounding(sBoundings[i]);
    }

    mpTagProcessor = nullptr;

    for (int i = 0; i < 7; i++) {
        mpSelectPanes[i] = mLyt.findPane(sSelectPanes[i]);
    }

    mBtnHelper.panes[0] = mpSelectPanes[0];
    mBtnHelper.panes[1] = mpSelectPanes[3];
    mBtnHelper.panes[2] = mpSelectPanes[4];
    mBtnHelper.panes[3] = mpSelectPanes[5];
    mBtnHelper.panes[4] = mpSelectPanes[6];
    mBtnHelper.panes[5] = mpBoundings[0];
    mBtnHelper.panes[6] = mpBoundings[1];
    mBtnHelper.panes[7] = mpBoundings[2];
    mBtnHelper.panes[8] = mpBoundings[3];

    mFlipBtnLayout = 0;

    mLyt.findPane("N_arrowIn_00")->SetVisible(true);

    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setForwardOnce();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setToStart();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setFrame(0.0f);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setFrame(0.0f);

    mLyt.calc();

    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(false);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setAnimEnable(false);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setAnimEnable(false);

    mLyt.findPane("N_arrowIn_00")->SetVisible(false);

    mStateMgr.changeState(StateID_Wait);

    mNumBtns = -1;
    mConfirmedBtnIdx = -1;

    field_0x9D0 = false;
    field_0x9D1 = 0;
    mSkipOutAnim = 0;
    mInSound = 0;
    field_0x9CE = 0;
    mPlayInSound = true;

    return true;
}

bool dLytMsgWindowSelectBtn_c::remove() {
    for (int i = 0; i < SELECT_BTN_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    if (mBtnHelper.mIsNavEnabled) {
        mBtnHelper.mIsNavEnabled = false;
        dPadNav::setNavEnabled(false, false);
    }

    return true;
}

bool dLytMsgWindowSelectBtn_c::execute() {
    if (*mStateMgr.getStateID() != StateID_Wait) {
        if (dLytMsgWindow_c::getInstance() != nullptr && dLytMsgWindow_c::getInstance()->getField_0x81B() != 0) {
            mIsVisible = false;
            mAnm[SELECT_BTN_ANIM_IN].setAnimEnable(true);
            mAnm[SELECT_BTN_ANIM_IN].setFrame(0.0f);
            mAnm[SELECT_BTN_ANIM_IN].setForwardOnce();
            mAnm[SELECT_BTN_ANIM_OUT].setAnimEnable(true);
            mAnm[SELECT_BTN_ANIM_OUT].setToEnd();
            mAnm[SELECT_BTN_ANIM_OUT].setForwardOnce();
            mLyt.calc();
            mAnm[SELECT_BTN_ANIM_IN].setAnimEnable(false);
            mAnm[SELECT_BTN_ANIM_OUT].setAnimEnable(false);
            mStateMgr.changeState(StateID_Wait);
            return true;
        }

        if (mCanceledViaBBtnIdx >= 0) {
            for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
                mParts[i].field_0x50 = 1;
                if (mCanceledViaBBtnIdx == i) {
                    mParts[i].mShouldBeOn = 1;
                } else {
                    mParts[i].mShouldBeOn = 0;
                }
            }
        } else {
            for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
                mParts[i].field_0x50 = 0;
                if (mBtnHelper.getSelectedtBtnIdx() == i) {
                    mParts[i].mShouldBeOn = 1;
                } else {
                    mParts[i].mShouldBeOn = 0;
                }
            }
        }
    }

    mStateMgr.executeState();
    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mParts[i].execute();
    }

    f32 angle = dLytMeter_c::getSelectBtnArrowAngle();
    f32 length = dLytMeter_c::getSelectBtnArrowLength();

    mVec3_c t1(0.0f, 0.0f, 0.0f);
    t1.z = angle;
    mpSelectPanes[SELECT_BTN_PANE_ITEM_ARROW_0]->SetRotate(t1);
    // But rotate the button and the pointer back so that
    // they point up
    mVec3_c t2(0.0f, 0.0f, 0.0f);
    t2.z = -angle;
    mpSelectPanes[SELECT_BTN_PANE_ARROW_HAND]->SetRotate(t2);
    mpSelectPanes[SELECT_BTN_PANE_BTN_0]->SetRotate(t2);

    f32 frame0 = mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].getAnimDuration();
    f32 frame1 = mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].getAnimDuration();

    if (length < 0.0f) {
        length = 0.0f;
    }
    if (length > 1.0f) {
        length = 1.0f;
    }

    if (length > 0.0f) {
        if (!mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].isEnabled()) {
            mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setAnimEnable(true);
        }

        if (field_0x9D0 == true && !mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].isEnabled()) {
            mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setAnimEnable(true);
        }
    }

    if (mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].isEnabled()) {
        mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setFrame(frame0 * length);
    }

    if (mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].isEnabled()) {
        mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setFrame(frame1 * length);
    }

    if (mAnm[SELECT_BTN_ANIM_LOOP_REMOCON].isEnabled()) {
        mAnm[SELECT_BTN_ANIM_LOOP_REMOCON].play();
    }

    mLyt.calc();

    return true;
}

bool dLytMsgWindowSelectBtn_c::draw() {
    if (mIsVisible) {
        mLyt.addToDrawList();
    }
    return true;
}

s32 dLytMsgWindowSelectBtn_c::getSelectBtnMode() {
    s32 mode = mNumBtns - 2;
    if (mFlipBtnLayout == 1 && mNumBtns == 3) {
        mode = SelectBtnHelper::MODE_3_DOWN;
    }
    return mode;
}
