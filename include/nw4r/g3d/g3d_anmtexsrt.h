#ifndef NW4R_G3D_ANMTEXSRT_H
#define NW4R_G3D_ANMTEXSRT_H
#include "g3d_restex.h"

namespace nw4r
{
	namespace g3d
	{
		struct TexSrtTypedef
		{
			enum TexMatrixMode
			{
				TEX_MTX_MAYA,
				TEX_MTX_XSI,
				TEX_MTX_3DSMAX
			};
		};

		struct TexSrt
		{
			enum Flag
			{
				FLAG_ANM_EXISTS,
			};

			float mScaleX; // at 0x0
			float mScaleY; // at 0x4
			float mRotation; // at 0x8
			float mTranslationX; // at 0xc
			float mTranslationY; // at 0x10
		};
		
		struct TexSrtAnmResult
		{
			enum
			{
				ANM_COUNT = 8
			};
			
			UNKWORD FLAGS_0x0;
			UNKWORD FLAGS_0x4;
			TexSrt mSrts[ANM_COUNT * 2]; // at 0x8
		};
	}
}

#endif