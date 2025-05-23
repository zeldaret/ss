#ifndef D_A_PLAYER_MDL_H
#define D_A_PLAYER_MDL_H

#include "d/col/cc/d_cc_d.h"
#include "d/d_player_act.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmchrblend.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_anmtexsrt.h"
#include "toBeSorted/stage_render_stuff.h"

struct PlayerAnimation {
    /* 0x00 */ const char *animName;
    /* 0x04 */ u8 field_0x04;
    /* 0x05 */ u8 field_0x05;
    /* 0x06 */ u16 mFaceTexPatIdx;
    /* 0x08 */ u16 mFaceTexSrtIdx;
    /* 0x0A */ u16 mFaceAnmChrIdx;
};

class daPlayerModelBase_c;

class daPlBaseScnObjCallback_c : public dScnCallback_c {
public:
    daPlBaseScnObjCallback_c(dAcObjBase_c *b) : dScnCallback_c(b), mpPlayer(b) {}

    /* vt 0x10 */ virtual void
    ExecCallback_CALC_MAT(nw4r::g3d::ScnObj::Timing, nw4r::g3d::ScnObj *, u32, void *) override;

    /* 0x18 */ virtual void ExecCallback_DRAW_OPA(nw4r::g3d::ScnObj::Timing, nw4r::g3d::ScnObj *, u32, void *) override;

private:
    /* 0x0C */ dAcObjBase_c *mpPlayer;
};

class daPlBaseMdlCallback_c : public m3d::mdl_c::mdlCallback_c {
public:
    virtual void ExecCallbackA(nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl, nw4r::g3d::FuncObjCalcWorld *) override;

    void setNodeIds(u16 min, u16 max) {
        mNodeIdMin = min;
        mNodeIdMax = max;
    }

private:
    /* 0x30 */ u16 mNodeIdMin;
    /* 0x32 */ u16 mNodeIdMax;
};

class daPlBaseMdl_c : public m3d::mdl_c {
public:
    daPlBaseMdl_c() : mpSoundData(nullptr) {}

    virtual bool setAnm(m3d::banm_c &anm) override;
    virtual void remove() override;

    bool create(
        daPlayerModelBase_c *player, nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption, u32 nView, u32 *pSize
    );
    void setBlendNodeRange(u16, u16, f32);

    nw4r::g3d::ChrAnmResult *getNodeResult(u16 node) {
        return mCallback.getNodeResult(node);
    }

    void setSoundRelated(u16 id, void *data) {
        field_0x5A = id;
        mpSoundData = data;
    }

    void setField_0x58(u8 val) {
        field_0x58 = val;
    }

    u16 getField_0x5A() const {
        return field_0x5A;
    }

    void *getSoundData() const {
        return mpSoundData;
    }

private:
    /* 0x24 */ daPlBaseMdlCallback_c mCallback;
    /* 0x58 */ u8 field_0x58;
    /* 0x5A */ u16 field_0x5A;
    /* 0x5C */ void *mpSoundData;
};

class daPlBaseMainCallback_c : public m3d::callback_c {
public:
    virtual void timingA(u32, nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl) override;
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;
    virtual void timingC(nw4r::math::MTX34 *, nw4r::g3d::ResMdl) override;

    void setPlayer(daPlayerModelBase_c *p) {
        mpPlayer = p;
    }

private:
    daPlayerModelBase_c *mpPlayer;
};

class daPlBaseAnmChr_c : public m3d::anmChr_c {
public:
    daPlBaseAnmChr_c() : field_0x35(0), field_0x36(0) {}

    void play() override;
    bool isFinished();

    void setField_0x35(u8 value) {
        field_0x35 = value;
    }

private:
    /* 0x35 */ u8 field_0x35;
    /* 0x36 */ u8 field_0x36;
};

class daPlBaseHandsCallback_c : public nw4r::g3d::ICalcWorldCallback {
public:
    virtual void ExecCallbackC(
        nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl,
        nw4r::g3d::FuncObjCalcWorld *pFuncObj
    ) override; // at 0x14

    void setPlayer(daPlayerModelBase_c *p) {
        mpPlayer = p;
    }

private:
    /* 0x04 */ daPlayerModelBase_c *mpPlayer;
};

class daPlBaseHeadCallback_c : public m3d::callback_c {
public:
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

    void setPlayer(daPlayerModelBase_c *p) {
        mpPlayer = p;
    }

private:
    daPlayerModelBase_c *mpPlayer;
};

// Vtable at 8050da00, dtor reveals two superclasses
// until dAcObjBase_c. Name unknown
class daPlayerModelBase_c : public daPlayerActBase_c {
public:
    daPlayerModelBase_c()
        : mSwordRes(nullptr),
          mAlinkRes(nullptr),
          mPlCommonAnimeRes(nullptr),
          mPlCommonSoundRes(nullptr),
          mCurrentRes(nullptr),
          mAlink2Res(nullptr),
          mScnObjCallback(this),
          mScnCallback1(this),
          mScnCallback2(this),
          mScnCallback3(this),
          mScnCallback4(this),
          mScnCallback5(this) {}
    virtual ~daPlayerModelBase_c() {}

    enum TunicType_e {
        TUNIC_0,
        TUNIC_1,
        TUNIC_SILENT,
    };

