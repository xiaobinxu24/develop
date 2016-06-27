//
// Created by v on 16-3-16.
// Author: jmdvirus@roamter.com
//

#include <unistd.h>
#include <rt_print.h>
#include <cmath>
#include <stdlib.h>
#include <errno.h>
#include <curses.h>
#include "kf_process.h"
#include "kf_data_parse.h"
#include "kf_sys_operation.h"
#include "main.h"
#include "kf_config.h"
#include "unixsocket/rt_unixsocket.h"
#include "kf_msg_data_parse.h"


//static RTUnixSocketServerHandle  serH;

#define KF_PROCESS_COMMAND_WAIT_TIME_ERROR   30  // Wait time when command send fail as network error

KFProcess::KFProcess() {
    this->conf = new KFConfig();
    this->conf->KFConfigInit();

    KFRemoteServerInfo *si = this->conf->KFConfigRemoteServerInfo();
    if (!si) {
        rterror("This can't be happend");
        return;
    }

    this->sysOp = new KFSysOperation();

    this->net = new KFNetwork(si->szHost, si->iPort,
                              si->szUrl, this->conf->KFConfigTimeout());

    this->stCheckNetAvailable  = new checkNetworkAvailable();

    this->dpQueryIP = new KFDataParseQueryIP();
    // TODO: don't forget to set it
    this->dpPingUpload = new KFDataParsePingUpload(this->sysOp->MacGet(), "", "");
    this->dpTraffic = new KFDataParseTraffic();
    this->dpOnlineNotify = new KFDataParseOnlineNotify();
    this->dpBind = new KFDataParseBind();
    this->dpError = new KFDataParseError();

    // process
    this->stProcQueryIP = new KFProcessQueryIP();
    this->stProcPingCheck = new KFProcessPingCheck();
    this->stProcPingUpload = new KFProcessPingUpload();
    this->stProcOnlineNotify = new KFProcessOnlineNotify();
    this->stProcBind = new KFProcessBind();
    this->stProcTraffic = new KFProcessTraffic();

    this->iUploadTrafficInterval = 180;  // 3 mins
    this->iWaitNextErrorInterval = 3; // 3 seconds
    this->iWaitNextInterval = 5; // 5 seconds
    this->iWaitBindInterval = 300; // 5 mins
    this->iWaitCheckOutsideInterval = 600; // 10 mins

    this->iCheckOutsideTimeOut  = 30; // 30 seconds

    this->iUploadTrafficIntervalLast = this->UploadTrafficInterval();
    this->iWaitBindIntervalLast = this->iWaitBindInterval;

    //vpn  mode
    this->iVpnMode = this->conf->KFGetVpnMode();
    this->iPreVpnMode = 0;

    this->iSt = KF_ST_NONE;
    this->CycleExit = false;
    this->RunOK = false;
    this->RecvMsg = false;

    this->bExit = false;
	this->bNeedBind = false;
    pthread_mutex_init(&this->mutex,NULL);
}

KFProcess::~KFProcess() {
    delete this->conf;
    delete this->net;
    delete this->dpQueryIP;
    delete this->dpTraffic;
    delete this->dpOnlineNotify;
    delete this->stProcQueryIP;
    delete this->stProcPingCheck;
    delete this->stProcPingUpload;
    delete this->stProcOnlineNotify;
    delete this->stCheckNetAvailable;
    pthread_mutex_destroy(&this->mutex);
}

int KFProcess::KFProcessInit() {

    return 0;
}

int KFProcess::KFProcessSSConfig() {
//    int ret = this->sysOp->KFSysOpSetSS(&this->stSSResultData, this->conf->GetSSFilename());
    int ret = this->conf->KFSaveConfigSSResultData(&this->stSSResultData);
    if (ret < 0) {
        rterror("Set ss filename failed %d", ret);
        return -1;
    }

    return 0;
}

int KFProcess::KFProcessSSRestartCheck() {
    rtinfo("Restart ss, and check outside if ok ....")

    if( KF_ST_VPN_MODE_CLOSE == this->iVpnMode ) {
        return 1;
    }

    system("/tmp/kf/ssshadowsocks.sh 100"); // restart shadowsocks.
        // Check if outside ok.
        // int ret = this->KFProcessCheckOutside();
    int ret = this->KFProcessPthreadCheckOutside();
    return ret;

}

