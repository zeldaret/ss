#include <d/tg/d_t_tackle.h>

SPECIAL_ACTOR_PROFILE(TACKLE_TAG, dTgTackle_c, fProfile::TACKLE_TAG, 0x0241, 0, 0);

int dTgTackle_c::create() {
    return SUCCEEDED;
}

int dTgTackle_c::doDelete() {
    return 1;
}

int dTgTackle_c::actorExecute() {
    return 1;
}

int dTgTackle_c::draw() {
    return 1;
}
