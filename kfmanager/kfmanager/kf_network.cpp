//
// Created by v on 16-3-15.
// Author: jmdvirus@roamter.com
//

#include <iostream>
#include <string.h>
#include <rt_print.h>
#include "kf_network.h"

using namespace std;

KFNetwork::KFNetwork(char *addr, int port, char *url, int timeout) {
    this->hc= new KFHttpClient();
    this->hc->KFHttpClientSet(addr, port, url, "", timeout);
    this->host = string(addr);
    this->port = port;
    this->url = string(url);
    this->timeout = timeout;
}

KFNetwork::~KFNetwork() {
    delete this->hc;
}

int KFNetwork::KFNetworkRequest(char *data, int timeout) {
    int ret = this->hc->KFHttpClientSend(data);

    return ret;
}

int KFNetwork::KFNetworkSendRequest(const char *data, int timeout, char *retData, int *iCode) {
    if (!data) return -1;
    KFHttpClient *out = new KFHttpClient();
    out->KFHttpClientSet(this->host, this->port, this->url, data, timeout);
    int ret = out->KFHttpClientSend(data);
    if (ret != 200) {
        if (iCode) {
            *iCode = out->KFHttpClientCode();
        }
        if (retData) {
            snprintf(retData, 1024, "%s", out->KFHttpClientError());  // max support 1024
        }
        delete out;
        return -2;
    }
    if (iCode) {
        *iCode = out->KFHttpClientBodyLen();
    }
    if (retData) {
        snprintf(retData, 2048, "%s", out->KFHttpClientBody()); // max support 2048
    }


    delete out;
    return ret;
}

int KFNetwork::KFNetworkCheckOutside(char *addr, int port, char *url, char *data) {
    if (!addr || port < 0 || port > 65535) return -1;
    KFHttpClient *out = new KFHttpClient();
    out->KFHttpClientSet(addr, port, url, data, 5);
    int ret = out->KFHttpClientSend(data);
    if (ret != 200) {
        delete out;
        return ret;
    }
    delete out;

    return 0;
}
