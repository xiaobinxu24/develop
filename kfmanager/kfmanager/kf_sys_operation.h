//
// Created by v on 16-3-20.
// Author: zhangtao@roamter.com
//

#ifndef KFROUTER_KF_SYS_OPERATION_H
#define KFROUTER_KF_SYS_OPERATION_H

#include <iostream>

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef __cplusplus
}
#endif

// this file will call lua for some operation.

// TODO:
// 1. /etc/model (just read this file)
// 2. wanip, dns, netmask, gateway (though lua to read)
// 3. /proc/uptime (read this file to parse start time)
// 4. /proc/meminfo (read this file, parse out freemem)
// 5. client numbers (see kfsys.lua get_client_conn_number)
// 6. connect numbers (cat /proc/net/nf_conntrack | wc -l)
// 7. outtraffic (download, iptables -n -v -L -t filter -x | grep delegate_input | head -n 1 | awk '{print $2}')
// 8. intraffic (download, cat /proc/net/dev | grep $extface | tr : " " | awk '{print $2}')
// 9. intraffic (upload, cat /proc/net/dev | grep $extface | tr : " " | awk '{print $10}')

// All above, put to KFTrafficInfo (main.h)
// please see /tmp/kf/sstraffic.sh and /tmp/kf/kfmanager/kfsys.lua

class KFSysOperation {
public:
    KFSysOperation();
    ~KFSysOperation();

    int KFSysOpPingCheck(KFDataParseIPProperties *prop, KFPingCheckResult *result, const char *string);
    //int KFSysOperation::KFSysOpPingCheck(KFDataParseIPProperties *prop, KFPingCheckResult *result, KFProcessPingCheck::args *curargs,int len)
    // > 0 : ok
    int KFSysOpPingAvail(KFDataParseIPProperties *prop, KFPingCheckResult *result);

    int KFSysOpPingDataReset();

    int KFSysOpSetSS(KFSSResultData *data, const char *filename);

    int KFSysOpGetPrevTraffic();
    int KFSysOpGetPrevForeignTraffic();
    char * KFSysOpExecuteCmd(char cmd[1024]);

    int KFSysOpGetTrafficInfo(KFTrafficInfo *result, long long *pt, long long *pft);//仅获取与流量有关的信息

    int KFGetTrafficInfo(long pt, long pft);//总的获取流量信息函数（包含启动时间／ｍａｃ地址／wan口信息等其他信息）
    //int KFSysOpTraffic(KFTrafficInfo *ti);

    int KFSysOpClearTraffic(KFTrafficInfo *ti);

    // deprecated.
    KFPingCheckResult *KFPingCheckResultGet() {
        return &this->stPingCheckResult;
    }

    int KFGetWanInfo(KFTrafficInfo *result);

    bool MacInit();

    const char *MacGet() {
        return this->szMac.c_str();
    }
    const char *ModelGet() {
        return this->szModel.c_str();
    }

    const char *WanIP() {
        return this->szWanIP.c_str();
    }

private:
    int KFSysOpGetMac(KFTrafficInfo *ti);
    int KFSysOpGetModel();
    double KFSysOpString2Double(char *str);
    int KFSysOpGetUptime(KFTrafficInfo *result);
    int KFGetMemInfo(KFTrafficInfo *result);
    int KFGetClientConnNums(KFTrafficInfo *result);
    int KFGetConnNums(KFTrafficInfo *ti);
    int KFSysOpGetDownloadOutTraffic();
    int KFSysOpGetDownloadInTraffic();
    int KFSysOpGetUploadInTraffic();


    lua_State *L;
    std::string   LuaPosition;
    float                 iWeigthDefault;
//    float               iWeigthNow;
    long long                lastin;
    long long                lastout;
    long long                DownloadOutTraffic;
    long long                DownloadInTraffic;
    long long                UploadInTraffic;
    long long                PrevTraffic;
    long long                PrevForeignTraffic;

    KFPingCheckResult   stPingCheckResult;
    //KFDataParseIPProperties prop;
    KFPingCheckResult   result;
    std::string         szMac;
    std::string         szModel;

    std::string         szWanIP;

    //std::string         CmdResult;
};

#endif //KFROUTER_KF_SYS_OPERATION_H
