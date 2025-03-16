#include "nw4r/g3d.h" // IWYU pragma: export

// TODO: Naming
enum TMemCachePlan {
    TMEM_CACHE_NONE,

    TMEM_CACHE_PLAN_0,
    TMEM_CACHE_PLAN_1,
    TMEM_CACHE_PLAN_2,

    TMEM_CACHE_MAX
};

enum TexRegionID {
    TEX_REGION_DEFAULT,
    TEX_REGION_DEFAULT_MIPMAP,

    TEX_REGION_CI,
    TEX_REGION_CI_MIPMAP,

    TEX_REGION_RGBA8,
    TEX_REGION_RGBA8_MIPMAP,

    TEX_REGION_MAX
};

struct TexRegionAddr {
    u32 addrDefaultEven[GX_MAX_TEXMAP];       // at 0x0
    u32 addrDefaultMipMapEven[GX_MAX_TEXMAP]; // at 0x20
    u32 addrDefaultMipMapOdd[GX_MAX_TEXMAP];  // at 0x40

    u32 addrCiEven[GX_MAX_TEXMAP];       // at 0x60
    u32 addrCiMipMapEven[GX_MAX_TEXMAP]; // at 0x80
    u32 addrCiMipMapOdd[GX_MAX_TEXMAP];  // at 0xA0

    u32 addrRgba8Even[GX_MAX_TEXMAP];       // at 0xC0
    u32 addrRgba8Odd[GX_MAX_TEXMAP];        // at 0xE0
    u32 addrRgba8MipMapEven[GX_MAX_TEXMAP]; // at 0x100
    u32 addrRgba8MipMapOdd[GX_MAX_TEXMAP];  // at 0x120
};

struct TexRegionSize {
    u32 sizeDefaultEven[GX_MAX_TEXMAP];       // at 0x0
    u32 sizeDefaultMipMapEven[GX_MAX_TEXMAP]; // at 0x20
    u32 sizeDefaultMipMapOdd[GX_MAX_TEXMAP];  // at 0x40

    u32 sizeCiEven[GX_MAX_TEXMAP];       // at 0x60
    u32 sizeCiMipMapEven[GX_MAX_TEXMAP]; // at 0x80
    u32 sizeCiMipMapOdd[GX_MAX_TEXMAP];  // at 0xA0

    u32 sizeRgba8Even[GX_MAX_TEXMAP];       // at 0xC0
    u32 sizeRgba8Odd[GX_MAX_TEXMAP];        // at 0xE0
    u32 sizeRgba8MipMapEven[GX_MAX_TEXMAP]; // at 0x100
    u32 sizeRgba8MipMapOdd[GX_MAX_TEXMAP];  // at 0x120
};

static GXTexRegion saaTexRegion[TEX_REGION_MAX][GX_MAX_TEXMAP];
static GXTlutRegion saaTlutRegion[GX_BIGTLUT3 + 1];

static TMemCachePlan sTMemCachePlan = TMEM_CACHE_NONE;
static u32 sTlutRegionNum = 0;

static GXTexRegionCallback sfpDefaultTexRegionCallback = NULL;
static GXTlutRegionCallback sfpDefaultTlutRegionCallback = NULL;

