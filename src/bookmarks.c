#include <swilib.h>
#include <sie/sie.h>
#include "config.h"


const char *GetBookmarkName(int id) {
    switch (id) {
        case 0: return CFG.bm1_name;
        case 1: return CFG.bm2_name;
        case 2: return CFG.bm3_name;
        case 3: return CFG.bm4_name;
        case 4: return CFG.bm5_name;
        case 5: return CFG.bm6_name;
        case 6: return CFG.bm7_name;
        case 7: return CFG.bm8_name;
        case 8: return CFG.bm9_name;
        default: return NULL;
    }
}

const char *GetBookmarkValue(int id) {
    switch (id) {
        case 0: return CFG.bm1_value;
        case 1: return CFG.bm2_value;
        case 2: return CFG.bm3_value;
        case 3: return CFG.bm4_value;
        case 4: return CFG.bm5_value;
        case 5: return CFG.bm6_value;
        case 6: return CFG.bm7_value;
        case 7: return CFG.bm8_value;
        case 8: return CFG.bm9_value;
        default: return NULL;
    }
}

int ExecuteBookmark(int id) {
    return Sie_Exec_Execute(GetBookmarkValue(id));
}
