//
// Created by v on 16-3-26.
// Author: zhangtao@roamter.com
//

#ifndef KFROUTER_KF_DATA_PARSE_ONLINENOTIFY_H
#define KFROUTER_KF_DATA_PARSE_ONLINENOTIFY_H

#include "kf_data_parse.h"
#include "main.h"

class KFDataParseOnlineNotify : public KFDataParse {
public:
    KFDataParseOnlineNotify();
    ~KFDataParseOnlineNotify();

    int OnlineNotifyGenerateData(KFOnlineNotify *on);
    //0        success
    //-1       NULL input
    //-2       Generral error
    //-3       Head error
    //-4       Result error
    int KFDataParseResult(const char *str, int len);

    const char *OnlineNotifyGenerateData() {
        return this->szGenData.c_str();
    }

};

#endif //KFROUTER_KF_DATA_PARSE_ONLINENOTIFY_H