int KFProcess::KFReportWarning(const char *data, const char *level, const char *code) {
    if (!data || !level || !code) return -1;
    KFErrorReport er;
    snprintf(er.szEntity, sizeof(er.szEntity), "%s", this->sysOp->MacGet());
    snprintf(er.szCode, sizeof(er.szCode), "%s", code);
    snprintf(er.szLevel, sizeof(er.szLevel), "%s", level);
    snprintf(er.szMsg, sizeof(er.szMsg), "%s", data);
    int ret = this->stProcError->DoErrorReport(this->dpError, &er, this->net);
    return ret;
}

int KFProcess::KFProcessCheckOutside() {
    rtinfo("================= Start Check outside ===============");
    int outerr = 0;  // how many times restart
    int ret = -1;  // default is fail
    while (true) {
        if (outerr > 1) break;
        int err = 0;  // how many times fail of outside t.co
        bool outok = false;
        while (true) {
            if (err >= 2) break;
//            int ret = this->net->KFNetworkCheckOutside("t.co", 80, "", "");
            int ret = this->KFProcessPthreadCheckOutside();
            if (ret <= 0) {
                // fail
                sleep(1);
                err += 1;
                rterror("Check outside fail %d", err);
                // do again.
                continue;
            }
            rtinfo("Check outside ok err %d, restart counts %d", err, outerr);
            outok = true;
            break;
        }
        if (outok) { ret = 1; rtinfo("outside ok ,exit "); break; } // outside ok

        system("/tmp/kf/ssshadowsocks.sh 100"); // restart shadowsocks.
        sleep(1);  // 1 s for start shadowsocks to ready.
        outerr += 1;
        char str[64] = {0};
        snprintf(str, sizeof(str), "shadowsocks-restart-%d-%d", err, outerr);
        this->KFReportWarning(str, "WARNING", KF_ERROR_OUTSIDE_CODE);
        rterror("Check outside for restart shadowsocks %d", outerr);


    }

    return ret;
}
//////////////////////////////////////////
//20160425
void * do_check_tco(void *args)
{
    pthread_detach(pthread_self());
    KFProcess *proc = (KFProcess*)args;
    int counts = 0;
    int ret = 0;
    while(TRUE)
    {
        if(counts > 2) break;

        ret = proc->network()->KFNetworkCheckOutside("t.co", 80, "", "");
        if(ret < 0)
        {
            rtdebug("t.co error: %d", ret);
            counts+=1;
            continue;
        } else{
            rtdebug("t.co is ok");

            pthread_mutex_lock(&proc->mutex);
            if(proc->iCheckOutsideTimes > 0)
            {
                pthread_mutex_unlock(&proc->mutex);
                pthread_exit(0);
            }
            proc->iCheckOutsideTimes = 1;
            pthread_mutex_unlock(&proc->mutex);

            break;
        }
    }
    rtdebug("t.co check exit");

}

void * do_check_USkfrouter(void *args)
{
    pthread_detach(pthread_self());
    KFProcess *proc = (KFProcess*)args;

    int counts = 0;
    int ret = 0;
    while(TRUE) {

        if(counts > 2) break;

        ret = proc->network()->KFNetworkCheckOutside("us.kfrouter.com", 80, "", "");
        if(ret < 0)
        {
            rtdebug("us error: %d", ret);
            counts+=1;
            continue;
        } else
        {
            rtdebug("us is ok");
            pthread_mutex_lock(&proc->mutex);
            if(proc->iCheckOutsideTimes > 0)
            {
                pthread_mutex_unlock(&proc->mutex);
                pthread_exit(0);
            }
            proc->iCheckOutsideTimes = 2;
            pthread_mutex_unlock(&proc->mutex);
            break;
        }

    }
    rtdebug("us check exit");

}


