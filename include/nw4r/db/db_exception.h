#ifndef NW4R_DB_EXCEPTION_H
#define NW4R_DB_EXCEPTION_H

#include <nw4r/db/db_console.h>
#include <rvl/OS.h>


namespace nw4r {
namespace db {

typedef struct {
    u16 error;
    OSContext *ctx;
    u32 dsisr;
    u32 dar;
} ExceptionCallbackParam;

void Exception_Init();
ConsoleHandle Exception_SetConsole(ConsoleHandle, const _GXRenderModeObj *);
ConsoleHandle Exception_GetConsole();
void Exception_SetUserCallback(bool (*)(ConsoleHandle, void *), void *);
u16 Exception_SetDisplayInfo(u16 newInfo);

} // namespace db
} // namespace nw4r

#endif
