//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_PROCESS_PINGUPLOAD_H
#define KFROUTER_KF_PROCESS_PINGUPLOAD_H

#include "kf_data_parse_pingupload.h"
#include "main.h"
#include "kf_network.h"

class KFProcessPingUpload {
public:
    KFProcessPingUpload();
    ~KFProcessPingUpload();

    int DoPingUpload(KFDataParsePingUpload *pu, KFNetwork *net, KFSSResultData *ssData, KFDataParseIPProperties *ips,
            KFPingCheckResult *pcResult);
};

#endif //KFROUTER_KF_PROCESS_PINGUPLOAD_H
