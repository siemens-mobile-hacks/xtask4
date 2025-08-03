#pragma once

#include <swilib.h>

typedef struct {
    CSM_RAM csm;
    int gui_id;
} MAIN_CSM;

extern int CSM_ID;

void CSM_Create();
