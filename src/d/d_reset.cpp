#include "d/d_reset.h"

#include "m/m_color.h"

namespace dReset {

Manage_c *Manage_c::s_pInstance;
mColor Manage_c::c_faderColor(0, 0, 0, 0xFF);

Manage_c *Manage_c::GetInstance() {
    return s_pInstance;
}

} // namespace dReset
