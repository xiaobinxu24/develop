//
// Created by v on 16-3-16.
// Author: jmdvirus@roamter.com
//

#include <rt_print.h>
#include "KFHttp.h"

KFHttpClient::KFHttpClient() {
    this->kfhttp = ghttp_request_new();
}

KFHttpClient::~KFHttpClient() {
    ghttp_request_destroy(this->kfhttp);
}

int KFHttpClient::KFHttpClientSet(std::string host, int port, std::string url, std::string data, int timeout) {
    char aurl[512] = {0};
    sprintf(aurl, "http://%s:%d/%s", host.c_str(), port, url.c_str());
    rtdebug("url: %s", aurl);

    int ret = ghttp_set_uri(this->kfhttp, aurl);
    if (ret < 0) {
        return -1;
    }

    ret = ghttp_set_type(this->kfhttp, ghttp_type_post);
    if (ret < 0) {
        return -2;
    }

    return 0;
}

int KFHttpClient::KFHttpClientSend(std::string data) {
    ghttp_flush_response_buffer(this->kfhttp);
    int ret = ghttp_set_body(this->kfhttp, (char*)data.c_str(), data.length());
    if (ret < 0) {
        return -1;
    }
    rtdebug("send body: %s", data.c_str());
    ret = ghttp_prepare(this->kfhttp);
    if (ret < 0) {
        this->szError = ghttp_get_error(this->kfhttp);
        return -2;
    }
    ret = ghttp_process(this->kfhttp);
    if (ret < 0) {
        this->szError = ghttp_get_error(this->kfhttp);
        return -3;
    }
    this->iCode = ghttp_status_code(this->kfhttp);

    this->szError = ghttp_get_error(this->kfhttp);

    this->iBodyLen = ghttp_get_body_len(this->kfhttp);
    this->szBody = ghttp_get_body(this->kfhttp);

    return this->iCode;
}

int KFHttpClient::KFHttpClientCode() {
    return this->iCode;
}

const char* KFHttpClient::KFHttpClientError() {
    return this->szError;
}

char *KFHttpClient::KFHttpClientBody() {
    return this->szBody;
}
