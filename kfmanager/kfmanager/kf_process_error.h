//
// Created by v on 16-3-29.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_PROCESS_ERROR_H
#define KFROUTER_KF_PROCESS_ERROR_H

#include "main.h"
#include "kf_network.h"
#include "kf_data_parse_error.h"
#include "kf_data_parse.h"

class KFProcessError {
public:
    KFProcessError();
    ~KFProcessError();


    // result
    // 0      success
    // -1     Input error
    // -2     GenerateData error
    // -3     NetworkRequest != 200
    //-4      DateParseResult error
    int DoErrorReport(KFDataParseError *dpe, KFErrorReport *er, KFNetwork *net);
};

#endif //KFROUTER_KF_PROCESS_ERROR_H