    enum PlayerMainModelFace_e {
        PLAYER_FACE_DEFAULT = 0,
        PLAYER_FACEMABA01 = 1,
        PLAYER_FACE_M = 2,
        PLAYER_FACE_HA = 3,
        PLAYER_FACE_CUT = 4,
        PLAYER_FACE_CUTAROUND = 5,
        PLAYER_FACE_DAMAGE = 6,
        PLAYER_FACE_DAMAGELARGE = 7,
        PLAYER_FACE_DIE = 8,
        PLAYER_FACE_DIELARGEDAMAGE = 9,
        PLAYER_FACE_DIEMAGMA = 10,
        PLAYER_FACE_PAIN = 11,
        PLAYER_FACE_WA = 12,
        PLAYER_FACE_DRINKST = 13,
        PLAYER_FACE_DRINK = 14,
        PLAYER_FACE_DRINKED = 15,
        PLAYER_FACE_BREATH = 16,
        PLAYER_FACE_CLOSE = 17,
        PLAYER_FACE_ENTRANCE = 18,
        PLAYER_FACE_BINOUT = 19,
        PLAYER_FACE_GETBIGWAIT = 20,
        PLAYER_FACE_GETAWAITNICE = 21,
        PLAYER_FACE_WAITLOOKROUND = 22,
        PLAYER_FACE_DANGER = 23,
        PLAYER_FACE_BINSHIELD = 24,
        PLAYER_FACE_SWIMDIEA = 25,
        PLAYER_FACE_WAITSERVICEA = 26,
        PLAYER_FACE_WAITSERVICEHOTA = 27,
        PLAYER_FACE_BOXOP = 28,
        PLAYER_FACE_NOTICE = 29,
        PLAYER_FACE_SWIMP = 30,
        PLAYER_FACE_BEDSLEEP = 31,
        PLAYER_FACE_BEDAWAKE = 32,
        PLAYER_FACE_SWORDPULLST = 33,
        PLAYER_FACE_SWORDPULLWAIT = 34,
        PLAYER_FACE_SWORDPULL = 35,
        PLAYER_FACE_SWORDSIRENST = 36,
        PLAYER_FACE_SWORDSIRENRAISE = 37,
        PLAYER_FACE_SWORDSIRENSTICK = 38,
        PLAYER_FACE_SWORDSIRENAFTER = 39,
        PLAYER_FACE_BIRD_REACH = 40,
        PLAYER_FACE_BIRD_GETWAIT = 41,
        PLAYER_FACE_DIETRUCK = 42,
        PLAYER_FACE_BIRD_FESTIVALSTART = 43,
        PLAYER_FACE_GETSURPRISE = 44,
        PLAYER_FACE_ASURACUT = 45,
        PLAYER_FACE_ASURACUTST = 46,
        PLAYER_FACE_ASURACUTED = 47,
        PLAYER_FACE_CANNONWAIT = 48,
        PLAYER_FACE_KRAKENAPPEARANCE = 49,
        PLAYER_FACE_GETFLOWER = 50,
        PLAYER_FACE_HARP = 51,
        PLAYER_FACE_SITWAITSERVICEA = 52,
        PLAYER_FACE_LIGHTNING = 53,
        PLAYER_FACE_LASTATTACK = 54,
        PLAYER_FACE_BIRD_CALL = 55,
        PLAYER_FACE_NONE = 56,

        PLAYER_FACE_MAX = 57,
    };

