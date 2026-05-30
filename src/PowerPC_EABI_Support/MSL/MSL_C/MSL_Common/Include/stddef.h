#ifndef _MSL_STDDEF_H_
#define _MSL_STDDEF_H_

#include <cstddef>

#ifdef __cplusplus
extern "C" {

namespace std {
    using ::ptrdiff_t;
    using ::size_t;
}

};
#endif

#endif
