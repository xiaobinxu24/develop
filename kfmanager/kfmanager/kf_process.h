//
// Created by v on 16-3-16.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_KF_PROCESS_H
#define KFROUTER_KF_PROCESS_H

#include <pthread.h>
#include "kf_network.h"
#include "kf_config.h"
#include "kf_data_parse.h"
#include "kf_data_parse_queryip.h"
#include "kf_data_parse_pingupload.h"
#include "kf_sys_operation.h"
#include "kf_data_parse_traffic.h"
#include "kf_data_parse_onlinenotify.h"

#include "kf_process_queryip.h"
#include "kf_process_pingcheck.h"
#include "kf_process_pingupload.h"
#include "kf_process_traffic.h"
#include "kf_process_online_notify.h"
#include "kf_data_parse_bind.h"
#include "kf_process_bind.h"
#include "kf_process_error.h"
#include "kf_msg_data_parse.h"
#include "checkNetworkAvailable.h"


enum {
    KF_ST_NONE = 0,
    KF_ST_UNINIT,
    KF_ST_INIT,
    KF_ST_QUERYIP,
    KF_ST_PINGCHECK,
    KF_ST_PINGUPLOAD,
    KF_ST_ONLINENOTIFY,
    KF_ST_DEFAULTCONFIG,
    KF_ST_CONFIG,
    KF_ST_DONE,
    KF_ST_CHECKOUTSIDE,
    KF_ST_WAIT_NEXT,    // some operation need next do again
    KF_ST_WAIT_NEXT_ERROR,  // some error operation need test again
    KF_ST_WAIT_CYCLE,  // upload traffic for cycle
    KF_ST_UPLOAD_TRAFFIC,
    KF_ST_CHECKNETAVAILABLE,
};


enum {
    KF_ST_VPN_MODE_NONE = 0,
    KF_ST_VPN_MODE_CLOSE,
    KF_ST_VPN_MODE_OPEN,
    KF_ST_VPN_MODE_OPENALL,
};

#define KF_PORTRELEASE "FAIL-801"
#define KF_MACINACTIVE "FAIL-018"

class KFProcess {
public:
    KFProcess();
    ~KFProcess();

    int KFProcessInit();

    int KFProcessRun();

    int UploadTrafficInterval() {
        return this->iUploadTrafficInterval;
    }
    int UploadTrafficLast() {
        return this->iUploadTrafficLast;
    }
    void UploadTrafficLastSet(int t) {
        this->iUploadTrafficLast = t;
    }

    int WaitBindInterval() {
        return this->iWaitBindInterval;
    }
    int WaitBindLast() {
        return this->iWaitBindLast;
    }
    int WaitBindLastSet(int t) {
        return this->iWaitBindLast = t;
    }
    int StatusSet(int st) {
        return this->iSt = st;
    }
    int StatusNextSet(int st) {
        return this->iNextSt = st;
    }
    int StatusPreSet(int st) {
        return this->iPreSt = st;
    }

    int WaitOutsideInterval() {
        return this->iWaitCheckOutsideInterval;
    }
    int WaitOutsideLast() {
        return this->iWaitCheckOutsideLast;
    }
    int WaitOusideLastSet(int t) {
        return this->iWaitCheckOutsideLast = t;
    }

    int VpnMode(){
        return  this->iVpnMode;
    }
    int VpnModeSet(int vms){
        return  this->iVpnMode = vms;
    }

    int PreVpnMode(){
        return  this->iPreVpnMode;
    }
    int PreVpnModeSet(int pvms){
        return  this->iPreVpnMode = pvms;
    }

    // return:
    // 0: true
    // -1: first lastTime is 0
    // -2: not timeout
    int KFTimeUp(int *lastTime, int interval);

    KFDataParseBind *DataParseBind() {
        return this->dpBind;
    }

    KFProcessBind *ProcessBind() {
        return this->stProcBind;
    }

    KFNetwork *network() {
        return this->net;
    }

    KFProcessTraffic *ProcessTraffic() {
        return this->stProcTraffic;
    }

    KFDataParseTraffic *DataParseTraffic() {
        return this->dpTraffic;
    }

    KFSysOperation *SysOperation() {
        return this->sysOp;
    }

    KFDataParseIPProperties  *DataParseIPProperties(){
        return this->curIPS;
    }

    KFConfig *Config() {
        return this->conf;
    }

    KFControlMsg *ControlMsg(){
        return  &this->ctlMsg;
    }


    bool          RunOK;  // run ok
    bool          CycleExit;
    bool          RecvMsg;  // recv msg from unixsocket

	bool      bNeedBind;

    int             iCheckOutsideTimes;
    pthread_mutex_t mutex;


    ////////////////
    int KFProcessDoControlMsg();

    int KFProcessPthreadCheckOutside();
    // Sometime, the command may send failed, we need wait some time resend it.
    // We need save the origin interval time, and then reset it after send success.
    int       iUploadTrafficIntervalLast;  // for upload traffic
    int       iWaitBindIntervalLast;       // for bind check interval

private:
    int KFProcessSSConfig();
    int KFProcessCheckOutside();

    int KFProcessSSRestartCheck();

    int KFReportWarning(const char *data, const char *level, const char *code);

private:
    KFNetwork *net;
    KFConfig *conf;

    KFDataParseQueryIP *dpQueryIP;
    KFDataParsePingUpload *dpPingUpload;
    KFDataParseTraffic *dpTraffic;
    KFDataParseOnlineNotify *dpOnlineNotify;
    KFDataParseBind  *dpBind;
    KFDataParseError *dpError;

    KFSysOperation *sysOp;
    checkNetworkAvailable  *stCheckNetAvailable;

    KFDataParseIPProperties  *curIPS;
    KFPingCheckResult     stPingCheckResult;

    KFSSResultData        stSSResultData;
    KFControlMsg     ctlMsg;

    KFProcessQueryIP      *stProcQueryIP;
    KFProcessPingCheck    *stProcPingCheck;
    KFProcessPingUpload   *stProcPingUpload;
    KFProcessTraffic      *stProcTraffic;
    KFProcessOnlineNotify *stProcOnlineNotify;
    KFProcessBind         *stProcBind;
    KFProcessError        *stProcError;

    int       iSt;
    int       iPreSt;
    int       iNextSt;

    bool      bExit;

    int       iUploadTrafficInterval;  // for upload traffic
    int       iWaitNextInterval;       // for command need exec again
    int       iWaitNextErrorInterval;  // for error need exec again
    int       iWaitBindInterval;       // for bind check interval
    int       iWaitCheckOutsideInterval;  // Checkoutside interval

    int       iCheckOutsideTimeOut;

    int       iUploadTrafficLast;
    int       iWaitNextLast;
    int       iWaitNextErrorLast;
    int       iWaitBindLast;
    int       iWaitCheckOutsideLast;

    int       iErrorQueryIPCounts;
    int       iErrorCheckOutsideCounts;

    int       iVpnMode;
    int       iPreVpnMode;

    pthread_t   cycleThread;
    pthread_t   recvMsgThread;

};

#endif //KFROUTER_KF_PROCESS_H
