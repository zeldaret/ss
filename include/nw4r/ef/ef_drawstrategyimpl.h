#ifndef NW4R_EF_DRAW_STRATEGY_IMPL_H
#define NW4R_EF_DRAW_STRATEGY_IMPL_H
#include "types_nw4r.h"
#include "ef_drawstrategy.h"
#include "math_types.h"

namespace nw4r
{
	namespace ef
	{
		class DrawStrategyImpl : public DrawStrategy
		{
		public:
			struct PrevTexture
			{
				UNKWORD WORD_0x0;
				f32 FLOAT_0x4;
				f32 FLOAT_0x8;
				f32 FLOAT_0xC;
				f32 FLOAT_0x10;
				UNKWORD WORD_0x14;
				UNKWORD WORD_0x18;
				math::VEC2 VEC2_0x1C;
				f32 FLOAT_0x24;
				math::VEC2 VEC2_0x28;
			};

		public:
			DrawStrategyImpl();
			virtual UNKTYPE GetGetFirstDrawParticleFunc(int);
			virtual UNKTYPE GetGetNextDrawParticleFunc(int);

			void InitTexture(const EmitterDrawSetting &);
			void InitTev(const EmitterDrawSetting &, const DrawInfo &);
			void InitColor(ParticleManager *, const EmitterDrawSetting &, const DrawInfo &);

		private:
			PrevTexture mPrevTextures[3]; // at 0x4
			char UNK_0x94[0xBC - 0x94];
		};
	}
}

#endif