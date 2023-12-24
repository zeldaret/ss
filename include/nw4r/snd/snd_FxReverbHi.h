#ifndef NW4R_SND_FX_REVERB_HI_H
#define NW4R_SND_FX_REVERB_HI_H
#include <AXFXReverbHi.h>
#include "snd_FxBase.h"
#include "snd_AxfxImpl.h"
#include "types_nw4r.h"

namespace nw4r
{
	namespace snd
	{
		struct FxReverbHi : FxBase
		{
			//sizeof(ReverbHiParam) = 0x18
			struct ReverbHiParam
			{
				float FLOAT_0x0;
				float FLOAT_0x4;
				float FLOAT_0x8;
				float FLOAT_0xC;
				float FLOAT_0x10;
				float FLOAT_0x14;
			};
			
			FxReverbHi();
			
			inline ~FxReverbHi()
			{
				Shutdown();
				ReleaseWorkBuffer();
			}
			
			u32 GetRequiredMemSize();
			virtual UNKTYPE AssignWorkBuffer(void *, u32); // at 0x18
			virtual UNKTYPE ReleaseWorkBuffer(); // at 0x1c
			
			bool StartUp(); // at 0xc
			UNKTYPE Shutdown(); // at 0x10
			bool SetParam(const ReverbHiParam &);
			UNKTYPE UpdateBuffer(int, void **, u32, SampleFormat, float, OutputMode); // at 0x14
			
			detail::AxfxImpl mAxfxImpl; // at 0xc
			ReverbHiParam mParam; // at 0x18
			
			AXFX_REVERBHI mAXFXReverbHi; // at 0x30
		};
	}
}

#endif