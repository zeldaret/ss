#ifndef D_LYT_SHOP_H
#define D_LYT_SHOP_H

class dLytShop_c {
public:
    static dLytShop_c *GetInstance() {
        return sInstance;
    }

private:
    static dLytShop_c *sInstance;
};

#endif
