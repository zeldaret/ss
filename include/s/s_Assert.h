#ifndef S_ASSERT_H
#define S_ASSERT_H

namespace sAssert {

typedef void (*AssertCallback)(void *);

void setAssertCallback(AssertCallback cb);
void assert();

} // namespace sAssert

#endif
