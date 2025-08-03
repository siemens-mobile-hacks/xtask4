#include <swilib.h>
#include <string.h>
#include "../nl.h"
#include "../csm_utils.h"
#include "icons.h"
#include "menu_options.h"

#define ITEMS_N 4


static HEADER_DESC HEADER_D = {{0, 0, 0, 0},NULL, (int)"Options", LGP_NULL};

static const int SOFTKEYS[] = {SET_LEFT_SOFTKEY, SET_MIDDLE_SOFTKEY, SET_RIGHT_SOFTKEY};

static const SOFTKEY_DESC SOFTKEYS_D[]= {
    {0x0018,0x0000,(int)"Select"},
    {0x003D,0x0000,(int)LGP_DOIT_PIC},
    {0x0001,0x0000,(int)"Back"},
};

static const SOFTKEYSTAB SOFTKEYS_TAB = {
    SOFTKEYS_D, 0
};

static int ICON[] = {ICON_EMPTY};

static const MENUITEM_DESC ITEMS[ITEMS_N] = {
    {ICON, (int)"Show IDLE", LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
    {ICON, (int)"Close CSM", LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
    {ICON, (int)"Close all CSM", LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
    {ICON, (int)"Exit", LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
};

void IDLE_Proc(GUI *gui) {
    MAIN_CSM *csm = MenuGetUserPointer(gui);
    GeneralFuncF1(1);
    GeneralFunc_flag1(csm->gui_id, 1);
    CSM_MoveToTop(CSM_root()->idle_id, -1);
}

void CloseCSM_Proc(GUI *gui) {
    MAIN_CSM *csm = MenuGetUserPointer(gui);
    void *main_gui = FindGUIbyId(csm->gui_id, (CSM_RAM**)(&csm));
    if (main_gui) {
        int cursor = GetCurMenuItem(main_gui);
        GeneralFuncF1(1);
        CSM_Close(cursor);
    }
}

void CloseAllCSM_Proc(GUI *gui) {
    CSM_CloseAll();
    ClearNL();
    GeneralFuncF1(1);
}

void Exit_Proc(GUI *gui) {
    extern void KillELF();

    MAIN_CSM *csm = MenuGetUserPointer(gui);
    GeneralFuncF1(1);
    GeneralFunc_flag1(csm->gui_id, 1);
    KillELF();
}

static const MENUPROCS_DESC PROCS[ITEMS_N] =  {
    IDLE_Proc,
    CloseCSM_Proc,
    CloseAllCSM_Proc,
    Exit_Proc,
};

static const MENU_DESC MENU_D = {
    8,
    NULL,
    NULL,
    NULL,
    SOFTKEYS,
    &SOFTKEYS_TAB,
    MENU_FLAGS_ENABLE_TEXT_SCROLLING | MENU_FLAGS_ENABLE_ICONS,
    NULL,
    ITEMS,
    PROCS,
    ITEMS_N
};

int MenuOptions_Create(MAIN_CSM *csm) {
    memcpy(&(HEADER_D.rc), GetOptionsHeaderRect(), sizeof(RECT));
    return CreateMenu(1, 0, &MENU_D, &HEADER_D, 0, ITEMS_N, csm, NULL);
}
