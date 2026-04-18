#ifndef EGG_PROCESS_METER_H
#define EGG_PROCESS_METER_H

#include "egg/core/eggPerf.h"
#include "egg/core/eggThread.h"

namespace EGG {

class ProcessMeter : public Thread, public PerformanceView {};

}; // namespace EGG

#endif // EGG_PROCESS_METER_H