    enum PlayerMainAnim_e {
        PLAYER_ANIM_ATRFW = 0,
        PLAYER_ANIM_ATRFW_R = 1,
        PLAYER_ANIM_ATRFD = 2,
        PLAYER_ANIM_ATRFD_R = 3,
        PLAYER_ANIM_ATRBW = 4,
        PLAYER_ANIM_ATRBW_R = 5,
        PLAYER_ANIM_ATRBD = 6,
        PLAYER_ANIM_ATRBD_R = 7,
        PLAYER_ANIM_ATRSWL = 8,
        PLAYER_ANIM_ATRSDL = 9,
        PLAYER_ANIM_ATRSWR = 10,
        PLAYER_ANIM_ATRSDR = 11,
        PLAYER_ANIM_ATSWL = 12,
        PLAYER_ANIM_ATSDL = 13,
        PLAYER_ANIM_ATSWR = 14,
        PLAYER_ANIM_ATSDR = 15,
        PLAYER_ANIM_ATSWL_RIGHT = 16,
        PLAYER_ANIM_ATSDL_RIGHT = 17,
        PLAYER_ANIM_ATL = 18,
        PLAYER_ANIM_ATR = 19,
        PLAYER_ANIM_DASHA = 20,
        PLAYER_ANIM_DASHS = 21,
        PLAYER_ANIM_ATRBWH = 22,
        PLAYER_ANIM_ATBW = 23,
        PLAYER_ANIM_ATBD = 24,
        PLAYER_ANIM_WALKH = 25,
        PLAYER_ANIM_WAITS = 26,
        PLAYER_ANIM_WALKS = 27,
        PLAYER_ANIM_STEPL = 28,
        PLAYER_ANIM_STEPR = 29,
        PLAYER_ANIM_ATSJLST = 30,
        PLAYER_ANIM_ATSJLED = 31,
        PLAYER_ANIM_ATSJRST = 32,
        PLAYER_ANIM_ATSJRED = 33,
        PLAYER_ANIM_ATSJFST = 34,
        PLAYER_ANIM_ATSJFED = 35,
        PLAYER_ANIM_ROLLBST = 36,
        PLAYER_ANIM_ROLLBED = 37,
        PLAYER_ANIM_WAITB = 38,
        PLAYER_ANIM_WAITBTOA = 39,
        PLAYER_ANIM_SJMPST = 40,
        PLAYER_ANIM_SJMPED = 41,
        PLAYER_ANIM_ROLLF = 42,
        PLAYER_ANIM_ROLLFMIS = 43,
        PLAYER_ANIM_JUMPST = 44,
        PLAYER_ANIM_JUMPSED = 45,
        PLAYER_ANIM_VJMP = 46,
        PLAYER_ANIM_VJMPCHA = 47,
        PLAYER_ANIM_VJMPCHB = 48,
        PLAYER_ANIM_VJMPCL = 49,
        PLAYER_ANIM_HANGL = 50,
        PLAYER_ANIM_HANGR = 51,
        PLAYER_ANIM_HANG = 52,
        PLAYER_ANIM_CLIMBHANG = 53,
        PLAYER_ANIM_CLIMBSLIDEL = 54,
        PLAYER_ANIM_CLIMBSLIDER = 55,
        PLAYER_ANIM_CLIMBHANGMISS = 56,
        PLAYER_ANIM_CLIMBSTRUGGLE = 57,
        PLAYER_ANIM_CLIMBIVYHANG = 58,
        PLAYER_ANIM_CLIMBIVYL = 59,
        PLAYER_ANIM_CLIMBIVYR = 60,
        PLAYER_ANIM_CLIMBIVYL_ST = 61,
        PLAYER_ANIM_CLIMBIVYR_ST = 62,
        PLAYER_ANIM_CLIMBIVYSLIDEL = 63,
        PLAYER_ANIM_CLIMBIVYSLIDER = 64,
        PLAYER_ANIM_CLIMBIVYSLANTLU_RD = 65,
        PLAYER_ANIM_CLIMBIVYSLIDERU_LD = 66,
        PLAYER_ANIM_LADUPST = 67,
        PLAYER_ANIM_LADUPEDL = 68,
        PLAYER_ANIM_LADUPEDR = 69,
        PLAYER_ANIM_LADDWST = 70,
        PLAYER_ANIM_LADDWEDL = 71,
        PLAYER_ANIM_LADDWEDR = 72,
        PLAYER_ANIM_LADRTOL = 73,
        PLAYER_ANIM_LADLTOR = 74,
        PLAYER_ANIM_LADJUMPUPST = 75,
        PLAYER_ANIM_LADJUMPDOWNST = 76,
        PLAYER_ANIM_LADJUMPUPED = 77,
        PLAYER_ANIM_LADJUMPDOWNED = 78,
        PLAYER_ANIM_CUTT = 79,
        PLAYER_ANIM_CUTTB = 80,
        PLAYER_ANIM_CUTTL = 81,
        PLAYER_ANIM_CUTTLB = 82,
        PLAYER_ANIM_CUTJST = 83,
        PLAYER_ANIM_CUTJED = 84,
        PLAYER_ANIM_DAMF = 85,
        PLAYER_ANIM_DAMB = 86,
        PLAYER_ANIM_DAML = 87,
        PLAYER_ANIM_DAMR = 88,
        PLAYER_ANIM_DAMAGEMFRONT = 89,
        PLAYER_ANIM_DAMAGEMBACK = 90,
        PLAYER_ANIM_DAMAGEMLEFT = 91,
        PLAYER_ANIM_DAMAGEMRIGHT = 92,
        PLAYER_ANIM_DAMFF = 93,
        PLAYER_ANIM_DAMFB = 94,
        PLAYER_ANIM_DAMFL = 95,
        PLAYER_ANIM_DAMFR = 96,
        PLAYER_ANIM_DAMFFUP = 97,
        PLAYER_ANIM_DAMFBUP = 98,
        PLAYER_ANIM_DAMFLUP = 99,
        PLAYER_ANIM_DAMFRUP = 100,
        PLAYER_ANIM_DAMAGELFRONTSLIDE = 101,
        PLAYER_ANIM_DAMAGELBACKSLIDE = 102,
        PLAYER_ANIM_DAMAGELLEFTSLIDE = 103,
        PLAYER_ANIM_DAMAGELRIGHTSLIDE = 104,
        PLAYER_ANIM_DAM = 105,
        PLAYER_ANIM_LANDDAMA = 106,
        PLAYER_ANIM_LANDDAMAST = 107,
        PLAYER_ANIM_CUTST = 108,
        PLAYER_ANIM_ATDEFNG = 109,
        PLAYER_ANIM_DIE = 110,
        PLAYER_ANIM_DIELARGEDAMAGEFRONT = 111,
        PLAYER_ANIM_DIELARGEDAMAGEBACK = 112,
        PLAYER_ANIM_DIELARGEDAMAGELEFT = 113,
        PLAYER_ANIM_DIELARGEDAMAGERIGHT = 114,
        PLAYER_ANIM_SWIMWAIT = 115,
        PLAYER_ANIM_SWIMINGA = 116,
        PLAYER_ANIM_SWIMP = 117,
        PLAYER_ANIM_ATSSWL = 118,
        PLAYER_ANIM_ATSSWR = 119,
        PLAYER_ANIM_ATBSW = 120,
        PLAYER_ANIM_SWIMDIVE = 121,
        PLAYER_ANIM_SWIMINGST = 122,
        PLAYER_ANIM_SWIMINGB = 123,
        PLAYER_ANIM_SWIMINGC = 124,
        PLAYER_ANIM_SWIMATTACKMISS = 125,
        PLAYER_ANIM_DAMSWF = 126,
        PLAYER_ANIM_DAMSWB = 127,
        PLAYER_ANIM_DAMSWL = 128,
        PLAYER_ANIM_DAMSWR = 129,
        PLAYER_ANIM_DAMSWW = 130,
        PLAYER_ANIM_SWIMDIEA = 131,
        PLAYER_ANIM_SWIMDIEP = 132,
        PLAYER_ANIM_DIEMAGMA = 133,
        PLAYER_ANIM_SLIDEF = 134,
        PLAYER_ANIM_SLIDEB = 135,
        PLAYER_ANIM_SLIDEFED = 136,
        PLAYER_ANIM_SLIDEBED = 137,
        PLAYER_ANIM_HSJMPL = 138,
        PLAYER_ANIM_HSJMPR = 139,
        PLAYER_ANIM_HSSHOOT = 140,
        PLAYER_ANIM_HSHANGEDL = 141,
        PLAYER_ANIM_HSHANGEDR = 142,
        PLAYER_ANIM_HSHANGL = 143,
        PLAYER_ANIM_HSHANGR = 144,
        PLAYER_ANIM_HSHANGWAITL = 145,
        PLAYER_ANIM_HSHANGWAITR = 146,
        PLAYER_ANIM_HSHANGSHOOTL = 147,
        PLAYER_ANIM_HSHANGSHOOTR = 148,
        PLAYER_ANIM_HSWALLEDL = 149,
        PLAYER_ANIM_HSWALLEDR = 150,
        PLAYER_ANIM_HSWALLL = 151,
        PLAYER_ANIM_HSWALLR = 152,
        PLAYER_ANIM_HSWALLWAITL = 153,
        PLAYER_ANIM_HSWALLWAITR = 154,
        PLAYER_ANIM_HSWALLSHOOTL = 155,
        PLAYER_ANIM_HSWALLSHOOTR = 156,
        PLAYER_ANIM_LIE = 157,
        PLAYER_ANIM_LIEF = 158,
        PLAYER_ANIM_WAITST = 159,
        PLAYER_ANIM_WAITSTRUGGLEDAMAGE = 160,
        PLAYER_ANIM_WAITPP = 161,
        PLAYER_ANIM_PUSHW = 162,
        PLAYER_ANIM_PULLW = 163,
        PLAYER_ANIM_JUMPBST = 164,
        PLAYER_ANIM_JUMPB = 165,
        PLAYER_ANIM_WAITD = 166,
        PLAYER_ANIM_WAITATOD = 167,
        PLAYER_ANIM_WAITSERVICEA = 168,
        PLAYER_ANIM_WAITSERVICEHOTA = 169,
        PLAYER_ANIM_DASHUPL = 170,
        PLAYER_ANIM_DASHUPR = 171,
        PLAYER_ANIM_CLIMBJUMPUST = 172,
        PLAYER_ANIM_CLIMBJUMPDST = 173,
        PLAYER_ANIM_CLIMBJUMPLST = 174,
        PLAYER_ANIM_CLIMBJUMPRST = 175,
        PLAYER_ANIM_CLIMBJUMPUED = 176,
        PLAYER_ANIM_CLIMBJUMPDED = 177,
        PLAYER_ANIM_CLIMBJUMPLED = 178,
        PLAYER_ANIM_CLIMBJUMPRED = 179,
        PLAYER_ANIM_HANGJUMPLST = 180,
        PLAYER_ANIM_HANGJUMPRST = 181,
        PLAYER_ANIM_HANGJUMPLED = 182,
        PLAYER_ANIM_HANGJUMPRED = 183,
        PLAYER_ANIM_DOORKNOBPULL = 184,
        PLAYER_ANIM_DOORKNOBPUSH = 185,
        PLAYER_ANIM_DOORKNOBPULLLOCK = 186,
        PLAYER_ANIM_DOORKNOBPUSHLOCK = 187,
        PLAYER_ANIM_DOOROPD = 188,
        PLAYER_ANIM_DASHB = 189,
        PLAYER_ANIM_MGDIG = 190,
        PLAYER_ANIM_MPCUTU = 191,
        PLAYER_ANIM_MPCUTU_RE = 192,
        PLAYER_ANIM_MPCUTLU = 193,
        PLAYER_ANIM_MPCUTLU_RE = 194,
        PLAYER_ANIM_MPCUTL = 195,
        PLAYER_ANIM_MPCUTL_RE = 196,
        PLAYER_ANIM_MPCUTLD = 197,
        PLAYER_ANIM_MPCUTLD_RE = 198,
        PLAYER_ANIM_MPCUTRD = 199,
        PLAYER_ANIM_MPCUTRD_RE = 200,
        PLAYER_ANIM_MPCUTR = 201,
        PLAYER_ANIM_MPCUTR_RE = 202,
        PLAYER_ANIM_MPCUTRU = 203,
        PLAYER_ANIM_MPCUTRU_RE = 204,
        PLAYER_ANIM_MPCUTD_RIGHT = 205,
        PLAYER_ANIM_MPCUTD_RIGHT_RE = 206,
        PLAYER_ANIM_MPCUTD_LEFT = 207,
        PLAYER_ANIM_MPCUTD_LEFT_RE = 208,
        PLAYER_ANIM_CUTPUSH = 209,
        PLAYER_ANIM_CUTPUSH_RE = 210,
        PLAYER_ANIM_ENEMYGUARDJUST = 211,
        PLAYER_ANIM_LIFTB_BEFORE = 212,
        PLAYER_ANIM_LIFTM_BEFORE = 213,
        PLAYER_ANIM_LIFTB = 214,
        PLAYER_ANIM_LIFTS = 215,
        PLAYER_ANIM_LIFTM = 216,
        PLAYER_ANIM_LIFTL = 217,
        PLAYER_ANIM_THROWBU = 218,
        PLAYER_ANIM_THROWSU = 219,
        PLAYER_ANIM_THROWMU = 220,
        PLAYER_ANIM_THROWLU = 221,
        PLAYER_ANIM_THROWBD = 222,
        PLAYER_ANIM_THROWSD = 223,
        PLAYER_ANIM_MPGUARDSHIELD = 224,
        PLAYER_ANIM_MPGUARDSHIELD_IMPACT = 225,
        PLAYER_ANIM_MPGUARDSHIELD_NORMAL = 226,
        PLAYER_ANIM_VJMPCHA_2 = 227,
        PLAYER_ANIM_ROPEHOLDWAIT = 228,
        PLAYER_ANIM_ROPECLIMBL = 229,
        PLAYER_ANIM_ROPEHOLDWAIT_2 = 230,
        PLAYER_ANIM_ROPEPULL = 231,
        PLAYER_ANIM_ROPETIGHTHOLD = 232,
        PLAYER_ANIM_ROPEFRONT = 233,
        PLAYER_ANIM_ROPEBACK = 234,
        PLAYER_ANIM_ROPESTRUGGLE = 235,
        PLAYER_ANIM_DIVEST = 236,
        PLAYER_ANIM_DIVE = 237,
        PLAYER_ANIM_STOLELAND = 238,
        PLAYER_ANIM_DIVEDROP = 239,
        PLAYER_ANIM_FLOAT = 240,
        PLAYER_ANIM_FLOATAWAY = 241,
        PLAYER_ANIM_DIVETOFLOAT = 242,
        PLAYER_ANIM_ROCKETBEETLE_WAIT = 243,
        PLAYER_ANIM_ROCKETBEETLE_SHOOT = 244,
        PLAYER_ANIM_ROCKETBEETLE_RESET = 245,
        PLAYER_ANIM_ROCKETBEETLE_BACK = 246,
        PLAYER_ANIM_WHIPU = 247,
        PLAYER_ANIM_WHIPLU = 248,
        PLAYER_ANIM_WHIPL = 249,
        PLAYER_ANIM_WHIPLD = 250,
        PLAYER_ANIM_WHIPRD = 251,
        PLAYER_ANIM_WHIPR = 252,
        PLAYER_ANIM_WHIPRU = 253,
        PLAYER_ANIM_WHIPBACKRD = 254,
        PLAYER_ANIM_WHIPBACKR = 255,
        PLAYER_ANIM_WHIPBACKRU = 256,
        PLAYER_ANIM_WHIPBACKU = 257,
        PLAYER_ANIM_WHIPBACKLU = 258,
        PLAYER_ANIM_WHIPBACKL = 259,
        PLAYER_ANIM_WHIPBACKLD = 260,
        PLAYER_ANIM_TURNBACK = 261,
        PLAYER_ANIM_ATLTOR = 262,
        PLAYER_ANIM_ATRTOL = 263,
        PLAYER_ANIM_CHUCHUSTRUGGLE = 264,
        PLAYER_ANIM_CHUCHUSTRUGGLE_BIG = 265,
        PLAYER_ANIM_CHUCHUSTRUGGLE_ACT = 266,
        PLAYER_ANIM_CHUCHUSTRUGGLE_ESCAPE = 267,
        PLAYER_ANIM_SURPRISE = 268,
        PLAYER_ANIM_DANGER = 269,
        PLAYER_ANIM_ROCKRIDE = 270,
        PLAYER_ANIM_NOTICE = 271,
        PLAYER_ANIM_WAITINSECT = 272,
        PLAYER_ANIM_NUMBA = 273,
        PLAYER_ANIM_NUMBB = 274,
        PLAYER_ANIM_DAMFEF = 275,
        PLAYER_ANIM_DAMFEB = 276,
        PLAYER_ANIM_DAMFEL = 277,
        PLAYER_ANIM_DAMFER = 278,
        PLAYER_ANIM_GETA = 279,
        PLAYER_ANIM_GETAWAIT = 280,
        PLAYER_ANIM_GETBIG = 281,
        PLAYER_ANIM_GETBIGWAIT = 282,
        PLAYER_ANIM_BOXOPSMALL = 283,
        PLAYER_ANIM_BOXOP = 284,
        PLAYER_ANIM_CUTUST = 285,
        PLAYER_ANIM_CUTU = 286,
        PLAYER_ANIM_CUTUED = 287,
        PLAYER_ANIM_CUTUNG = 288,
        PLAYER_ANIM_WAITLOOKROUND = 289,
        PLAYER_ANIM_ENTRANCE = 290,
        PLAYER_ANIM_MPPOSEPOWER = 291,
        PLAYER_ANIM_MPPOSEPOWERL = 292,
        PLAYER_ANIM_BINSWINGS = 293,
        PLAYER_ANIM_BINSWINGU = 294,
        PLAYER_ANIM_BINOUT = 295,
        PLAYER_ANIM_DRIFT = 296,
        PLAYER_ANIM_SWIMDIEAREVIVE = 297,
        PLAYER_ANIM_MGLIEFREVIVE = 298,
        PLAYER_ANIM_CATCHL = 299,
        PLAYER_ANIM_CATCHR = 300,
        PLAYER_ANIM_BATTLEREADY = 301,
        PLAYER_ANIM_ROLLJUMP = 302,
        PLAYER_ANIM_TANSUOPEN = 303,
        PLAYER_ANIM_TANSUCLOSE = 304,
        PLAYER_ANIM_BEDSLEEPL = 305,
        PLAYER_ANIM_BEDSLEEP = 306,
        PLAYER_ANIM_BEDAWAKE = 307,
        PLAYER_ANIM_BIRD_GLIDE = 308,
        PLAYER_ANIM_BIRD_GLIDEAROUND = 309,
        PLAYER_ANIM_BIRD_REACH = 310,
        PLAYER_ANIM_BIRD_GETWAIT = 311,
        PLAYER_ANIM_BIRD_ACCEL = 312,
        PLAYER_ANIM_BIRD_DAMAGE = 313,
        PLAYER_ANIM_BIRD_DAMAGESMALL = 314,
        PLAYER_ANIM_BIRD_DAMAGETORNADO = 315,
        PLAYER_ANIM_BIRD_CALL = 316,
        PLAYER_ANIM_SWORDPULLST = 317,
        PLAYER_ANIM_SWORDPULLWAIT = 318,
        PLAYER_ANIM_SWORDPULL = 319,
        PLAYER_ANIM_SWORDSIRENST = 320,
        PLAYER_ANIM_SWORDSIRENSTWAIT = 321,
        PLAYER_ANIM_SWORDSIRENRAISE = 322,
        PLAYER_ANIM_SWORDSIRENRAISEWAIT = 323,
        PLAYER_ANIM_SWORDSIRENSTICK = 324,
        PLAYER_ANIM_SWORDSIRENAFTER = 325,
        PLAYER_ANIM_SWITCHDIAL_ST = 326,
        PLAYER_ANIM_SWITCHDIAL_STL = 327,
        PLAYER_ANIM_SWITCHDIAL_LTOR = 328,
        PLAYER_ANIM_SWITCHDIAL_PUSH = 329,
        PLAYER_ANIM_TRUCKLTOR = 330,
        PLAYER_ANIM_TRUCKFRONTTOBACK = 331,
        PLAYER_ANIM_TRUCKJUMPST = 332,
        PLAYER_ANIM_TRUCKJUMP = 333,
        PLAYER_ANIM_TRUCKJUMPEND = 334,
        PLAYER_ANIM_TRUCKBRAKE = 335,
        PLAYER_ANIM_TRUCKIN = 336,
        PLAYER_ANIM_TRUCKOUT = 337,
        PLAYER_ANIM_DIETRUCK = 338,
        PLAYER_ANIM_DIETRUCKREVIVE = 339,
        PLAYER_ANIM_MGDIGIN = 340,
        PLAYER_ANIM_MGDIGOUT = 341,
        PLAYER_ANIM_MGLIEF_L = 342,
        PLAYER_ANIM_MGLIEF_R = 343,
        PLAYER_ANIM_MGATTACKL = 344,
        PLAYER_ANIM_MGATTACKR = 345,
        PLAYER_ANIM_MGDASHL = 346,
        PLAYER_ANIM_MGDASHR = 347,
        PLAYER_ANIM_MGDAMAGE = 348,
        PLAYER_ANIM_MGWAITBREATH = 349,
        PLAYER_ANIM_MGLIEUP = 350,
        PLAYER_ANIM_LIFTREMLY = 351,
        PLAYER_ANIM_THROWREMLYUP = 352,
        PLAYER_ANIM_LIFTREMLYUTOM = 353,
        PLAYER_ANIM_BIRD_FESTIVALSTART = 354,
        PLAYER_ANIM_GETSURPRISE = 355,
        PLAYER_ANIM_GIRAHIMGRAPPLE = 356,
        PLAYER_ANIM_GIRAHIMPULL = 357,
        PLAYER_ANIM_GIRAHIMPULLUP = 358,
        PLAYER_ANIM_GIRAHIMCHANGE = 359,
        PLAYER_ANIM_ROPEWALK = 360,
        PLAYER_ANIM_ROPEBEND = 361,
        PLAYER_ANIM_ROPEHANGBEND = 362,
        PLAYER_ANIM_ROPEHANGUP = 363,
        PLAYER_ANIM_ROPEBALANCE = 364,
        PLAYER_ANIM_SETSEKIBANA = 365,
        PLAYER_ANIM_SETSEKIBANB = 366,
        PLAYER_ANIM_SETSEKIBANC = 367,
        PLAYER_ANIM_PUSHTSWST = 368,
        PLAYER_ANIM_PUSHTSW = 369,
        PLAYER_ANIM_PUSHLOG = 370,
        PLAYER_ANIM_ASURACUTRIGHT = 371,
        PLAYER_ANIM_ASURACUTLEFT = 372,
        PLAYER_ANIM_ASURACUTUP = 373,
        PLAYER_ANIM_ASURACUTRIGHTST = 374,
        PLAYER_ANIM_ASURACUTLEFTST = 375,
        PLAYER_ANIM_ASURACUTUPST = 376,
        PLAYER_ANIM_ASURACUTRIGHTED = 377,
        PLAYER_ANIM_ASURACUTLEFTED = 378,
        PLAYER_ANIM_ASURACUTUPED = 379,
        PLAYER_ANIM_ASURASWORDWAIT = 380,
        PLAYER_ANIM_ASURASWORDLIFT = 381,
        PLAYER_ANIM_BINSCOOP = 382,
        PLAYER_ANIM_FIRE = 383,
        PLAYER_ANIM_CROUCH = 384,
        PLAYER_ANIM_CANNONWAIT = 385,
        PLAYER_ANIM_SEAL = 386,
        PLAYER_ANIM_SEALLOOP = 387,
        PLAYER_ANIM_SEALFINISH = 388,
        PLAYER_ANIM_FINISHED = 389,
        PLAYER_ANIM_SITDOWN = 390,
        PLAYER_ANIM_SITWAIT = 391,
        PLAYER_ANIM_SITWAITSERVICEA = 392,
        PLAYER_ANIM_ROOFWAIT = 393,
        PLAYER_ANIM_ROOFHANG = 394,
        PLAYER_ANIM_MGSTRUGGLE = 395,
        PLAYER_ANIM_MGSTRUGGLE_ACT = 396,
        PLAYER_ANIM_MGSTRUGGLE_ESCAPE = 397,
        PLAYER_ANIM_KRAKENAPPEARANCE = 398,
        PLAYER_ANIM_MGBURY = 399,
        PLAYER_ANIM_GETFLOWER = 400,
        PLAYER_ANIM_DOORLARGEOPEN = 401,
        PLAYER_ANIM_DOORLARGECLOSE = 402,
        PLAYER_ANIM_HARPPLAYFINISH = 403,
        PLAYER_ANIM_LIGHTNINGL = 404,
        PLAYER_ANIM_LIGHTNINGR = 405,
        PLAYER_ANIM_LASTATTACK = 406,
        PLAYER_ANIM_AT = 407,
        PLAYER_ANIM_TAKE = 408,
        PLAYER_ANIM_TAKEL = 409,
        PLAYER_ANIM_TAKER = 410,
        PLAYER_ANIM_TAKE2 = 411,
        PLAYER_ANIM_WAITATOS = 412,
        PLAYER_ANIM_DAMD = 413,
        PLAYER_ANIM_ASHOOTWAIT = 414,
        PLAYER_ANIM_ARELORD = 415,
        PLAYER_ANIM_ASHOOT = 416,
        PLAYER_ANIM_ARELORDTAME = 417,
        PLAYER_ANIM_ASHOOTWAIT_2 = 418,
        PLAYER_ANIM_ARELORD_2 = 419,
        PLAYER_ANIM_ASHOOT_2 = 420,
        PLAYER_ANIM_DAMSW = 421,
        PLAYER_ANIM_HSWAIT = 422,
        PLAYER_ANIM_VACUUMWAIT = 423,
        PLAYER_ANIM_MPPOSEFRONT = 424,
        PLAYER_ANIM_MPPOSEAROUND = 425,
        PLAYER_ANIM_MPPOSEAROUNDBACK = 426,
        PLAYER_ANIM_MPPOSEDASHFRONT = 427,
        PLAYER_ANIM_MPPOSEDASHAROUND = 428,
        PLAYER_ANIM_MPPOSEDASHAROUNDBACK = 429,
        PLAYER_ANIM_LIFTB_UTOD = 430,
        PLAYER_ANIM_LIFTS_UTOD = 431,
        PLAYER_ANIM_LIFTLWALK = 432,
        PLAYER_ANIM_MPGUARDPOSE = 433,
        PLAYER_ANIM_ROCKRIDE_2 = 434,
        PLAYER_ANIM_BINDRINKST = 435,
        PLAYER_ANIM_BINDRINK = 436,
        PLAYER_ANIM_BINDRINKED = 437,
        PLAYER_ANIM_HARPPOSE = 438,
        PLAYER_ANIM_HARPPLAYGO = 439,
        PLAYER_ANIM_HARPPLAYCOME = 440,
        PLAYER_ANIM_BINHOLD = 441,
        PLAYER_ANIM_BINSHIELD = 442,