void * do_check_JPkfrouter(void *args)
{
    pthread_detach(pthread_self());
    KFProcess *proc = (KFProcess*)args;

    int counts = 0;
    int ret = 0;

    while(TRUE) {

        if(counts > 2) break;

        ret = proc->network()->KFNetworkCheckOutside("jp.kfrouter.com", 80, "", "");
        if(ret < 0)
        {
            rtdebug("jp fail :%d", ret);
            counts+=1;
            continue;
        } else
        {
            rtdebug("jp is ok");
            pthread_mutex_lock(&proc->mutex);
            if(proc->iCheckOutsideTimes > 0)
            {
                pthread_mutex_unlock(&proc->mutex);
                pthread_exit(0);
            }
            proc->iCheckOutsideTimes = 3;
            pthread_mutex_unlock(&proc->mutex);
            break;
        }
    }
    rtdebug("jp check exit");
}

void * do_check_SGPkfrouter(void *args)
{
    pthread_detach(pthread_self());
    KFProcess *proc = (KFProcess*)args;
    int counts = 0;
    int ret = 0;

    while(TRUE) {

        if(counts > 2) break;

        ret = proc->network()->KFNetworkCheckOutside("sgp.kfrouter.com", 80, "", "");
        if(ret < 0)
        {
            rtdebug("sgp error: %d", ret);
            counts+=1;
            continue;
        } else
        {
            rtdebug("sgp is ok");
            pthread_mutex_lock(&proc->mutex);
            if(proc->iCheckOutsideTimes > 0)
            {
                pthread_mutex_unlock(&proc->mutex);
                pthread_exit(0);
            }
            proc->iCheckOutsideTimes = 4;
            pthread_mutex_unlock(&proc->mutex);
            break;
        }
    }
    rtdebug("sgp check exit");
}

int KFProcess::KFProcessPthreadCheckOutside() {
    int outerr = 0;  // how many times restart
    int ret = 0;
    pthread_mutex_lock(&this->mutex);
    this->iCheckOutsideTimes = 0;
    pthread_mutex_unlock(&this->mutex);

    rtinfo("Check out start ");
    pthread_t pthreadID[4];

   // while(TRUE) {

     //   if(outerr > 1) break;

        ret = pthread_create(&pthreadID[0], NULL, do_check_tco, this);
        if (ret < 0) {
            rterror("Thread create failed, %d, %s", ret, strerror(errno));
            return -1;
        }
        usleep(100);
        ret = pthread_create(&pthreadID[1], NULL, do_check_USkfrouter, this);
        if (ret < 0) {
            rterror("Thread create failed, %d, %s", ret, strerror(errno));
            return -1;
        }
        usleep(100);

        ret = pthread_create(&pthreadID[2], NULL, do_check_JPkfrouter, this);
        if (ret < 0) {
            rterror("Thread create failed, %d, %s", ret, strerror(errno));
            return -1;
        }
        usleep(100);

        ret = pthread_create(&pthreadID[3], NULL, do_check_SGPkfrouter, this);
        if (ret < 0) {
            rterror("Thread create failed, %d, %s", ret, strerror(errno));
            return -1;
        }

///        pthread_join(pthreadID[0], NULL);
///        pthread_join(pthreadID[1], NULL);
///        pthread_join(pthreadID[2], NULL);
///        pthread_join(pthreadID[3], NULL);

#if 0
        if( 0 ==  this->iCheckOutsideTimes ){
            outerr+=1;
            system("/tmp/kf/ssshadowsocks 100"); // restart shadowsocks.
            sleep(1);  // 1 s for start shadowsocks to ready.

        } else if(this->iCheckOutsideTimes > 0) {

            this->iCheckOutsideTimes = 0;
            return 0;

        }
#else

        int  lasttime = 0;
        for (;;) {
            ret = this->KFTimeUp(&lasttime, this->iCheckOutsideTimeOut);
            if(0  ==  ret){
                break;
            }

            pthread_mutex_lock(&this->mutex);
            if (this->iCheckOutsideTimes > 0)
            {
                pthread_mutex_unlock(&this->mutex);
                usleep(500);
                break;
            }
            pthread_mutex_unlock(&this->mutex);

            usleep(50000);
        }

        rtdebug("KFProcessPthreadCheckOutside   outerr = [%d]   iCheckOutsideTimes = %d", outerr,this->iCheckOutsideTimes);


    pthread_mutex_lock(&this->mutex);
    if (this->iCheckOutsideTimes > 0) {
        rtinfo("Some one success number: %d", this->iCheckOutsideTimes);
        this->iCheckOutsideTimes = 0;
        pthread_mutex_unlock(&this->mutex);
        return 1;
    }
    else
    {
        rtinfo("All of them failed*******");
        this->iCheckOutsideTimes = 0;
        pthread_mutex_unlock(&this->mutex);

        pthread_cancel(pthreadID[0]);
        usleep(500000);
        pthread_cancel(pthreadID[1]);
        usleep(500000);
        pthread_cancel(pthreadID[2]);
        usleep(500000);
        pthread_cancel(pthreadID[3]);

        return 0;
    }
#endif
    rtinfo("Check outside end");

    //}

    return -2;
}
//////////////////////////////////////////
int KFProcess::KFTimeUp(int *lasttime, int interval) {
    time_t now = time(NULL);
    // Clear if time is 0
    if (*lasttime == 0) {
        *lasttime = now;
        return -1;
    }
    if (now - *lasttime > interval) {
        *lasttime = now;
        return 0;
    }
    return -2;
}


