//
// Created by v on 16-3-15.
// Author: jmdvirus@roamter.com
//

#include "main.h"
#include <iostream>
#include <unistd.h>
#include "kf_process.h"
#include "rt_print.h"

using namespace std;

int main(int argc, char **args) {
    int debug = 0;
    int c;
    while ((c = getopt(argc, args, "d")) != -1) {
        switch (c) {
            case 'd':
                debug = 1;
            break;
            default: break;
        }
    }

    rt_print_set_save_type(RT_PRINT_LOG_TYPE_FILE);
    rt_print_init_file_conf("/etc/zlog.conf", "kfmanager");
    KFProcess *proc = new KFProcess();
    rt_print_change_level(RT_PRINT_ALL & ~RT_PRINT_DEBUG);

    if (debug == 1) {
        rt_print_change_level(RT_PRINT_ALL);
    }

    rtinfo("Program start to ready...");
    proc->KFProcessInit();

    rtinfo("Program start to run");
    proc->KFProcessRun();

    rtinfo("Program exit");

    delete proc;

    return 0;
}
