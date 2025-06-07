#include "d/a/d_a_base.h"

extern "C" dSoundSourceIf_c *soundForActorInitRelated_803889c0(int, fBase_c *, char *, u8);

extern "C" bool fn_8002C250(SoundInfo *p1, int someNum, char *name, mVec3_c *position) {
    p1->sound_source = soundForActorInitRelated_803889c0(someNum, p1->actor, name, 0);
    if (!p1->sound_source) {
        return false;
    }
    p1->actor->sound_list.append(p1);
    p1->obj_pos = position;
    return true;
}
