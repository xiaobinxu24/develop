//
// Created by v on 16-3-16.
// Author: jmdvirus@roamter.com
//

#include <stdio.h>
#include <json/json/json.h>
#include "kf_config.h"
#include "main.h"
#include <fcntl.h>
#include <cstring>
#include <rt_print.h>
#include <cstdlib>
#include <curses.h>
#include <complex>
#include <valarray>
#include <unistd.h>


KFConfig::KFConfig() {
}

KFConfig::~KFConfig() {
}

int KFConfig::KFConfigInit() {

    int ret = this->KFGetRemoteServerInfo(&this->remoteServerInfo);
    if(ret < 0)
    {
        this->KFRemoteServerInfoInit();
    }

    this->KFGetConfigIPProperties(&this->ips);
    this->KFGetConfigTraffic(&this->trafficInfo);

    this->iTimeout = 5;
    this->ssFilename = std::string("/etc/shadowsocks/config.json");
    this->iPingUploadCountsMax = 3;

    return 0;
}



KFRemoteServerInfo *  KFConfig::KFConfigRemoteServerInfo(){
    return  &this->remoteServerInfo;
}

KFTrafficInfo     * KFConfig::KFConfigTrafficInfo(){
    return  &this->trafficInfo;
}

KFDataParseIPProperties * KFConfig::KFConfigIPProperties(){
    this->KFGetConfigIPProperties(&this->ips);
    return  &this->ips;
}


int KFConfig::KFConfigTimeout() {
    return this->iTimeout;
}

bool KFConfig::KFConfigHasConfig() {
    return this->hasConfig;
}

int KFConfig::KFRemoteServerInfoInit(){

    sprintf(this->remoteServerInfo.szHost,"%s","api.melinkr.com");
    this->remoteServerInfo.iPort = 8085;
    sprintf(this->remoteServerInfo.szUrl,"%s","unite/service");

    return  0;
}

int KFConfig::KFGetConfigSSResultData(KFSSResultData *srData)
{
    if( !srData )return -1;

    char  data[4096]={0};

    int ret = this->KFReadConfigFile(KF_CONFIG_SHADOWSOCKS,data,sizeof(data));
    if(ret < 0)
    {
        return -2;
    }

    Json::Reader jrd;
    Json::Value json_result;
    jrd.parse(data,json_result,0);

    if (json_result[KF_STR_SZSERVERIP] != Json::nullValue) {
        snprintf( srData->szServerIp,sizeof(srData->szServerIp),"%s",json_result[KF_STR_SZSERVERIP].asString().c_str());
    }
    Json::Value serverport = json_result[KF_STR_ISERVERPORT];
    if (serverport != Json::nullValue) {
        srData->iServerPort   = atoi( serverport.asString().c_str());
    }

    if (json_result[KF_STR_SZMETHOD] != Json::nullValue) {
        snprintf( srData->szMethod,sizeof(srData->szMethod),"%s",json_result[KF_STR_SZMETHOD].asString().c_str());
    }

    if (json_result[KF_STR_SZPASSWORD] != Json::nullValue) {
        snprintf( srData->szPassword,sizeof(srData->szPassword),"%s",json_result[KF_STR_SZPASSWORD].asString().c_str());
    }

    Json::Value localport = json_result[KF_STR_ISERVERPORT];
    if (localport != Json::nullValue) {
        srData->iLocalPort   = atoi( localport.asString().c_str());
    }

    memcpy(&this->ssResultData, srData, sizeof(KFSSResultData));

    rtdebug("KFGetConfigSSResultData:  szServerIp=%s  iServerPort=%d  szMethod=%s  szPassword=%s iLocalPort=%d",srData->szServerIp,
                                             srData->iServerPort,srData->szMethod,srData->szPassword,srData->iLocalPort);

    return  0;
}

