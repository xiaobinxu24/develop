//
// Created by v on 16-3-18.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_DATA_PARSE_QUERYIP_H
#define KFROUTER_KF_DATA_PARSE_QUERYIP_H

#include "kf_data_parse.h"

#include "main.h"

class KFDataParseQueryIP:public KFDataParse {
public:
    
    KFDataParseQueryIP();
    ~KFDataParseQueryIP();
    
    // Generate query IP string (json)
    int KFDataParseQueryIPString(const char *id, bool errAlways, bool initDevice);
    // Parse result from QueryIP response result
    int KFDataParseQueryIPResult(const char *result, int len);

    int KFSetIPProperties(KFDataParseIPProperties *prop, int counts);

    bool KFErrorAlways() {
        return this->bErrorAlways;
    }
    void KFSetErrorAlways(bool always) {
        this->bErrorAlways = always;
    }

protected:
    int KFDataParseQueryIPResultData(const char *result);

public:
    int KFDataParseIPCounts(){
        return this->iIPCnts;
    }
    
    KFDataParseIPProperties *KFDataParseIPS() {
        return &stIPS[0];
    };
    
    char *KFDataParseCurIP() {
        return this->szCurIP;
    }
    
    int KFDataParseCurPort() {
        return this->iCurPort;
    }

private:
    KFDataParseIPProperties     stIPS[5];
    char         *szCurIP;
    int           iCurPort;

    bool          bErrorAlways;

    int           iIPCnts;
};

#endif //KFROUTER_KF_DATA_PARSE_QUERYIP_H