// clang-format off
static const TexRegionAddr scaaaTexRegionAddr[TMEM_CACHE_MAX - 1] = {
    // TMEM_CACHE_PLAN_0
    {
        {0x00000, 0x40000, 0x50000, 0x60000, 0x70000, 0x30000, 0x20000, 0x10000}, // addrDefaultEven      
        {0x00000, 0xB0000, 0x50000, 0x90000, 0x70000, 0xB0000, 0x20000, 0x90000}, // addrDefaultMipMapEven
        {0x80000, 0x40000, 0xA0000, 0x60000, 0x80000, 0x30000, 0xA0000, 0x10000}, // addrDefaultMipMapOdd 
        {0x00000, 0x40000, 0x50000, 0x60000, 0x70000, 0x30000, 0x20000, 0x10000}, // addrCiEven
        {0x00000, 0x40000, 0x50000, 0x60000, 0x70000, 0x30000, 0x20000, 0x10000}, // addrCiMipMapEven     
        {0x20000, 0x48000, 0x58000, 0x68000, 0x78000, 0x38000, 0x28000, 0x18000}, // addrCiMipMapOdd      
        {0x00000, 0xB0000, 0x50000, 0x90000, 0x70000, 0xB0000, 0x20000, 0x90000}, // addrRgba8Even        
        {0x80000, 0x40000, 0xA0000, 0x60000, 0x80000, 0x30000, 0xA0000, 0x10000}, // addrRgba8Odd
        {0x00000, 0xB0000, 0x50000, 0x90000, 0x70000, 0xB0000, 0x20000, 0x90000}, // addrRgba8MipMapEven  
        {0x80000, 0x40000, 0xA0000, 0x60000, 0x80000, 0x30000, 0xA0000, 0x10000}, // addrRgba8MipMapOdd   
    },
    // TMEM_CACHE_PLAN_1
    {
        {0x00000, 0x40000, 0x50000, 0x60000, 0x70000, 0x78000, 0x68000, 0x58000}, // addrDefaultEven      
        {0x00000, 0xC0000, 0x50000, 0xE0000, 0x70000, 0xD8000, 0x68000, 0xB8000}, // addrDefaultMipMapEven
        {0x80000, 0x40000, 0xA0000, 0x60000, 0x80000, 0x30000, 0xA0000, 0x10000}, // addrDefaultMipMapOdd 
        {0x00000, 0x40000, 0x50000, 0x60000, 0x70000, 0x78000, 0x68000, 0x58000}, // addrCiEven
        {0x00000, 0x40000, 0x50000, 0x60000, 0x70000, 0x70000, 0x60000, 0x50000}, // addrCiMipMapEven     
        {0x20000, 0x48000, 0x58000, 0x68000, 0x78000, 0x78000, 0x68000, 0x58000}, // addrCiMipMapOdd      
        {0x00000, 0xC0000, 0x50000, 0xE0000, 0x70000, 0xD8000, 0x68000, 0xB8000}, // addrRgba8Even        
        {0x80000, 0x40000, 0xD0000, 0x60000, 0xE8000, 0x78000, 0xC8000, 0x58000}, // addrRgba8Odd
        {0x00000, 0xC0000, 0x50000, 0xE0000, 0x70000, 0xD0000, 0x60000, 0xB0000}, // addrRgba8MipMapEven  
        {0x80000, 0x40000, 0xD0000, 0x60000, 0xE0000, 0x70000, 0xC0000, 0x50000}, // addrRgba8MipMapOdd   
    },
    // TMEM_CACHE_PLAN_2
    {
        {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000}, // addrDefaultEven      
        {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000}, // addrDefaultMipMapEven
        {0x80000, 0xC0000, 0xC8000, 0xD0000, 0xD0000, 0xC8000, 0xC0000, 0xB0000}, // addrDefaultMipMapOdd
        {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000}, // addrCiEven
        {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000}, // addrCiMipMapEven
        {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000}, // addrCiMipMapOdd
        {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000}, // addrRgba8Even
        {0x80000, 0xC0000, 0xC8000, 0xD0000, 0xD0000, 0xC8000, 0xC0000, 0xB0000}, // addrRgba8Odd
        {0x00000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x00000}, // addrRgba8MipMapEven
        {0x80000, 0xC0000, 0xD0000, 0xE0000, 0xE0000, 0xD0000, 0xC0000, 0xB0000}, // addrRgba8MipMapOdd
    },
};
// clang-format on