        PLAYER_ANIM_MAX = 443,
    };

    // Alink.arc > g3d > model.brres > 3DModels(NW4R) > al > Bones > ...
    enum PlayerMainModelNode_e {
        PLAYER_MAIN_NODE_CENTER = 0,
        PLAYER_MAIN_NODE_BACKBONE_1 = 1,
        PLAYER_MAIN_NODE_BACKBONE_2 = 2,
        PLAYER_MAIN_NODE_HEAD = 4,
        PLAYER_MAIN_NODE_POD = 5,

        PLAYER_MAIN_NODE_SHOULDER_L = 6,
        PLAYER_MAIN_NODE_ARM_L1 = 7,
        PLAYER_MAIN_NODE_ARM_L2 = 8,
        PLAYER_MAIN_NODE_HAND_L = 9,
        PLAYER_MAIN_NODE_WEAPON_L = 10,

        PLAYER_MAIN_NODE_SHOULDER_R = 11,
        PLAYER_MAIN_NODE_ARM_R1 = 12,
        PLAYER_MAIN_NODE_ARM_R2 = 13,
        PLAYER_MAIN_NODE_HAND_R = 14,
        PLAYER_MAIN_NODE_WEAPON_R = 15,
        PLAYER_MAIN_NODE_WAIST = 16,

