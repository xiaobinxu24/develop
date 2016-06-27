//
// Created by v on 16-3-29.
// Author: xiaobin_xu@roamter.com
//
#include <stdio.h>
#include <rt_print.h>
#include <stdlib.h>
#include <string.h>
#include "kf_data_parse_error.h"
#include "main.h"

KFDataParseError::KFDataParseError() {

}

KFDataParseError::~KFDataParseError() {

}

//KFErrorReport er = {"34BDF9832FE0","FAIL-017","路由器使用信息查询失败【sql】"};
int KFDataParseError::GenerateData(KFErrorReport *er) {
    if (!er) return -1;
    rtdebug("Enter the error");
    Json::Value errorreport;
    this->KFDataParseGenerateHead(&errorreport, "uploadWarningMsg", KF_STR_SYSID);
    Json::Value bodys;
    bodys[KF_STR_PARAMS][KF_STR_WARNINGENTITY] = er->szEntity;
    bodys[KF_STR_PARAMS][KF_STR_WARINGCODE] = er->szCode;
    bodys[KF_STR_PARAMS][KF_STR_WARNINGMSG] = er->szMsg;
    bodys[KF_STR_PARAMS][KF_STR_WARNINGLEVEL] = er->szLevel;
    errorreport["MsgBody"][0] = bodys;

    Json::StreamWriterBuilder b;
    this->szGenData = Json::writeString(b, errorreport);

    rtdebug("%s", this->szGenData.c_str());
    rtdebug("exit the error");

    return 0;
}

int KFDataParseError::KFDataParseResult(const char *str, int len){
    if (!str) return -1;

    char data[1024] = {0};
    // check if has common error
    strncpy(data, str, len);
    Json::Reader jrd;
    Json::Value json_result;
    jrd.parse(data, json_result, 0);
    int ret = this->KFDataParseGeneralError(json_result);
    if (ret < 0) {
        // Result is fail. do something.
        return -2;
    }
    ret = this->KFDataParseHeadValid(json_result);
    if (ret < 0) {
        return -3;
    }

    ret = this->KFDataParseResultError(json_result[KF_STR_MSGBODY][KF_STR_RESULT]);
    if (ret < 0) {
        return -4;
    }

    return 0;
}