// clang-format off
static const TexRegionSize scaaaTexRegionSize[TMEM_CACHE_MAX - 1] = {
    // TMEM_CACHE_PLAN_0
    {
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeDefaultEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeDefaultMipMapEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeDefaultMipMapOdd
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeCiEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeCiMipMapEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeCiMipMapOdd
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeRgba8Even
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeRgba8Odd
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeRgba8MipMapEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeRgba8MipMapOdd
    },
    // TMEM_CACHE_PLAN_1
    {
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeDefaultEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeDefaultMipMapEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeDefaultMipMapOdd
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeCiEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeCiMipMapEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeCiMipMapOdd
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeRgba8Even
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeRgba8Odd
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeRgba8MipMapEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeRgba8MipMapOdd
    },
    // TMEM_CACHE_PLAN_2
    {
        {2, 2, 2, 2, 2, 2, 2, 2}, // sizeDefaultEven
        {2, 2, 2, 2, 2, 2, 2, 2}, // sizeDefaultMipMapEven
        {1, 1, 1, 1, 1, 1, 0, 0}, // sizeDefaultMipMapOdd
        {2, 2, 2, 2, 2, 2, 2, 2}, // sizeCiEven
        {2, 2, 2, 2, 2, 2, 2, 2}, // sizeCiMipMapEven
        {1, 1, 1, 1, 1, 1, 0, 0}, // sizeCiMipMapOdd
        {2, 2, 2, 2, 2, 2, 2, 2}, // sizeRgba8Even
        {1, 1, 1, 1, 1, 1, 0, 0}, // sizeRgba8Odd
        {1, 1, 1, 1, 1, 1, 1, 1}, // sizeRgba8MipMapEven
        {1, 0, 0, 0, 0, 0, 0, 0}, // sizeRgba8MipMapOdd
    },
};
// clang-format on

// Forward declarations
static GXTexRegion *TexRegionCallback(const GXTexObj *pObj, GXTexMapID map);
static GXTlutRegion *TlutRegionCallback(u32 id);
static void setTexRegion_(TMemCachePlan plan) __attribute__((never_inline));

namespace nw4r {
namespace g3d {
namespace tmem {

void SetTMemLayout(TMemLayout layout) {
    if (sfpDefaultTexRegionCallback == NULL) {
        // clang-format off
        sfpDefaultTexRegionCallback = GXSetTexRegionCallback(TexRegionCallback);
        GXSetTexRegionCallback(sfpDefaultTexRegionCallback);

        sfpDefaultTlutRegionCallback = GXSetTlutRegionCallback(TlutRegionCallback);
        GXSetTlutRegionCallback(sfpDefaultTlutRegionCallback);
        // clang-format on
    }

    if (sfpDefaultTexRegionCallback == NULL) {
        return;
    }

    sTMemCachePlan = static_cast<TMemCachePlan>(layout);
    setTexRegion_(sTMemCachePlan);

    switch (layout) {
        case TMEM_LAYOUT_NONE:
        case TMEM_LAYOUT_1:    {
            GXSetTlutRegionCallback(sfpDefaultTlutRegionCallback);
            break;
        }

        case TMEM_LAYOUT_2: {
            int i;
            u32 num = 0;

            u32 addr;
            GXTlutRegion *pRegion = saaTlutRegion;

            for (i = 0, addr = 0xF0000; i < GX_MAX_TEXMAP; i++, pRegion++, num++, addr += 0x2000) {
                GXInitTlutRegion(pRegion, addr, 16);
            }

            sTlutRegionNum = num;
            GXSetTlutRegionCallback(TlutRegionCallback);
            break;
        }

        // TODO: Seems to imply no TMEM_LAYOUT_3 exists. Then why the [plan - 1]?
        default: {
            break;
        }
    }
}

} // namespace tmem
} // namespace g3d
} // namespace nw4r

