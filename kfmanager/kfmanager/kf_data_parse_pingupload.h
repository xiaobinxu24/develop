//
// Created by v on 16-3-25.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_DATA_PARSE_PINGUPLOAD_H
#define KFROUTER_KF_DATA_PARSE_PINGUPLOAD_H

#include "kf_data_parse.h"
#include "main.h"

class KFDataParsePingUpload : public KFDataParse {
public:
    KFDataParsePingUpload(const char *id, const char *wanip, const char *resourceip);
    ~KFDataParsePingUpload();

    int KFDataParseGenerateData(KFDataParseIPProperties *ips, KFPingCheckResult *result);


    //0        success
    //-1       NULL input
    //-2       Generral error
    //-3       Head error
    //-4       Result error
    int KFDataParseResult(KFSSResultData *rdata, const char *str, int len);

    int SetInfo(const char *id, const char *wanip) {
        this->szMac = id;
        this->szWanIP = wanip;
        return 0;
    }

    const char *KFDataParseGetData() {
        return this->szGenData.c_str();
    }

    int GetPingUploadCounts() {
        return this->iCounts;
    }
    int SetPingUploadCounts() {
        return this->iCounts ++;
    }
    int ClearPingUploadCounts() {
        this->iCounts = 0;
        return 0;
    }

private:
    int              iCounts;

    // Pointer to the real position of kfconfig
    const char             *szMac;
    const char             *szWanIP;
    const char             *szResourceIP;
};

#endif //KFROUTER_KF_DATA_PARSE_PINGUPLOAD_H
