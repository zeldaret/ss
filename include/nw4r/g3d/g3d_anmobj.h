#ifndef NW4R_G3D_ANMOBJ_H
#define NW4R_G3D_ANMOBJ_H
#include "types_nw4r.h"
#include "g3d_rescommon.h"
#include "g3d_resmdl.h"
#include "g3d_obj.h"

namespace nw4r
{
    namespace g3d
    {
        enum AnmPolicy
        {
            ANM_POLICY_ONETIME,
            ANM_POLICY_LOOP,
            ANM_POLICY_MAX
        };

        typedef f32 (* PlayPolicyFunc)(f32, f32, f32);
        f32 PlayPolicy_Onetime(f32, f32, f32);
        f32 PlayPolicy_Loop(f32, f32, f32);

        inline PlayPolicyFunc GetAnmPlayPolicy(AnmPolicy policy)
        {
            static PlayPolicyFunc policyTable[ANM_POLICY_MAX] = {
                PlayPolicy_Onetime,
                PlayPolicy_Loop
            };
            return policyTable[policy];
        }

        struct FrameCtrl
        {
            FrameCtrl(f32 f1, f32 f2, PlayPolicyFunc policy)
                : mFrame(0.0f), mRate(1.0f), FLOAT_0x8(f1), mEndFrame(f2), mPolicy(policy) {}
            
            f32 GetFrm() const
            {
                return mFrame;
            }

            f32 GetRate() const
            {
                return mRate;
            }

            void SetFrm(f32 frm)
            {
                f32 newFrm = mPolicy(FLOAT_0x8, mEndFrame, frm);
                mFrame = newFrm;
            }

            void SetRate(f32 rate)
            {
                mRate = rate;
            }

            void SetPolicy(PlayPolicyFunc func)
            {
                mPolicy = func;    
            }

            void UpdateFrm()
            {
                SetFrm(mFrame + mRate * smBaseUpdateRate);
            }

            f32 mFrame; // at 0x0
            f32 mRate; // at 0x4
            f32 FLOAT_0x8;
            f32 mEndFrame;
            PlayPolicyFunc mPolicy; // at 0x10

            static f32 smBaseUpdateRate;
        };

        class AnmObj : public G3dObj
        {
        public:
            enum AnmFlag
            {
                ANMFLAG_2 = 0x2,
                ANMFLAG_ISBOUND = 0x4
            };

        public:
            AnmObj(MEMAllocator *pAllocator, G3dObj *pParent)
                : G3dObj(pAllocator, pParent), mFlags(0) {}

            virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
            {
                return (other == GetTypeObjStatic()) ? true
                    : G3dObj::IsDerivedFrom(other);
            }
            virtual void G3dProc(u32, u32, void *) = 0; // at 0xC
            virtual ~AnmObj() {} // at 0x10
            virtual const TypeObj GetTypeObj() const // at 0x14
            {
                return TypeObj(TYPE_NAME);
            }
            virtual const char * GetTypeName() const // at 0x18
            {
                return GetTypeObj().GetTypeName();
            }
            virtual void SetFrame(f32) = 0; // at 0x1C
            virtual f32 GetFrame() const = 0; // at 0x20
            virtual void UpdateFrame() = 0; // at 0x24
            virtual void SetUpdateRate(f32) = 0; // at 0x28
            virtual f32 GetUpdateRate() const = 0; // at 0x2C
            virtual bool Bind(ResMdl) = 0; // at 0x30
            virtual void Release(); // at 0x34

            static const TypeObj GetTypeObjStatic()
            {
                return TypeObj(TYPE_NAME);
            }

            void SetAnmFlag(AnmFlag, bool);
            bool TestAnmFlag(AnmFlag) const;

        private:
            u32 mFlags; // at 0x4

            NW4R_G3D_TYPE_OBJ_DECL(AnmObj);
        };
    }
}

#endif