//
// Created by v on 16-3-16.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_CONFIG_H
#define KFROUTER_KF_CONFIG_H

#include <iostream>
#include <string.h>
#include "main.h"

// some config, please see reletive lua pages.
// TODO:
// Save with json string, files put in /etc/kfconfig, (take your filename self, but should write done)
// 1. Server config: KFDataParseIPProperties (main.h) (fives)
// 2. Server config (current use) KFDataParseIPProperties (main.h) only one
// 3. If has config (bool)
// 4. Traffic (save six traffic of KFTrafficInfo (main.h)
// 5. Remote server Info (KFRemoteServerInfo) (readonly)

// 2,3 can be combined together. (one json string)


#define  KF_CONFIG_SHADOWSOCKS             "/etc/shadowsocks/config.json"
#define  KF_CONFIG_REMOTESERVERINFO        "/etc/kf/kfServerConfig.json"
#define  KF_CONFIG_QUERYIPPROPERTIES       "/etc/kfconfig/kfQueryIPConfig.json"
#define  KF_CONFIG_IPPROPERTIES            "/etc/kfconfig/kfIPConfig.json"
#define  KF_CONFIG_TRAFFIC                 "/etc/kfconfig/kfTraffic.json"


typedef struct {
    char               szHost[128];
    int                iPort;
    char               szUrl[64];
} KFRemoteServerInfo;

class KFConfig {
    public:
        KFConfig();
        ~KFConfig();


    int       KFConfigTimeout();

    int       KFConfigInit();

    bool      KFConfigHasConfig();

    KFRemoteServerInfo  * KFConfigRemoteServerInfo();

    KFTrafficInfo       * KFConfigTrafficInfo();

    KFDataParseIPProperties * KFConfigIPProperties();

    int KFSaveConfigQueryIPProperties(KFDataParseIPProperties *prop,int len);

    int KFGetConfigQueryIPProperties(KFDataParseIPProperties *prop,int *len);

    int KFSaveConfigIPProperties(KFDataParseIPProperties *prop);

    int KFSaveConfigIPPropertiesWithFlag(KFDataParseIPProperties *prop, bool flag);

    int KFGetConfigIPProperties(KFDataParseIPProperties *prop);

    int KFSaveConfigTraffic(KFTrafficInfo *ti);

    int KFGetConfigTraffic(KFTrafficInfo *ti);

    int KFGetConfigSSResultData(KFSSResultData *srData);

    int KFSaveConfigSSResultData(KFSSResultData *srData);

    bool KFGetHasConfig(){
        return this->hasConfig;
    }

    KFSSResultData *KFConfigGetSSResultData() {
        return &this->ssResultData;
    }

    bool KFConfigSetSSResultData(KFSSResultData *data) {
        memcpy(&this->ssResultData, data, sizeof(KFSSResultData));
        return true;
    }

    int KFSaveConfigFlag(bool flag);

    const char *GetSSFilename() {
        return this->ssFilename.c_str();
    }

    void PingUploadCountsAdd() {
        this->iPingUploadCounts++;
    }
    int PingUploadCountsGet() {
        return this->iPingUploadCounts;
    }
    void PingUploadCountsClear() {
        this->iPingUploadCounts = 0;
    }
    int PingUploadCountsMaxGet() {
        return this->iPingUploadCountsMax;
    }


    int VpnMode(){
        return  this->iVpnMode;
    }
    int VpnModeSet(int vms){
        return  this->iVpnMode = vms;
    }

    int KFGetVpnMode();

protected:


    int KFGetRemoteServerInfo(KFRemoteServerInfo *remoteServerInfo);

    int KFWriteConfigFile(const char *filename,const char *data,int len);

    int KFReadConfigFile(const char *filename,char *data,int len);

    int KFRemoteServerInfoInit();


private:

    KFDataParseIPProperties     ips;
    KFTrafficInfo               trafficInfo;
    KFRemoteServerInfo          remoteServerInfo;

    int            iVpnMode;

    int             iTimeout;
    bool            hasConfig;
    KFSSResultData   ssResultData;

    int             iPingUploadCounts;
    int             iPingUploadCountsMax;

    std::string     ssFilename;
};

#endif //KFROUTER_KF_CONFIG_H
