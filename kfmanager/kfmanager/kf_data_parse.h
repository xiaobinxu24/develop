//
// Created by v on 16-3-15.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_DATA_PARSE_H
#define KFROUTER_KF_DATA_PARSE_H

#include "json/json.h"

class KFDataParse {
public:
    KFDataParse();
    ~KFDataParse();

    // Generated string
    // Each call after call relative Generate command.
    char *KFDataParseDataString() {
        return this->szDataString;
    };

    char *KFDataParseCmdErrorCode() {
        return this->szCmdErrorCode;
    }
    char *KFDataParseCmdErrorMsg() {
        return this->szCmdErrorMsg;
    }

    std::string KFDataParseResultCode() {
        return this->szResultCode;
    }
    std::string KFDataParseResultMsg() {
        return this->szResultMsg;
    }

protected:
    int KFDataParseGeneralError(Json::Value json_result);
    int KFDataParseResultError(Json::Value json_result);
    int KFDataParseHeadValid(Json::Value json_result);

    int KFDataParseGenerateHead(Json::Value *ijson, const char *code, const char *id);
    int KFDataParseGenerateMD5(unsigned char *input, int len, char *save);

    char          szDataString[1024]; // many common use.
    std::string   szGenData;  // Generated string

private:

    char          szCmdErrorCode[16];
    char          szCmdErrorMsg[1024];

    std::string        szResultCode;
    std::string        szResultMsg;
//    char          szResultCode[16]; // String parse result success/fail
//    char          szResultMsg[1024]; // some result msg of fail.
};

#endif //KFROUTER_KF_DATA_PARSE_H
