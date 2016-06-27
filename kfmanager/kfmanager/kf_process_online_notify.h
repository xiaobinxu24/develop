//
// Created by v on 16-3-29.
// Author: zhangtao@roamter.com
//

#ifndef KFROUTER_KF_PROCESS_ONLINE_NOTIFY_H
#define KFROUTER_KF_PROCESS_ONLINE_NOTIFY_H

#include "main.h"
#include "kf_network.h"
#include "kf_data_parse_onlinenotify.h"
#include "kf_data_parse.h"

class KFProcessOnlineNotify {
public:
    KFProcessOnlineNotify();
    ~KFProcessOnlineNotify();

    // result
    // 0       success
    // -2     BindGenerateData error
    // -3     NetworkRequest != 200
    //-4      DateParseResult error
    int DoOnlineNotify(KFDataParseOnlineNotify *dpon, KFOnlineNotify * on, KFNetwork *net);
};

#endif //KFROUTER_KF_PROCESS_ONLINE_NOTIFY_H
