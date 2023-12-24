#ifndef NW4R_EF_EFFECTSYSTEM
#define NW4R_EF_EFFECTSYSTEM
#include "types_nw4r.h"
#include "ef_creationqueue.h"
#include "ef_ref.h"
#include "ef_rand.h"
#include "ef_list.h"

namespace nw4r
{
	namespace ef
	{
		struct EffectSystem
		{
			static bool mDisplayVersion;
			
			static EffectSystem instance;
			
			MemoryManager * mMemoryManager; // at 0x0
			DrawOrder * mDrawOrder; // at 0x4
			DrawStrategyBuilder * mDrawStrategyBuilder; // at 0x8
			EmitFormBuilder * mEmitFormBuilder; // at 0xc
			CreationQueue mCreationQueue; // at 0x10
			u32 mNumGroup;
			ActivityList * ARR_0x5018;
			Random mRandom; // at 0x501C
			math::VEC3 VEC_0x5020;
			math::MTX34 MTX_0x502C;
			float FLOAT_0x505C;
			float FLOAT_0x5060;
			char BYTE_0x5064;
			
			inline MemoryManager * GetMemoryManager() const
			{
				return mMemoryManager;
			}
			
			EffectSystem();
			~EffectSystem();
			
			static EffectSystem * GetInstance();
			
			bool Initialize(u32);
			bool Closing(Effect *);
			Effect * CreateEffect(const char *, u32, u16);
			bool RetireEffect(Effect *);
			u16 RetireEffectAll(u32);
			u16 RetireEmitterAll(u32);
			u16 RetireParticleAll(u32);
			
			void Calc(u32, bool);
			void Draw(const DrawInfo &, u32);
		};
	}
}

#endif