#include <swilib.h>

#define DATA_SIZE 32767

char CSM_LIST_DATA[DATA_SIZE];

extern char CFG_CSM_LIST_PATH[128];

void ReadCSMList() {
    int size = 0;
    uint32_t err;

    int fp = sys_open(CFG_CSM_LIST_PATH, A_ReadOnly + A_BIN, P_READ, &err);
    if (fp != -1) {
        size = sys_read(fp, CSM_LIST_DATA, DATA_SIZE, &err);
        sys_close(fp, &err);
    }
    if (size >= 0) {
        CSM_LIST_DATA[size] = 0;
    }
}
