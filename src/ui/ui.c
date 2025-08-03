#include <swilib.h>
#include <string.h>
#include "menu_options.h"
#include "menu_bookmarks.h"
#include "../nl.h"
#include "../csm.h"
#include "../csm_utils.h"

extern int REALD_COUNT, NSD_COUNT;
extern int SHOW_DAEMONS;
extern int GetNumberOfDialogs();

char UI_HDR_TXT[32];
static HEADER_DESC HEADER_D = {{0, 0, 0, 0}, NULL, (int)UI_HDR_TXT, LGP_NULL};

static const int SOFTKEYS[] = {SET_LEFT_SOFTKEY, SET_RIGHT_SOFTKEY, SET_MIDDLE_SOFTKEY};

static const SOFTKEY_DESC SOFTKEYS_D[] = {
    {0x0018, 0x0000, (int)"Options"},
    {0x0001, 0x0000, (int)"Back"},
    {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB SOFTKEYS_TAB = {
    SOFTKEYS_D, 0
};

static void ItemProc(void *data, int item_n, void *unk) {
    WSHDR *ws;
    void *item = AllocMenuItem(data);
    NAMELIST *nl = GetNLItem(item_n);
    if (nl) {
        if (nl->name) {
            ws = AllocMenuWS(data, nl->name->wsbody[0] + 2);
            wstrcpy(ws, nl->name);
            if (nl->is_daemon) {
                wsInsertChar(ws, 0x0002, 1);
                wsInsertChar(ws, 0xE008, 1);
            }
        }
        else {
            ws = AllocMenuWS(data, 16);
            wsprintf(ws, "no name???");
        }
    }
    else {
        ws = AllocMenuWS(data, 16);
        wsprintf(ws, "error!!!");
    }
    SetMenuItemText(data, item, ws, item_n);
}

static int OnKey(GUI *gui, GUI_MSG *msg) {
    MAIN_CSM *csm = MenuGetUserPointer(gui);
    if (msg->gbsmsg->msg == KEY_DOWN) {
        int submess = msg->gbsmsg->submess;
        if (submess == '0') {
            MenuBookmarks_Create(csm);
            return -1;
        } else if ((submess >= '1') && (submess <= '9')) {
            if (ExecuteBookmark(submess - '1')) {
                return 1;
            }
            return -1;
        }
        switch (submess) {
            case LEFT_BUTTON:
                CSM_Close(GetCurMenuItem(gui));
                return -1;
            case '#':
                CSM_Close(GetCurMenuItem(gui));
                return 0;
        case '*':
            SHOW_DAEMONS = !SHOW_DAEMONS;
            RefreshGUI();
            return 0;
        case LEFT_SOFT:
            MenuOptions_Create(csm);
            return -1;
        case ENTER_BUTTON:
            NAMELIST *nl = GetNLItem(GetCurMenuItem(gui));
            if (!nl->is_daemon) {
                CSM_MoveToTop(((CSM_RAM*)(nl->p))->id, -1);
            }
            return 1;
        case RIGHT_SOFT:
            return 1;
        }
    }
    return 0;
}

void GHook(GUI *gui, int cmd) {
    if (cmd == 0xA) { // focus
        DisableIDLETMR();
        int cursor = GetCurMenuItem(gui);
        int dialogs = GetNumberOfDialogs();
        Menu_SetItemCountDyn(gui, dialogs);
        if (cursor >= dialogs) {
            SetCursorToMenuItem(gui, dialogs - 1);
        }
    } else if (cmd == 0x09) { //unfocus
        // ClearNL();
    }
}

static const MENU_DESC MENU_D = {
    8,
    OnKey,
    GHook,
    NULL,
    SOFTKEYS,
    &SOFTKEYS_TAB,
    MENU_FLAGS_ENABLE_TEXT_SCROLLING,
    ItemProc,
    NULL,
    NULL,
    0
};

int UI_Create(MAIN_CSM *csm) {
    memcpy(&(HEADER_D.rc), GetHeaderRECT(), sizeof(RECT));

    int selected_item = (REALD_COUNT > 1) ? 1 : 0;
    return CreateMenu(0, 0, &MENU_D, &HEADER_D, selected_item, REALD_COUNT, csm, 0);
}