        PLAYER_MAIN_NODE_CLOTCH_L = 17,
        PLAYER_MAIN_NODE_LEG_L1 = 18,
        PLAYER_MAIN_NODE_LEG_L2 = 19,
        PLAYER_MAIN_NODE_FOOT_L = 20,
        PLAYER_MAIN_NODE_TOE_L = 21,

        PLAYER_MAIN_NODE_CLOTCH_R = 22,
        PLAYER_MAIN_NODE_LEG_R1 = 23,
        PLAYER_MAIN_NODE_LEG_R2 = 24,
        PLAYER_MAIN_NODE_FOOT_R = 25,
        PLAYER_MAIN_NODE_TOE_R = 26,

        PLAYER_MAIN_NODE_FSKIRT_L1 = 27,
        PLAYER_MAIN_NODE_FSKIRT_L2 = 28,

        PLAYER_MAIN_NODE_FSKIRT_R1 = 29,
        PLAYER_MAIN_NODE_FSKIRT_R2 = 30,

        PLAYER_MAIN_NODE_RSKIRT_L1 = 31,
        PLAYER_MAIN_NODE_RSKIRT_L2 = 32,
        PLAYER_MAIN_NODE_RSKIRT_R1 = 33,
        PLAYER_MAIN_NODE_RSKIRT_R2 = 34,
    };

