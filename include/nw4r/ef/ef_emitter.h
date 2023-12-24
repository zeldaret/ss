#ifndef NW4R_EF_EMITTER
#define NW4R_EF_EMITTER
#include <math.h>
#include "types_nw4r.h"
#include "ef_ref.h"
#include "ut_list.h"
#include "ef_list.h"
#include "ef_rand.h"
#include "ef_util.h"
#include "ef_animcurve.h"

namespace nw4r
{
	namespace ef
	{
		struct ResEmitter
		{
			EmitterResource * mPtr;
			
			inline EmitterResource * ptr() { return mPtr; }
		};
		
		struct EmitterInheritSetting
		{
			s16 SHORT_0x0;
			u8 BYTE_0x2;
			u8 BYTE_0x3;
			u8 BYTE_0x4;
			u8 BYTE_0x5;
			u8 BYTE_0x6;
			u8 BYTE_0x7;
		};

		struct EmitterDesc
		{
			UNKWORD FLAGS_0x0; // at 0x0/0x8
			UNKWORD EMFORMTYPE_0x4; // at 0x4/0xc
			u16 SHORT_0x8; // at 0x8/0x10
			u16 SHORT_0xA; // at 0xa/0x12
			s8 SBYTE_0xC; // at 0xc/0x14
			s8 SBYTE_0xD; // at 0xd/0x15
			s8 SBYTE_0xE; // at 0xe/0x16
			s8 SBYTE_0xF; // at 0xf/0x17
			float FLOAT_0x10; // at 0x10/0x18
			u16 SHORT_0x14; // at 0x14/0x1c
			u16 SHORT_0x16; // at 0x16/0x1e
			u16 SHORT_0x18; // at 0x18/0x20
			s8 BYTE_0x1A; // at 0x1a/0x22
			s8 BYTE_0x1B; // at 0x1b/0x23
			float FLOAT_0x1C;
			float FLOAT_0x20;
			float FLOAT_0x24;
			float FLOAT_0x28;
			float FLOAT_0x2C;
			float FLOAT_0x30;
			u16 SHORT_0x34;
			u8 BYTE_0x36;
			u8 BYTE_0x37;
			float FLOAT_0x38;
			float FLOAT_0x3C;
			float FLOAT_0x40;
			float FLOAT_0x44;
			float FLOAT_0x48;
			float FLOAT_0x4C;
			float FLOAT_0x50;
			float FLOAT_0x54;
			float FLOAT_0x58;
			float FLOAT_0x5C;
			
			float FLOAT_0x60;
			float FLOAT_0x64;
			float FLOAT_0x68;
			float FLOAT_0x6C;
			float FLOAT_0x70;
			float FLOAT_0x74;
			
			float FLOAT_0x78;
			float FLOAT_0x7C;
			float FLOAT_0x80;
			
			u8 BYTE_0x84; // at 0x84/0x8c
			u8 BYTE_0x85; // at 0x85/0x8d
			u8 BYTE_0x86; // at 0x86/0x8e
			UNKWORD WORD_0x88;
		};
		
		struct EmitTrack
		{
			u8 BYTE_0x0;
			u8 BYTE_0x1;
			u16 SHORT_0x2;
			
			u8 BYTE_0x4;
		};
		
		struct EmitterResource
		{
			char UNK_0x0[0x4];
			u32 OFFSET_0x4;
			
			EmitterDesc STRUCT_0x8;
			
			inline UNKTYPE * SkipEmitterDesc()
			{
				return (char *)&STRUCT_0x8 + OFFSET_0x4;
			}
			
			inline UNKTYPE * SkipParticleParameterDesc()
			{
				char * particleDesc = (char *)SkipEmitterDesc();
				
				return particleDesc + *(int *)particleDesc;
			}
			
			inline u16 NumEmitInitTrack()
			{
				u16 * ptr = (u16 *)SkipParticleParameterDesc();
				
				return ptr[(4**(ptr += 2))+3];
			}
			
			inline u16 NumEmitTrack()
			{
				u16 * ptr = (u16 *)SkipParticleParameterDesc();
				
				return ptr[(4**(ptr += 2))+2];
			}
			
			inline EmitTrack * GetEmitTrack(u16 i)
			{
				u16 * ptr = (u16 *)SkipParticleParameterDesc();
				
				return reinterpret_cast<EmitTrack **>(ptr)[(2**(ptr += 1))+1+i];
			}
		};
		
