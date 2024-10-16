#ifndef M_PAD_H
#define M_PAD_H

#include "egg/core/eggController.h"

namespace mPad {

extern EGG::CoreControllerMgr *g_padMg;
extern int g_currentCoreId;
extern EGG::CoreController *g_currentCore;
extern EGG::CoreController *g_core[4];

void create();
void beginPad();
void endPad();

} // namespace mPad

#endif
