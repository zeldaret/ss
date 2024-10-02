#include <d/d_fader.h>
#include <m/m_fader.h>

static mColor mFaderColor = mColor(200, 200, 200, 0xFF);

dFader_c::~dFader_c() {
    if (mpScreenFader != nullptr) {
        delete mpScreenFader;
        mpScreenFader = nullptr;
    }
    if (mpLytFader != nullptr) {
        delete mpLytFader;
        mpLytFader = nullptr;
    }
    if (mpColorFader != nullptr) {
        delete mpColorFader;
        mpColorFader = nullptr;
    }
}

bool dFader_c::create() {
    mpColorFader = new mColorFader_c(mColor(0, 0, 0, 0x80), mFaderBase_c::FADED_OUT);
    if (!mpColorFader) {
        return false;
    }

    mpLytFader = new dLytFader_c(mColor(0, 0, 0, 0xFF), mFaderBase_c::FADED_OUT);
    if (!mpLytFader) {
        return false;
    }

    mpScreenFader = new dScreenFader_c(mColor(0, 0, 0, 0xFF), mFaderBase_c::FADED_OUT);
    if (!mpScreenFader) {
        return false;
    }

    mFadeOutType = 0;
    mFadeInType = 0;
    field_0x20 = 0x1E;
    field_0x22 = 0x1E;
    setFader(0);

    return true;
}

bool dFader_c::setFader(s32 fader) {
    switch (fader) {
    case 0:
        if (mFader_c::setFader(mpColorFader)) {
            if (mpColorFader->getStatus() == mFaderBase_c::FADED_IN) {
                mpColorFader->setColor(mColor(0, 0, 0, 0xFF));
            }
            return true;
        }
        break;
    case 1:
        if (mFader_c::setFader(mpColorFader)) {
            if (mpColorFader->getStatus() == mFaderBase_c::FADED_IN) {
                mpColorFader->setColor(mFaderColor);
            }
            return true;
        }
        break;
    case 2:
        if (mFader_c::setFader(mpLytFader)) {
            if (mpLytFader->getStatus() == mFaderBase_c::FADED_IN) {
                mpLytFader->setColor(mColor(0, 0, 0, 0xFF));
            }
            return true;
        }
        break;
    case 3:
        if (mFader_c::setFader(mpScreenFader)) {
            if (mpScreenFader->getStatus() == mFaderBase_c::FADED_IN) {
                mpScreenFader->setColor(mColor(0, 0, 0, 0xFF));
            }
            return true;
        }
        break;
    }

    return false;
}

u8 dFader_c::getColorFaderAlpha() {
    if (mpColorFader != nullptr) {
        return mpColorFader->mFaderColor.a;
    }
    return 0xFF;
}

bool dFader_c::initForLyt() {
    bool ok = false;
    if (mpLytFader != nullptr) {
        ok = mpLytFader->init();
    }
    return ok;
}
