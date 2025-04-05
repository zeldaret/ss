#include "d/lyt/msg_window/d_lyt_msg_window_common.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"

bool dLytMsgWindowSubtype::build(d2d::ResAccIf_c *, d2d::ResAccIf_c *, dTagProcessor_c *) {
    return true;
}

bool dLytMsgWindowSubtype::remove() {
    return true;
}

bool dLytMsgWindowSubtype::execute() {
    return true;
}

void dLytMsgWindowSubtype::open(dAcObjBase_c *obj, u32 param) {}

bool dLytMsgWindowSubtype::isDoneOpening() const {
    return true;
}

void dLytMsgWindowSubtype::close() {}

bool dLytMsgWindowSubtype::isDoneClosing() const {
    return true;
}

bool dLytMsgWindowSubtype::setText(const wchar_t *text) {
    return true;
}

static const char *sSwordPane = "RootPane";
static const char *sSwordTextBoxes[] = {
    "T_font_00",
    "T_fontS_00",
    "T_fontS_01",
};

bool dLytTextSword::build(d2d::ResAccIf_c *resAcc, ESwordType type) {
    mType = type;
    mLyt.setResAcc(resAcc);
    if (mType == GHIRAHIM) {
        mLyt.build("textGirahim00.brlyt", nullptr);
    } else if (mType == LASTBOSS) {
        mLyt.build("textLastBoss_00.brlyt", nullptr);
    } else {
        mLyt.build("textSword_00.brlyt", nullptr);
    }

    mLyt.setPriority(0x8A);

    if (mType == GHIRAHIM) {
        mAnm.init("textGirahim00_in.brlan", resAcc, mLyt.getLayout(), "G_inOut_00");
    } else if (mType == LASTBOSS) {
        mAnm.init("textLastBoss_00_in.brlan", resAcc, mLyt.getLayout(), "G_inOut_00");
    } else {
        mAnm.init("textSword_00_in.brlan", resAcc, mLyt.getLayout(), "G_inOut_00");
    }

    mAnm.bind(false);
    mAnm.setAnimEnable(true);

    mpPane = mLyt.findPane(sSwordPane);

    for (int i = 0; i < 3; i++) {
        mpTexts[i] = mLyt.getTextBox(sSwordTextBoxes[i]);
    }

    return true;
}

bool dLytTextSword::remove() {
    mAnm.remove();
    return true;
}

void dLytTextSword::draw(const wchar_t *str, s8 timer, mVec3_c pos, f32 fScale) {
    for (int i = 0; i < 3; i++) {
        mpTexts[i]->setTextWithGlobalTextProcessor(str, nullptr);
    }

    f32 f = fScale / dTagProcessor_c::fn_800B8040(0, 0);
    nw4r::math::VEC2 scale = nw4r::math::VEC2(f, f);

    mAnm.setFrame(timer);
    mpPane->SetLocationAdjust(true);
    mpPane->SetTranslate(pos);
    mpPane->SetScale(scale);
    mLyt.calc();
    mpPane->CalculateMtx(mLyt.getDrawInfo());
    mpPane->Draw(mLyt.getDrawInfo());
}

f32 dLytTextSword::getAnimDuration() const {
    return mAnm.getAnimDuration();
}

static const char *sLightPane = "RootPane";

bool dLytTextLight::build(d2d::ResAccIf_c *resAcc, ETextType type) {
    mType = type;
    mLyt.setResAcc(resAcc);

    if (type == DEMO) {
        mLyt.build("textLightDemo_00.brlyt", nullptr);
    } else {
        mLyt.build("textLight_00.brlyt", nullptr);
    }

    mLyt.setPriority(0x8A);

    if (mType == DEMO) {
        mAnm.init("textLightDemo_00_loop.brlan", resAcc, mLyt.getLayout(), "G_loop_00");
    } else {
        mAnm.init("textLight_00_loop.brlan", resAcc, mLyt.getLayout(), "G_loop_00");
    }

    mAnm.bind(false);
    mAnm.setAnimEnable(true);

    mpPane = mLyt.findPane(sLightPane);

    return true;
}

bool dLytTextLight::remove() {
    mAnm.remove();
    return true;
}

void dLytTextLight::draw(s8 frame, mVec3_c pos, u8 alpha, f32 fScale) {
    f32 f = fScale / dTagProcessor_c::fn_800B8040(0, 0);
    nw4r::math::VEC2 scale = nw4r::math::VEC2(f, f);

    mAnm.setFrame(frame);
    mpPane->SetLocationAdjust(true);
    mpPane->SetInfluencedAlpha(true);
    mpPane->SetAlpha(alpha);
    mpPane->SetTranslate(pos);
    mpPane->SetScale(scale);
    mLyt.calc();
    mpPane->CalculateMtx(mLyt.getDrawInfo());
    mpPane->Draw(mLyt.getDrawInfo());
}

f32 dLytTextLight::getAnimDuration() const {
    return mAnm.getAnimDuration();
}
