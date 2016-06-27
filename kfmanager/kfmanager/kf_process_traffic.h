//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_PROCESS_TRAFFIC_H
#define KFROUTER_KF_PROCESS_TRAFFIC_H

#include "main.h"
#include "kf_network.h"
#include "kf_sys_operation.h"
#include "kf_data_parse_traffic.h"
#include "kf_config.h"

class KFProcessTraffic {
public:
    KFProcessTraffic();
    ~KFProcessTraffic();

    int ClearTraffic(KFNetwork *net, KFSysOperation *sysOp, KFDataParseTraffic *traffic, KFConfig *conf);
    int UploadTraffic(KFNetwork *net, KFSysOperation *sysOp, KFDataParseTraffic *traffic, KFConfig *conf);

private:
    KFTrafficInfo     stTraffic;
};

#endif //KFROUTER_KF_PROCESS_TRAFFIC_H
