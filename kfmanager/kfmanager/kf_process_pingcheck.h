//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_PROCESS_PINGCHECK_H
#define KFROUTER_KF_PROCESS_PINGCHECK_H
#include "kf_data_parse_queryip.h"
#include "main.h"
#include "kf_sys_operation.h"

class KFProcessPingCheck {
public:
    KFProcessPingCheck();
    ~KFProcessPingCheck();

    int DoPingCheck(KFDataParseQueryIP *dp, KFSysOperation *sys, KFPingCheckResult *result, KFDataParseIPProperties **tips);
    int MultiThreadDoPingCheck(KFDataParseQueryIP *dp, KFSysOperation *sys, KFPingCheckResult *result, KFDataParseIPProperties **tips);
    pthread_t   pingcycleThread[5];
    pthread_mutex_t mutex;
    int threadFinishflag;
    struct args {
        KFDataParseIPProperties *dpip;
        KFPingCheckResult *pcresult;
        KFSysOperation  *sys;
        std::string pingresult[5];
    } *curargs, pingarg[5];

    int i;
private:

};

#endif //KFROUTER_KF_PROCESS_PINGCHECK_H
