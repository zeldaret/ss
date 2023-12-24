#ifndef NW4R_EF_HANDLE_H
#define NW4R_EF_HANDLE_H
#include "types_nw4r.h"

namespace nw4r
{
	namespace ef
	{
		struct LinkedObject
		{
			char UNK_0x0[8];
			LinkedObject * PTR_0x8;
		};
		
		struct HandleBase
		{
			LinkedObject * PTR_0x0;
			LinkedObject * mPtr;
			
			HandleBase();
			HandleBase & operator=(const HandleBase &);
			HandleBase & operator=(LinkedObject *);
			bool IsValid() const;
			LinkedObject * GetPtr() const;
		};
	}
}

#endif