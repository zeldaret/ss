#include "d/lyt/d_lyt_boss_gauge.h"

STATE_DEFINE(dLytBossGauge_c, None);
STATE_DEFINE(dLytBossGauge_c, In);
STATE_DEFINE(dLytBossGauge_c, Move);
STATE_DEFINE(dLytBossGauge_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {    "s_bossGauge_00_in.brlan",        "G_inOut_00"},
    {    "bossGauge_00_out.brlan",         "G_inOut_00"},
    {    "bossGauge_00_dangerLoop.brlan",  "G_danger_00"},
    {    "bossGauge_00_move.brlan",        "G_move_00"},
};

void dLytBossGauge_c::fn_80158290(){return;}
void dLytBossGauge_c::fn_801583E0(){return;}
void dLytBossGauge_c::fn_801583F0(){return;}

void dLytBossGauge_c::fn_80158400(){
    if (mAnmGroups[0].isEndReached()) {
        mAnmGroups[0].setAnimEnable(false);        
        mStateMgr.changeState(dLytBossGauge_c::StateID_Move);
    }
    return;
}

void dLytBossGauge_c::fn_80158480(){return;}
void dLytBossGauge_c::fn_80158490(){return;}

void dLytBossGauge_c::fn_801584A0(){
    if (field_0x545) {
        mAnmGroups[1].setAnimEnable(true);
        mAnmGroups[1].setFrame(0.0f);
        mStateMgr.changeState(dLytBossGauge_c::StateID_Out);  
        field_0x545=false;
    }
    return;
}

void dLytBossGauge_c::fn_80158520(){return;}
void dLytBossGauge_c::fn_80158530(){return;}

void dLytBossGauge_c::fn_80158540(){
    if(mAnmGroups[1].isEndReached()){
        mAnmGroups[1].setAnimEnable(false);
        field_0x546=true;        
        mStateMgr.changeState(dLytBossGauge_c::StateID_None);
    }
}

void dLytBossGauge_c::fn_801585C0(){return;}

void dLytBossGauge_c::fn_801582A0(){
    
    if(field_0x544){
        
        mAnmGroups[0].setAnimEnable(true);
        mAnmGroups[0].setFrame(0.0f);
        
        SizedString<64> name;
        name[0] = 0;
        
        for(int i = 0; i < 3; i++ ){
            name.sprintf("N_bigBoss_%02d", i);
            
            nw4r::lyt::Pane *pane = mLyt.findPane(name);
            
            if(i == field_0x548){
               pane->SetVisible(true);
            }else{
               pane->SetVisible(false);
            }
        
        }
        mAnmGroups[2].setAnimEnable(true);
        mAnmGroups[2].setFrame(0.0f);
                
        mLyt.calc();
        
        mAnmGroups[2].setAnimEnable(false);
        mStateMgr.changeState(dLytBossGauge_c::StateID_In);
        field_0x546=0;
        field_0x544=0;
    }
}

bool dLytBossGauge_c::build(){
    sInstance = this;
    field_0x54c = 0.8f;

    void *data = LayoutArcManager::GetInstance()->getLoadedData("BossGauge");
    
    mResAcc.attach(data, "arc");
    mLyt.setResAcc(&mResAcc);
    mLyt.build("bossGauge_00.brlyt", nullptr);
    
    for(int i =0 ; i<4 ; i++){
        mAnmGroups[i].init(brlanMap[i].mFile, &mResAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].bind(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    mAnmGroups[0].setAnimEnable(true);
    mAnmGroups[0].setFrame(0.0f);
    
    mAnmGroups[3].setAnimEnable(true);
    mAnmGroups[3].setFrame(0.0f);

    mLyt.calc();

    mAnmGroups[0].setAnimEnable(false);
    mAnmGroups[3].setAnimEnable(false);

    mStateMgr.changeState(dLytBossGauge_c::StateID_None);

    field_0x544=0;
    field_0x545=0;
    field_0x546=0;
    field_0x548=0;

    return true;
}

bool dLytBossGauge_c::remove(){
    mLyt.unbindAnims();
    int iVar1 = 0;  
  
    do {        
        mAnmGroups[iVar1].remove();
        iVar1++;
    } while (iVar1 < 4);

    return true;
}

bool dLytBossGauge_c::execute(){
    
    mStateMgr.executeState();

    if(*mStateMgr.getStateID() != dLytBossGauge_c::StateID_None){
        for(int i = 0; i < 3 ; i++){ 
            if(mAnmGroups[i].isEnabled() ){
                if(i < 2 && mAnmGroups[i].isStop()){
                   mAnmGroups[i].setAnimEnable(false);                   
                }
                mAnmGroups[i].play();
            }            
        }
        mLyt.calc();    
    }    
    return true;
}

bool dLytBossGauge_c::draw(){
    
    if(*mStateMgr.getStateID() != dLytBossGauge_c::StateID_None){        
       mLyt.addToDrawList();                
    }
    return true;
}

void dLytBossGauge_c::setMode(s32 mode){   
    field_0x548 = mode;
    field_0x544 = 1;
    return;
}
