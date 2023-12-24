#ifndef NW4R_EF_EFFECT
#define NW4R_EF_EFFECT
#include "types_nw4r.h"
#include "math_types.h"
#include "ef_ref.h"
#include "ut_list.h"
#include "ef_list.h"
#include "ef_util.h"

namespace nw4r
{
	namespace ef
	{
		struct Effect : ReferencedObject
		{
			EffectSystem * mSystem; // at 0x20
			ActivityList mEmitters; // at 0x24
			u32 INDEX_0x40;
			
			void (* mEmissionPrologue)(Emitter *, ParticleManager *, int *, u32 *, float *, u16 *, float *, math::MTX34 *); // at 0x44
			
			UNKWORD WORD_0x48;
			UNKWORD WORD_0x4C;
			u32 mFlags; // at 0x50
			math::MTX34 mRootMtx; // at 0x54
			math::VEC3 VEC_0x84;
			ut::List UTLIST_0x90;
			DrawOrder * mDrawOrder; // at 0x9c
			
			Effect();
			~Effect();
			
			virtual bool Initialize(EffectSystem *, EmitterResource *, u16);
			virtual Emitter * CreateEmitter(ResEmitter, u8, u16);
			virtual void Calc(bool);
			virtual void Draw(const DrawInfo &);
			
			bool SendClosing();
			UNKTYPE DestroyFunc();
			bool Closing(Emitter *);
			Emitter * CreateEmitter(EmitterResource *, u8, u16);
			bool RetireEmitter(Emitter *);
			u16 RetireEmitterAll();
			u16 RetireParticleAll();
			u16 GetNumEmitter() const;
			Emitter * GetEmitter(u16);
			u16 ForeachParticleManager(Action, u32, bool);
			u16 ForeachEmitterFrom(Action, u32, bool, Emitter *);
			UNKTYPE SetRootMtx(const math::MTX34 &);
			
			inline bool GetFlagDisableDraw()
			{
				return mFlags & 2;
			}
		};
	}
}

#endif