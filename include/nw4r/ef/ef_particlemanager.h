#ifndef NW4R_EF_PARTICLEMANAGER
#define NW4R_EF_PARTICLEMANAGER
#include "types_nw4r.h"
#include "math_types.h"
#include "ef_list.h"
#include "ef_ref.h"
#include "ut_Color.h"

namespace nw4r
{
    namespace ef
    {
        struct ParticleManager : ReferencedObject
        {
            struct ParticleModifier
            {
                math::VEC2 mScale; // at 0x0
                math::VEC3 mRotate; // at 0x8

                struct SimpleLight
                {
                    u8 mType; // at 0x0
                    ut::Color mAmbColor; // at 0x1
                } mLight;
            };

            UNKTYPE BeginDraw();
            UNKTYPE EndDraw();
            UNKTYPE BeginCalc(bool);
            UNKTYPE EndCalc();
            
            bool SendClosing();
            UNKTYPE DestroyFunc();
            virtual bool Initialize(Emitter *, EmitterResource *);
            virtual UNKTYPE CreateParticle(u16, math::VEC3, math::VEC3, const math::MTX34 *, float, const EmitterInheritSetting *, Particle *, u16);
            virtual UNKTYPE Calc();
            virtual UNKTYPE Draw(const DrawInfo &);
            
            Emitter * mEmitter; // at 0x20
            EmitterResource * mResource; // at 0x24
            UNKWORD FLAGS_0x28;
            char UNK_0x2C[0xC];
            ActivityList EFLIST_0x38;
            char UNK_0x54[0x4];

            ParticleModifier mMod; // at 0x58

            char UNK_0x74[0x14];

            s8 BYTE_0x88;
            u8 BYTE_0x89;
            bool BOOL_0x8A;

            UNKWORD FLAGS_0xB4;
            
            u16 RetireParticleAll();
            
            UNKTYPE CalcGlobalMtx(math::MTX34 *);

            static void ModifierTravFunc_SetSimpleLightType(void *p, u32 payload)
            {
                ParticleManager *mgr = (ParticleManager *)p;
                u8 type = (u8)payload;
                mgr->mMod.mLight.mType = type;
            }

            static void ModifierTravFunc_SetSimpleLightAmbient(void *p, u32 payload)
            {
                ParticleManager *mgr = (ParticleManager *)p;
                ut::Color *color = (ut::Color *)payload;
                mgr->mMod.mLight.mAmbColor = *color;
            }

            static void ModifierTravFunc_SetScale(void *p, u32 payload)
            {
                ParticleManager *mgr = (ParticleManager *)p;
                math::VEC2 *scale = (math::VEC2 *)payload;
                mgr->mMod.mScale = *scale;
            }

            static void ModifierTravFunc_SetRotate(void *p, u32 payload)
            {
                ParticleManager *mgr = (ParticleManager *)p;
                math::VEC3 *rot = (math::VEC3 *)payload;
                mgr->mMod.mRotate = *rot;
            }
        };
    }
}

#endif