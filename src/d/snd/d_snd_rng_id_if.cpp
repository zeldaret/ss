#include "d/snd/d_snd_rng_id_if.h"

bool dSndRngIdIf_c::doNextIdNoReuse() {
    return handleId(nextIdNoReuse());
}

bool dSndRngIdIf_c::doRndId() {
    return handleId(rndId());
}

bool dSndRngIdIf_c::doRndIdNoReuse() {
    return handleId(rndIdNoReuse());
}

bool dSndRngIdIf_c::doRndIdNotSame() {
    return handleId(rndIdNotSame());
}