static GXTexRegion *TexRegionCallback(const GXTexObj *pObj, GXTexMapID map) {
    GXTexFmt fmt = GXGetTexObjFormat(pObj);
    GXBool mipmap = GXGetTexObjMipMap(pObj);

    switch (fmt) {
        case GX_TF_RGBA8: {
            return mipmap ? &saaTexRegion[TEX_REGION_RGBA8_MIPMAP][map] : &saaTexRegion[TEX_REGION_RGBA8][map];
        }

        case GX_TF_C4:
        case GX_TF_C8:
        case GX_TF_C14X2: {
            return mipmap ? &saaTexRegion[TEX_REGION_CI_MIPMAP][map] : &saaTexRegion[TEX_REGION_CI][map];
        }

        default: {
            return mipmap ? &saaTexRegion[TEX_REGION_DEFAULT_MIPMAP][map] : &saaTexRegion[TEX_REGION_DEFAULT][map];
        }
    }
}

static GXTlutRegion *TlutRegionCallback(u32 id) {
    GXTlutRegion *pRegion = NULL;

    if (id < sTlutRegionNum) {
        pRegion = &saaTlutRegion[id];
    }

    return pRegion;
}

static void setTexRegion_(TMemCachePlan plan) {
    if (plan == TMEM_CACHE_NONE) {
        GXSetTexRegionCallback(sfpDefaultTexRegionCallback);
        return;
    }

    for (int i = 0; i < GX_MAX_TEXMAP; i++) {
        // clang-format off
        GXInitTexCacheRegion(
            &saaTexRegion[TEX_REGION_DEFAULT][i],
            FALSE,
            scaaaTexRegionAddr[plan - 1].addrDefaultEven[i],
            scaaaTexRegionSize[plan - 1].sizeDefaultEven[i],
            NULL,
            3);

        GXInitTexCacheRegion(
            &saaTexRegion[TEX_REGION_DEFAULT_MIPMAP][i],
            FALSE,
            scaaaTexRegionAddr[plan - 1].addrDefaultMipMapEven[i],
            scaaaTexRegionSize[plan - 1].sizeDefaultMipMapEven[i],
            scaaaTexRegionAddr[plan - 1].addrDefaultMipMapOdd[i],
            scaaaTexRegionSize[plan - 1].sizeDefaultMipMapOdd[i]);

        GXInitTexCacheRegion(
            &saaTexRegion[TEX_REGION_CI][i],
            FALSE,
            scaaaTexRegionAddr[plan - 1].addrCiEven[i],
            scaaaTexRegionSize[plan - 1].sizeCiEven[i],
            NULL,
            3);

        GXInitTexCacheRegion(
            &saaTexRegion[TEX_REGION_CI_MIPMAP][i],
            FALSE,
            scaaaTexRegionAddr[plan - 1].addrCiMipMapEven[i],
            scaaaTexRegionSize[plan - 1].sizeCiMipMapEven[i],
            scaaaTexRegionAddr[plan - 1].addrCiMipMapOdd[i],
            scaaaTexRegionSize[plan - 1].sizeCiMipMapOdd[i]);

        GXInitTexCacheRegion(
            &saaTexRegion[TEX_REGION_RGBA8][i],
            FALSE,
            scaaaTexRegionAddr[plan - 1].addrRgba8Even[i],
            scaaaTexRegionSize[plan - 1].sizeRgba8Even[i],
            scaaaTexRegionAddr[plan - 1].addrRgba8Odd[i],
            scaaaTexRegionSize[plan - 1].sizeRgba8Odd[i]);

        GXInitTexCacheRegion(
            &saaTexRegion[TEX_REGION_RGBA8_MIPMAP][i],
            FALSE,
            scaaaTexRegionAddr[plan - 1].addrRgba8MipMapEven[i],
            scaaaTexRegionSize[plan - 1].sizeRgba8MipMapEven[i],
            scaaaTexRegionAddr[plan - 1].addrRgba8MipMapOdd[i],
            scaaaTexRegionSize[plan - 1].sizeRgba8MipMapOdd[i]);
        // clang-format on
    }

    GXSetTexRegionCallback(TexRegionCallback);
}
