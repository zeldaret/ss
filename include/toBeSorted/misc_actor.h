#ifndef MISC_ACTOR_H
#define MISC_ACTOR_H

// TODO - this header might be unnecessary; a lot of functions in
// the split corresponding to this header clearly are REL functions
// that were moved to the DOL because they need to be called from the DOL.
// So ideally all of these declarations were moved to REL class definitions...

bool checkIsInSkykeepPuzzle();
void restoreSkykeepPuzzleFromGlobal();

#endif
