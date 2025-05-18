#ifndef SAVE_RELATED_H
#define SAVE_RELATED_H

class SaveRelated {
public:
    static void create();
    static void remove();

    static SaveRelated *GetInstance() {
        return sInstance;
    }

    void fn_80015F40();

private:
    static SaveRelated *sInstance;
};

#endif
