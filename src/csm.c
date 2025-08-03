#include <swilib.h>
#include "ui/ui.h"
#include "nl.h"
#include "csm.h"
#include "csm_list.h"
#include "config_loader.h"

extern int REALD_COUNT;
extern int GetNumberOfDialogs();

const int minus11 = -11;
int CSM_ID;

void OnCreate(CSM_RAM *data) {
    MAIN_CSM *csm = (MAIN_CSM*)data;
    csm->csm.state = 0;
    csm->csm.unk1 = 0;
    ReadCSMList();
    GetNumberOfDialogs();
    csm->gui_id = UI_Create(csm);
}

static void OnClose(CSM_RAM *csm) {
    ClearNL();
    CSM_ID = 0;
}

static int OnMessage(CSM_RAM *data, GBS_MSG *msg) {
    MAIN_CSM *csm = (MAIN_CSM*)data;
    if (msg->msg == MSG_CSM_DESTROYED) {
        RefreshGUI();
    } else if (msg->msg == MSG_GUI_DESTROYED) {
        if ((int)msg->data0 == csm->gui_id) {
            csm->csm.state = -3;
        }
    } else if (msg->msg == MSG_INCOMMING_CALL) {
        csm->csm.state = -3;
    }
    return 1;
}

const CSM_DESC maincsm = {
    OnMessage,
    OnCreate,
    #ifdef NEWSGOLD
        0,
        0,
        0,
        0,
    #endif
    OnClose,
    sizeof(MAIN_CSM),
    1,
    &minus11
};

void CSM_Create() {
    MAIN_CSM main_csm;
    InitConfig();
    LockSched();
    if (!CSM_ID) {
        CSM_ID = CreateCSM(&maincsm, &main_csm, 2);
    }
    UnlockSched();
}
