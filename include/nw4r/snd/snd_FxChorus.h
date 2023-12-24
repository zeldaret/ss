#ifndef NW4R_SND_FX_CHORUS_H
#define NW4R_SND_FX_CHORUS_H
#include <AXFXChorus.h>
#include "types_nw4r.h"
#include "snd_FxBase.h"
#include "snd_AxfxImpl.h"

namespace nw4r
{
	namespace snd
	{
		struct FxChorus : FxBase
		{
			//sizeof(ChorusParam) = 0xc
			struct ChorusParam
			{
				float FLOAT_0x0;
				float FLOAT_0x4;
				float FLOAT_0x8;
			};
			
			FxChorus();
			
			inline ~FxChorus()
			{
				Shutdown();
				ReleaseWorkBuffer();
			}
			
			u32 GetRequiredMemSize();
			virtual UNKTYPE AssignWorkBuffer(void *, u32); // at 0x18
			virtual UNKTYPE ReleaseWorkBuffer(); // at 0x1c
			
			bool StartUp(); // at 0xc
			UNKTYPE Shutdown(); // at 0x10
			bool SetParam(const ChorusParam &);
			UNKTYPE UpdateBuffer(int, void **, u32, SampleFormat, float, OutputMode); // at 0x14
			
			detail::AxfxImpl mAxfxImpl;
			
			ChorusParam mParam; // at 0x18
			AXFX_CHORUS mAXFXChorus; // 0x24
		};
	}
}

#endif