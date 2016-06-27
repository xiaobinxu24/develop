//
// Created by v on 16-3-26.
// Author: zhangtao@roamter.com
//

#include <stdio.h>
#include "rt_print.h"
#include <string.h>
#include <string.h>
#include "main.h"
#include "kf_data_parse_onlinenotify.h"

KFDataParseOnlineNotify::KFDataParseOnlineNotify() {

}

KFDataParseOnlineNotify::~KFDataParseOnlineNotify() {

}
//KFOnlineNotify on = {}
int KFDataParseOnlineNotify::OnlineNotifyGenerateData(KFOnlineNotify *on) {
    rtinfo("Enter data parse online notify");
    Json::Value OnlineNotify;
    this->KFDataParseGenerateHead(&OnlineNotify, KF_STR_ONLINENOTYFY, KF_STR_SYSID);
    Json::Value bodys;
    bodys[KF_STR_PARAMS][KF_STR_MAC] = on->szMac;
    OnlineNotify["MsgBody"][0] = bodys;
    
    Json::StreamWriterBuilder b;
    this->szGenData = Json::writeString(b, OnlineNotify);
    rtdebug("%s",this->szGenData.c_str());
    rtinfo("Exit data parse online notify");
    return 0;
}

int KFDataParseOnlineNotify::KFDataParseResult(const char *str, int len) {
    if (!str) return -1;
    char data[1024] = {0};
    //check if has common error
    strncpy(data, str, len);
    Json::Reader jrd;
    Json::Value json_result;
    jrd.parse(data, json_result, 0);
    int ret = this->KFDataParseGeneralError(json_result);
    if (ret < 0) {
        //Result is fail .do something.
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
