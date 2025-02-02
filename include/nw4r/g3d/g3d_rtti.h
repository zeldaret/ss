#ifndef NW4R_G3D_RTTI_H
#define NW4R_G3D_RTTI_H

/**
 * (Internal) Declare TypeObj for an object class.
 */
#define __NW4R_G3D_TYPEOBJ_DECL(T) static const nw4r::g3d::G3dObj::ResNameDataT<sizeof(#T)> TYPE_NAME;

/**
 * (Internal) Declare data and methods common between base and derived types.
 */
#define __NW4R_G3D_RTTI_DECL(T)                                                                                        \
public:                                                                                                                \
    virtual const TypeObj GetTypeObj() const {                                                                         \
        return nw4r::g3d::G3dObj::TypeObj(TYPE_NAME);                                                                  \
    } /* at 0x14 */                                                                                                    \
                                                                                                                       \
    static const G3dObj::TypeObj GetTypeObjStatic() {                                                                  \
        return nw4r::g3d::G3dObj::TypeObj(TYPE_NAME);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    virtual const char *GetTypeName() const {                                                                          \
        return GetTypeObj().GetTypeName();                                                                             \
    } /* at 0x18 */                                                                                                    \
                                                                                                                       \
private:                                                                                                               \
    __NW4R_G3D_TYPEOBJ_DECL(T);

/**
 * Declare data and methods common between base and derived types.
 */
#define NW4R_G3D_RTTI_DEF(T) const nw4r::g3d::G3dObj::ResNameDataT<sizeof(#T)> T::TYPE_NAME = {sizeof(#T), #T}

/**
 * Define type RTTI (base type).
 */
#define NW4R_G3D_RTTI_DECL_BASE(T)                                                                                     \
    __NW4R_G3D_RTTI_DECL(T);                                                                                           \
                                                                                                                       \
public:                                                                                                                \
    virtual bool IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj other) const {                                               \
        return other == GetTypeObjStatic();                                                                            \
    } /* at 0x8 */

/**
 * Define type RTTI (derived type).
 */
#define NW4R_G3D_RTTI_DECL_DERIVED(T, BASE)                                                                            \
    __NW4R_G3D_RTTI_DECL(T);                                                                                           \
                                                                                                                       \
public:                                                                                                                \
    virtual bool IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj other) const {                                               \
        return other == GetTypeObjStatic() ? true : BASE::IsDerivedFrom(other);                                        \
    } /* at 0x8 */

#endif
