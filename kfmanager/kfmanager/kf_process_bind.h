//
// Created by v on 16-3-29.
// Author: zhangtao@roamter.com
//
#ifndef KFROUTER_KF_PROCESS_BIND_H
#define KFROUTER_KF_PROCESS_BIND_H

#include "main.h"
#include "kf_network.h"
#include "kf_data_parse_bind.h"
#include "kf_data_parse.h"



class KFProcessBind {
public:
    KFProcessBind();
    ~KFProcessBind();

    // result 
    // 1      resultMsg = "Y"
    // -5     resultMsg = "S"
    // -6     resultMsg = "N"
    // -1     Input error
    // -2     BindGenerateData error
    // -3     NetworkRequest != 200
    //-4      DateParseResult error 
    int DoBind(KFDataParseBind *dpb, KFBindJudge *bj, KFNetwork *net);

};

#endif //KFROUTER_KF_PROCESS_BIND_H
