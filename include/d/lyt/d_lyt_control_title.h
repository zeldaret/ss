#ifndef D_LYT_CONTROL_TITLE_H
#define D_LYT_CONTROL_TITLE_H

#include "d/d_base.h"

class dLytControlTitle_c : public dBase_c { 
public:
	dLytControlTitle_c();
	virtual ~dLytControlTitle_c();

	virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

private:
};
#endif
