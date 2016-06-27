//
// Created by v on 16-3-29.
// Author: xiaobin_xu@roamter.com
//

#ifndef KFROUTER_KF_DATA_PARSE_ERROR_H
#define KFROUTER_KF_DATA_PARSE_ERROR_H

#include "kf_data_parse.h"
#include "main.h"

class KFDataParseError : public KFDataParse {
public:
    KFDataParseError();
    ~KFDataParseError();

    int GenerateData(KFErrorReport *er);

    //0        success
    //-1       NULL input
    //-2       Generral error
    //-3       Head error
    //-4       Result error
    int KFDataParseResult(const char *str, int len);
    const char *GeneratedData() {
        return this->szGenData.c_str();
    }

};

#endif //KFROUTER_KF_DATA_PARSE_ERROR_H
