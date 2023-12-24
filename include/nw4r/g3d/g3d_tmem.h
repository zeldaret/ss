#ifndef NW4R_G3D_TMEM_H
#define NW4R_G3D_TMEM_H
#include "types_nw4r.h"

namespace nw4r
{
    namespace g3d
    {
        namespace tmem
        {
            enum TMemLayout
            {
                TMEM_LAYOUT_0,
                TMEM_LAYOUT_1
            };

            void SetTMemLayout(TMemLayout);
        }
    }
}

#endif