    // Alink.arc > g3d > model.brres > 3DModels(NW4R) > al_head > Bones > ...
    enum PlayerHeadModelNode_e {
        PLAYER_HEAD_NODE_HAIR_L = 1,
        PLAYER_HEAD_NODE_HAIR_R1 = 2,
        PLAYER_HEAD_NODE_HAIR_R2 = 3,
        PLAYER_HEAD_NODE_MOMI_L = 4,
        PLAYER_HEAD_NODE_MOMI_R = 5,
        PLAYER_HEAD_NODE_MOMI_Z_CAP_1 = 6,
    };

    // Alink.arc > g3d > model.brres > 3DModels(NW4R) > al_hands > Bones > ...
    enum PlayerHandsModelNode_e {
        PLAYER_HANDS_NODE_HAND_L = 1,
        PLAYER_HANDS_NODE_HAND_R = 2,
    };

    void fn_8005F890(nw4r::math::MTX34 *);
    void fn_8005FB90(nw4r::math::MTX34 *);
    bool fn_80061410();

    static void freeFrmHeap(mHeapAllocator_c *allocator);
    static void allocFrmHeap(mHeapAllocator_c *allocator, u32 size, const char *name);
    void allocExternalDataBuffers();
    void initModelHeaps();
    void updateSwordShieldModelsIfNeeded();
    m3d::anmTexSrt_c *createAnmTexSrt(const char *resName, m3d::bmdl_c &mdl);

    static bool createGenericMdl(nw4r::g3d::ResMdl resMdl, m3d::mdl_c &mdl, mAllocator_c *alloc, u32 bufferOption);
    static bool createGenericSmdl(nw4r::g3d::ResMdl resMdl, m3d::smdl_c &mdl, mAllocator_c *alloc, u32 bufferOption);
    static void setTransformAndCalc(m3d::scnLeaf_c &lf, const mMtx_c *mtx);
    void setShieldTransform(bool inHand);

