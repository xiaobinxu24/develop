//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//


#include <stdio.h>
#include <rt_print.h>
#include <cstring>
#include "kf_data_parse_traffic.h"



KFDataParseTraffic::KFDataParseTraffic() {

}

KFDataParseTraffic::~KFDataParseTraffic() {

}

int KFDataParseTraffic::KFClearTraffic(KFTrafficInfo *ti) {

    if (!ti) return -1;
    rtdebug("Enter data parse clear traffic");

    ti->lTotalTraffic=(Json::Value::UInt64)0;
    ti->lTotalForeignTraffic=(Json::Value::UInt64)0;
    ti->lPrevTraffic=(Json::Value::UInt64)0;
    ti->lPrevTotalForeignTraffic=(Json::Value::UInt64)0;
    ti->lCircleTotalTraffic = (Json::Value::UInt64)0;
    ti->lCircleForeignTotalTraffic =(Json::Value::UInt64) 0;

    this->KFGenerateTrafficData(ti);

    rtdebug("Exit data parse clear traffic");
    return 0;
}

int KFDataParseTraffic::KFGenerateTraffic(KFTrafficInfo *ti) {
    if (!ti) return -1;

    rtdebug("Enter data parse upload traffic");


    this->KFGenerateTrafficData(ti);


    rtdebug("Exit data parse upload traffic");


    return 0;
}



int KFDataParseTraffic::KFGenerateTrafficData(KFTrafficInfo *ti){
    if (!ti) return -1;
    rtdebug("Enter generate traffic  data");

    Json::Value uploadRouteMsg;
    this->KFDataParseGenerateHead(&uploadRouteMsg,KF_STR_UPLOADROUTEMSG, KF_STR_SYSID);

    Json::Value bodys;
    Json::Value params;

    params[KF_STR_MAC] = ti->szMac;
    params[KF_STR_WANIP] = ti->szWanIP;
    params[KF_STR_STARTUPTIMESTAMP] = ti->szStartupTime;
    params[KF_STR_REMAINRAM] =  Json::valueToString((Json::Value::Int)ti->iRemainRam);
    params[KF_STR_RESOURCEIP] = ti->szResourceIP;
    params[KF_STR_RESOURCEPORT] = Json::valueToString((Json::Value::Int)ti->iResourcePort);
    params[KF_STR_CLIENTCONNNUM] = Json::valueToString((Json::Value::Int)ti->iClientConnNum);
    params[KF_STR_REQUESTURLNUM] = Json::valueToString((Json::Value::Int)ti->iRequestUrlNum);
    params[KF_STR_TOTALTRAFFIC] = Json::valueToString((Json::Value::UInt64)ti->lTotalTraffic);
    params[KF_STR_TOTALFOREIGNTRAFFIC] =  Json::valueToString((Json::Value::UInt64)ti->lTotalForeignTraffic);
    params[KF_STR_PREVTRAFFIC] =  Json::valueToString((Json::Value::UInt64)ti->lPrevTraffic);
    params[KF_STR_PREVTOTALFOREIGNTRAFFIC] =  Json::valueToString((Json::Value::UInt64)ti->lPrevTotalForeignTraffic);
    params[KF_STR_CIRCLETOTALTRAFFIC] =  Json::valueToString((Json::Value::UInt64)ti->lCircleTotalTraffic);
    params[KF_STR_CIRCLEFOREIGNTOTALTRAFFIC] =  Json::valueToString((Json::Value::UInt64)ti->lCircleForeignTotalTraffic);


    bodys[KF_STR_PARAMS][0] = params;
    uploadRouteMsg[KF_STR_MSGBODY][0] = bodys;

    Json::StreamWriterBuilder b;
    this->szData = Json::writeString(b, uploadRouteMsg);

    snprintf(this->szDataString, sizeof(this->szDataString), "%s", this->szData.c_str());

    rtdebug("%s",this->szDataString);

    rtdebug("exit generate traffic  data ");

    return 0;

}



int KFDataParseTraffic::KFDataParseTrafficResult(const char *result, int len) {
    if (!result) return -1;
    char data[4048] = {0};
    // check if has common error
    strncpy(data, result, len);
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

    if (json_result[KF_STR_MSGBODY][KF_STR_RESULT][KF_STR_RESULTMSG] != Json::nullValue) {

        std::string resultMsg = json_result[KF_STR_MSGBODY][KF_STR_RESULT][KF_STR_RESULTMSG].asString();

        rtdebug(" KFDataParseTrafficResult resultMsg is : %s", resultMsg.c_str());

    }

    return 0;
}