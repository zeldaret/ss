#ifndef RVL_SDK_DSP_H
#define RVL_SDK_DSP_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct DSPTask;

// General-purpose typedef
typedef void* DSPMail;

BOOL DSPCheckMailToDSP(void);
BOOL DSPCheckMailFromDSP(void);
DSPMail DSPReadMailFromDSP(void);
void DSPSendMailToDSP(DSPMail mail);
void DSPAssertInt(void);
void DSPInit(void);
BOOL DSPCheckInit(void);
struct DSPTask* DSPAddTask(struct DSPTask* task);
struct DSPTask* DSPAssertTask(struct DSPTask* task);

#ifdef __cplusplus
}
#endif
#endif
