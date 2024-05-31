#include <common.h>
#include <s/s_StateInterfaces.hpp>
#include <s/s_StateID.hpp>
#include <s/s_StateMethodUsr_FI.hpp>
#include <s/s_StateIDChk.hpp>

sStateID_c::NumberMemo_c sStateID_c::sm_numberMemo;
sStateID_c sStateID::null(nullptr);

sStateID_c::sStateID_c(const char *name) {
    mpName = name;
    mNumber = (name != nullptr) ? sm_numberMemo.get() : 0;
}

sStateID_c::~sStateID_c() {}

bool sStateID_c::isNull() const {
    return mNumber == 0;
}

bool sStateID_c::isEqual(const sStateIDIf_c &other) const {
    return number() == other.number();
}

bool sStateID_c::operator==(const sStateIDIf_c &other) const {
    return isEqual(other);
}

bool sStateID_c::operator!=(const sStateIDIf_c &other) const {
    return !isEqual(other);
}

bool sStateID_c::isSameName(const char *name) const {
    // Just return false, the full implementation is in sFStateID_c
    return false;
}

const char *sStateID_c::name() const {
    return mpName;
}

unsigned int sStateID_c::number() const {
    return mNumber;
}

sStateFctIf_c::~sStateFctIf_c() {}
sStateIDChkIf_c::~sStateIDChkIf_c() {}
sStateIDChk_c::~sStateIDChk_c() {}
sStateIDIf_c::~sStateIDIf_c() {}
sStateIf_c::~sStateIf_c() {}
sStateMethodIf_c::~sStateMethodIf_c() {}
sStateMethodUsr_FI_c::~sStateMethodUsr_FI_c() {}
sStateMgrIf_c::~sStateMgrIf_c() {}
