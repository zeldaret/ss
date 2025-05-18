#include "d/d_dvd_drive_error.h"

#include "d/d_dvd_unk.h"
#include "d/d_textunk.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggScreen.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "toBeSorted/lyt_related_floats.h"
#include "toBeSorted/reload_color_fader.h"


/** 805750d0 */
dDvdDriveError_c *dDvdDriveError_c::sInstance;

/** 80052300 */
void dDvdDriveError_c::create(EGG::Heap *heap) {
    sInstance = new (heap, 0x04) dDvdDriveError_c();
    sInstance->init();
}

/** 80052340 */
bool dDvdDriveError_c::isError() const {
    return mIsError;
}

// The alternative would be "-str readonly,noreuse"

static const wchar_t sErrEnGeneral[] = L"Please insert The Legend of Zelda:\nSkyward Sword Game Disc.";
static const wchar_t sErrEnDiskId[] = L"Please insert The Legend of Zelda:\nSkyward Sword Game Disc.";
static const wchar_t sErrEnDiskError[] = L"The disc could not be read.\nRefer to the Wii Operations\nManual for details.";

static const wchar_t *sErrorsEn[] = {
    sErrEnGeneral,
    sErrEnDiskId,
    sErrEnDiskError,
};

static const wchar_t sErrEsGeneral[] = L"Por favor, inserta el disco\nde The Legend of Zelda:\nSkyward Sword.";
static const wchar_t sErrEsDiskId[] = L"Por favor, inserta el disco\nde The Legend of Zelda:\nSkyward Sword.";
static const wchar_t sErrEsDiskError[] = L"No se puede leer el disco.\nConsulta el manual de operaciones\nde la consola Wii para obtener m\xe1"
    "s\ninformaci\xf3"
    "n.";

static const wchar_t *sErrorsEs[] = {
    sErrEsGeneral,
    sErrEsDiskId,
    sErrEsDiskError,
};

static const wchar_t sErrFrGeneral[] = L"Veuillez ins\xe9"
    "rer le disque\nThe Legend of Zelda:\nSkyward Sword.";
static const wchar_t sErrFrDiskId[] = L"Veuillez ins\xe9"
    "rer le disque\nThe Legend of Zelda:\nSkyward Sword.";
static const wchar_t sErrFrDiskError[] = L"Impossible de lire le disque.\nVeuillez vous r\xe9"
    "f\xe9"
    "rer au mode\nd'emploi de la Wii pour plus\nde d\xe9"
    "tails.";

static const wchar_t *sErrorsFr[] = {
    sErrFrGeneral,
    sErrFrDiskId,
    sErrFrDiskError,
};

extern "C" u8 fn_80054F30();

/** 80052350 */
void dDvdDriveError_c::draw() {
    EGG::Screen screen;
    screen.SetCanvasMode(EGG::Screen::CANVASMODE_0);
    screen.SetProjectionType(EGG::Frustum::PROJ_ORTHO);
    screen.SetNearFar(0.0f, 500.0f);
    screen.SetScale(mVec3_c(get_80575190() / get_80575144(), 1.0f, 1.0f));
    screen.SetProjectionGX();
    f32 x = get_80575144() * 0.5f;
    f32 mx = -x;
    f32 y = get_80575148() * 0.5f;
    f32 my = -y;

    EGG::DrawGX::BeginDrawScreen(screen.GetCanvasMode() == EGG::Screen::CANVASMODE_1, false, false);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    mMtx_c mtx;
    f32 dy, dx;
    dx = x - mx;
    dy = my - y;
    screen.CalcMatrixForDrawQuad(mtx, mx, y, nw4r::math::FAbs(dx), nw4r::math::FAbs(dy));

    EGG::DrawGX::DrawDL(EGG::DrawGX::DL_17, mtx, EGG::DrawGX::BLACK);

    nw4r::math::MTX44 mtx2;
    f32 a = get_8057511C();
    f32 b = get_80575118();
    C_MTXOrtho(mtx2, 0.0f, a, 0.0f, b, 0.0f, 1.0f);
    GXSetProjection(mtx2, GX_ORTHOGRAPHIC);
    GXSetScissor(0, 0, b, a);
    mMtx_c mtx3;
    MTXIdentity(mtx3);
    GXLoadPosMtxImm(mtx3, 0);
    GXSetCurrentMtx(0);
    u32 idx = 0;
    if (mDvdDriveStatus == DVD_STATE_WRONG_DISK_ID) {
        idx = 1;
    } else if (mDvdDriveStatus == DVD_STATE_DISK_ERROR) {
        idx = 2;
    }

    u8 lang = fn_80054F30();
    const wchar_t *str;
    if (lang == 3) {
        str = sErrorsFr[idx];
    } else if (lang == 4) {
        str = sErrorsEs[idx];
    } else {
        str = sErrorsEn[idx];
    }

    nw4r::ut::TextWriterBase<wchar_t> textWriter;

    textWriter.SetFont(dDvdUnk::FontUnk::GetInstance()->getFont());

    textWriter.SetScale(
        UnkTextThing::getFn800B1F70() * UnkTextThing::getFn800B1FD0(),
        UnkTextThing::getFn800B1F70() * UnkTextThing::getFn800B1FD0()
    );

    textWriter.SetupGX();
    textWriter.SetDrawFlag(0x110);
    textWriter.SetCursorX(get_8057514C());
    textWriter.SetCursorY(get_80575150());

    textWriter.Print(str, wcslen(str));
}

/** 800526b0 */
void dDvdDriveError_c::execute() {
    mDvdDriveStatus = DVDGetDriveStatus();
    if (!mIsError) {
        if (mDvdDriveStatus != DVD_STATE_IDLE && mDvdDriveStatus != DVD_STATE_BUSY) {
            mIsError = true;
            dDvdUnk::FontUnk::GetInstance()->fn_80052C60();
        }
    } else if (mDvdDriveStatus == DVD_STATE_IDLE) {
        if (!ReloadColorFader::GetInstance()->is1Or5()) {
            mIsError = false;
        }
    }
}

/** 80052750 */
void dDvdDriveError_c::init() {
    mDvdDriveStatus = DVD_STATE_IDLE;
    mIsError = false;
}
