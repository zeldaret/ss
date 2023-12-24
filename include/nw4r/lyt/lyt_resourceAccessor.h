#ifndef NW4R_LYT_RESOURCE_ACCESSOR_H
#define NW4R_LYT_RESOURCE_ACCESSOR_H
#include "types_nw4r.h"
#include "ut_Font.h"

namespace nw4r
{
    namespace lyt
    {
        class ResourceAccessor
        {
        public:
            ResourceAccessor();
            virtual ~ResourceAccessor(); // at 0x8
            virtual UNKTYPE * GetResource(u32, const char *, u32 *) = 0; // at 0xC
            virtual ut::Font * GetFont(const char *); // at 0x10
        };
    }
}

#endif