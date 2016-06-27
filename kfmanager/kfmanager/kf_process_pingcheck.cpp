//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#include <rt_print.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "kf_process_pingcheck.h"
#include "main.h"

KFProcessPingCheck::KFProcessPingCheck() {

    threadFinishflag = 0;
    i = 0;
}

KFProcessPingCheck::~KFProcessPingCheck() {

}

int KFProcessPingCheck::DoPingCheck(KFDataParseQueryIP *dp, KFSysOperation *sys, KFPingCheckResult *result, KFDataParseIPProperties **tips) {
    if (!dp || !sys || !result || !tips) return -1;
    rtdebug("Enter do ping check");

    int ret = 0;
    int cnts = dp->KFDataParseIPCounts();
    KFDataParseIPProperties *ips = dp->KFDataParseIPS();
    float lastWeight = 0;
    bool hasUsable = false;
    rtinfo("how many ips to check [%d]", cnts);
    for (int i = 0; i < cnts; i++) {
        KFDataParseIPProperties *curips = ips +i;
//        char *ip = (ips+i)->szIP;
//        int iport = (ips+i)->iPort;
        // Go to ping check
        KFPingCheckResult stResult;
        memset(&stResult, 0, sizeof(KFPingCheckResult));
        //sys->KFSysOpPingCheck(curips, &stResult);//pingIP
        int ret = sys->KFSysOpPingAvail(curips, &stResult);
        if (ret <= 0) {
            continue; // Ignore this one
        }
        hasUsable = true;
        float newWeight = stResult.iWeightNow;
        if (i > 0) {
//            rtdebug("weight: [%lf-%lf]", newWeight, lastWeight);
            if (newWeight-lastWeight > 0) {
                // Choose current one.
                *result = stResult;
                *tips = curips;
                lastWeight = newWeight;
                rtinfo("has usabled servers [%d]", i);
            }
        } else if (i == 0) {
            lastWeight = newWeight;
            *tips = curips;
            *result = stResult;
        }
    }
    rtdebug("Which one to choose [%s] weight[%lf] avg [%lf] pkgloss [%lf]",
            (*tips)->szIP, (*result).iWeightNow,
            (*result).avg, (*result).pkgloss);

    out:
        rtdebug("Exit do ping check");

    return hasUsable == true;
}

/*
void *MultiThreadPingCheck(void *arg)
{

    struct KFProcessPingCheck::args *curargss;
    rtinfo("start to creat a new thread");
    KFProcessPingCheck *ppc = (KFProcessPingCheck*)arg;
    pthread_mutex_lock(&ppc->mutex);
    curargss =  &ppc->pingarg[ppc->i];
    pthread_mutex_unlock(&ppc->mutex);
    int ret;
    curargss->sys->KFSysOpPingCheck(curargss->dpip, curargss->pcresult);
    ret = curargss->sys->KFSysOpPingAvail(curargss->dpip, curargss->pcresult);
    //pthread_mutex_lock(&ppc->mutex);
    ppc->threadFinishflag = ppc->threadFinishflag + 1;//线程结束标志位加一
    //pthread_mutex_unlock(&ppc->mutex);

    rtinfo("threadFinishflag = %d",ppc->threadFinishflag);
    rtinfo("packagelost = %f",curargss->pcresult->pkgloss);
    rtinfo("finish a thread");
}
*/

typedef struct {
    KFDataParseIPProperties *pc;
    std::string      result;
    int          threadCounts;
} DoArgs;

static DoArgs inArgs[5];