    static nw4r::g3d::ResFile getItemResFile(const char *name, mAllocator_c &allocator);
    nw4r::g3d::ResFile getExternalCompressedFile(const char *name, const char *extension, void *dest, u32 maxSize);
    nw4r::g3d::ResAnmChr getExternalAnmChr(const char *name, void *dest, u32 maxSize);
    nw4r::g3d::ResAnmTexPat getExternalAnmTexPat(const char *name, void *dest, u32 maxSize);
    nw4r::g3d::ResAnmTexSrt getExternalAnmTexSrt(const char *name, void *dest, u32 maxSize);

    static bool isBodyAnmPart_0_2_4(s32 part);

    static u32 getCurrentShieldPouchSlot();
    static s32 getShieldType(s32 item);
    static s32 getCurrentlyEquippedShieldId();

    void updateEarringsColor();
    void loadBodyModels();
    void loadBody();
    void loadHandsModels();
    void setSwordAnm(const char *name);
    void initSwordModel();
    void updateSwordModel();
    void initShieldModel();
    void updateShieldModel();

    // Own virtual methods

    /* vt 0x2E0 */ virtual void transformBackbone1(nw4r::g3d::WorldMtxManip *) {}
    /* vt 0x2E4 */ virtual void transformModelCenter(mMtx_c *) {}
    /* vt 0x2E8 */ virtual bool vt_0x2E8(nw4r::math::MTX34 *, const u16 *, bool) {
        return true;
    }
    /* vt 0x2EC */ virtual void isOnClawTargetMaybe(s32 arm, mAng &a1, mAng &a2) {
        a1 = 0;
        a2 = 0;
    }
    /* vt 0x2F0 */ virtual bool isMPPose() {
        return false;
    }
    /* vt 0x2F4 */ virtual bool isOnTightRope() {
        return false;
    }
    /* vt 0x2F8 */ virtual bool isOnVines() {
        return false;
    }
    /* vt 0x2FC */ virtual bool alwaysRet0() {
        return 0;
    }
    /* vt 0x300 */ virtual bool canBlockAttack() {
        return false;
    }
    /* vt 0x304 */ virtual const mVec3_c &vt_0x304() const {
        return mVec3_c::Zero;
    }
    // r = 1, l = 0, backbone = 2
    /* vt 0x308 */ virtual mAng getArmZRot(s32 which) {
        return 0;
    }
    /* vt 0x30C */ virtual void vt_0x30C(mVec3_c &res) {
        res = mVec3_c::Ex;
    }

    // Overrides

    /* vt 0x0E4 */ virtual const mVec3_c &getCenterTranslation() const override {
        return mCenterTranslation;
    }
    /* vt 0x0E8 */ virtual const mVec3_c &vt_0x0E8() const override {
        return field_0x12F4;
    }
    /* vt 0x0EC */ virtual const mVec3_c &vt_0x0EC() const override {
        return mHeadTranslation;
    }
    /* vt 0x0F0 */ virtual const mVec3_c &vt_0x0F0() const override {
        return mTranslationHand[1];
    }
    /* vt 0x0F4 */ virtual const mVec3_c &vt_0x0F4() const override {
        return mTranslationHand[0];
    }

    /* vt 0x114 */ virtual void somethingWithCarriedActorFlags() override;
    /* vt 0x118 */ virtual dAcObjBase_c *getCurrentCarriedActor() override {
        return mCarriedActorRef.get();
    }

    /* vt 0x18C */ virtual void getBodyMtx(mMtx_c *out_mtx, int boneIdx) override {
        mMainMdl.getNodeWorldMtx(boneIdx, *out_mtx);
    }
    /* vt 0x190 */ virtual void getSheathModelMatrix(mMtx_c *out_mtx) override {
        mSheathMdl.getLocalMtx(*out_mtx);
    }
    /* vt 0x194 */ virtual void getSwordModelMatrix(mMtx_c *out_mtx) override {
        mSwordMdl.getLocalMtx(*out_mtx);
    }
    /* vt 0x198 */ virtual mAng vt_0x198() override {
        return field_0x1268;
    }
    /* vt 0x19C */ virtual mAng vt_0x19C() override {
        return field_0x126A;
    }

    u8 getHandMat(s32 idx) {
        return field_0x1208[idx];
    }

    inline bool checkSwordAndMoreStates(u32 mask) const {
        return (mSwordAndMoreStates & mask) != 0;
    }

    inline void offSwordAndMoreStates(u32 mask) {
        mSwordAndMoreStates &= ~mask;
    }

    bool canStart(bool force, u16 newIdx, u16 invalidValue, u16 *out1, u16 *out2) const;
    void setFaceTexPat(s32 faceIdx, bool force);
    void checkFaceTexPat();
    void setFaceTexSrt(s32 faceIdx, bool force);
    void checkFaceTexSrt();
    void setFaceAnmChr(s32 faceIdx, bool force);
    void checkFaceAnmChr();

    void setPosCopy3();

    void loadTwoAnmChrs(s32 childIdx, s32 animIdx, s32 animIdx2, bool b);
    void loadAnmChr(s32 childIdx, s32 animIdx, void *dest, u32 maxSize);
    void removeAnmChr(s32 childIdx);
    void loadSound(nw4r::g3d::ResFile &file, const char *name, s32 childIdx);
    void loadSoundForAnim(s32 childIdx);

    void syncSoundWithAnim();
    static void registMassObj(cCcD_Obj *obj, u8 priority);
    void updateModelColliders();
    void updateCachedPositions();

    // Model callbacks - dropping the mdl argument, apparently
    void mainModelTimingA(u32 nodeId, nw4r::g3d::ChrAnmResult *result);
    void adjustMainModelChrAnm(PlayerMainModelNode_e nodeId, nw4r::g3d::ChrAnmResult *result);

    void mainModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result);
    void adjustMainModelWorldMtx(PlayerMainModelNode_e nodeId, nw4r::g3d::WorldMtxManip *result);

    void mainModelTimingC(nw4r::math::MTX34 *result);
    void headModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result);

    void handsCallbackC(nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *pFuncObj);

    void updateBlendWeights(PlayerMainModelNode_e nodeId);
    void updateMainBlend1(f32 blend);
    void updateMainBlend2(f32 blend, bool save);
    void applyWorldRotationMaybe(nw4r::math::MTX34 *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order);
    void applyWorldRotationMaybe(nw4r::g3d::WorldMtxManip *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order);

    static const PlayerAnimation sAnimations[443];
    static const s32 sUnkDuration;
    static const u8 sShieldDurabilities[10];
    static const u8 sShieldRegenTimes[10];
    static mColor sGuideColor1;
    static mColor sGuideColor2;
    static mColor sGuideColor3;

