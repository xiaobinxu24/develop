//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#include <rt_print.h>
#include <unistd.h>
#include "kf_process_queryip.h"
#include "kf_process.h"

KFProcessQueryIP::KFProcessQueryIP() {
}

KFProcessQueryIP::~KFProcessQueryIP() {

}

int KFProcessQueryIP::DoQueryIP(KFDataParseQueryIP *dp, KFNetwork *net, KFConfig *conf, KFSysOperation *sysop) {
    if (!dp || !net || !sysop) return -1;
    rtdebug("Enter process queryip ...");

    // TODO: don't forget set it
    int ret = dp->KFDataParseQueryIPString(sysop->MacGet(), dp->KFErrorAlways(), false);
    dp->KFSetErrorAlways(false);
    if (ret < 0) {
        rterror("query ip failed %d", ret);
        sleep(1);
    } else {
        char *data = dp->KFDataParseDataString();
        if (data) {
            //int ret = net->KFNetworkRequest(data, 10);
            char outData[2048] = {0};
            int  iDataLen = 0;
            ret = net->KFNetworkSendRequest(data,10,outData,&iDataLen);
            if (ret != 200) {
                rterror("error query ip %d, [%s]", ret, net->KFNetworkError());
                ret = -2;
                goto out;
            } else {
                rtdebug("result data: %s, %d", outData, iDataLen);
                int ret = dp->KFDataParseQueryIPResult(outData, iDataLen);
                if (ret >= 0) {
                    // Save ips
                    conf->KFSaveConfigQueryIPProperties(dp->KFDataParseIPS(), dp->KFDataParseIPCounts());
                    rtinfo("Queryip response result success");
                } else {
                    rterror("Parse result head failed %d", ret);
                    rterror("result fail msg : %s", dp->KFDataParseCmdErrorMsg());
                    ret = -3;
                    goto out;
                }
            }
        }
    }
    out:
    rtdebug("Exit process queryip ...");

    return 0;
}