int  KFProcess::KFProcessDoControlMsg()
{
    if( !this->ControlMsg()->cmd || !this->ControlMsg()->args ) return -1;

    const  char *networkoption=  "NetworkOptimization";
    const  char *switchData  = "OutsideControl";

    if( strlen(this->ControlMsg()->cmd) >= strlen(networkoption) ) {

        if (0 == strncmp(networkoption, this->ControlMsg()->cmd, strlen(this->ControlMsg()->cmd) )) {
            bool flag = false;
            this->conf->KFSaveConfigIPPropertiesWithFlag(this->curIPS, flag);
            this->conf->KFGetConfigIPProperties(this->curIPS);
            this->iSt = KF_ST_QUERYIP;
            rtinfo(" NetworkOptimization iSt=%d   iPreSt=%d  iNextSt=%d", this->iSt,this->iPreSt,this->iNextSt);

        }
    }

    if( strlen(this->ControlMsg()->cmd) >= strlen(switchData) ) {

        if (0 == strncmp(switchData, this->ControlMsg()->cmd, strlen(this->ControlMsg()->cmd))) {

               char data[64] ={0};
               int ret = sscanf(this->ControlMsg()->args,"%*[^1-9]%s",data);
               if(ret < 0){
                   return  -2;
               }

               this->iPreVpnMode = this->iVpnMode;
               this->iVpnMode = atoi(data);

               if( (KF_ST_VPN_MODE_CLOSE != this->iVpnMode)  && ( KF_ST_VPN_MODE_CLOSE == this->iPreVpnMode) ) {
                    //close  to  open
                    rtinfo(" close  to  open  %d %d", this->iPreVpnMode,this->iVpnMode);
                    this->iSt = KF_ST_UNINIT;
                    rtinfo(" OutsideControl iSt=%d   iPreSt=%d  iNextSt=%d", this->iSt,this->iPreSt,this->iNextSt);


               } else if(( KF_ST_VPN_MODE_CLOSE == this->iVpnMode) && ( KF_ST_VPN_MODE_CLOSE != this->iPreVpnMode)) {
                    /*
                       char str[128] = {0};
                       struct tm *p;
                       time_t timep;
                       time(&timep);
                       p = gmtime(&timep);
                       snprintf(str, sizeof(str), "%d-%d-%d-%d-%d-%d-UserDisableOutside",(1900 + p->tm_year),(1+p->tm_mon),p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
                       //SendNotify
                       this->KFReportWarning(str, "WARNING","VPNSwitch");*/

                    // open  to close
                    rtinfo(" open  to  close  %d %d", this->iPreVpnMode,this->iVpnMode);
                    this->iSt = KF_ST_ONLINENOTIFY;
                    this->iNextSt = KF_ST_UPLOAD_TRAFFIC;
                    rtinfo(" OutsideControl iSt=%d   iPreSt=%d  iNextSt=%d", this->iSt,this->iPreSt,this->iNextSt);

               }
        }
    }

    return 0;
}


