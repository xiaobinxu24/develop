//
// Created by v on 16-3-26.
// Author: jmdvirus@roamter.com
//

#include <rt_print.h>
#include <string.h>
#include "kf_process_pingupload.h"

KFProcessPingUpload::KFProcessPingUpload() {

}

KFProcessPingUpload::~KFProcessPingUpload() {

}

int KFProcessPingUpload::DoPingUpload(KFDataParsePingUpload *pu, KFNetwork *net, KFSSResultData *ssData,
        KFDataParseIPProperties *ips, KFPingCheckResult *pcResult) {
    if (!pu || !net || !ssData) return -1;
    rtdebug("Enter do ping upload");

    int ret = pu->KFDataParseGenerateData(ips, pcResult);
    if (ret < 0) return -2;
    const char *data = pu->KFDataParseGetData();
    if (data) {
        rtdebug("data is : %s", data);
        char outData[2048] = {0};
        int  iDataLen = 0;
        ret = net->KFNetworkSendRequest((char*)data, 0, outData, &iDataLen);
      //  ret = net->KFNetworkRequest((char*)data, 0);
        if (ret != 200) {
            rterror("error ping upload [%s] ret: [%d]", outData, ret);
            rterror("error ping upload [%s]", net->KFNetworkError());
            return -3;
        } else {
            rtdebug("result data: %s, %d", outData, iDataLen);
            ret = pu->KFDataParseResult(ssData,outData, iDataLen);
            if (ret < 0) {
                char *errorCode = pu->KFDataParseCmdErrorCode();
                const char *resultCode = pu->KFDataParseResultCode().c_str();
                rterror("parse ping upload failed %d , errorcode [%s] resultCode [%s]", ret, errorCode, resultCode);
                if (strcmp(resultCode, "FAIL-013") == 0) {
                    return -11;
                }
                return -4;
            } else {
            }
        }
    }

    rtdebug("Exit do ping upload");

    return 0;
}
