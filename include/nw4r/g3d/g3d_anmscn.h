#ifndef NW4R_G3D_ANMSCN_H
#define NW4R_G3D_ANMSCN_H
#include "types_nw4r.h"
#include "g3d_obj.h"
#include "g3d_light.h"
#include "g3d_fog.h"
#include "g3d_camera.h"

namespace nw4r
{
    namespace g3d
    {
        class AnmScnRes // : public AnmScn
        {
        };

        class AnmScn : G3dObj
        {
        public:
            AnmScn(MEMAllocator *);
            virtual bool IsDerivedFrom(TypeObj other) const; // at 0x8
            virtual ~AnmScn(); // at 0x10
            virtual const TypeObj GetTypeObj() const; // at 0x14
            virtual const char * GetTypeName() const; // at 0x18
            virtual void SetFrame(f32) = 0; // at 0x1C
            virtual f32 GetFrame() const = 0; // at 0x20
            virtual void SetUpdateRate(f32) = 0; // at 0x24
            virtual f32 GetUpdateRate() const = 0; // at 0x28
            virtual void UpdateFrame() = 0; // at 0x2C
            virtual void Attach(int, AnmScnRes); // at 0x30
            virtual void Detach(); // at 0x34
            virtual int GetNumLightSet() const = 0; // at 0x38
            virtual int GetNumAmbLight() const = 0; // at 0x3C
            virtual int GetNumDiffuseLight() const = 0; // at 0x40
            virtual int GetNumSpecularLight() const = 0; // at 0x44
            virtual int GetNumFog() const = 0; // at 0x48
            virtual int GetNumCamera() const = 0; // at 0x4C
            virtual int GetLightSetMaxRefNumber() const = 0; // at 0x50
            virtual int GetAmbLightMaxRefNumber() const = 0; // at 0x54
            virtual int GetDiffuseLightMaxRefNumber() const = 0; // at 0x58
            virtual int GetFogMaxRefNumber() const = 0; // at 0x5C
            virtual int GetCameraMaxRefNumber() const = 0; // at 0x60
            virtual void GetLightSet(LightSet, u32) = 0; // at 0x64
            virtual ut::Color GetAmbLightColor(u32) = 0; // at 0x68
            virtual void GetLight(LightObj *, LightObj *, u32) = 0; // at 0x6C
            virtual void GetFog(Fog, u32) = 0; // at 0x70
            virtual void GetCamera(Camera, u32) = 0; // at 0x74
            virtual int GetSpecularLightID(u32) const = 0; // at 0x78
            virtual bool HasSpecularLight(u32) const = 0; // at 0x7C
            virtual void GetAmbLightResult(AmbLightAnmResult *, u32) = 0; // at 0x80
            virtual void GetLightResult(LightAnmResult *, u32) = 0; // at 0x84
            virtual void GetFogResult(FogAnmResult *, u32) = 0; // at 0x88
            virtual void GetFogResult(CameraAnmResult *, u32) = 0; // at 0x8C

            void GetLightSetting(LightSetting *);
        };
    }
}

#endif