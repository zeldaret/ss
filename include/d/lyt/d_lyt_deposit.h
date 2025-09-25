#ifndef D_LYT_DEPOSIT_H
#define D_LYT_DEPOSIT_H

class dLytDeposit_c {
public:
    static dLytDeposit_c *GetInstance() {
        return sInstance;
    }

private:
    static dLytDeposit_c *sInstance;
};

#endif
