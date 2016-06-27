//
// Created by v on 16-3-16.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KFHTTP_H
#define KFROUTER_KFHTTP_H

#include <iostream>

extern "C" {
#include "ghttp.h"
};

class KFHttpClient {
private:
        ghttp_request *kfhttp;

private:
        int            iCode;
        const char    *szError;
        char          *szBody;
        int            iBodyLen;

public:
        KFHttpClient();
        ~KFHttpClient();

        int KFHttpClientSet(std::string host, int port, std::string url, std::string data, int timeout);
        int KFHttpClientSend(std::string data);

        int KFHttpClientCode();
        const char *  KFHttpClientError();
        char * KFHttpClientBody();

    int KFHttpClientBodyLen() {
        return this->iBodyLen;
    }
};

#endif //KFROUTER_KFHTTP_H
