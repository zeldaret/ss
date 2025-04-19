#ifndef D_LYT_SIMPLE_WINDOW_H
#define D_LYT_SIMPLE_WINDOW_H

class dLytSimpleWindow_c {
public:
    dLytSimpleWindow_c() {}
    virtual ~dLytSimpleWindow_c() {}

    static dLytSimpleWindow_c *getInstance() {
        return sInstance;
    }

    bool fn_8012B000();

private:
    static dLytSimpleWindow_c *sInstance;
};

#endif
