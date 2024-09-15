#ifndef D_T_NOEFFECT_AREA_H
#define D_T_NOEFFECT_AREA_H

#include <d/tg/d_tg.h>
#include <f/f_list_mg.h>
#include <m/m_mtx.h>

class dTgNoEffectArea_c : public dTg_c {
public:
    dTgNoEffectArea_c() : mEntry(this) {}
    virtual ~dTgNoEffectArea_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    static fLiNdBa_c *getList();

    static const f32 sFloat1;
    static const f32 sFloat2;

private:
    void addToList(fLiMgBa_c &list);

    static fLiMgBa_c sList;
    fLiNdBa_c mEntry;
    mMtx_c mMtx;
};

#endif