void *MultiThreadPingIP(void *arg)
{
    DoArgs *iArgs = (DoArgs*)arg;
    rtdebug("start to ping ip:[%s]",iArgs->pc->szIP);
    FILE *fp = NULL;
#if MTK
    std::string cmdpart1 = "ping -i 0.001 -c 10 ";
#else
    std::string cmdpart1 = "ping -i 1 -c 3 ";
#endif
    std::string cmd = cmdpart1+iArgs->pc->szIP;
    rtdebug("cmd[%s]", cmd.c_str());
    char result[2048]={0};
    fp = popen(cmd.c_str(), "r");
    if(fp==NULL)
    {
        rterror("execute [%s] error!", cmd.c_str());
        pclose(fp);
        return 0;
    }
    int iresult = 0;
    while (true) {
        if (feof(fp) != 0) break;
        char iret[128] = {0};
        char *ret = fgets(iret, sizeof(iret), fp);
        if (ret != NULL) {
            int iretlen = strlen(iret);
            strncpy(result+iresult, iret, iretlen);
            iresult += iretlen;
        }
        usleep(500);
    }
    rtdebug("result of exe [%s] is [%s]", cmd.c_str(), result);
    iArgs->result = std::string(result);
    iArgs->threadCounts += 1;
    pclose(fp);
    return 0 ;

}
int KFProcessPingCheck::MultiThreadDoPingCheck(KFDataParseQueryIP *dp, KFSysOperation *sys, KFPingCheckResult *result, KFDataParseIPProperties **tips) {
    if (!dp || !sys || !result || !tips) return -1;
    rtdebug("Enter do ping check");

    pthread_mutex_init(&this->mutex,NULL);

    int ret = 0;
    int cnts = dp->KFDataParseIPCounts();
    if(cnts == 0){
        rterror("give no ip");
        return  -2;
    }
    if (cnts > 5) cnts = 5;
    KFDataParseIPProperties *ips = dp->KFDataParseIPS();
    float lastWeight = 0;
    bool hasUsable = false;
    rtdebug("how many ips to check [%d]", cnts);

    KFPingCheckResult stResult[cnts];

    for (int i = 0; i < cnts; i++) {
        rtinfo("start to enter for cycle i = %d", i);
        KFDataParseIPProperties * curips  = ips+i;
        memset( &stResult[i],0,sizeof(stResult[i]));

        this->pingarg[i].dpip = curips;
        this->pingarg[i].sys  = sys;
        this->pingarg[i].pcresult = &stResult[i];

        DoArgs *iArgs = &inArgs[i];
        iArgs->threadCounts = 0;
        iArgs->pc = curips;
        pthread_create(&this->pingcycleThread[i], NULL, MultiThreadPingIP, iArgs);

        usleep(500);
    }
    rtinfo("start to check if all thread finish......");

    while(true)
    {
        int allcnts = 0;
        for (int i = 0; i < cnts; i++) {
            if (inArgs[i].threadCounts > 0) {
                allcnts += 1;
            }
        }
        if (allcnts >= cnts) break;
        sleep(1);
    }

    for (int i=0; i < cnts; i ++ ){
        pthread_detach(this->pingcycleThread[i]);
        sys->KFSysOpPingCheck(this->pingarg[i].dpip, this->pingarg[i].pcresult, inArgs[i].result.c_str());
        sys->KFSysOpPingAvail(this->pingarg[i].dpip, this->pingarg[i].pcresult);
    }
    for (int i=0; i < cnts; i ++ )
    {
        rtinfo("pingarg[%d]->pcresult->pkgloss=%f", i, this->pingarg[i].pcresult->iWeightNow);
    }

    rtinfo("start to cpmpare weight!");
    float lastweight = 0;
    lastweight = this->pingarg[0].pcresult->iWeightNow;
    result = this->pingarg[0].pcresult;
    *tips = this->pingarg[0].dpip;
    rtinfo("first weight = %f", lastweight);

    for (int k=1;k< cnts;k++)
    {
        rtinfo("lastweight [%d] = %f", k,lastweight)
        rtinfo("this->pingarg[%d].pcresult->iWeightNow = %f",k , this->pingarg[k].pcresult->iWeightNow);
        if(this->pingarg[k].pcresult->iWeightNow > lastweight)
        {
            rtinfo("this->pingarg[%d].pcresult->iWeightNow = %f > lastWeight = %f",k,this->pingarg[k].pcresult->iWeightNow,lastWeight);
            lastweight= this->pingarg[k].pcresult->iWeightNow;
            // Choose current one.
            result = this->pingarg[k].pcresult;
            *tips = ips+k;
            rtdebug("has usabled servers [%d]", k);
        }
        hasUsable = true;

    }

    rtdebug("Which one to choose [%s] weight[%lf] avg [%lf] pkgloss [%lf]",
            (*tips)->szIP, (*result).iWeightNow,
            (*result).avg, (*result).pkgloss);
    out:
    rtdebug("Exit do ping check");

    return hasUsable == true;

}
