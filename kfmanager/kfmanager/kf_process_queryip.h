//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_PROCESS_QUERYIP_H
#define KFROUTER_KF_PROCESS_QUERYIP_H

#include "kf_data_parse_queryip.h"
#include "kf_network.h"
#include "main.h"
#include "kf_config.h"
#include "kf_sys_operation.h"

class KFProcessQueryIP {
public:
    KFProcessQueryIP();
    ~KFProcessQueryIP();

    int DoQueryIP(KFDataParseQueryIP *dp, KFNetwork *net, KFConfig *conf, KFSysOperation *sysop);
};

#endif //KFROUTER_KF_PROCESS_QUERYIP_H
