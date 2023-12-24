#ifndef NW4R_G3D_SCNOBJ_H
#define NW4R_G3D_SCNOBJ_H
#include "types_nw4r.h"
#include "math_types.h"
#include "math_geometry.h"
#include "g3d_obj.h"

namespace nw4r
{
    namespace g3d
    {
        namespace
        {
            static inline u32 align4(u32 x) { return x + 3 & ~3; }
        }

        struct IScnObjGather
        {
            virtual ~IScnObjGather();
            virtual UNKWORD Add(ScnObj *, bool, bool) = 0;
        };

        class ScnObj : public G3dObj
        {
        public:
            enum ForEachResult
            {
                FOREACH_RESULT_0,
                FOREACH_RESULT_1
            };

            enum ScnObjFlag
            {
                FLAG_1 = 0x1,
                FLAG_2 = 0x2,
                FLAG_4 = 0x4,
                FLAG_8 = 0x8,
                FLAG_10 = 0x10,
                FLAG_20 = 0x20,
                FLAG_40 = 0x40,
                FLAG_80 = 0x80,
                FLAG_10000000 = 0x10000000,
                FLAG_20000000 = 0x20000000,
                FLAG_40000000 = 0x40000000,
                FLAG_80000000 = 0x80000000,

                FLAG_60 = FLAG_40 | FLAG_20
            };

            enum ScnObjMtxType
            {
                MTX_TYPE_0,
                MTX_TYPE_WORLD,
                MTX_TYPE_VIEW,
                MTX_TYPE_MAX
            };

            enum Timing
            {
                TIMING_1 = 0x1,
                TIMING_2 = 0x2,
                TIMING_4 = 0x4
            };

            enum ExecOp
            {
                EXEC_OP_1 = 0x1,
                EXEC_OP_2 = 0x2,
                EXEC_OP_4 = 0x4
            };

            enum ScnObjBoundingVolumeType
            {
                BOUNDING_0,
                BOUNDING_1,
                BOUNDING_MAX
            };

            enum ScnObjOption
            {
                OPTION_VISIBLE = (1 << 0)
            };

            typedef ForEachResult (* ForEachAction)(ScnObj *, void *);

        public:
            ScnObj(MEMAllocator *);

            virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
            {
                return (other == GetTypeObjStatic()) ? true
                    : G3dObj::IsDerivedFrom(other);
            }
            virtual void G3dProc(u32, u32, void *) = 0; // at 0xC
            virtual ~ScnObj(); // at 0x10
            virtual const TypeObj GetTypeObj() const // at 0x14
            {
                return TypeObj(TYPE_NAME);
            }
            virtual const char * GetTypeName() const // at 0x18
            {
                return GetTypeObj().GetTypeName();
            }
            virtual ForEachResult ForEach(ForEachAction, void *, bool) = 0; // at 0x1C
            virtual bool SetScnObjOption(u32, u32); // at 0x20
            virtual bool GetScnObjOption(u32, u32 *) const; // at 0x24
            virtual f32 GetValueForSortOpa() const; // at 0x28
            virtual f32 GetValueForSortXlu() const; // at 0x2C
            virtual void CalcWorldMtx(const math::MTX34 *, u32 *); // at 0x30

            static const G3dObj::TypeObj GetTypeObjStatic()
            {
                return TypeObj(TYPE_NAME);
            }

            void CalcViewMtx(const math::MTX34 *);
            bool SetMtx(ScnObjMtxType, const math::MTX34 *);
            bool GetMtx(ScnObjMtxType, math::MTX34 *) const;
            void SetPriorityDrawOpa(int);
            void SetPriorityDrawXlu(int);
            void EnableScnObjCallbackTiming(Timing);
            void EnableScnObjCallbackExecOp(ExecOp);
            bool SetBoundingVolume(ScnObjBoundingVolumeType, const math::AABB *);
            bool GetBoundingVolume(ScnObjBoundingVolumeType, math::AABB *) const;

            const math::MTX34 * GetMtxPtr(ScnObjMtxType type) const
            {
                return &mMatrices[type];
            }

            void SetScnObjFlag(ScnObjFlag f, u32 set)
            {
                if (set)
                {
                    mFlags |= f;
                }
                else
                {
                    mFlags &= ~f;
                }
            }
            bool TestScnObjFlag(ScnObjFlag f) const
            {
                return mFlags & f;
            }

            inline void CheckCallback_CALC_VIEW(Timing timing, u32 r5, void *r6);
            inline void CheckCallback_CALC_MAT(Timing timing, u32 r5, void *r6);
            inline void CheckCallback_CALC_WORLD(Timing timing, u32 r5, void *r6);

            bool IsG3dProcDisabled(u32 task) const
            {
                if ((task < 9) && (1 << (task - 1) & mFlags)) return true;
                return false;
            }

        protected:
            math::MTX34 mMatrices[MTX_TYPE_MAX]; // at 0xC
            math::AABB mBounds[BOUNDING_MAX]; // at 0x9C

            u32 mFlags; // at 0xCC
            u8 mPriorityDrawOpa; // at 0xD0
            u8 mPriorityDrawXlu; // at 0xD1
            u8 BYTE_0xD2;
            u8 BYTE_0xD3;
            IScnObjCallback *mCallback; // at 0xD4
            u8 mTiming; // at 0xD8
            u8 BYTE_0xD9;
            u16 mExecOp; // at 0xDA