int KFConfig::KFSaveConfigSSResultData(KFSSResultData *srData){
    if( !srData )return -1;

    Json::Value jstr;

    jstr[KF_STR_SZSERVERIP]    = srData->szServerIp;
    jstr[KF_STR_ISERVERPORT]     = Json::valueToString(Json::Value::Int(srData->iServerPort));
    jstr[KF_STR_SZMETHOD]     = srData->szMethod;
    jstr[KF_STR_SZPASSWORD]     = srData->szPassword;

    jstr[KF_STR_ILOCALPORT]     =  Json::valueToString(Json::Value::Int(srData->iLocalPort));
    jstr[KF_STR_LOCALADDRESS]    = "0.0.0.0";

    std::string retstr = jstr.toStyledString();

    int ret =this->KFWriteConfigFile(KF_CONFIG_SHADOWSOCKS,retstr.c_str(),retstr.size());
    if(ret < 0)
    {
        return  -2;
    }
    memcpy(&this->ssResultData, srData, sizeof(KFSSResultData));

    return 0;
}


int KFConfig::KFGetRemoteServerInfo(KFRemoteServerInfo *remoteServerInfo){
    if( !remoteServerInfo )return -1;

    char  data[4096]={0};

    int ret = this->KFReadConfigFile(KF_CONFIG_REMOTESERVERINFO,data,sizeof(data));
    if(ret < 0)
    {
        return -2;
    }

    Json::Reader jrd;
    Json::Value json_result;
    jrd.parse(data,json_result,0);

    if (json_result[KF_STR_IPORT] != Json::nullValue) {
        remoteServerInfo->iPort = atoi(json_result[KF_STR_IPORT].asString().c_str());
    }
    Json::Value szHost = json_result[KF_STR_SZHOST];
    if (szHost != Json::nullValue) {
        snprintf(remoteServerInfo->szHost, sizeof(remoteServerInfo->szHost), "%s", szHost.asString().c_str());
    }

    Json::Value szUrl = json_result[KF_STR_SZURL];
    if (szHost != Json::nullValue) {
        snprintf(remoteServerInfo->szUrl, sizeof(remoteServerInfo->szUrl), "%s", szUrl.asString().c_str());
    }

    rtdebug("----------KFRemoteServerInfo ----iPort=%d  szHost=%s  szUrl=%s",remoteServerInfo->iPort,remoteServerInfo->szHost,remoteServerInfo->szUrl);

    return  0;

}

int KFConfig::KFSaveConfigQueryIPProperties(KFDataParseIPProperties  *prop,int len){
    if(!prop || (len < 0 )) return -1;

    Json::Value root;
    Json::Value jstr;

    for(int i=0;i<len;i++)
    {

        jstr[KF_STR_SERVERIP] = (prop+i)->szIP;

        jstr[KF_STR_BELONGGROUP] = Json::valueToString(Json::Value::Int((prop+i)->iBelongGroup));
        jstr[KF_STR_DISTANCE] = (prop+i)->szDistance;
        jstr[KF_STR_LATITUDE] = Json::valueToString((prop+i)->fLatitude);
        jstr[KF_STR_LONGITUDE] = Json::valueToString((prop+i)->fLongitude);

        root[KF_STR_PARAMS][i] = jstr;

    }

    std::string retstr = root.toStyledString();

    int ret = this->KFWriteConfigFile(KF_CONFIG_QUERYIPPROPERTIES,retstr.c_str(),retstr.size());
    if(ret < 0)
    {
        return -2;
    }

    return 0;
}

