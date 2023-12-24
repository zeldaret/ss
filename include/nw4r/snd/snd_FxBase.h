#ifndef NW4R_SND_FX_BASE_H
#define NW4R_SND_FX_BASE_H
#include "types_nw4r.h"
#include "snd_Common.h"
#include "ut_LinkList.h"

namespace nw4r
{
	namespace snd
	{
		enum SampleFormat
		{
			SAMPLE_FORMAT_PCM_S32,
			SAMPLE_FORMAT_PCM_S16,
			SAMPLE_FORMAT_PCM_S8,
			SAMPLE_FORMAT_DSP_ADPCM
		};
		
		struct FxBase
		{
			inline virtual ~FxBase() {} // at 0x8
			inline virtual bool StartUp() {} // at 0xc
			inline virtual UNKTYPE Shutdown() {} // at 0x10
			inline virtual UNKTYPE UpdateBuffer(int, void **, u32, SampleFormat, float, OutputMode) {} // at 0x14
			
			NW4R_UT_LIST_NODE_DECL(); // at 0x4
		};

		NW4R_UT_LIST_TYPEDEF_DECL(FxBase);
	}
}

#endif