		struct Emitter : ReferencedObject
		{
			virtual bool Initialize(Effect *, EmitterResource *, u8); // at 0x10
			virtual Emitter * CreateEmitter(EmitterResource *, EmitterInheritSetting *, Particle *, u16); // at 0x14
			virtual void CalcEmitter(); // at 0x18
			virtual void CalcParticle(); // at 0x1c
			virtual void CalcEmission(); // at 0x20
			virtual void CalcBillboard(); // at 0x24
			
			u32 mFlags;
			UNKWORD WORD_0x24;
			
			//char UNK_0x28[0x8C];
			float FLOAT_0x28;
			float FLOAT_0x2C;
			short SHORT_0x30;
			short SHORT_0x32;
			float FLOAT_0x34;
			float FLOAT_0x38;
			short SHORT_0x3C;
			float FLOAT_0x40;
			float FLOAT_0x44;
			float FLOAT_0x48;
			float FLOAT_0x4C;
			float FLOAT_0x50;
			float FLOAT_0x54;
			float FLOAT_0x58;
			float FLOAT_0x5C;
			float FLOAT_0x60;
			
			u8 BYTE_0x64;
			s8 BYTE_0x65;
			u8 BYTE_0x66;
			s8 BYTE_0x67;
			
			float FLOAT_0x68;
			float FLOAT_0x6C;
			float FLOAT_0x70;
			float FLOAT_0x74;
			float FLOAT_0x78;
			float FLOAT_0x7C;
			float FLOAT_0x80;
			float FLOAT_0x84;
			float FLOAT_0x88;
			float FLOAT_0x8C;
			/*
			float FLOAT_0x90;
			float FLOAT_0x94;
			float FLOAT_0x98;
			*/
			math::VEC3 VEC_0x90;
			/*
			float FLOAT_0x9C;
			float FLOAT_0xA0;
			float FLOAT_0xA4;
			*/
			math::VEC3 VEC_0x9C;
			/*
			float FLOAT_0xA8;
			float FLOAT_0xAC;
			float FLOAT_0xB0;
			*/
			math::VEC3 VEC_0xA8;
			
			UNKWORD WORD_0xB4;
			EmitterResource * mResource; // at 0xb8
			Effect * mEffect; // at 0xbc
			ActivityList mManagers; // at 0xc0
			char UNK_0xDA[0x2];
			u8 BYTE_0xDC;
			u16 SHORT_0xDE;
			u16 SHORT_0xE0;
			UNKWORD WORD_0xE4;
			u16 SHORT_0xE8;
			u16 SHORT_0xEA;
			Random mRandom; // at 0xec
			EmitterForm * mForm; // at 0xf0
			Emitter * mParent; // at 0xf4
			Particle * REF_0xF8; // at 0xf8
			
			EmitterInheritSetting mInheritSettings; // at 0xfc
			
			bool mMtxDirtyFlag; // at 0x104
			
			math::MTX34 mGlobalMtx; // at 0x108
			
			Emitter();
			~Emitter();
			
			u16 RetireParticleAll();
			bool SendClosing();
			void DestroyFunc();
			bool Closing(ParticleManager *);
			bool RetireParticleManager(ParticleManager *);
			u16 RetireParticleManagerAll();
			bool InitializeDatas(EmitterResource *, Effect *);
			
			ParticleManager * FindParticleManager(EmitterResource *, bool, bool, s8, u8);
			UNKTYPE CreateEmitterTmp(EmitterResource *, EmitterInheritSetting *, Particle *, u16);
			
			float GetLODratio(math::VEC3 &, math::VEC3 &, float, float, float, float);
			
			u16 ForeachParticleManager(Action, u32, bool, bool);
			
			UNKTYPE Emission(ParticleManager *, const math::MTX34 *);
			
			math::MTX34 * CalcGlobalMtx(math::MTX34 *);
			UNKTYPE SetMtxDirty();
			
			static math::MTX34 * RestructMatrix(math::MTX34 *, math::MTX34 *, bool, bool, s8);
			
			u16 GetNumParticleManager() const;
			ParticleManager * GetParticleManager(u16);
			
			inline bool GetFlagDisableCalc() const
			{
				return mFlags & 0x200;
			}
		};
	}
}

#endif