int KFConfig::KFGetConfigQueryIPProperties(KFDataParseIPProperties *prop,int *len){
    if(!prop )return -1;

    char  data[4096]={0};

    int ret = this->KFReadConfigFile(KF_CONFIG_QUERYIPPROPERTIES,data,sizeof(data));
    if(ret < 0)
    {
        return -2;
    }

    Json::Reader jrd;
    Json::Value json_result;
    jrd.parse(data, json_result, 0);

    Json::Value json_rdata = json_result[KF_STR_PARAMS];

    for(int it = 0; it < json_rdata.size(); ++it) {
        Json::Value rd = json_rdata[it];
        if (rd[KF_STR_BELONGGROUP] != Json::nullValue) {
            (prop+it)->iBelongGroup = atoi(rd[KF_STR_BELONGGROUP].asString().c_str());
        }
        Json::Value distance = rd[KF_STR_DISTANCE];
        if (distance != Json::nullValue) {
            snprintf((prop+it)->szDistance, sizeof((prop+it)->szDistance), "%s", distance.asString().c_str());
        }
        Json::Value lat = rd[KF_STR_LATITUDE];
        if (lat != Json::nullValue) {
            (prop+it)->fLatitude = atof(lat.asString().c_str());
        }
        Json::Value longi = rd[KF_STR_LONGITUDE];
        if (longi != Json::nullValue) {
            (prop+it)->fLongitude = atof(longi.asString().c_str());
        }
        Json::Value sip = rd[KF_STR_SERVERIP];
        if (sip != Json::nullValue) {
            snprintf((prop+it)->szIP, sizeof((prop+it)->szIP), "%s", sip.asString().c_str());
            rtdebug("The ip is [%s]", (prop+it)->szIP);
        }

        rtdebug("----- it=%d  iBelongGroup=%d szDistance=%s fLongitude =%f fLatitude=%f",it,(prop+it)->iBelongGroup,(prop+it)->szDistance,(prop+it)->fLongitude, (prop+it)->fLatitude);
    }

    *len = json_rdata.size();

    return 0;
}

int KFConfig::KFSaveConfigIPProperties(KFDataParseIPProperties *prop){
    if(!prop )return -1;

    rtinfo("Save config of IP properties ======================= ");
    this->hasConfig =TRUE;

    Json::Value jstr;

    jstr[KF_STR_CONFIG]      = Json::valueToString(Json::Value::Int(this->hasConfig));

    jstr[KF_STR_SERVERIP]    = prop->szIP;
    jstr[KF_STR_BELONGGROUP] = Json::valueToString(Json::Value::Int(prop->iBelongGroup));
    jstr[KF_STR_DISTANCE]    = prop->szDistance;

    jstr[KF_STR_LATITUDE]    = Json::valueToString(prop->fLatitude);
    jstr[KF_STR_LONGITUDE]   = Json::valueToString(prop->fLongitude);

    std::string retstr = jstr.toStyledString();

    int ret =this->KFWriteConfigFile(KF_CONFIG_IPPROPERTIES,retstr.c_str(),retstr.size());
    if(ret < 0)
    {
        return -2;
    }

    return 0;
}

int KFConfig::KFSaveConfigIPPropertiesWithFlag(KFDataParseIPProperties *prop, bool flag){
    if(!prop )return -1;

    this->hasConfig = flag;

    Json::Value jstr;

    jstr[KF_STR_CONFIG]      = Json::valueToString(Json::Value::Int(this->hasConfig));

    jstr[KF_STR_SERVERIP]    = prop->szIP;
    jstr[KF_STR_BELONGGROUP] = Json::valueToString(Json::Value::Int(prop->iBelongGroup));
    jstr[KF_STR_DISTANCE]    = prop->szDistance;

    jstr[KF_STR_LATITUDE]    = Json::valueToString(prop->fLatitude);
    jstr[KF_STR_LONGITUDE]   = Json::valueToString(prop->fLongitude);

    std::string retstr = jstr.toStyledString();

    int ret =this->KFWriteConfigFile(KF_CONFIG_IPPROPERTIES,retstr.c_str(),retstr.size());
    if(ret < 0)
    {
        return -2;
    }

    return 0;
}


