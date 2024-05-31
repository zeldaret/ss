#ifndef NW4R_DB_ASSERT_H
#define NW4R_DB_ASSERT_H

#include <nw4r/db/db_console.h>

namespace nw4r {
namespace db {

typedef detail::ConsoleHead *ConsoleHandle;

ConsoleHandle Assertion_SetConsole(ConsoleHandle handle);

} // namespace db
} // namespace nw4r

#endif
