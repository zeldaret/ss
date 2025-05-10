#ifndef D_SYS_H
#define D_SYS_H

#include "common.h"
#include "m/m_color.h"

namespace dSys {

void setFrameRate(u32);
u8 getFrameRate();
void setClearColor(mColor clr);

} // namespace dSys_c

#endif
