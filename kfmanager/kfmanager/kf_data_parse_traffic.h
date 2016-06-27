//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_DATA_PARSE_TRAFFIC_H
#define KFROUTER_KF_DATA_PARSE_TRAFFIC_H

#include "kf_data_parse.h"
#include "main.h"


class KFDataParseTraffic:public KFDataParse {
public:
    KFDataParseTraffic();
    ~KFDataParseTraffic();

    int KFClearTraffic(KFTrafficInfo *ti);

    int KFGenerateTraffic(KFTrafficInfo *ti);

    int KFGenerateTrafficData(KFTrafficInfo *ti);

    // 0      success
    // -1     input NULL
    // -2     GeneralError
    // -3     dataparse head error
    // -4     dataparse result error

    int KFDataParseTrafficResult(const char *result, int len);

    const char *GeneratedData() {
        return this->szData.c_str();
    }

private:
    std::string          szData; // for save generated string.

};

#endif //KFROUTER_KF_DATA_PARSE_TRAFFIC_H
