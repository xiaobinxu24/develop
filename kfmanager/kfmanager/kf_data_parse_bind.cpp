//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#include <stdio.h>
#include "rt_print.h"
#include <string.h>
#include "main.h"
#include "kf_data_parse_bind.h"

KFDataParseBind::KFDataParseBind() {

}

KFDataParseBind::~KFDataParseBind() {

}

int KFDataParseBind::KFDataParseBindGenerateData(KFBindJudge *bj)
{
    rtinfo("Enter data parse bind");
    Json::Value Bind;
    this->KFDataParseGenerateHead(&Bind, KF_STR_BINDJUDGE, KF_STR_SYSID);
    Json::Value bodys;
    bodys[KF_STR_PARAMS][KF_STR_MAC] = bj->szMac;
    Bind["MsgBody"][0] = bodys;

    Json::StreamWriterBuilder b;
    this->szGenData = Json::writeString(b, Bind);
    rtdebug("%s", this->szGenData.c_str());
    rtinfo("Exit data parse bind");
    
    return 0;
}

int KFDataParseBind::KFDataParseResult(const char *str, int len)
{
    if (!str) return -1;
    char data[1024] = {0};
    //check if has common error
    strncpy(data, str, len);
    Json::Reader jrd;
    Json::Value json_result;
    jrd.parse(data, json_result, 0);
    int ret = this->KFDataParseGeneralError(json_result);
    if (ret < 0)
    {
        //Result is fail , do something.
        return -2;
    }
    ret = this->KFDataParseHeadValid(json_result);
    if (ret < 0)
    {
        return -3;
    }

    ret = this->KFDataParseResultError(json_result[KF_STR_MSGBODY][KF_STR_RESULT]);
    if (ret < 0)
    {
        return -4;
    }
    if (json_result[KF_STR_MSGBODY][KF_STR_RESULT][KF_STR_RESULTMSG] != Json::nullValue) {

       std::string resultMsg = json_result[KF_STR_MSGBODY][KF_STR_RESULT][KF_STR_RESULTMSG].asString();

        this->stBindResult = resultMsg;
        rtinfo("Bind resultMsg %s", this->stBindResult.c_str());
    }
    return 0;
}
