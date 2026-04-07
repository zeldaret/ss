#include "toBeSorted/mpls.h"

#include "s/s_Assert.h"

#include "rvl/OS.h"
#include "rvl/SC.h"

#include <cstring>

bool sMplsInitialized;
bool sSkipMplsMovie;
bool sIsNormalMode;

void Mpls::initialize(int argc, char **argv) {
    if (argc == 2) {
        if (strcmp("MPLS_FIRST", argv[1]) == 0) {
            sSkipMplsMovie = true;
        } else if (strcmp("MPLS_NORMAL", argv[1]) == 0) {
            sSkipMplsMovie = true;
            sIsNormalMode = true;
        }
    }

    sMplsInitialized = true;
}

void Mpls::tryLaunchMovie() {
    if (!sSkipMplsMovie && !SCGetMplsMoviePlay()) {
        OSExecl("/sys/mpls_movie/player.dol", "", "", "ASPECT_RATIO_AUTO", "DEFAULT", nullptr);
        sAssert::assert();
    }
}

void Mpls::forceLaunchMovie() {
    OSExecl("/sys/mpls_movie/player.dol", "", "", "ASPECT_RATIO_AUTO", "FORCE_MENU", nullptr);
    sAssert::assert();
}