int KFConfig::KFGetConfigIPProperties(KFDataParseIPProperties *prop) {
    if( !prop )return -1;

    char  data[4096]={0};

    int ret =this->KFReadConfigFile(KF_CONFIG_IPPROPERTIES,data,sizeof(data));
    if(ret < 0)
    {
        return -2;
    }

    Json::Reader jrd;
    Json::Value json_result;
    jrd.parse(data,json_result,0);


    if (json_result[KF_STR_BELONGGROUP] != Json::nullValue) {
        prop->iBelongGroup = atoi(json_result[KF_STR_BELONGGROUP].asString().c_str());
    }
    Json::Value distance = json_result[KF_STR_DISTANCE];
    if (distance != Json::nullValue) {
        snprintf(prop->szDistance, sizeof(prop->szDistance), "%s", distance.asString().c_str());
    }
    Json::Value lat = json_result[KF_STR_LATITUDE];
    if (lat != Json::nullValue) {
        prop->fLatitude = atof(lat.asString().c_str());
    }
    Json::Value longi = json_result[KF_STR_LONGITUDE];
    if (longi != Json::nullValue) {
        prop->fLongitude = atof(longi.asString().c_str());
    }
    Json::Value sip = json_result[KF_STR_SERVERIP];
    if (sip != Json::nullValue) {
        snprintf(prop->szIP, sizeof(prop->szIP), "%s", sip.asString().c_str());
        rtdebug("The ip is [%s]", prop->szIP);
    }

    Json::Value hasconfig = json_result[KF_STR_CONFIG];
    if (hasconfig != Json::nullValue) {

        this->hasConfig = (bool) atoi(hasconfig.asString().c_str());
    }


    rtdebug("KFDataParseIPProperties  data ======== %s  %d  %s  %f  %f",prop->szIP,prop->iBelongGroup,prop->szDistance,prop->fLatitude,prop->fLongitude);
    rtdebug("this hasConfig ---- %d",this->hasConfig);

    return 0;
}

int KFConfig::KFSaveConfigFlag(bool flag) {
    // If flag same with config file, needn't do anything.
    if (flag == this->hasConfig) return 0;
    this->hasConfig = flag;

    KFDataParseIPProperties prop;
    int ret = this->KFGetConfigIPProperties(&prop);
    if (ret < 0) return -2;

    return this->KFSaveConfigIPPropertiesWithFlag(&prop, flag);
}

int KFConfig::KFSaveConfigTraffic(KFTrafficInfo *ti){
    if(!ti )return -1;

    Json::Value jstr;

    jstr[KF_STR_TOTALTRAFFIC]              = Json::valueToString((Json::Value::UInt64)ti->lTotalTraffic);
    jstr[KF_STR_TOTALFOREIGNTRAFFIC]       = Json::valueToString((Json::Value::UInt64)ti->lTotalForeignTraffic);
    jstr[KF_STR_PREVTRAFFIC]               = Json::valueToString((Json::Value::UInt64)ti->lPrevTraffic);
    jstr[KF_STR_PREVTOTALFOREIGNTRAFFIC]   = Json::valueToString((Json::Value::UInt64)ti->lPrevTotalForeignTraffic);
    jstr[KF_STR_CIRCLETOTALTRAFFIC]        = Json::valueToString((Json::Value::UInt64)ti->lCircleTotalTraffic);
    jstr[KF_STR_CIRCLEFOREIGNTOTALTRAFFIC] = Json::valueToString((Json::Value::UInt64)ti->lCircleForeignTotalTraffic);

    std::string retstr = jstr.toStyledString();

    int ret =this->KFWriteConfigFile(KF_CONFIG_TRAFFIC,retstr.c_str(),retstr.size());
    if(ret < 0)
    {
        return -2;
    }
    memcpy(&this->trafficInfo, ti, sizeof(KFTrafficInfo));

    return 0;
}

