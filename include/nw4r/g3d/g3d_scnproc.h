#ifndef NW4R_G3D_SCNPROC_H
#define NW4R_G3D_SCNPROC_H
#include "types_nw4r.h"
#include "g3d_scnobj.h"

namespace nw4r
{
    namespace g3d
    {
        class ScnProc : public ScnLeaf
        {
        public:
            typedef void (* DrawProc)(ScnProc *, bool opa);

            ScnProc(MEMAllocator *allocator, DrawProc proc, void *userdata, bool set1, bool set2)
                : ScnLeaf(allocator), mFlags(0), mDrawProc(proc), mUserData(userdata)
            {
                if (set1) mFlags |= 0x1;
                if (set2) mFlags |= 0x2;
            }

            static ScnProc * Construct(MEMAllocator *, u32 *, DrawProc, bool, bool, u32);

            virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
            {
                return other == GetTypeObjStatic() ? true
                    : ScnLeaf::IsDerivedFrom(other);
            };
            virtual void G3dProc(u32, u32, void *); // at 0xC
            virtual ~ScnProc() {} // at 0x10
            virtual const TypeObj GetTypeObj() const // at 0x14
            {
                return TypeObj(TYPE_NAME);
            }
            virtual const char * GetTypeName() const // at 0x18
            {
                return GetTypeObj().GetTypeName();
            }
            
            static const G3dObj::TypeObj GetTypeObjStatic() { return TypeObj(TYPE_NAME); }

            void * GetUserData() { return mUserData; }
            void SetUserData(void *data) { mUserData = data; }

            void SetDrawProc(DrawProc proc, bool r5, bool r6)
            {
                mDrawProc = proc;

                if (r5)
                {
                    mFlags |= 0x1;
                }
                else
                {
                    mFlags &= ~0x1;
                }

                if (r6)
                {
                    mFlags |= 0x2;
                }
                else
                {
                    mFlags &= ~0x2;
                }
            }

        private:
            u32 mFlags; // at 0xE8
            DrawProc mDrawProc; // at 0xEC
            void *mUserData; // at 0xF0

            NW4R_G3D_TYPE_OBJ_DECL(ScnProc);
        };
    }
}

#endif