void *do_recv_msg(void *args) {

    KFProcess *proc = (KFProcess*)args;
    if (!proc) {
        rterror("No, process cycle thread failed");
        return NULL;
    }

    RTUnixSocketServerHandle h;
    int ret = rt_unixsocket_server_init(&h, "/tmp/kfmng.sock");
    if (ret < 0) {
        rterror("init failed: %d", ret);
        return NULL;
    }
    rtdebug("start to accept client.");

    while (proc->RecvMsg) {
        int client = rt_unixsocket_server_accept(h);
        int bread = 1;
        rtdebug("start to read data from client.");
        while (bread) {
            char data[1024] = {0};
            int len = 1024;
            int n = rt_unixsocket_server_recv(h, client, data, &len);
            rtinfo("recv data: %d, [%s]", n, data);
            if (n <= 0) {
                rt_unixsocket_server_client_close(client);
                bread = 0;
                break;
            }
            recvMsgDataParse(data,proc->ControlMsg());
            proc->KFProcessDoControlMsg();
            usleep(5000);
        }
        usleep(500000);
    }

    rt_unixsocket_server_deinit(&h);

    rtinfo("exit program");
    return 0;

}

void *do_process_cycle(void *args) {
    KFProcess *proc = (KFProcess*)args;
    if (!proc) {
        rterror("No, process cycle thread failed.");
        return NULL;
    }
    rtinfo("Process cycle thread start ...");
    while(!proc->CycleExit) {
        int ret = 0;
        if(KF_ST_VPN_MODE_CLOSE  !=  proc->VpnMode()) {

            int waitBindLast = proc->WaitBindLast();
            ret = proc->KFTimeUp(&waitBindLast, proc->iWaitBindIntervalLast);
            if (ret == 0) {
                proc->WaitBindLastSet(waitBindLast);
                KFBindJudge bj;
                // TODO: don't forget set it
                snprintf(bj.szMac, sizeof(bj.szMac), "%s", proc->SysOperation()->MacGet());
                ret = proc->ProcessBind()->DoBind(proc->DataParseBind(), &bj, proc->network());
                if (ret == 1) {
                    bool flag = false;
                    proc->Config()->KFSaveConfigIPPropertiesWithFlag(proc->DataParseIPProperties(), flag);
                    proc->Config()->KFGetConfigIPProperties(proc->DataParseIPProperties());
                    // Goto bind, If need bind, goto query ip effectively.
					proc->bNeedBind = true;
                    //proc->StatusSet(KF_ST_QUERYIP);
                    continue;
                } else if (ret == -3) {
                    // Send fail of network
                    proc->iWaitBindIntervalLast = KF_PROCESS_COMMAND_WAIT_TIME_ERROR;
                } else {
                    proc->iWaitBindIntervalLast = proc->WaitBindInterval();
                }
            } else if (ret == -1) {
                proc->WaitBindLastSet(waitBindLast);
            }
        }

        if (!proc->RunOK) {
            usleep(500000);
            continue;
        }

        int uploadTrafficLast = proc->UploadTrafficLast();
        ret = proc->KFTimeUp(&uploadTrafficLast, proc->iUploadTrafficIntervalLast);
        if (ret == 0) {
            ret = proc->ProcessTraffic()->UploadTraffic(proc->network(), proc->SysOperation(),
                                                        proc->DataParseTraffic(), proc->Config());
            if (ret >= 0) {
                proc->UploadTrafficLastSet(uploadTrafficLast);
                proc->iUploadTrafficIntervalLast = proc->UploadTrafficInterval();
            } else if (ret < 0) {
                proc->UploadTrafficLastSet(uploadTrafficLast);
                proc->iUploadTrafficIntervalLast = KF_PROCESS_COMMAND_WAIT_TIME_ERROR;
            }
        } else if (ret == -1) {
            proc->UploadTrafficLastSet(uploadTrafficLast);
        }

        usleep(500000);
    }
    rtwarn("Should not done here.");
    return NULL;
}

