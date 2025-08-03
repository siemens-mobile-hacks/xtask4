#include <swilib.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <xtask_ipc.h>
#include "nl.h"
#include "csm.h"
#include "csm_list.h"
#include "csm_utils.h"
#include "idle_hook.h"
#include "config_loader.h"

#ifdef NEWSGOLD
    #define USE_ONE_KEY
#endif

typedef struct {
    CSM_RAM csm;
    int unk1;
    int bearer;
    int gui_id;
    int unk2;
    int param_R1;
    int param_ZERO;
} JAVAINTERFACE_CSM;

extern int CFG_ENABLE_HELLO_MSG;
extern char CFG_UNDER_IDLE_CONSTR[];
extern int CFG_ACTIVATION_STYLE, CFG_ACTIVATION_KEY, CFG_ENABLE_LONG_PRESS, CFG_RED_BUTTON_MODE;
extern int CFG_ENABLE_LOCK;
extern int CFG_SHOW_DAEMONS, CFG_SHOW_IDLE;

extern char UI_HDR_TXT[32];

CSM_RAM *CSM_UNDER_IDLE;

GBSTMR TMR_START;
volatile int SHOW_LOCK;
volatile int CALLHIDE_MODE = 0;

int REALD_COUNT; // Number of real dialogs
int NSD_COUNT; // Number of new style dialogs
int SHOW_DAEMONS;

// -1 - XTask GUI present
// 0 - XTask GUI absent
// 1 - IBUT longpressed, ready for exit
int MODE;

// 0 - no press
// 1 - long press REDBUT
int MODE_RED;

// 0 - no press
// 1 - long press ENTER_BUTTON
// 2 - disable KEY_UP process
int MODE_ENTER;


CSM_RAM *GetUnderIdleCSM(void) {
    CSM_DESC *UnderIdleDesc;
    if (strlen(CFG_UNDER_IDLE_CONSTR) == 8) {
        UnderIdleDesc = (CSM_DESC*)strtoul(CFG_UNDER_IDLE_CONSTR, 0, 0x10);
    }
    else {
        UnderIdleDesc = ((CSM_RAM*)(FindCSMbyID(CSM_root()->idle_id))->prev)->constr;
        sprintf(CFG_UNDER_IDLE_CONSTR, "%08X", (uint32_t)UnderIdleDesc);
        SaveConfig();
    }
    LockSched();
    CSM_RAM *csm = CSM_root()->csm_q->csm.first;
    while (csm && csm->constr != UnderIdleDesc) {
        csm = csm->next;
    }
    UnlockSched();
    return csm;
}


char *FindCSMName(CSM_RAM *csm) {
    char s[40];
    CSM_DESC *desc = csm->constr;
    sprintf(s, "%08X ", (uint32_t)desc);
    char *p = strstr(CSM_LIST_DATA, s);
    if (p) {
        return p + 9;
    } else {
        static char u[40];
        sprintf(u, "Unknown %p!", desc);
        return u;
    }
}

