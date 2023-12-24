#ifndef NW4R_G3D_ANMTEXPAT_H
#define NW4R_G3D_ANMTEXPAT_H
#include "g3d_restex.h"

namespace nw4r
{
	namespace g3d
	{
		struct TexPatAnmResult
		{
			enum
			{
				ANM_COUNT = 8
			};
			
			u8 mTexFlags; // at 0x0
			u8 mPlttFlags; // at 0x1
			ResTex mTexs[ANM_COUNT]; // at 0x4
			ResPltt mPltts[ANM_COUNT]; // at 0x24
		};
	}
}

#endif