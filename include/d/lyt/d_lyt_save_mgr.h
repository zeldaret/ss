#ifndef D_LYT_SAVE_MGR_H
#define D_LYT_SAVE_MGR_H

class dLytSaveMgr_c {
public:

    static dLytSaveMgr_c* GetInstance() {
        return sInstance;
    }

    bool fn_80285650() const;

private:
    static dLytSaveMgr_c *sInstance;
};

#endif
