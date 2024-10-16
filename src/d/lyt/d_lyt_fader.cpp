#include "d/lyt/d_lyt_fader.h"

#include "toBeSorted/arc_managers/layout_arc_manager.h"


static const d2d::LytBrlanMapping blanMap[] = {
    {"wipe_01_wipeStart.brlan", "G_wipe_00"},
    { "wipe_01_wipeOpen.brlan", "G_wipe_00"},
    {      "wipe_01_out.brlan",  "G_out_00"},
};

dLytFader_c::dLytFader_c(const mColor &color, EStatus status) : mFaderBase_c(color, status), field_0x4DD(false) {}

dLytFader_c::~dLytFader_c() {
    0.0f; // float ordering

    if (field_0x4DD == true) {
        for (int i = 0; i < 3; i++) {
            mLytStructAs[i].afterUnbind();
        }
        mResAcc.detach();
    }
}

void dLytFader_c::setStatus(EStatus status) {
    if (status == FADED_OUT) {
        mStatus = FADED_OUT;
        mLytBase.unbindAnims();
        d2d::AnmGroup_c *s = &mLytStructAs[0];
        s->setDirection(false);
        s->setToEnd();
        s->setAnimEnable(true);
        s->play();
        mLytBase.calc();
        fn_80175BC0(2);
        mLytBase.getLayout()->GetRootPane()->SetVisible(true);
    } else if (status == FADED_IN) {
        mStatus = FADED_IN;
        mLytBase.unbindAnims();
        d2d::AnmGroup_c *s = &mLytStructAs[0];
        s->setDirection(false);
        s->setToStart();
        s->setAnimEnable(true);
        mLytBase.calc();
        fn_80175BC0(2);
        mLytBase.getLayout()->GetRootPane()->SetVisible(false);
    }
}

bool dLytFader_c::calc() {
    if (field_0x4DD == true) {
        switch (field_0x4D4) {
            case 0:  fn_801759B0(); break;
            case 1:  fn_80175B10(); break;
            default: break;
        }
        return field_0x4D4 == 2;
    } else {
        return mFaderBase_c::calc();
    }
}

extern "C" void fn_80016200();

void dLytFader_c::draw() {
    if (field_0x4DD) {
        fn_80016200();
        mLytBase.draw();
    }
}

bool dLytFader_c::fadeIn() {
    bool res = mFaderBase_c::fadeIn();
    if (res) {
        fn_80175A50();
    }
    return res;
}

bool dLytFader_c::fadeOut() {
    bool res = mFaderBase_c::fadeOut();
    if (res) {
        fn_801758F0();
    }
    return res;
}

bool dLytFader_c::init() {
    if (field_0x4DD == true) {
        return true;
    } else {
        void *data = LayoutArcManager::sInstance->getLoadedData("System2D");
        if (!data) {
            return false;
        }
        mResAcc.attach(data, "");
        mLytBase.setResAcc(&mResAcc);
        mLytBase.build("wipe_01.brlyt", nullptr);

        for (int i = 0; i < 3; i++) {
            mLytStructAs[i].init(blanMap[i].mFile, &mResAcc, mLytBase.getLayout(), blanMap[i].mName);
        }
        field_0x4DD = true;
        return true;
    }
}

void dLytFader_c::fn_801758F0() {
    fn_80175BC0(0);
    mLytBase.unbindAnims();
    d2d::AnmGroup_c *s = &mLytStructAs[0];
    s->setDirection(false);
    s->setFrame(0.0f);
    s->setRate(20.0f / (mFrame - 1));
    mLytBase.getLayout()->GetRootPane()->SetVisible(true);
}

void dLytFader_c::fn_801759B0() {
    d2d::AnmGroup_c *s = &mLytStructAs[0];
    if (s->isEndReached() == true) {
        fn_80175BC0(2);
        mStatus = FADED_OUT;
    }
    s->play();
    mLytBase.calc();
}

void dLytFader_c::fn_80175A50() {
    fn_80175BC0(1);
    mLytBase.unbindAnims();
    d2d::AnmGroup_c *s = &mLytStructAs[1];
    s->setDirection(false);
    s->setFrame(0.0f);
    s->setRate(20.0f / (mFrame - 1));
    mLytBase.getLayout()->GetRootPane()->SetVisible(true);
}

void dLytFader_c::fn_80175B10() {
    d2d::AnmGroup_c *s = &mLytStructAs[1];
    if (s->isEndReached() == true) {
        fn_80175BC0(2);
        mStatus = FADED_IN;
        mLytBase.getLayout()->GetRootPane()->SetVisible(false);
    }
    s->play();
    mLytBase.calc();
}

void dLytFader_c::fn_80175BC0(s32 arg) {
    field_0x4D4 = arg;
    field_0x4D8 = 0;
}
