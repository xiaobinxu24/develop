//
// Created by v on 16-3-29.
// Author: jmdvirus@roamter.com
//
#include <stdio.h>
#include <rt_print.h>
#include <string.h>
#include "kf_process_error.h"


KFProcessError::KFProcessError() {

}

KFProcessError::~KFProcessError() {

}

int KFProcessError::DoErrorReport(KFDataParseError *dpe, KFErrorReport *er, KFNetwork *net) {
    if(!er || !net) return -1;
    rtinfo("Please Enter the error");
    int ret = dpe->GenerateData(er);
    if (ret < 0) return -2;
    const char *data = dpe->GeneratedData();
    if (data)
    {
        rtdebug("data is : %s", data);
        char outData[2048] = {0};
        int  iDataLen = 0;
       // ret = net->KFNetworkRequest((char*)data, 0);
        ret = net->KFNetworkSendRequest((char*)data, 0,outData,&iDataLen);
        if (ret != 200)
        {
            //rterror("error  report [%s]", net->KFNetworkError());
            rterror("error  [%s] ret: [%d]", outData, ret);
            rtdebug("the ret is %d", ret);
            return -3;
        }
        else
        {
            rtdebug("result data: %s, %d", outData, iDataLen);
            ret = dpe->KFDataParseResult(outData, iDataLen);
            if (ret < 0)
            {
                char *errorCode = dpe->KFDataParseCmdErrorCode();
                const char *resultCode = dpe->KFDataParseResultCode().c_str();
                rterror("parse error report failed %d , errorcode [%s] resultCode [%s]", ret, errorCode, resultCode);
                return -4;
            }
            else
            {
                rtdebug("the result data prase success!");
            }
        }
    }
    return 0;
}