int GetNumberOfDialogs() {
    int count = 0;
    int c;
    int i;
    CSM_RAM *icsm;
    WSHDR *ws;
    char ss[64];

    void *ircsm = FindCSMbyID(CSM_root()->idle_id);
    ClearNL();
    //Find new style daemons
    NSD_COUNT = 0;
    if (SHOW_DAEMONS) {
        icsm = ((CSM_RAM*)(CSM_root()->csm_q->csm.first))->next;
        while (((unsigned int)(icsm->constr) >> 27) == 0x15) {
            WSHDR *tws = (WSHDR*)(((char*)icsm->constr) + sizeof(CSM_DESC));
            if ((tws->ws_malloc == NAMECSM_MAGIC1) && (tws->ws_mfree == NAMECSM_MAGIC2)) {
                ws = AllocWS(64);
                wstrcpy(ws, tws);
                AddNL(ws, 1);
                NLTOP->p = icsm;
                count++;
                NSD_COUNT++; // count new style dialogs
            }
            icsm = icsm->next;
        }
    }
    icsm = CSM_UNDER_IDLE->next;
    do {
        if (icsm == ircsm) {
            if (CFG_SHOW_IDLE) {
                ws = AllocWS(40);
                wsprintf(ws, "IDLE Screen");
                AddNL(ws, 0);
                NLTOP->p = icsm;
                count++;
            }
        }
        else {
            extern CSM_DESC maincsm;
            if (icsm->constr != &maincsm) {
                WSHDR *tws = (WSHDR*)(((char*)icsm->constr) + sizeof(CSM_DESC));
                char *s;
                if ((tws->ws_malloc == NAMECSM_MAGIC1) && (tws->ws_mfree == NAMECSM_MAGIC2)) {
                    ws = AllocWS(64);
                    wstrcpy(ws, tws);
                    AddNL(ws, 0);
                    NLTOP->p = icsm;
                    count++;
                }
                else {
                    s = FindCSMName(icsm);
#ifdef NEWSGOLD
                    if (!strncasecmp(s, "Java", 4)) {
                        int i = ((JAVAINTERFACE_CSM*)icsm)->bearer;
                        int j = ((JAVAINTERFACE_CSM*)icsm)->param_R1;
                        if (i == 2) continue;
                        ws = AllocWS(64);
                        switch (i) {
                        case 1:
                            wsprintf(ws, "Browser", j);
                            break;
                        case 2:
                            wsprintf(ws, "Jam %d", j);
                            break;
                        case 3:
                            switch (j) {
                            case 2:
                                wsprintf(ws, "Phone Java");
                                break;
                            case 3:
                                wsprintf(ws, "User Java");
                                break;
                            default:
                                wsprintf(ws, "Unknown Java (%d)", j);
                                break;
                            }
                            break;
                        default:
                            wsprintf(ws, "Unknown %d(%d) bearer", i, j);
                            break;
                        }
                        goto L_ADD;
                    }
#else
          if (!strncmp_nocase(s,"Java",4))
	  {
	    typedef struct
	    {
	      CSM_RAM csm;
	      int bearer;
	      int gui_id;
	      int gui_id2;
	    }JAVAINTERFACE_CSM;
	    int i=((JAVAINTERFACE_CSM *)icsm)->bearer;
	    ws=AllocWS(64);
	    switch(i)
	    {
	    case 1:
	      wsprintf(ws,"Browser");
	      break;
	    case 0xF:
              wsprintf(ws,"User Java");
	      break;
            case 0x11:
#ifdef X75
              wsprintf(ws,"Java: %t",GetLastJavaApplication());
#else
              wsprintf(ws,"Java");
#endif
	      break;
            case 18:
              wsprintf(ws,"E-mail client");
	      break;
            default:
              wsprintf(ws,"Unknown %d bearer",i);
              break;
	    }
	    goto L_ADD;
	  }
#endif
                    if (strncmp(s, "!SKIP!", 6)) {
                        ws = AllocWS(64);
                        i = 0;
                        while ((c = *s++) >= ' ') {
                            if (i < (sizeof(ss) - 1)) ss[i++] = c;
                        }
                        ss[i] = 0;
                        wsprintf(ws, "%t", ss);
                    L_ADD:
                        AddNL(ws, 0);
                        NLTOP->p = icsm;
                        count++;
                    }
                }
            }
        }
    }
    while ((icsm = icsm->next));
    sprintf(UI_HDR_TXT, "XTask3.0 : %d dialogs", count);
    REALD_COUNT = count - NSD_COUNT;
    return count;
}

