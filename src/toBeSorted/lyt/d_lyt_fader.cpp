#include <toBeSorted/arc_managers/layout_arc_manager.h>
#include <toBeSorted/lyt/d_lyt_fader.h>


static const d2d::LytBrlanMapping blanMap[] = {
        {"wipe_01_wipeStart.brlan", "G_wipe_00"},
        {"wipe_01_wipeOpen.brlan", "G_wipe_00"},
        {"wipe_01_out.brlan", "G_out_00"},
};

dLytFader_c::dLytFader_c(const mColor &color, EStatus status) : mFaderBase_c(color, status), field_0x4DD(false) {}

dLytFader_c::~dLytFader_c() {
    0.0f; // float ordering

    if (field_0x4DD == true) {
        for (int i = 0; i < 3; i++) {
            mLytStructAs[i].fn_800AC860();
        }
        mFont.fn_800A9D30();
    }
}

void dLytFader_c::setStatus(EStatus status) {
    if (status == FADED_OUT) {
        mStatus = FADED_OUT;
        mLytBase.fn_800AAB70();
        d2d::dLytStructA *s = &mLytStructAs[0];
        s->fn_800AC6D0(false);
        s->mpFrameCtrl->setToEnd();
        s->fn_800AC910();
        s->fn_800AC780(true);
        s->mpFrameCtrl->play();
        s->fn_800AC910();
        mLytBase.fn_Multi_c_0x14();
        fn_80175BC0(2);
        mLytBase.getLayout()->GetRootPane()->SetVisible(true);
    } else if (status == FADED_IN) {
        mStatus = FADED_IN;
        mLytBase.fn_800AAB70();
        d2d::dLytStructA *s = &mLytStructAs[0];
        s->fn_800AC6D0(false);
        s->mpFrameCtrl->setToStart();
        s->fn_800AC910();
        s->fn_800AC780(true);
        mLytBase.fn_Multi_c_0x14();
        fn_80175BC0(2);
        mLytBase.getLayout()->GetRootPane()->SetVisible(false);
    }
}

bool dLytFader_c::calc() {
    if (field_0x4DD == true) {
        switch (field_0x4D4) {
        case 0:
            fn_801759B0();
            break;
        case 1:
            fn_80175B10();
            break;
        default:
            break;
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
        mFont.fn_800A9D90(data, "");
        mLytBase.setResAcc(&mFont);
        mLytBase.build("wipe_01.brlyt", nullptr);

        for (int i = 0; i < 3; i++) {
            mLytStructAs[i].init(blanMap[i].mFile, &mFont, mLytBase.getLayout(), blanMap[i].mName);
        }
        field_0x4DD = true;
        return true;
    }
}

void dLytFader_c::fn_801758F0() {
    fn_80175BC0(0);
    mLytBase.fn_800AAB70();
    d2d::dLytStructA *s = &mLytStructAs[0];
    s->fn_800AC6D0(false);
    s->mpFrameCtrl->setFrame(0.0f);
    s->fn_800AC910();
    s->mpFrameCtrl->setRate(20.0f / (mFrame - 1));
    s->fn_800AC780(true);
    mLytBase.getLayout()->GetRootPane()->SetVisible(true);
}

void dLytFader_c::fn_801759B0() {
    d2d::dLytStructA *s = &mLytStructAs[0];
    if (s->mpFrameCtrl->isEndReached() == true) {
        fn_80175BC0(2);
        mStatus = FADED_OUT;
    }
    s->mpFrameCtrl->play();
    s->fn_800AC910();
    mLytBase.fn_Multi_c_0x14();
}

void dLytFader_c::fn_80175A50() {
    fn_80175BC0(1);
    mLytBase.fn_800AAB70();
    d2d::dLytStructA *s = &mLytStructAs[1];
    s->fn_800AC6D0(false);
    s->mpFrameCtrl->setFrame(0.0f);
    s->fn_800AC910();
    s->mpFrameCtrl->setRate(20.0f / (mFrame - 1));
    s->fn_800AC780(true);
    mLytBase.getLayout()->GetRootPane()->SetVisible(true);
}

void dLytFader_c::fn_80175B10() {
    d2d::dLytStructA *s = &mLytStructAs[1];
    if (s->mpFrameCtrl->isEndReached() == true) {
        fn_80175BC0(2);
        mStatus = FADED_IN;
        mLytBase.getLayout()->GetRootPane()->SetVisible(false);
    }
    s->mpFrameCtrl->play();
    s->fn_800AC910();
    mLytBase.fn_Multi_c_0x14();
}


void dLytFader_c::fn_80175BC0(s32 arg) {
    field_0x4D4 = arg;
    field_0x4D8 = 0;
}
