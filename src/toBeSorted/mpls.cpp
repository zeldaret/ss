#include "toBeSorted/mpls.h"

#include "s/s_Assert.h"

#include "rvl/OS.h"
#include "rvl/SC.h"

#include <cstring>

bool lbl_80574FA8;
bool lbl_80574FA9;
bool lbl_80574FAA;

void fn_80006CE0(int argc, char **argv) {
    if (argc == 2) {
        if (strcmp("MPLS_FIRST", argv[1]) == 0) {
            lbl_80574FA9 = true;
        } else if (strcmp("MPLS_NORMAL", argv[1]) == 0) {
            lbl_80574FA9 = true;
            lbl_80574FAA = true;
        }
    }

    lbl_80574FA8 = true;
}

void fn_80006D60() {
    if (!lbl_80574FA9 && !SCGetMplsMoviePlay()) {
        OSExecl("/sys/mpls_movie/player.dol", "", "", "ASPECT_RATIO_AUTO", "DEFAULT", nullptr);
        sAssert::assert();
    }
}

void fn_80006DC0() {
    OSExecl("/sys/mpls_movie/player.dol", "", "", "ASPECT_RATIO_AUTO", "FORCE_MENU", nullptr);
    sAssert::assert();
}
