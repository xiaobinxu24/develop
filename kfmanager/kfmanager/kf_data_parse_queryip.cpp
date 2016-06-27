//
// Created by v on 16-3-18.
// Author: jmdvirus@roamter.com
//

#include <string.h>
#include <stdlib.h>
#include <rt_print.h>
#include "kf_data_parse_queryip.h"

#include "json/json.h"
#include "main.h"

KFDataParseQueryIP::KFDataParseQueryIP() {
    this->bErrorAlways = false;
}

KFDataParseQueryIP::~KFDataParseQueryIP() {

}

int KFDataParseQueryIP::KFSetIPProperties(KFDataParseIPProperties *prop, int counts) {
    if (!prop || counts < 0) return -1;
    memcpy(this->stIPS, prop, sizeof(KFDataParseIPProperties)*counts);
    this->iIPCnts = counts;
    return 0;
}

int KFDataParseQueryIP::KFDataParseQueryIPString(const char *id, bool errAlways, bool initDevice) {
    Json::Value queryip;
    this->KFDataParseGenerateHead(&queryip, "queryAvailableServerIp", "0001");
    Json::Value bodys;
    bodys["PARAMS"]["mac"] = id;
    if (errAlways) {
        bodys["PARAMS"]["vipFlag"] = "T";
    }
    if (initDevice) {
        bodys["PARAMS"]["type"] = "1";
    } else {
        bodys["PARAMS"]["type"] = "0";
    }
    queryip["MsgBody"][0] = bodys;

    Json::StreamWriterBuilder b;
    std::string dataStr = Json::writeString(b, queryip);
    snprintf(this->szDataString, sizeof(this->szDataString), "%s", dataStr.c_str());
    return 0;
}

int KFDataParseQueryIP::KFDataParseQueryIPResult(const char *result, int len) {
    char data[10240] = {0};
    // check if has common error
    strncpy(data, result, len);
    Json::Reader jrd;
    Json::Value json_result;
    jrd.parse(data, json_result, 0);
    int ret = this->KFDataParseGeneralError(json_result);
    if (ret < 0) {
        // Result is fail. do something.
        return ret;
    }
    ret = this->KFDataParseHeadValid(json_result);
    if (ret < 0) {
        return ret;
    }

    ret = this->KFDataParseResultError(json_result[KF_STR_MSGBODY][KF_STR_RESULT]);
    if (ret < 0) {
        return ret;
    }

    Json::Value jData = json_result[KF_STR_MSGBODY][KF_STR_RESULT];
    if (jData[KF_STR_RESULTDATA] != Json::nullValue) {
        rtdebug("Start to parse resultData");
        Json::Value json_rdata = jData[KF_STR_RESULTDATA];
        KFDataParseIPProperties *prop = &this->stIPS[0];
        this->iIPCnts = 0;
        for(int it = 0; it < json_rdata.size(); ++it) {
            Json::Value rd = json_rdata[it];
            if (rd[KF_STR_BELONGGROUP] != Json::nullValue) {
                prop->iBelongGroup = atoi(rd[KF_STR_BELONGGROUP].asString().c_str());
            }
            Json::Value distance = rd[KF_STR_DISTANCE];
            if (distance != Json::nullValue) {
                snprintf(prop->szDistance, sizeof(prop->szDistance), "%s", distance.asString().c_str());
            }
            Json::Value lat = rd[KF_STR_LATITUDE];
            if (lat != Json::nullValue) {
                prop->fLatitude = atof(lat.asString().c_str());
            }
            Json::Value longi = rd[KF_STR_LONGITUDE];
            if (longi != Json::nullValue) {
                prop->fLongitude = atof(longi.asString().c_str());
            }
            Json::Value sip = rd[KF_STR_SERVERIP];
            if (sip != Json::nullValue) {
                snprintf(prop->szIP, sizeof(prop->szIP), "%s", sip.asString().c_str());
                rtdebug("The ip is [%s]", prop->szIP);
            }
            this->iIPCnts++;
            prop++;
        }
    } else {
        rterror("didn't give result data when success");
        return -2;
    }

    return 0;
}