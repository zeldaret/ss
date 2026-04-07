#ifndef EGG_PROCESS_METER_H
#define EGG_PROCESS_METER_H

#include "egg/core/eggThread.h"

namespace EGG {

class PerformanceView {};

class ProcessMeter : public Thread, public PerformanceView {};

};

#endif // EGG_PROCESS_METER_H