int KeyHook(int submsg, int msg) {
#ifdef NEWSGOLD
    void *icsm = FindCSMbyID(CSM_root()->idle_id);
    if ((submsg == RED_BUTTON) && (CFG_RED_BUTTON_MODE)) {
        if ((CSM_root()->csm_q->csm.last == icsm) || (IsCalling()))
        //(IsGuiOnTop(((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4]))
        {
            if (msg == KEY_UP) {
                if (MODE_RED != 2) {
                    MODE_RED = 0;
                    return KEYHOOK_BREAK;
                }
            }
            MODE_RED = 2; //Ложим на отпускания
        }
        else {
            if (msg == KEY_DOWN) {
                if (MODE_RED == 1) {
                    MODE_RED = 0;
                    return KEYHOOK_NEXT; //Long press, continue with REDB PRESS
                }
            }
            if (msg == KEY_UP) {
                if (MODE_RED) {
                    MODE_RED = 0; //Release after longpress
                    return KEYHOOK_NEXT;
                }
                else { //Release after short press
                    if (CFG_RED_BUTTON_MODE == 1) {
                        GBS_SendMessage(MMI_CEPID, KEY_DOWN, RIGHT_SOFT);
                    }
                    else {
                        if (!CSM_ID) {
                            CSM_MoveToTop(CSM_root()->idle_id, -1);
                        }
                    }
                }
            }
            if (msg == LONG_PRESS) {
                MODE_RED = 1;
                GBS_SendMessage(MMI_CEPID, KEY_DOWN, RED_BUTTON);
            }
            return KEYHOOK_BREAK;
        }
    }
#endif
    if (CFG_ACTIVATION_STYLE == 3) {
        if (submsg != ENTER_BUTTON) {
            return KEYHOOK_NEXT;
        }
        switch (msg) {
            case KEY_DOWN:
                if (MODE_ENTER == 2) {
                    GBS_SendMessage(MMI_CEPID, KEY_UP, ENTER_BUTTON);
                    return KEYHOOK_NEXT;
                }
                MODE_ENTER = 0;
                return KEYHOOK_BREAK;
            case KEY_UP:
                if (MODE_ENTER == 0) {
                    MODE_ENTER = 2;
                    GBS_SendMessage(MMI_CEPID, KEY_DOWN, ENTER_BUTTON);
                    return KEYHOOK_BREAK;
                }
                if (MODE_ENTER == 2) {
                    MODE_ENTER = 0;
                    return KEYHOOK_NEXT;
                }
                MODE_ENTER = 0;
                return KEYHOOK_BREAK;
            case LONG_PRESS:
                MODE_ENTER = 1;
                if (IsUnlocked() || CFG_ENABLE_LOCK) {
                    CSM_Create();
                }
                MODE = 0;
                return KEYHOOK_BREAK;
            }
    }
    // * + # implementation
    if ((CFG_ACTIVATION_STYLE == 2) && !(CSM_ID)) {
        if (msg == KEY_UP) {
            MODE = 0;
            return KEYHOOK_NEXT;
        }
        if (msg == KEY_DOWN) {
            switch (submsg) {
                case '*':
                    MODE = 1;
                    return (0);
                case '#':
                    if (MODE == 1) {
                        if (IsUnlocked() || CFG_ENABLE_LOCK) {
                            CSM_Create();
                        }
                        else MODE = 0;
                    }
                    else return KEYHOOK_NEXT;
                }
            }
        }
    if (CFG_ACTIVATION_STYLE < 2) {
        if (submsg != CFG_ACTIVATION_KEY) {
            return KEYHOOK_NEXT;
        }
        if (CSM_ID) {
            if (msg == KEY_UP) {
                MAIN_CSM *csm = (MAIN_CSM*)FindCSMbyID(CSM_ID);
                if (csm && IsGuiOnTop(csm->gui_id)) {
                    GBS_SendMessage(MMI_CEPID, KEY_DOWN, ENTER_BUTTON);
                }
            }
            return KEYHOOK_BREAK;
        }
        switch (msg) {
            case KEY_DOWN:
                MODE = 0;
                if (CFG_ACTIVATION_STYLE == 0) {
                    return KEYHOOK_BREAK;
                }
                else {
                    return KEYHOOK_NEXT;
                }
            case KEY_UP:
                if (MODE == 1) {
                    //Release after longpress
                    MODE = 0;
                    if ((CFG_ACTIVATION_STYLE == 1) || (CFG_ENABLE_LONG_PRESS == 3)) {
                        //Launch on LongPress or Extra on LP - Launch
                        if (IsUnlocked() || CFG_ENABLE_LOCK) {
                            CSM_Create();
                        }
                        return KEYHOOK_BREAK;
                    }
                    if (CFG_ENABLE_LONG_PRESS == 1) {
                        return KEYHOOK_BREAK;
                    }
                    if (CFG_ENABLE_LONG_PRESS == 2) {
                        CSM_MoveToTop(CSM_root()->idle_id, -1);
                        return KEYHOOK_BREAK;
                    }
                    if (CFG_ENABLE_LONG_PRESS == 4) {
                        CSM_MoveToTop(CSM_root()->idle_id, -1);
                        KbdLock();
                        return KEYHOOK_BREAK;
                    }
                    break;
                }
                if (CFG_ACTIVATION_STYLE == 0) {
                    if (IsUnlocked() || CFG_ENABLE_LOCK) {
                        CSM_Create();
                    }
                    return KEYHOOK_BREAK;
                }
                break;
            case LONG_PRESS:
                MODE = 1;
                #ifndef NEWSGOLD
                    if (ACTIVE_KEY_STYLE == 1) {
                        if (ENA_LONG_PRESS) {
                            return KEYHOOK_NEXT;
                        } else {
                            return KEYHOOK_BREAK;
                        }
                #else
                    return KEYHOOK_BREAK;
                #endif
                break;
        }
    }
    return KEYHOOK_NEXT;
}

int OnMessage(CSM_RAM *data, GBS_MSG *msg) {
    int csm_result;
    int icgui_id;
    //  int ocgui_id;
    int idlegui_id;

#ifndef NEWSGOLD
#define EXT_BUTTON 0x63
  if ((ACTIVE_KEY_STYLE!=2)&&(ACTIVE_KEY_STYLE!=3)) //не "* + #" и не "Enter Button"
  {//чтоб можно было вызвать браузер при этих режимах
    if (ACTIVE_KEY==EXT_BUTTON) //мнимая кнопка браузера
    {
      if (msg->msg==439) //вызван браузер
      {
        switch (msg->submess)
        {
	case 1:
	  GBS_SendMessage(MMI_CEPID,LONG_PRESS,EXT_BUTTON);
          break;
	case 2:
	  GBS_SendMessage(MMI_CEPID,KEY_UP,EXT_BUTTON);
          break; // Никакого default!!!
        }
      }
      else //браузер не вызывался
	goto L1;
    }
    else //кнопка вызова не является мнимой кнопкой вызова браузера
      goto L1;
  }
  else
L1:
  csm_result=old_icsm_onMessage(data,msg);
#else
    csm_result = IDLE_OnMessage(data, msg); //Вызываем старый обработчик событий
#endif

    icgui_id = ((int*)data)[DISPLACE_OF_INCOMMINGGUI / 4];
    idlegui_id = ((int*)data)[DISPLACE_OF_IDLEGUI_ID / 4];
    //  ocgui_id=((int *)data)[DISPLACE_OF_OUTGOINGGUI/4];
    if (!icgui_id) CALLHIDE_MODE = 0;
    //  if (!ocgui_id) callhide_mode=0;

    if (msg->msg == MSG_RECONFIGURE_REQ) {
        if (strcmpi(CFG_PATH, (char*)msg->data0) == 0) {
            ShowMSG(1, (int)"XTask config updated!");
            InitConfig();
        }
    }
    else if (msg->msg == MSG_IPC) {
        if (msg->submess != 392305998) {
            IPC_REQ *ipc;
            if ((ipc = (IPC_REQ*)msg->data0)) {
                if (strcmp(ipc->name_to, IPC_XTASK_NAME) == 0) {
                    switch (msg->submess) {
                        case IPC_XTASK_SHOW_CSM:
                            if (CSM_ID) {
                                break;
                            }
                            if (!IsCalling() && !SHOW_LOCK) {
                                if ((CSM_root()->csm_q->csm.last != data) || IsGuiOnTop(idlegui_id)) {
                                    CSM_MoveToTop((int)(ipc->data), -1);
                                }
                            }
                        break;
                        case IPC_XTASK_IDLE:
                            if (CSM_ID) {
                                break;
                            }
                            if (!IsCalling()) {
                                CSM_MoveToTop(CSM_root()->idle_id, -1);
                            }
                        break;
                        case IPC_XTASK_LOCK_SHOW:
                            SHOW_LOCK++;
                        break;
                        case IPC_XTASK_UNLOCK_SHOW:
                            if (SHOW_LOCK) {
                                SHOW_LOCK--;
                            }
                        break;
                    }
                }
            }
        }
    } else if (msg->msg == MSG_INCOMMING_CALL) {
        CALLHIDE_MODE = 1;
    }
    if (CALLHIDE_MODE) {
        if ((IsGuiOnTop(icgui_id))) {
            CSM_MoveToTop(CSM_root()->idle_id, ((CSM_RAM*)(CSM_root()->csm_q->csm.last))->id);
            CALLHIDE_MODE = 0;
        }
    }
    return csm_result;
}

void KillELF() {
    LockSched();
    GBS_DelTimer(&TMR_START);
    RemoveKeybMsgHook(KeyHook);
    IDLE_DisableHook();
    UnlockSched();
    SUBPROC(kill_elf);
}

void OnClose(CSM_RAM *data) {
    IDLE_OnClose(data);
    KillELF();
}

void DoSplices(GBSTMR *) {
    SHOW_DAEMONS = CFG_SHOW_DAEMONS;
    LockSched();
    if (!AddKeybMsgHook_end((void*)KeyHook)) {
        ShowMSG(1, (int)"Невозможно зарегистрировать обработчик!");
        SUBPROC(kill_elf);
    }
    else {
        if (CFG_ENABLE_HELLO_MSG) {
            ShowMSG(1, (int)"XTask3 установлен!");
        }
        IDLE_EnableHook(OnMessage, OnClose);
        CSM_UNDER_IDLE = GetUnderIdleCSM(); //Ищем idle_dialog
    }
    UnlockSched();
}

int main() {
    MODE = 0;
    if (InitConfig() != 2) {
        GBS_StartTimerProc(&TMR_START, SecToTicks(60), DoSplices);
    } else {
        DoSplices(NULL);
    }
    return 0;
}
