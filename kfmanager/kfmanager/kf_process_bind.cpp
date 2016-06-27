//
// Created by v on 16-3-29.
// Author: zhangtao@roamter.com
//

#include <stdio.h>
#include <stdlib.h>
#include <rt_print.h>
#include <string.h>
#include "kf_process_bind.h"
#include "main.h"

#include "kf_data_parse_pingupload.h"
KFProcessBind::KFProcessBind() {

}

KFProcessBind::~KFProcessBind() {

}

int KFProcessBind::DoBind(KFDataParseBind *dpb, KFBindJudge *bj, KFNetwork *net) 
{
    if (!bj || !net) return -1;
    rtinfo("=========== Start check bind judge ================");
    rtdebug("Enter do bindjudge");
    int ret = dpb->KFDataParseBindGenerateData(bj);
    if (ret < 0) return -2;
    const char *data = dpb->KFDataParseBindGenerateData();
    if (data)
    {
        rtdebug("data is : %s", data);
        char outData[2048] = {0};
        int  iDataLen = 0;
//        ret = net->KFNetworkRequest((char*)data, 0);
        ret = net->KFNetworkSendRequest((char*)data, 0, outData, &iDataLen);
        if (ret != 200)
        {
            rterror("error bindjudge [%s] ret: [%d]", outData, ret);
            return -3;
        }
        else
        {
            rtdebug("result data: %s, %d", outData, iDataLen);

            ret = dpb->KFDataParseResult(outData, iDataLen);
            if (ret < 0)
            {
                char *errorCode = dpb->KFDataParseCmdErrorCode();
                const char *resultCode = dpb->KFDataParseResultCode().c_str();
                rterror("parse bindjudge failed %d , errorcode [%s] resultCode [%s]", ret, errorCode, resultCode);
                return -4;
            }
            else
            {
                std::string result = dpb->BindResult();
                if (result.compare("Y") == 0) {
                    return 1;
                } 
                else if (result.compare("S") == 0) {
                    return -5;
                } // "S"
                else if (result.compare("N") == 0) {
                    return -6;
                } // "N"
            }
        }
    }
    rtdebug("Exit do bindjudge");
    return 0;
}
