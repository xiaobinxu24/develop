//
// Created by v on 16-3-25.
// Author: jmdvirus@roamter.com
//

#include <stdio.h>
#include <rt_print.h>
#include <stdlib.h>
#include <string.h>
#include "kf_data_parse_pingupload.h"
#include "main.h"

KFDataParsePingUpload::KFDataParsePingUpload(const char *id, const char *wanip, const char *resourceip) {
    this->szMac = id;
    this->szWanIP = wanip;
    this->szResourceIP = resourceip;
}

KFDataParsePingUpload::~KFDataParsePingUpload() {

}

int KFDataParsePingUpload::KFDataParseGenerateData(KFDataParseIPProperties *ips, KFPingCheckResult *result) {
    if (!ips || !result) return -1;

    Json::Value pingUpload;
    this->KFDataParseGenerateHead(&pingUpload, KF_STR_PINGUPLOAD, KF_STR_SYSID);
    Json::Value bodys;
    bodys[KF_STR_PARAMS][KF_STR_MAC] = this->szMac;
    bodys[KF_STR_PARAMS][KF_STR_WANIP] = this->szWanIP;
    bodys[KF_STR_PARAMS][KF_STR_RESOURCEIP] = ips->szIP;
    char avg[16] = {0};
    snprintf(avg, sizeof(avg), "%.03f", result->avg);
    bodys[KF_STR_PARAMS][KF_STR_PINGVAL] = avg;
    char pkgloss[16] = {0};
    snprintf(pkgloss, sizeof(pkgloss), "%.03f", result->pkgloss);
    bodys[KF_STR_PARAMS][KF_STR_PKGLOSSRATE] = pkgloss;
    pingUpload["MsgBody"][0] = bodys;

    Json::StreamWriterBuilder b;
    this->szGenData = Json::writeString(b, pingUpload);

    return 0;
}

int KFDataParsePingUpload::KFDataParseResult(KFSSResultData *rdata, const char *str, int len) {
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

    Json::Value jData = json_result[KF_STR_MSGBODY][KF_STR_RESULT];
    if (jData[KF_STR_RESULTDATA] != Json::nullValue) {
        rtdebug("Start to parse resultData of Pingupload");
        Json::Value json_rdata = jData[KF_STR_RESULTDATA];
        Json::Value ret = json_rdata[KF_STR_LOCALPORT];
        if (ret != Json::nullValue) {
            rdata->iLocalPort = atoi(ret.asString().c_str());
        }
        ret = json_rdata[KF_STR_METHOD];
        if (ret != Json::nullValue) {
            snprintf(rdata->szMethod, sizeof(rdata->szMethod), "%s", ret.asString().c_str());
        }
        ret = json_rdata[KF_STR_PASSWORD];
        if (ret != Json::nullValue) {
            snprintf(rdata->szPassword, sizeof(rdata->szPassword), "%s", ret.asString().c_str());
        }
        ret = json_rdata[KF_STR_SERVER];
        if (ret != Json::nullValue) {
            snprintf(rdata->szServerIp, sizeof(rdata->szServerIp), "%s", ret.asString().c_str());
        }
        ret = json_rdata[KF_STR_SERVERPORT];
        if (ret != Json::nullValue) {
            rdata->iServerPort = atoi(ret.asString().c_str());
        }
    }
    rtdebug("Get ping upload result: server[%s], port [%d], method[%s] localport [%d]",
            rdata->szServerIp, rdata->iServerPort, rdata->szMethod, rdata->iLocalPort);

    return 0;
}