#include "toBeSorted/d_lib.h"

#include <cstring>

u32 lbl_80574FA0;
u32 lbl_80574FA4;

void unknownStub() {}

bool strequals(const char *lhs, const char *rhs) {
    if (!rhs) {
        return false;
    }

    size_t lhs_len = strlen(lhs);
    size_t rhs_len = strlen(rhs);

    return (lhs_len == rhs_len) && strncmp(lhs, rhs, lhs_len) == 0;
}
