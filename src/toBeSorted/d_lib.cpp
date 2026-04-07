#include "toBeSorted/d_lib.h"

#include <cstring>

void dLib::unknownStub() {}

bool dLib::strequals(const char *lhs, const char *rhs) {
    if (!rhs) {
        return false;
    }

    size_t lhs_len = strlen(lhs);
    size_t rhs_len = strlen(rhs);

    return (lhs_len == rhs_len) && strncmp(lhs, rhs, lhs_len) == 0;
}
