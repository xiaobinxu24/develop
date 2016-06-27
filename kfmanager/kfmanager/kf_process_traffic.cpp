//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#include <string.h>
#include <unistd.h>
#include <rt_print.h>
#include "kf_process_traffic.h"
#include "main.h"
#include "kf_config.h"


KFProcessTraffic::KFProcessTraffic() {

}

KFProcessTraffic::~KFProcessTraffic() {

}

int KFProcessTraffic::ClearTraffic(KFNetwork *net, KFSysOperation *sysOp, KFDataParseTraffic *traffic, KFConfig *conf) {
    if (!net || !sysOp || !traffic || !conf) return -1;

    sysOp->KFSysOpClearTraffic(&this->stTraffic);
    // Get ss resource ip
    snprintf(this->stTraffic.szResourceIP, sizeof(this->stTraffic.szResourceIP), conf->KFConfigGetSSResultData()->szServerIp);
    this->stTraffic.iResourcePort = conf->KFConfigGetSSResultData()->iServerPort;

    int ret = traffic->KFClearTraffic(&this->stTraffic);
    if (ret < 0) {
        rterror("clear traffic fail %d", ret);
        return -1;
    }
    // Need send and wait repsonse
    const char *data = traffic->GeneratedData();
    if (data) {
        rtdebug("data is : %s",data);
        char outData[2048] = {0};
        int  iDataLen = 0;
        ret = net->KFNetworkSendRequest((char*)data, 5, outData, &iDataLen);
        //ret = net->KFNetworkRequest((char *) data, 10);
        if (ret != 200) {
            rterror("error clear traffic %d, [%s]", ret, outData);
           // rterror("error clear traffic %d, [%s]", ret, net->KFNetworkError());
            ret = -2;
            goto out;
        }
        else{
            rtdebug("result data: %s, len: %d", outData, iDataLen);
//            rtdebug("result data: %s", net->KFNetworkBody());
            int ret = traffic->KFDataParseTrafficResult(outData, iDataLen);
            if (ret >= 0) {
                rtinfo("ClearTraffic response result success");
            } else {
                rterror("Parse result head failed %d", ret);
                rterror("result fail msg : %s", traffic->KFDataParseCmdErrorMsg());
                ret = -3;
                goto out;
            }
        }
    }

    out:
    rtdebug("Exit process ClearTraffic ...");

    return ret;
}

int KFProcessTraffic::UploadTraffic(KFNetwork *net, KFSysOperation *sysOp, KFDataParseTraffic *traffic, KFConfig *conf) {
    if (!net || !sysOp) return -1;
    rtinfo("=========== Start for upload traffic ==============");

    memset(&this->stTraffic,0,sizeof(this->stTraffic));
/////

    conf->KFGetConfigTraffic(&this->stTraffic);

////
    sysOp->KFSysOpGetTrafficInfo(&this->stTraffic, &conf->KFConfigTrafficInfo()->lPrevTraffic, &conf->KFConfigTrafficInfo()->lPrevTotalForeignTraffic);
    snprintf(this->stTraffic.szResourceIP, sizeof(this->stTraffic.szResourceIP), conf->KFConfigGetSSResultData()->szServerIp);
    this->stTraffic.iResourcePort = conf->KFConfigGetSSResultData()->iServerPort;

    int ret = traffic->KFGenerateTraffic(&this->stTraffic);
    if (ret < 0) {
        rterror("generate traffic fail %d", ret);
        return -1;
    }
    // Send traffic
    const char *data = traffic->GeneratedData();
    if (data) {
        rtdebug("data is : %s",data);
        char outData[2048] = {0};
        int  iDataLen = 0;
        ret = net->KFNetworkSendRequest((char*)data, 5, outData, &iDataLen);
//        ret = net->KFNetworkRequest((char *) data, 5);
        if (ret != 200) {
            rterror("error upload traffic %d, [%s]", ret, outData);
            ret = -2;
            goto out;
        }
        else {

            rtdebug("result data: %s, len: %d", outData, iDataLen);

            ret = traffic->KFDataParseTrafficResult(outData, iDataLen);
            if (ret >= 0) {
                // Save traffic
                conf->KFSaveConfigTraffic(&this->stTraffic);
                rtinfo("UploadTraffic response result success %d", ret);
            } else {
                rterror("Parse result head failed %d", ret);
                rterror("result fail msg : %s", traffic->KFDataParseCmdErrorMsg());
                ret = -3;
                goto out;
            }


        }
    }
    // return success/fail

    out:
    rtdebug("Exit process UploadTraffic ...");

    return ret;
}