protected:
    /* 0x370 */ mHeapAllocator_c mModelAllocator;
    /* 0x38C */ mHeapAllocator_c mSwordAllocator;
    /* 0x3A8 */ mHeapAllocator_c mShieldAllocator;
    /* 0x3C4 */ void *mpExternalAnmCharBuffer;
    /* 0x3C8 */ nw4r::g3d::ResFile mSwordRes;
    /* 0x3CC */ nw4r::g3d::ResFile mAlinkRes;
    /* 0x3D0 */ nw4r::g3d::ResFile mPlCommonAnimeRes;
    /* 0x3D4 */ nw4r::g3d::ResFile mPlCommonSoundRes;
    /* 0x3D8 */ nw4r::g3d::ResFile mCurrentRes;
    /* 0x3DC */ nw4r::g3d::ResFile mAlink2Res;
    /* 0x3E0 */ daPlBaseMdl_c mMainMdl;
    /* 0x440 */ daPlBaseMainCallback_c mMainBodyCallback;
    /* 0x448 */ m3d::mdl_c mHeadMdl;
    /* 0x46C */ daPlBaseHeadCallback_c mHeadCallback;
    /* 0x474 */ m3d::smdl_c mFaceMdl;
    /* 0x490 */ m3d::anmChr_c mFaceAnmChr;
    /* 0x4C8 */ void *mpAnmCharBuffer;
    /* 0x4CC */ m3d::anmTexPat_c mFaceTexPat;
    /* 0x4F8 */ void *mpTexPatBuffer;
    /* 0x4FC */ m3d::anmTexSrt_c mFaceTexSrt;
    /* 0x528 */ void *mpTexSrtBuffer;
    /* 0x52C */ m3d::anmChr_c mHeadAnmChr;
    /* 0x564 */ m3d::smdl_c mHandsMdl;
    /* 0x580 */ daPlBaseHandsCallback_c mCalcWorldCallback;
    /* 0x588 */ daPlBaseScnObjCallback_c mScnObjCallback;
    /* 0x598 */ m3d::smdl_c mSwordMdl;
    /* 0x5B4 */ m3d::anmMatClr_c mSwordAnmMatClr;
    /* 0x5E0 */ m3d::smdl_c mShieldMdl;
    /* 0x5FC */ m3d::anmTexPat_c mShieldAnmTexPat1;
    /* 0x628 */ m3d::anmTexPat_c mShieldAnmTexPat2;
    /* 0x654 */ m3d::smdl_c mSheathMdl;
    /* 0x670 */ m3d::smdl_c mUnkMdl1;
    /* 0x68C */ m3d::anmChr_c mAnmChr;
    /* 0x6C4 */ m3d::anmChrBlend_c mAnmChrBlend;
    /* 0x6EC */ dScnCallback_c mScnCallback1;
    /* 0x6F8 */ dScnCallback_c mScnCallback2;
    /* 0x704 */ dScnCallback_c mScnCallback3;
    /* 0x710 */ dScnCallback_c mScnCallback4;
    /* 0x71C */ dScnCallback_c mScnCallback5;
    /* 0x728 */ daPlBaseAnmChr_c mAnmChrs[6];
    /* 0x878 */ dCcD_Cyl mCcCyls[3];
    /* 0xC68 */ dCcD_Cps mCcCpss[3];
    /* 0x10B8 */ dCcD_Cyl mCcCyl;
    /* 0x1208 */ u8 field_0x1208[2];
    /* 0x120A */ u8 field_0x120A;
    /* 0x120B */ u8 mTunicType;
    /* 0x120C */ u8 mCurrentAnmChrIdx;
    /* 0x120D */ u8 mShieldPouchSlot;
    /* 0x120E */ s8 mPouchMatId;
    /* 0x120F */ s8 mBeltMatId;
    /* 0x1210 */ s8 mLowbodyMapId;
    /* 0x1211 */ s8 mLowotherMapId;
    /* 0x1212 */ s8 mPouch2MatId;
    /* 0x1213 */ s8 mUpbodyMatId;
    /* 0x1214 */ s8 mUpotherMatId;
    /* 0x1215 */ s8 mBelt2MatId;
    /* 0x1216 */ s8 mCapMatId;
    /* 0x1217 */ s8 mSkinMatId;
    /* 0x1218 */ u8 mSkywardStrikeChargeDuration;
    /* 0x1219 */ u8 field_0x1219;
    /* 0x121A */ u16 mAnimations[6];
    /* 0x1226 */ u16 mFaceAnmChrIdx1;
    /* 0x1228 */ u16 mFaceAnmChrIdx2;
    /* 0x122A */ u16 mFaceAnmTexPatIdx1;
    /* 0x122C */ u16 mFaceAnmTexPatIdx2;
    /* 0x122E */ u16 mFaceAnmTexSrtIdx1;
    /* 0x1230 */ u16 mFaceAnmTexSrtIdx2;
    /* 0x1232 */ u16 mBreakingEffect;
    /* 0x1234 */ mAng mWaistZRot;
    /* 0x1236 */ mAng mWaistYRot;
    /* 0x1238 */ mAng field_0x1238[5];
    /* 0x1242 */ mAng field_0x1242[5];
    /* 0x124C */ u8 _0x124C[0x1256 - 0x124C];
    /* 0x1256 */ mAng field_0x1256;
    /* 0x1258 */ mAng field_0x1258;
    /* 0x125A */ mAng field_0x125A;
    /* 0x125C */ mAng mRSkirtR1Rot;
    /* 0x125E */ mAng mRSkirtL1Rot;
    /* 0x1260 */ mAng mRightHandRotation;
    /* 0x1262 */ mAng mHeadZRot;
    /* 0x1264 */ mAng mHeadYRot;
    /* 0x1266 */ mAng mHeadXRot;
    /* 0x1268 */ mAng field_0x1268;
    /* 0x126A */ mAng field_0x126A;
    /* 0x126C */ mAng field_0x126C;
    /* 0x1270 */ f32 field_0x1270;
    /* 0x1274 */ f32 field_0x1274;
    /* 0x1278 */ f32 field_0x1278;
    /* 0x127C */ mVec3_c field_0x127C;
    /* 0x1288 */ mVec3_c mCenterTranslation;
    /* 0x1294 */ mVec3_c mHeadTranslation;
    /* 0x12A0 */ mVec3_c mPositionAboveLink;
    /* 0x12AC */ mVec3_c mTranslationHand[2];
    /* 0x12C4 */ mVec3_c mTranslationWeapon[2];
    /* 0x12DC */ mVec3_c mToeTranslation[2];
    /* 0x12F4 */ mVec3_c field_0x12F4;
    /* 0x1300 */ mQuat_c field_0x1300[4];
    /* 0x1340 */ mQuat_c mQuat1;
    /* 0x1350 */ mQuat_c mQuat2;
    /* 0x1360 */ dAcRef_c<dAcObjBase_c> mCarriedActorRef; // not sure about the class
    /* 0x136C */ mAng *field_0x136C;
    /* 0x1370 */ mAng *field_0x1370;
    /* 0x1374 */ mAng *field_0x1374; // x3
    /* 0x1378 */ mAng *field_0x1378; // x3
};

#endif
