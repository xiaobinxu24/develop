//
// Created by v on 16-3-15.
// Author: jmdvirus@roamter.com
//
#include <time.h>
#include <sys/time.h>
#include <rt_print.h>
#include "kf_data_parse.h"
#include "main.h"

#include "json/json.h"

#include "ngx_md5.h"

#define KF_DATA_PUBLICKEY "09d08113-74b4-4ada-a04e-17358b6d0203"

KFDataParse::KFDataParse() {

}

KFDataParse::~KFDataParse() {

}

static char kfmd5value[32];

int KFDataParse::KFDataParseGenerateMD5(unsigned char *input, int len, char *save) {
    unsigned char output[16];
    kfmd5_ngx(input, len, output);
    kfmd5_ngx_tostring(output, save);
    return 0;
}
/*
int KFDataParse::KFDataParseGenerateHead(Json::Value *ijson, const char *code, const char *id) {
    (*ijson)["MsgHead"]["ServiceCode"] = code;
    (*ijson)["MsgHead"]["SrcSysID"] = id;
    char string[128] = {0};
    sprintf(string, "%s-%s-%ld-%s", code, id, time(NULL), KF_DATA_PUBLICKEY);
    this->KFDataParseGenerateMD5((unsigned char*)string, strlen(string), kfmd5value);
    (*ijson)["MsgHead"]["SrcSysSign"] = kfmd5value;
    char tmstr[32] = {0};

    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t time_ms = (tv.tv_usec / 1000) + (tv.tv_sec * 1000);

    sprintf(tmstr, "%ld", time_ms);
    (*ijson)["MsgHead"]["transactionID"] = tmstr;
    return 0;
}*/
int KFDataParse::KFDataParseGenerateHead(Json::Value *ijson, const char *code, const char *id) {
    (*ijson)["MsgHead"]["ServiceCode"] = code;
    (*ijson)["MsgHead"]["SrcSysID"] = id;
    char string[128] = {0};
    char tmstr[32] = {0};

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long time_ms = (long long)(tv.tv_usec / 1000) + (((long long)tv.tv_sec) * 1000);
    sprintf(tmstr, "%lld", time_ms);
    (*ijson)["MsgHead"]["transactionID"] = tmstr;

    sprintf(string, "%s-%s-%s-%s", code, id, tmstr, KF_DATA_PUBLICKEY);
    this->KFDataParseGenerateMD5((unsigned char*)string, strlen(string), kfmd5value);
    (*ijson)["MsgHead"]["SrcSysSign"] = kfmd5value;

    return 0;
}
// Cmd is not valid.
int KFDataParse::KFDataParseGeneralError(Json::Value json_result) {
    int retr = 0;
    Json::Value ret = json_result;
    if (ret[KF_STR_ERRORCODE] != Json::nullValue) {
        std::string errorCode = ret[KF_STR_ERRORCODE].asString();
        snprintf(this->szCmdErrorCode, sizeof(this->szCmdErrorCode), "%s", errorCode.c_str());
        retr = -1;
    }
    if (ret[KF_STR_ERRORMSG] != Json::nullValue) {
        std::string errorMsg = ret[KF_STR_ERRORMSG].asString();
        snprintf(this->szCmdErrorMsg, sizeof(this->szCmdErrorMsg), "%s", errorMsg.c_str());
    }

    return retr;
}

// Head is valid, check result .
int KFDataParse::KFDataParseResultError(Json::Value json_result) {
    if (json_result[KF_STR_RESULTCODE] != Json::nullValue) {
        this->szResultCode = json_result[KF_STR_RESULTCODE].asString();
        if (this->szResultCode.compare(KF_STR_SUCCESS) != 0) {
            rterror("FAIL CODE is %s", this->szResultCode.c_str());
            if (json_result[KF_STR_RESULTMSG] != Json::nullValue) {
                this->szResultMsg = json_result[KF_STR_RESULTMSG].asString();
            } else {
                rtwarn("Not give result msg when failed");
            }
            return -1;
        } else {
            rtinfo("result success");
        }
    } else {
        rterror("Didn't give result code");
        return -2;
    }
    return 0;
}

void rm_nl(std::string &s, char *ch) {
    for (int p = s.find(ch); p != (int) std::string::npos; p = s.find(ch))
        s.erase(p,1);
}

// Check is head valid
int KFDataParse::KFDataParseHeadValid(Json::Value json_result) {
    Json::Value ret = json_result;
    if (ret["MsgHead"] != Json::nullValue) {
        std::string code = ret["MsgHead"]["ServiceCode"].asString();
        std::string id = ret["MsgHead"]["SrcSysID"].asString();
        std::string sign = ret["MsgHead"]["SrcSysSign"].asString();
        std::string trans = ret["MsgHead"]["transactionID"].asString();
        char checkstr[1024] = {0};
        sprintf(checkstr, "%s-%s-%s-%s", code.c_str(), id.c_str(), trans.c_str(), KF_DATA_PUBLICKEY);
        char checksign[34] = {0};  // really is 32, but we make 2 more, or it will be segment fault. because the upper level use size of 34

        this->KFDataParseGenerateMD5((unsigned char*)checkstr, strlen(checkstr), checksign);
        if (strcmp(sign.c_str(), checksign) == 0) {
            return 0;
        } else {
            rterror("invalid sign [%s][%s]", sign.c_str(), checksign);
        }
    } else {
        rterror("didn't give msg head");
    }

    return -1;
}

///// Get information
