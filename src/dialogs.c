#include <swilib.h>
#include <stdio.h>
#include <string.h>
#include "nl.h"
#include "config.h"
#include "csm_list.h"
#include "ui/ui.h"

int REALD_COUNT; // Number of real dialogs
int NSD_COUNT; // Number of new style dialogs
int SHOW_DAEMONS;

extern CSM_RAM *CSM_UNDER_IDLE;

typedef struct {
    CSM_RAM csm;
#ifdef NEWSGOLD
    int unk1;
    int bearer;
    int gui_id;
    int unk2;
    int param_R1;
    int param_ZERO;
#else
    int bearer;
    int gui_id;
    int gui_id2;
#endif
} RB_CSM;

static char *FindCSMName(const CSM_RAM *csm) {
    char s[40];
    CSM_DESC *desc = csm->constr;
    sprintf(s, "%08X ", (uint32_t)desc);
    char *p = strstr(CSM_LIST_DATA, s);
    if (p) {
        return p + 9;
    } else {
        static char u[40];
        sprintf(u, "Unknown %08X!", (uint32_t)desc);
        return u;
    }
}

int BuildDialogList() {
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
            if (CFG.show_idle) {
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
                    if (strcmpi(s, "Java") == 0) {
                        int bearer = ((RB_CSM*)icsm)->bearer;
                        int j = ((RB_CSM*)icsm)->param_R1;
                        if (bearer == 2) continue;
                        ws = AllocWS(64);
                        switch (bearer) {
                            case 1:
                                wsprintf(ws, "Browser", j);
                            break;
                            case 2:
                                wsprintf(ws, "Jam %d", j);
                            break;
                            case 3:
                                if (j == 2 || j == 3 || j == 4) {
                                    const int slot_id = (j > 2);
                                    const char *java_name = GetLastJavaApplication(slot_id);
                                    if (java_name) {
                                        wsprintf(ws, "Java: %t", java_name);
                                    } else if (slot_id == 0) {
                                        wsprintf(ws, "Java Slot 1");
                                    } else {
                                        wsprintf(ws, "Java Slot 2");
                                    }
                                } else {
                                    wsprintf(ws, "Unknown Java (%d)", j);
                                }
                            break;
                            default:
                                wsprintf(ws, "Unknown %d(%d) bearer", bearer, j);
                            break;
                        }
                        goto L_ADD;
                    }
#else
          if (!strncmp_nocase(s,"Java",4))
	  {
	    int i=((RB_CSM *)icsm)->bearer;
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
                    if (strncmp(s, "!SKIP!", 6) != 0) {
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
    } while ((icsm = icsm->next));
    UI_SetExtraHeader(count);
    REALD_COUNT = count - NSD_COUNT;
    return count;
}