int KFConfig::KFGetConfigTraffic(KFTrafficInfo *ti){
    if(!ti )return -1;
    char  data[4096]={0};

    int ret=this->KFReadConfigFile(KF_CONFIG_TRAFFIC,data,sizeof(data));
    if(ret < 0)
    {
        return -2;
    }

    Json::Reader jrd;
    Json::Value json_result;
    jrd.parse(data,json_result,0);

    if (json_result[KF_STR_TOTALTRAFFIC] != Json::nullValue) {
        ti->lTotalTraffic = atol(json_result[KF_STR_TOTALTRAFFIC].asString().c_str());
    }

    if (json_result[KF_STR_TOTALFOREIGNTRAFFIC] != Json::nullValue) {
        ti->lTotalForeignTraffic = atol(json_result[KF_STR_TOTALFOREIGNTRAFFIC].asString().c_str());
    }
    if (json_result[KF_STR_PREVTRAFFIC] != Json::nullValue) {
        ti->lPrevTraffic = atol(json_result[KF_STR_PREVTRAFFIC].asString().c_str());
    }
    if (json_result[KF_STR_PREVTOTALFOREIGNTRAFFIC] != Json::nullValue) {
        ti->lPrevTotalForeignTraffic = atol(json_result[KF_STR_PREVTOTALFOREIGNTRAFFIC].asString().c_str());
    }
    if (json_result[KF_STR_CIRCLETOTALTRAFFIC] != Json::nullValue) {
        ti->lCircleTotalTraffic = atol(json_result[KF_STR_CIRCLETOTALTRAFFIC].asString().c_str());
    }
    if (json_result[KF_STR_CIRCLEFOREIGNTOTALTRAFFIC] != Json::nullValue) {
        ti->lCircleForeignTotalTraffic = atol(json_result[KF_STR_CIRCLEFOREIGNTOTALTRAFFIC].asString().c_str());
    }

    rtdebug("KFTrafficInfo  data ========lTotalTraffic=%ld   lTotalForeignTraffic=%ld   lPrevTraffic=%ld",ti->lTotalTraffic,ti->lTotalForeignTraffic,ti->lPrevTraffic);

    rtdebug("KFTrafficInfo  data ======== lPrevTotalForeignTraffic=%ld  lCircleTotalTraffic=%ld   lCircleForeignTotalTraffic=%ld",ti->lPrevTotalForeignTraffic,ti->lCircleTotalTraffic,ti->lCircleForeignTotalTraffic);

    return 0;
}



int KFConfig::KFGetVpnMode()
{
    rtinfo("start to get vpn  mode");
    FILE *fp;
    char cmd[1024] = "uci -c /etc/kfconfig get kfconfig.vpn.mode";
    char result[1024]={0};
    fp=popen(cmd, "r");
    if(fp==NULL)
    {
        rterror("execute [%s] error!", cmd);
        pclose(fp);
        return -1;
    }
    if ( fgets(result, sizeof(result), fp) != NULL  )
    {
        rtinfo("The result of execute [%s] is:%s ", cmd,result);
        this->iVpnMode = atoi(result);
        rtinfo("finish to get vpn  mode  is: %d",this->iVpnMode);
        pclose(fp);
        return this->iVpnMode;
    }

    rterror("No result of execute [%s] ", cmd);
    pclose(fp);
    return -2;
}

int KFConfig::KFWriteConfigFile(const char *filename,const char *data,int  len){
    if(!data || !filename)return -1;

    int f = open(filename, O_WRONLY|O_CREAT|O_TRUNC,0777);
    if (!f) return -2;
    rtdebug("start to write to file [%s][%d]",data,len);
    int n = write(f,data,len);
    if (n != len) {
        close(f);
        return -3;
    }
    if (f) close(f);

    return 0;
}

int KFConfig::KFReadConfigFile(const char *filename,char *data,int len) {
    if (!data || !filename)return -1;

    if (access(filename, F_OK) == 0) {

          int f = open(filename, O_RDONLY);
          if (f < 0) return -2;

          int n = read(f, data, len);
          if (n < 0) {
              close(f);
              return -3;
          }

          rtdebug("read from  file [%s][%d]", data, len);
          if (f) close(f);
    }
    else
    {
        rtdebug("KFReadConfigFile: [%s]  is  not exist",filename);
        return -1;
    }

    return 0;
}

