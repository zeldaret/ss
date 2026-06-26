#include "d/lyt/d_lyt_control_title.h"

#include "f/f_base.h"


SPECIAL_BASE_PROFILE(LYT_CONTROL_TITLE, dLytControlTitle_c, fProfile::LYT_CONTROL_TITLE, 0X2AB, 0);

dLytControlTitle_c::dLytControlTitle_c() {}


int dLytControlTitle_c::create() {
    return SUCCEEDED;
}



int dLytControlTitle_c::doDelete() {
    return SUCCEEDED;
}

int dLytControlTitle_c::execute() {
    return SUCCEEDED;
}

int dLytControlTitle_c::draw() {
    return SUCCEEDED;
}

dLytControlTitle_c::~dLytControlTitle_c() {}