int KFProcess::KFProcessRun() {

    // Check if mac address ok cycle.
    while (true) {
        const char *mac = this->sysOp->MacGet();
        if (strlen(mac) <= 0) {
            sleep(1);
            // If fail need mac init
            this->sysOp->MacInit();
            continue;
        }
        break;
    }

    int ret = pthread_create(&this->cycleThread, NULL, do_process_cycle, this);
    if (ret < 0) {
        rterror("Thread create failed, %d, %s", ret, strerror(errno));
        return -1;
    }

    usleep(100);

    ret = pthread_create(&this->recvMsgThread, NULL, do_recv_msg, this);
    if (ret < 0) {
        rterror("Thread create failed, %d, %s", ret, strerror(errno));
        return -1;
    }
    this->RecvMsg = true;

#if 1  // needn't
    // We must call it when system first start.
    KFBindJudge bj;
    snprintf(bj.szMac, sizeof(bj.szMac), "%s", this->SysOperation()->MacGet());
    ret = this->ProcessBind()->DoBind(this->DataParseBind(), &bj, this->network());
    if (ret == 1) {
        this->conf->KFSaveConfigFlag(false);
        this->iSt = KF_ST_QUERYIP;
    }
#endif

    while(!this->bExit) {
        int ret = 0;
		if (this->bNeedBind) {
            this->iSt = KF_ST_QUERYIP;
            this->conf->KFSaveConfigFlag(false);
			this->bNeedBind = false;
		}
        switch (this->iSt) {
            case KF_ST_NONE:
                this->iSt = KF_ST_UNINIT;
                break;
            case KF_ST_UNINIT:
            rtinfo("current status: %d [UNINIT]", this->iSt);
                if (this->conf->KFConfigHasConfig()) {
                    this->iSt = KF_ST_PINGCHECK;
                    // Read from file out save info of current ips
                    KFDataParseIPProperties *prop = this->conf->KFConfigIPProperties();
                    // Save to queryip for ping
                    this->dpQueryIP->KFSetIPProperties(prop, 1);

                    this->conf->KFGetConfigSSResultData(&this->stSSResultData);
//                    this->conf->KFSaveConfigSSResultData(&this->stSSResultData);
                    // All above just for the PINGCHECK STATUS
                } else {
                    this->iSt = KF_ST_QUERYIP;
                }
                break;
            case KF_ST_QUERYIP:
            rtinfo("current status: %d [QUERYIP]", this->iSt);
                if (this->iErrorQueryIPCounts > 3) {
                    this->iErrorQueryIPCounts = 0;
                    this->dpQueryIP->KFSetErrorAlways(true);
                }
                this->iErrorQueryIPCounts += 1;
                ret = this->stProcQueryIP->DoQueryIP(this->dpQueryIP, this->net, this->conf, this->sysOp);
                if (ret < 0) {

                    this->iPreSt  = this->iSt;
                    this->iSt = KF_ST_CHECKNETAVAILABLE;
                    this->conf->KFSaveConfigFlag(false);
                    sleep(1);
                    break;
                } else {
                    this->iSt = KF_ST_PINGCHECK;
                }
                break;
            case KF_ST_PINGCHECK:
            rtinfo("current status: %d [PINGCHECK]", this->iSt);

                ret = this->stProcPingCheck->MultiThreadDoPingCheck(this->dpQueryIP, this->sysOp, &this->stPingCheckResult, &this->curIPS);
                if (ret >= 0) {  // == 0: false
                    if (this->conf->KFConfigHasConfig()) {
                        this->iSt = KF_ST_CONFIG;
                        break;
                    }
                    this->iSt = KF_ST_PINGUPLOAD;
                    // Save current available ip
//                    this->conf->KFSaveConfigIPProperties(this->curIPS);
                } else {
                    // Fail, need requery.
                    this->iSt = KF_ST_QUERYIP;
                }
                break;
            case KF_ST_PINGUPLOAD: {
                rtinfo("current status: %d [PINGUPLOAD] cnts [%d]", this->iSt,
                        this->dpPingUpload->GetPingUploadCounts());
                KFTrafficInfo ti;  // Don't clear it as we just not use it.
                this->sysOp->KFGetWanInfo(&ti);
                this->dpPingUpload->SetInfo(this->sysOp->MacGet(), this->sysOp->WanIP());
                ret = this->stProcPingUpload->DoPingUpload(this->dpPingUpload, this->net, &this->stSSResultData,
                                                           this->curIPS, &this->stPingCheckResult);

                if( -3 == ret)
                {
                    this->iPreSt  = this->iSt;
                    this->iSt = KF_ST_CHECKNETAVAILABLE;
                    break;
                }
                // -11 means server ip can't usable, requery ip for another one.
                if (ret == -11) {
                    this->iSt = KF_ST_QUERYIP;
                    break;
                } // Unsable, Port can't be use.
                if (ret == 0) {
                    this->dpPingUpload->ClearPingUploadCounts();
                    this->iSt = KF_ST_CONFIG;
                } else {
                    rtdebug("error ret %d", ret);
                    this->dpPingUpload->SetPingUploadCounts();
                    if (this->dpPingUpload->GetPingUploadCounts() >= this->conf->PingUploadCountsMaxGet()) {
                        this->iSt = KF_ST_QUERYIP;
                    }
                }
            }
                break;
            case KF_ST_CONFIG:
            rtinfo("current status: %d [CONFIG]", this->iSt);
                if (this->conf->KFConfigHasConfig()) {
                    rtinfo("Has config ");
                    ret = 0;
                } else {
                    rtinfo("didn't config , do save");
                    ret = this->KFProcessSSConfig();
                    this->conf->KFSaveConfigIPProperties(this->curIPS);
                }
                if (ret < 0) {
                    // ? how to do
                } else {
                    ret = this->KFProcessSSRestartCheck();
                    if (ret <= 0) {
                        rtinfo("Check fail, need re-queryip %d", ret);
                        this->iPreSt  = this->iSt;
                        this->iSt = KF_ST_CHECKNETAVAILABLE;
                       // this->iSt = KF_ST_QUERYIP;  // fail requery
                        break;
                    }
                    this->iSt = KF_ST_ONLINENOTIFY;
                }
                break;
            case KF_ST_DEFAULTCONFIG:
            rtinfo("current status: %d [DEFAULTCONFIG]", this->iSt);
                // Just restart shadowsocks and check if outside ok
                // If all fail, goto queryip
                // If success, goto online notify
                ret = this->KFProcessSSRestartCheck();
                if (ret <= 0) {

                    this->iPreSt  = this->iSt;
                    this->iSt = KF_ST_CHECKNETAVAILABLE;
                  //  this->iSt = KF_ST_QUERYIP;  // fail requery
                    break;
                }
                this->iSt = KF_ST_ONLINENOTIFY;
                break;
            case KF_ST_ONLINENOTIFY: {
                rtinfo("current status: %d [ONLINENOTIFY]", this->iSt);
                // If online notify, goto done
                KFOnlineNotify on;
                // TODO: don't forget set it
                sprintf(on.szMac, "%s", this->sysOp->MacGet());
                ret = this->stProcOnlineNotify->DoOnlineNotify(this->dpOnlineNotify, &on, this->net);
                if (ret < 0) {
                    rterror("online notify failed [%d]", ret);
                    // fail, need re online and check fail code
                    const char *error = this->dpOnlineNotify->KFDataParseResultCode().c_str();
                    rterror("result error info : [%s]", error);
                    if (strcmp(error, KF_PORTRELEASE) == 0 || strcmp(error, KF_MACINACTIVE) == 0) {
                        this->iSt = KF_ST_QUERYIP;
                        this->conf->KFSaveConfigFlag(false);
                        break;
                    }

                    this->iPreSt  = this->iSt;
                    this->iSt = KF_ST_CHECKNETAVAILABLE;

                    //this->iSt = KF_ST_WAIT_NEXT;
                    //this->iNextSt = KF_ST_ONLINENOTIFY;
                } else {
                    this->iSt = KF_ST_DONE;
                }
                break;
            }
            case KF_ST_DONE:
            rtinfo("current status: %d [DONE]", this->iSt);
                // If queryip result can't be used, it will never come here.
                this->iErrorQueryIPCounts = 0;  // Clear queryip counts
                // Send command for clean traffic
                ret = this->stProcTraffic->ClearTraffic(this->net, this->sysOp, this->dpTraffic, this->conf);
                if (ret < 0) {
                    rterror("clear traffic failed %d", ret);
                    this->iPreSt  = this->iSt;
                    this->iSt = KF_ST_CHECKNETAVAILABLE;

                //    this->iSt = KF_ST_WAIT_NEXT;
                 //   this->iNextSt = KF_ST_DONE;  // for clear traffic again until success

                } else {
                    this->iSt = KF_ST_WAIT_CYCLE; // goto cycle, do anything other
                    this->RunOK = true; // Set this to true for the thread to check upload traffic.
                }
                break;
            case KF_ST_UPLOAD_TRAFFIC:
                rtwarn("should be here for : %d [UPLOAD TRAFFIC]", this->iSt);
                // upload traffic
                ret = this->stProcTraffic->UploadTraffic(this->net, this->sysOp, this->dpTraffic, this->conf);
                if (ret < 0) {
                    rterror("upload traffic failed %d", ret);
                    this->iPreSt  = this->iSt;
                    this->iSt = KF_ST_CHECKNETAVAILABLE;

                  //  this->iSt = KF_ST_WAIT_NEXT;
                  //  this->iNextSt = KF_ST_UPLOAD_TRAFFIC;  // upload again, until success
                } else {
                    this->iSt = KF_ST_WAIT_CYCLE;
                }
                break;
            case KF_ST_WAIT_NEXT:
                // As time goto upload traffic
                ret = this->KFTimeUp(&this->iWaitNextLast, this->iWaitNextInterval);
                if (ret == 0) {
                    this->iSt = this->iNextSt;
                    this->iNextSt = KF_ST_NONE;
                }
                break;
            case KF_ST_WAIT_NEXT_ERROR:
                ret = this->KFTimeUp(&this->iWaitNextErrorLast, this->iWaitNextErrorInterval);
                if (ret == 0) {
                    this->iSt = this->iNextSt;
                    this->iNextSt = KF_ST_NONE;
                }
                break;
            case KF_ST_WAIT_CYCLE:
                ret = this->KFTimeUp(&this->iWaitCheckOutsideLast, this->iWaitCheckOutsideInterval);
                if (ret == 0) {
                    this->iSt = KF_ST_CHECKOUTSIDE;
                }
                /*
                 * We needn't it, as we put them to thread. Anytime exec them ok.
                ret = this->KFTimeUp(&this->iWaitBindLast, this->iWaitBindInterval);
                if (ret) {
                    KFBindJudge bj;
                    // TODO: don't forget set it
                    snprintf(bj.szMac, sizeof(bj.szMac), "%s", "macaddr");
                    ret = this->stProcBind->DoBind(this->dpBind, &bj, this->net);
                    if (ret) {
                        this->iSt = KF_ST_QUERYIP;
                        break;   // Just to to requery ip
                    }
                }
                ret = this->KFTimeUp(&this->iUploadTrafficLast, this->iUploadTrafficInterval);
                if (ret) {
                    this->iSt = KF_ST_UPLOAD_TRAFFIC;
                    this->iNextSt = KF_ST_NONE;
                }
                 */
                break;
            case KF_ST_CHECKOUTSIDE:
                rtdebug("Goto check outside : %d [CHECKOUTSIDE]", this->iSt);
                // If fail, goto queryip
                // If success, goto wait next
                if(KF_ST_VPN_MODE_CLOSE == this->iVpnMode)
                {
                    this->iSt = KF_ST_WAIT_CYCLE;
                    break;
                }
                ret = this->KFProcessCheckOutside();
                if (ret < 0) {
                    rterror("Sorry, check outside failed %d", ret);
                    if(this->iErrorCheckOutsideCounts > 2 ){
                        this->iSt = KF_ST_QUERYIP;
                        this->iErrorCheckOutsideCounts = 0;
                    }
                    this->iErrorCheckOutsideCounts +=1;

                    this->iPreSt  = this->iSt;
                    this->iSt = KF_ST_CHECKNETAVAILABLE;
                    // fail, need requeryip
                   // this->iSt = KF_ST_QUERYIP;
                } else {
                    this->iErrorCheckOutsideCounts = 0;
                    this->iSt = KF_ST_WAIT_CYCLE;
                }

                break;
            case KF_ST_CHECKNETAVAILABLE:
                rtdebug("Goto check network available : %d [CHECKNETAVAILABLE]", this->iSt);

                for(;;)
                {
                        ret = this->stCheckNetAvailable->getNetworkAvailabel();
                        if(ret >= 0)
                        {
                            break;
                        }
                        sleep(2);
                }

                this->iSt   = this->iPreSt;
                this->iPreSt = KF_ST_NONE;

                break;
        }

        usleep(800000);
    }

    this->RecvMsg = false;
    rtwarn("Should not done here.");
    pthread_join(this->cycleThread, NULL);
    pthread_join(this->recvMsgThread, NULL);

    return 0;
}
