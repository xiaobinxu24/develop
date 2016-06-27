//
// Created by v on 16-3-29.
// Author: zhangtao@roamter.com
//

#include <stdio.h>
#include <rt_print.h>
#include "kf_process_online_notify.h"

KFProcessOnlineNotify::KFProcessOnlineNotify() {

}

KFProcessOnlineNotify::~KFProcessOnlineNotify() {

}


int KFProcessOnlineNotify::DoOnlineNotify(KFDataParseOnlineNotify *dpon, KFOnlineNotify *on, KFNetwork *net)
{
    if (!on || !net) return -1;
    rtdebug("Enter do onlinenotify");
    int ret = dpon->OnlineNotifyGenerateData(on);
    if (ret < 0) return -2;
    const char *data = dpon->OnlineNotifyGenerateData();
    if (data) 
	{
        rtdebug("data is : %s", data);
        char outData[2048] = {0};
        int  iDataLen = 0;
        ret = net->KFNetworkSendRequest((char*)data, 0, outData, &iDataLen);
        //ret = net->KFNetworkRequest((char*)data, 0);
        if (ret != 200) 
		{
            rterror("error onlinenotify [%s] ret: [%d]", outData, ret);
            rterror("error onlinenotify [%s]", net->KFNetworkError());
			rtdebug("the ret is %d", ret);
            return -3;
        } 
		else 
		{
            rtdebug("result onlinenotify [%s] ret: [%d]", outData, ret);
            rtdebug("result data: %s, %d", outData, iDataLen);

            ret = dpon->KFDataParseResult(outData, iDataLen);
            if (ret < 0) 
			{
                char *errorCode = dpon->KFDataParseCmdErrorCode();
                const char *resultCode = dpon->KFDataParseResultCode().c_str();
                rterror("parse onlinenotify failed %d , errorcode [%s] resultCode [%s]", ret, errorCode, resultCode);
                return -4;
            }
        }
    }
    rtdebug("Exit do onlinenotify");
    return 0;
}
