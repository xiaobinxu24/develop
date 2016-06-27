//
// Created by v on 16-3-15.
// Author: jmdvirus@roamter.com
//

#include <iostream>
#include <rt_print.h>
#include "KFHttp.h"

using namespace std;

int main() {
    KFHttpClient *cli = new KFHttpClient();
    int ret = cli->KFHttpClientSet("114.215.128.113", 8085, "unite/service", "", 5);
    rterror("set result: %d", ret);
//    std::string sdata("{\"MsgBody\": [{\"PARAMS\": { } } ], \"MsgHead\": {\"ServiceCode\": "
//                          "    \"queryAvailableServerIp\", \"SrcSysID\": \"0001\", \"SrcSysSign\": \"46c541617d7dddd7936175412a99137d\", "
//                          "    \"transactionID\": \"1439347340985\"} }");
    std::string sdata("{\"MsgBody\":[{\"PARAMS\":{\"mac\":\"20F41BF69886\",\"version\":\"1501.16.0311.2245.1fa87\"}}],\"MsgHead\":{\"ServiceCode\":\"updateJudge\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"3cc558f5d95361a7112f761dfff43b77\",\"transactionID\":\"1458110401616\"}}");
    ret = cli->KFHttpClientSend(sdata);
    rterror("set send result : %d", ret);

    rtinfo("%d, %s", cli->KFHttpClientCode(), cli->KFHttpClientBody());
    rtinfo("%s", cli->KFHttpClientError());

    return 0;
}