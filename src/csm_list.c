#include <swilib.h>
#include "config.h"

#define DATA_SIZE 32767

char CSM_LIST_DATA[DATA_SIZE];

void ReadCSMList() {
    int size = 0;
    uint32_t err;

    int fp = sys_open(CFG.csm_list_path, A_ReadOnly | A_BIN, P_READ, &err);
    if (fp != -1) {
        size = sys_read(fp, CSM_LIST_DATA, DATA_SIZE, &err);
        sys_close(fp, &err);
    }
    if (size >= 0) {
        CSM_LIST_DATA[size] = 0;
    }
}
