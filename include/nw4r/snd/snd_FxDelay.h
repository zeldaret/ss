#ifndef NW4R_SND_FX_DELAY_H
#define NW4R_SND_FX_DELAY_H
#include <AXFXDelay.h>
#include "snd_FxBase.h"
#include "snd_AxfxImpl.h"
#include "types_nw4r.h"

namespace nw4r
{
	namespace snd
	{
		struct FxDelay : FxBase
		{
			//sizeof(FxDelayParam) = 0xc
			struct DelayParam
			{
				float FLOAT_0x0;
				float FLOAT_0x4;
				float FLOAT_0x8;
			};
			
			FxDelay();
			
			inline ~FxDelay()
			{
				Shutdown();
				ReleaseWorkBuffer();
			}
			
			u32 GetRequiredMemSize();
			virtual UNKTYPE AssignWorkBuffer(void *, u32); // at 0x18
			virtual UNKTYPE ReleaseWorkBuffer(); // at 0x1c
			
			bool StartUp(); // at 0xc
			UNKTYPE Shutdown(); // at 0x10
			bool SetParam(const DelayParam &);
			UNKTYPE UpdateBuffer(int, void **, u32, SampleFormat, float, OutputMode); // at 0x14
			
			detail::AxfxImpl mAxfxImpl; // at 0xc
			DelayParam mParam; // at 0x18
			
			AXFX_DELAY mAXFXDelay; // at 0x24
		};
	}
}

#endif