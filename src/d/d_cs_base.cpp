#include "d/d_cs_base.h"

#include "common.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

SPECIAL_BASE_PROFILE(C_BASE, dCsBase_c, fProfile::C_BASE, 0x2C0, 0x06F9);

dCsBase_c::dCsBase_c()
    : mpCurrLyt(nullptr), field_0x6F0(0.0f), field_0x6F4(0.0f), field_0x6F8(0.0f), field_0x6FC(0.0f), field_0x700(0),
      field_0x702(0), field_0x703(0), field_0x704(0), field_0x705(0) {
    sInstance = this;
}

dCsBase_c::~dCsBase_c() {
    sInstance = nullptr;
}

extern "C" void fn_8016B2B0();
extern "C" void fn_8016B2E0();
extern "C" bool lbl_80572D10;
extern "C" int lbl_80573438;

static const char *sLytNames[] = {
    "P1_Def.brlyt",
    "P1_Cat.brlyt",
};

#pragma explicit_zero_data on
int dCsBase_c::sfield0x700Init = 0;
#pragma explicit_zero_data off

int dCsBase_c::create() {
    void *data = LayoutArcManager::sInstance->getLoadedData("cursor");
    mResAcc.attach(data, "arc");

    for (int i = 0; i < 2; i++) {
        mLyts[i].setResAcc(&mResAcc);
        mLyts[i].build(sLytNames[i], nullptr);
        mLyts[i].setPriority(0x88);
        mLyts[i].calc();
    }
    setCurrentLyt(0);
    mStructC.field_0x10 = 1;
    d2d::dLytStructDList::sInstance->appendToList1(&mStructC);
    fn_8016B2B0();
    field_0x6F0 = 0.0f;
    field_0x6F4 = 0.0f;
    field_0x6F8 = 0.0f;
    field_0x6FC = 0.0f;

    field_0x700 = sfield0x700Init;
    field_0x702 = 0;
    field_0x703 = 0;

    return SUCCEEDED;
}

int dCsBase_c::doDelete() {
    d2d::dLytStructDList::sInstance->removeFromList1(&mStructC);
    fn_8016B2E0();
    mResAcc.detach();
    return SUCCEEDED;
}

int dCsBase_c::execute() {
    // TODO this needs player / controller / d_pad stuff?

    return SUCCEEDED;
}

int dCsBase_c::draw() {
    if (field_0x704 == 1) {
        return SUCCEEDED;
    }

    if (field_0x703 && lbl_80572D10 && field_0x702) {
        nw4r::lyt::Pane *p = mpCurrLyt->getLayout()->GetRootPane();
        nw4r::math::VEC3 pos;
        pos.x = field_0x6F0;
        pos.y = field_0x6F4;
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

int dCsBase_c::similarToDraw() {
    if (field_0x704 != 1) {
        return SUCCEEDED;
    }

    if (field_0x703 && lbl_80572D10 && field_0x702) {
        nw4r::lyt::Pane *p = mpCurrLyt->getLayout()->GetRootPane();
        nw4r::math::VEC3 pos;
        pos.x = field_0x6F0;
        pos.y = field_0x6F4;
        pos.z = 0.0f;
        p->SetTranslate(pos);
        mpCurrLyt->calc();
        mpCurrLyt->draw();
    }
    mCursorStick.similarToDraw();

    return SUCCEEDED;
}
