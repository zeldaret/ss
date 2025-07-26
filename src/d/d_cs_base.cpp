#include "d/d_cs_base.h"

#include "common.h"
#include "d/d_gfx.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/lyt/d_lyt_cursor_stick.h"
#include "egg/math/eggMath.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_pad.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"
#include "s/s_Math.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

SPECIAL_BASE_PROFILE(C_BASE, dCsBase_c, fProfile::C_BASE, 0x2C0, 0x06F9);

dCsBase_c *dCsBase_c::sInstance;

dCsBase_c::dCsBase_c()
    : mpCurrLyt(nullptr),
      mCsPosition(0.0f, 0.0f),
      mCsVelocity(0.0f, 0.0f),
      field_0x700(0),
      mAlpha(0),
      field_0x703(0),
      field_0x704(0),
      field_0x705(0) {
    sInstance = this;
}

dCsBase_c::~dCsBase_c() {
    sInstance = nullptr;
}

static const char *sLytNames[] = {
    "P1_Def.brlyt",
    "P1_Cat.brlyt",
};

int dCsBase_c::create() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("cursor");
    mResAcc.attach(data, "arc");

    for (int i = 0; i < 2; i++) {
        mLyts[i].setResAcc(&mResAcc);
        mLyts[i].build(sLytNames[i], nullptr);
        mLyts[i].setPriority(0x88);
        mLyts[i].calc();
    }
    setCurrentLyt(0);
    mCursorIf.setCursorMask(1);
    dCsMgr_c::GetInstance()->registCursor(&mCursorIf);
    dLytCursorStick_c::build();
    mCsPosition.set(0.0f, 0.0f);
    mCsVelocity.set(0.0f, 0.0f);

    field_0x700 = 0;
    mAlpha = 0;
    field_0x703 = 0;

    return SUCCEEDED;
}

int dCsBase_c::doDelete() {
    dCsMgr_c::GetInstance()->unregistCursor(&mCursorIf);
    dLytCursorStick_c::remove();
    mResAcc.detach();
    return SUCCEEDED;
}

int dCsBase_c::execute() {
    mVec2_c dpdPosScreen = dPad::ex_c::getInstance()->getDpdPosScreen();
    u8 oldAlpha = mAlpha;
    bool bIsInBounds = false;
    if (field_0x705 == 1) {
        mVec2_c dpdPos = mPad::getDpdRawPos();
        dpdPosScreen.x = dGfx_c::getWidth4x3F() * 0.5f * (1.f + dpdPos.x) + dGfx_c::getWidth4x3LeftF();
        dpdPosScreen.y = dGfx_c::getCurrentScreenHeightF() * -0.5f * (1.f + dpdPos.y) + dGfx_c::getCurrentScreenTopF();

        if (dPad::ex_c::getInstance()->field_0x22D8 == 0) {
            dpdPosScreen.x = EGG::Math<f32>::maxNumber();
            dpdPosScreen.y = EGG::Math<f32>::maxNumber();
        }
    }

    if (dGfx_c::getWidth4x3LeftF() <= dpdPosScreen.x && dpdPosScreen.x <= dGfx_c::getWidth4x3RightF()) {
        if (dGfx_c::getCurrentScreenBottomF() <= dpdPosScreen.y && dpdPosScreen.y <= dGfx_c::getCurrentScreenTopF()) {
            bIsInBounds = true;
        }
    }

    mCsVelocity = dpdPosScreen - mCsPosition;
    mCsPosition = dpdPosScreen;

    sLib::addCalcAngle(&field_0x700.mVal, mCsVelocity.ang(), 0x10, 2000);
    sLib::chaseUC(&mAlpha, bIsInBounds ? 255 : 0, 10);
    if (oldAlpha != mAlpha) {
        mLyts[0].getLayout()->GetRootPane()->SetAlpha(mAlpha);
        mLyts[1].getLayout()->GetRootPane()->SetAlpha(mAlpha);
    }

    if (dPadNav::isPointerVisible()) {
        mCursorStick.setShouldBeOn(false);
    } else if (dPadNav::isCursorStickVisible()) {
        mCursorStick.setShouldBeOn(true);
    }

    mCursorStick.execute();
    return SUCCEEDED;
}

int dCsBase_c::draw() {
    if (field_0x704 == 1) {
        return SUCCEEDED;
    }

    if (field_0x703 && dPadNav::isPointerVisible() && mAlpha) {
        nw4r::lyt::Pane *p = mpCurrLyt->getLayout()->GetRootPane();
        nw4r::math::VEC3 pos;
        pos.x = mCsPosition.x;
        pos.y = mCsPosition.y;
        pos.z = 0.0f;
        p->SetTranslate(pos);
        mpCurrLyt->calc();
        mpCurrLyt->addToDrawList();
    }
    mCursorStick.draw();

    return SUCCEEDED;
}

void dCsBase_c::setCurrentLyt(int lyt) {
    mpCurrLyt = &mLyts[lyt];
}

int dCsBase_c::update() {
    return execute();
}

bool dCsBase_c::drawDirectly() {
    if (field_0x704 != 1) {
        return true;
    }

    if (field_0x703 && dPadNav::isPointerVisible() && mAlpha) {
        nw4r::lyt::Pane *p = mpCurrLyt->getLayout()->GetRootPane();
        nw4r::math::VEC3 pos;
        pos.x = mCsPosition.x;
        pos.y = mCsPosition.y;
        pos.z = 0.0f;
        p->SetTranslate(pos);
        mpCurrLyt->calc();
        mpCurrLyt->draw();
    }
    mCursorStick.drawDirectly();

    return true;
}
