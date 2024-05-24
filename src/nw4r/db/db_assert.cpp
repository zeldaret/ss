#include <common.h>
#include <nw4r/db/db_assert.h>

namespace nw4r {
namespace db {

static ConsoleHandle sConsoleHandle = nullptr;

ConsoleHandle Assertion_SetConsole(ConsoleHandle handle) {
    ConsoleHandle oldHandle = sConsoleHandle;
    sConsoleHandle = handle;
    return oldHandle;
}

} // namespace db
} // namespace nw4r
