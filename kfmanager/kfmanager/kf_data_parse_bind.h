//
// Created by v on 16-3-26.
// Author: zhangtao@roamter.com
//

#ifndef KFROUTER_KF_DATA_PARSE_BIND_H
#define KFROUTER_KF_DATA_PARSE_BIND_H

#include "kf_data_parse.h"
#include "main.h"

class KFDataParseBind:public KFDataParse {
public:
    KFDataParseBind();
    ~KFDataParseBind();

    int KFDataParseBindGenerateData(KFBindJudge *bj);

    //0        success
    //-1       NULL input
    //-2       Generral error
    //-3       Head error
    //-4       Result error
    int KFDataParseResult(const char *str, int len);

    const char *KFDataParseBindGenerateData() {
        return this->szGenData.c_str();
    }

    const char *BindResult() {
        return this->stBindResult.c_str();
    }

private:
    std::string   stBindResult;
};

#endif //KFROUTER_KF_DATA_PARSE_BIND_H
