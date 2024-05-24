#ifndef DB_ASSERT_H
#define DB_ASSERT_H

namespace nw4r {
namespace db {

namespace detail {
    struct ConsoleHead {

    };
}

typedef detail::ConsoleHead* ConsoleHandle;

ConsoleHandle Assertion_SetConsole(ConsoleHandle handle);

} // namespace db
} // namespace nw4r

#endif
