//
// Created by v on 16-3-15.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_NETWORK_H
#define KFROUTER_KF_NETWORK_H

#include <net/http/KFHttp.h>

class KFNetwork:public KFHttpClient {
public:
    KFNetwork(char *addr, int port, char *url, int timeout);

    ~KFNetwork();

    int KFNetworkRequest(char *data, int timeout);

    int KFNetworkCheckOutside(char *addr, int port, char *url, char *data);

    // Call this must has called KFNetwork, and set addr, port, url and timeout
    // 0 success,
    // others: fail.
    // If success: iCode -- data length, retData -- data content
    //   You must define retData mini 1024 size
    // If fail: iCode -- fail code, retData -- Fail message
    //   You must define retDAta mini 2048 size
    int KFNetworkSendRequest(const char *data, int timeout, char *retData, int *iCode);

    char* KFNetworkBody() {
        return this->hc->KFHttpClientBody();
    };

    const char *KFNetworkError() {
        return this->hc->KFHttpClientError();
    }

    int KFNetworkCode() {
        return this->KFHttpClientCode();
    }

    int KFNetworkBodyLen() {
        return this->hc->KFHttpClientBodyLen();
    }

private:
    KFHttpClient *hc;
    std::string   host;
    int           port;
    std::string   url;
    int           timeout;
};

#endif //KFROUTER_KF_NETWORK_H
