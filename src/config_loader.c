#include <sie/sie.h>

char CFG_PATH[] = "0:\\zbin\\etc\\XTask4.bcfg";

int InitConfig() {
    __CONFIG_EXTERN(0, cfghdr_0);
    int result = Sie_Config_Load(CFG_PATH, __CONFIG(0, cfghdr_0));
    if (!result) {
        CFG_PATH[0] = '4';
        Sie_Config_Load(CFG_PATH, __CONFIG(0, cfghdr_0));
    }
    if (!result) {
        CFG_PATH[0] = '0';
    }
    return result;
}

void SaveConfig() {
    __CONFIG_EXTERN(0, cfghdr_0);
    if (!Sie_Config_Save(CFG_PATH, __CONFIG(0, cfghdr_0))) {
        CFG_PATH[0] = '4';
        if (!Sie_Config_Save(CFG_PATH, __CONFIG(0, cfghdr_0))) {
            CFG_PATH[0] = '0';
        }
    }
}