            NW4R_G3D_TYPE_OBJ_DECL(ScnObj);
        };

        struct IScnObjCallback
        {
            virtual ~IScnObjCallback() {} // at 0x8
            virtual void ExecCallback_CALC_WORLD(ScnObj::Timing, ScnObj *, u32, void *) {} // at 0xC
            virtual void ExecCallback_CALC_MAT(ScnObj::Timing, ScnObj *, u32, void *) {} // at 0x10
            virtual void ExecCallback_CALC_VIEW(ScnObj::Timing, ScnObj *, u32, void *) {} // at 0x14
        };

        // Is there a better way of resolving this dependency?
        void ScnObj::CheckCallback_CALC_VIEW(Timing timing, u32 r5, void *r6)
        {
            if (mCallback != NULL)
            {
                if ((mExecOp & EXEC_OP_4) && (mTiming & timing))
                {
                    mCallback->ExecCallback_CALC_VIEW(timing, this, r5, r6);
                }
            }  
        }

        void ScnObj::CheckCallback_CALC_MAT(Timing timing, u32 r5, void *r6)
        {
            if (mCallback != NULL)
            {
                if ((mExecOp & EXEC_OP_2) && (mTiming & timing))
                {
                    mCallback->ExecCallback_CALC_MAT(timing, this, r5, r6);
                }
            }
        }

        void ScnObj::CheckCallback_CALC_WORLD(Timing timing, u32 r5, void *r6)
        {
            if (mCallback != NULL)
            {
                if ((mExecOp & EXEC_OP_1) && (mTiming & timing))
                {
                    mCallback->ExecCallback_CALC_WORLD(timing, this, r5, r6);
                }
            }
        }    

        class ScnLeaf : public ScnObj
        {
        public:
            // Unofficial name, however GetScaleProperty needs to return an enum to match
            enum ScaleProperty
            {
                SCALE_PROPERTY_0,
                SCALE_PROPERTY_1,
                SCALE_PROPERTY_2,
            };

        public:
            ScnLeaf(MEMAllocator *allocator) : ScnObj(allocator), mScale(1.0f, 1.0f, 1.0f) {}

            virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
            {
                return (other == GetTypeObjStatic()) ? true
                    : ScnObj::IsDerivedFrom(other);
            }
            virtual void G3dProc(u32, u32, void *) = 0; // at 0xC
            virtual ~ScnLeaf() {} // at 0x10
            virtual const TypeObj GetTypeObj() const // at 0x14
            {
                return TypeObj(TYPE_NAME);
            }
            virtual const char * GetTypeName() const // at 0x18
            {
                return GetTypeObj().GetTypeName();
            }
            virtual ForEachResult ForEach(ForEachAction, void *, bool); // at 0x1C
            virtual bool SetScnObjOption(u32, u32); // at 0x20
            virtual bool GetScnObjOption(u32, u32 *) const; // at 0x24
            virtual void CalcWorldMtx(const math::MTX34 *, u32 *); // at 0x30

            static const G3dObj::TypeObj GetTypeObjStatic()
            {
                return TypeObj(TYPE_NAME);
            }

            ScaleProperty GetScaleProperty() const;
            void DefG3dProcScnLeaf(u32, u32, void *);

        private:
            math::VEC3 mScale;

            NW4R_G3D_TYPE_OBJ_DECL(ScnLeaf);
        };

        class ScnGroup : public ScnObj
        {
        public:
            ScnGroup(MEMAllocator *, ScnObj **, u32);

            virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
            {
                return (other == GetTypeObjStatic()) ? true
                    : ScnObj::IsDerivedFrom(other);
            }
            virtual void G3dProc(u32, u32, void *); // at 0xC
            virtual ~ScnGroup(); // at 0x10
            virtual const TypeObj GetTypeObj() const // at 0x14
            {
                return TypeObj(TYPE_NAME);
            }
            virtual const char * GetTypeName() const // at 0x18
            {
                return GetTypeObj().GetTypeName();
            }
            virtual ForEachResult ForEach(ForEachAction, void *, bool); // at 0x1C
            virtual bool Insert(u32, ScnObj *); // at 0x34
            virtual ScnObj * Remove(u32); // at 0x38
            virtual bool Remove(ScnObj *); // at 0x3C

            static const G3dObj::TypeObj GetTypeObjStatic()
            {
                return TypeObj(TYPE_NAME);
            }

            bool Empty() const
            {
                return mSize == 0;
            }

            u32 Size() const
            {
                return mSize;
            }

            ScnObj * PopBack()
            {
                if (!Empty())
                {
                    return Remove(Size() - 1);
                }

                return NULL;
            }

            void Clear()
            {
                while (!Empty())
                {
                    PopBack();
                }
            }

            void ScnGroup_G3DPROC_GATHER_SCNOBJ(u32, IScnObjGather *);
            void ScnGroup_G3DPROC_CALC_WORLD(u32, const math::MTX34 *);
            void ScnGroup_G3DPROC_CALC_MAT(u32, void *);
            void ScnGroup_G3DPROC_CALC_VIEW(u32, const math::MTX34 *);
            void DefG3dProcScnGroup(u32, u32, void *);

            bool PushBack(ScnObj *);
            
            ScnObj **mObjects; // at 0xDC
            u32 mCapacity; // at 0xE0
            u32 mSize; // at 0xE4

        private:
            NW4R_G3D_TYPE_OBJ_DECL(ScnGroup);
        };
    }
}

#endif