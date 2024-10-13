#ifndef D_BG_S_POLY_PASS_CHK_H
#define D_BG_S_POLY_PASS_CHK_H

class cBgS_PolyPassChk {
public:
    virtual ~cBgS_PolyPassChk() {}
};

class dBgS_PolyPassChk : public cBgS_PolyPassChk {
public:
    dBgS_PolyPassChk();
    virtual ~dBgS_PolyPassChk();

    bool ChkObj() {
        return mObject;
    }
    bool ChkCam() {
        return mCamera;
    }
    bool ChkLink() {
        return mLink;
    }
    bool ChkArrow() {
        return mArrow;
    }
    bool ChkBomb() {
        return mBomb;
    }
    bool ChkWhip() {
        return mWhip;
    }
    bool ChkBeetle() {
        return mBeetle;
    }
    bool ChkClawshot() {
        return mClawshot;
    }
    bool ChkUnderwaterRoof() {
        return mUnderwaterRoof;
    }
    bool Chk0xD() {
        return mField_0xD;
    }
    bool Chk0xE() {
        return mField_0xE;
    }

    void ClrObj() {
        mObject = false;
    }
    void ClrCam() {
        mCamera = false;
    }
    void ClrLink() {
        mLink = false;
    }
    void ClrArrow() {
        mArrow = false;
    }
    void ClrBomb() {
        mBomb = false;
    }
    void ClrWhip() {
        mWhip = false;
    }
    void ClrBeetle() {
        mBeetle = false;
    }
    void ClrClawshot() {
        mClawshot = false;
    }
    void ClrUnderwaterRoof() {
        mUnderwaterRoof = false;
    }
    void Clr0xD() {
        mField_0xD = false;
    }
    void Clr0xE() {
        mField_0xE = false;
    }

    void SetObj() {
        mObject = true;
    }
    void SetCam() {
        mCamera = true;
    }
    void SetLink() {
        mLink = true;
    }
    void SetArrow() {
        mArrow = true;
    }
    void SetBomb() {
        mBomb = true;
    }
    void SetWhip() {
        mWhip = true;
    }
    void SetBeetle() {
        mBeetle = true;
    }
    void SetClawshot() {
        mClawshot = true;
    }
    void SetUnderwaterRoof() {
        mUnderwaterRoof = true;
    }
    void Set0xD() {
        mField_0xD = true;
    }
    void Set0xE() {
        mField_0xE = true;
    }

    void SetPassChkInfo(dBgS_PolyPassChk &chk);

private:
    /* 0x4 */ bool mObject;
    /* 0x5 */ bool mCamera;
    /* 0x6 */ bool mLink;
    /* 0x7 */ bool mArrow;
    /* 0x8 */ bool mBomb;
    /* 0x9 */ bool mWhip;
    /* 0xA */ bool mBeetle;
    /* 0xB */ bool mClawshot;
    /* 0xC */ bool mUnderwaterRoof;
    /* 0xD */ bool mField_0xD;
    /* 0xE */ bool mField_0xE;
};

#endif
