//
// Created by v on 16-3-15.
// Author: jmdvirus@roamter.com
//

#include <string.h>
#include <rt_print.h>
#include <cstdlib>
#include "kf_msg_data_parse.h"


int  recvMsgDataParse(char *recvData,KFControlMsg *ctlMsg)
{
    if(!recvData  || !ctlMsg )  return -1;

    char version[64]={0};
    char seq[128]={0};

    int ret = sscanf(recvData,"%s %s %s %s %s",version,ctlMsg->sender,seq,ctlMsg->cmd,ctlMsg->args);
    if(ret <  0)
    {
        return -2;
    }

    ctlMsg->version = atoi(version);
    ctlMsg->seq  = atoi(seq);

    return 0;
}

