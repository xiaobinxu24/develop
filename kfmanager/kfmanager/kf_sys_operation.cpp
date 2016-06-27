//
// Created by v on 16-4-14.
// Author: zhangtao@roamter.com
//

#include "kf_sys_operation.h"
#include "main.h"
#include <string.h>
#include "rt_print.h"
#include "kf_process_pingcheck.h"
#include <stdlib.h>
#include <json/json/value.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <math.h>
#include <cstdio>
#include <stdio.h>
using namespace std;

#define LUA_POSITION_DEFAULT  "/tmp/kf/liblua"
#define KF_SYS_WEIGHT_DEFAULT 50
#define MODELMAXLEN  64
#define UPTIMEMAXLEN 256

KFSysOperation::KFSysOperation() {
    this->L = luaL_newstate();
    luaL_openlibs(this->L);
    this->LuaPosition = LUA_POSITION_DEFAULT;

    this->stPingCheckResult.avg = -1;
    this->stPingCheckResult.max = -1;
    this->stPingCheckResult.min = -1;
    this->stPingCheckResult.pkgloss = -1;
    this->stPingCheckResult.iWeightNow = -1;
    this->iWeigthDefault = KF_SYS_WEIGHT_DEFAULT;
    
    this->lastin = 0;
    this->lastout = 0;

    // /etc/config/kfmac
    //this->szMac = "";
    this->MacInit();
}

KFSysOperation::~KFSysOperation() {
    lua_close(this->L);
}

int KFSysOperation::KFSysOpPingDataReset() {
    this->stPingCheckResult.avg = -1;
    this->stPingCheckResult.max = -1;
    this->stPingCheckResult.min = -1;
    this->stPingCheckResult.pkgloss = -1;
    this->stPingCheckResult.iWeightNow = -1;
    return 0;
}
/*
int KFSysOperation::KFSysOpPingCheck(KFDataParseIPProperties *prop, KFPingCheckResult *result) {
    if (!prop && !result) return -1;

    this->KFSysOpPingDataReset();

    char luafile[512];
    //sprintf(luafile, "%s/%s", this->LuaPosition.c_str(), "kf_net_op.lua");
    sprintf(luafile, "%s", "/home/pig/work/kfrouter-conn/kfmanager/liblua/kf_net_op.lua");
    int status = luaL_loadfile(this->L, luafile);
    if (status) {
        rterror("error lua file [%s] [%d]", luafile, status);
        return -1;
    }

    lua_pcall(this->L, 0, LUA_MULTRET, 0);
    lua_getglobal(this->L, "kf_ping_server");

    lua_pushstring(this->L, prop->szIP);
    if (lua_pcall(this->L, 1, 4, 0) != 0) {
        rterror("call failed %s", lua_tostring(this->L, -1));
        return -2;
    }
    const char *ret = lua_tostring(this->L, -1);
    if (ret) {
        result->max = atof(ret);
    }
    ret = lua_tostring(this->L, -2);
    if (ret) {
        result->avg = atof(ret);
    }
    ret = lua_tostring(this->L, -3);
    if (ret) {
        result->min = atof(ret);
    }
    ret = lua_tostring(this->L, -4);
    if (ret) {
        result->pkgloss = atof(ret);
    }

    rtinfo("ping result: server: %s, pkgloss: %lf, avg: %lf, min: %lf, max: %lf", prop->szIP,
        result->pkgloss, result->avg,
        result->min, result->max);
    return 0;
}
*/
int KFSysOperation::KFSysOpPingCheck(KFDataParseIPProperties *prop, KFPingCheckResult *result, const char *string) {
    if (!prop && !result) return -1;
    rtinfo("start to run KFSysOpPingCheck with szip = %s ", string);
    this->KFSysOpPingDataReset();

    char luafile[512];
    sprintf(luafile, "%s/%s", this->LuaPosition.c_str(), "kf_net_op.lua");
    //sprintf(luafile, "%s", "/home/pig/work/kfrouter-conn/kfmanager/liblua/kf_net_op.lua");
    int status = luaL_loadfile(this->L, luafile);
    if (status) {
        rterror("error lua file [%s] [%d]", luafile, status);
        return -1;
    }

    lua_pcall(this->L, 0, LUA_MULTRET, 0);
    lua_getglobal(this->L, "kf_ping_parse");

    lua_pushstring(this->L, string);
    if (lua_pcall(this->L, 1, 4, 0) != 0) {
        rterror("call failed %s", lua_tostring(this->L, -1));
        return -2;
    }
    const char *ret = lua_tostring(this->L, -1);
    if (ret) {
        result->max = atof(ret);
    }
    ret = lua_tostring(this->L, -2);
    if (ret) {
        result->avg = atof(ret);
    }
    ret = lua_tostring(this->L, -3);
    if (ret) {
        result->min = atof(ret);
    }
    ret = lua_tostring(this->L, -4);
    if (ret) {
        result->pkgloss = atof(ret);
        rtinfo("pkgloss = %f", atof(ret));
    }

    rtdebug("ping result: server: %s, pkgloss: %lf, avg: %lf, min: %lf, max: %lf", prop->szIP,
           result->pkgloss, result->avg,
           result->min, result->max);
    return 0;
}
int KFSysOperation::KFSysOpPingAvail(KFDataParseIPProperties *prop, KFPingCheckResult *result) {
    rtinfo("start to run KFSysOpPingAvail and packagelost = %f ", result->pkgloss);
    if (!prop && !result) return -1;
    float dis = atof(prop->szDistance);
    float val;
    if(result->avg == 0)
    {
        val = 0;
    }
    else{
        val = ((100-result->pkgloss) + (dis/result->avg/3)) * 0.5;
    }
    result->iWeightNow = val;
    rtdebug("ping available: server: %s, weight: [%lf]-[%lf]", prop->szIP, result->iWeightNow, this->iWeigthDefault);
    if (result->iWeightNow >= this->iWeigthDefault) {
        return 1;
    }
    return 0;
}

int KFSysOperation::KFSysOpSetSS(KFSSResultData *data, const char *filename) {
    if (!data || !filename) return -1;

    Json::Value jstr;
    jstr[KF_STR_LOCALPORT] = data->iLocalPort;
    jstr[KF_STR_METHOD] = data->szMethod;
    jstr[KF_STR_PASSWORD] = data->szPassword;
    jstr[KF_STR_SERVER] = data->szServerIp;
    jstr[KF_STR_SERVERPORT] = data->iServerPort;

    std::string retstr = jstr.toStyledString();
    int f = open(filename, O_WRONLY|O_TRUNC|O_CREAT);
    if (!f) return -2;
    rtdebug("start to write to file [%s][%lu]", retstr.c_str(), retstr.size());
    int n = write(f, retstr.c_str(), retstr.size());
    if (n != retstr.size()) {
        close(f);
        return -3;
    }
    if (f) close(f);

    return 0;
}

bool KFSysOperation::MacInit() {
    ifstream macfile("/etc/config/kfmac");
    char mac[16] = {0};
    if (macfile.is_open()) {
        macfile.getline(mac, sizeof(mac));
        size_t len = strlen(mac);
        if (mac[len -1] == '\n') {
            mac[len -1] = '\0';
        }
        this->szMac = string(mac);
        macfile.close();
        return true;
    }
    return false;
}

/************************************/
//author:zhangtao@roamter.com
//date:2016.04.19
//apiname:KFSysOpGetMac()
//funtion:return device mac
//input:null
//output:mac
/***********************************/

int KFSysOperation::KFSysOpGetMac(KFTrafficInfo *ti)
{
    rtdebug("start to get mac");
    char mac[64]={0};
    ifstream  macfile("/etc/config/kfmac");
    if ( macfile.is_open() )
    {
        macfile.getline(mac, 64);
        
        //delet '\n' if has
        int len = strlen(mac);
        if (mac[len - 1] == '\n')
        {
            mac[len - 1] = '\0';
        }
        //close file?
        rtinfo("mac = %s",mac);
        strncpy(ti->szMac, mac, strlen(mac));
        macfile.close();
        return 0;
    }
    rterror("Open file error");
    macfile.close();
    return -1;
}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.11
//apiname:KFSysOpGetModel()
//funtion:return device model
//input:null
//output:
//success:model
//error:NULL
/***********************************/

int KFSysOperation::KFSysOpGetModel()
{
    rtdebug("start to get model");
    char model[MODELMAXLEN];
    ifstream  modelfile("/etc/model");
    if ( modelfile.is_open() )
    {
        modelfile.getline(model, MODELMAXLEN);
        
        //delet '\n' if has
        int len = strlen(model);
        if (model[len - 1] == '\n')
        {
            model[len - 1] = '\0';
        }
        //close file?
        rtinfo("model = %s",model);
        this->szModel = model;
        modelfile.close();
        return 0;
    }
    rterror("Open file error");
    modelfile.close();
    return -1;
}

/************************************/
//author:zhangtao@roamter.com
//date:2016.04.11
//apiname:KFSysOpString2Double()
//funtion:convert string to double
//input:char *str
//output:double num
/***********************************/
double KFSysOperation::KFSysOpString2Double(char *str)
{
    double num;//最终结果
    int len = strlen(str);
    int i = 0;
    int pointPosition = 0;//小数点位置

    for (i; i < len; i++)
    {
        if( str[i] == '.')
        {
            pointPosition = i;//记录小数点位置
            break;
        }
    }
    if ( pointPosition != 0 )
    {
        //计算小数点后数据
        int decimalStartPosition = pointPosition + 1;//小数开始的位置
        double count = 1;//用于记录循环了多少次
        for(decimalStartPosition; decimalStartPosition<len; decimalStartPosition++)
        {
            num = num + (str[decimalStartPosition] - '0') * pow(0.1, count);
            ++count;
        }
        //计算整数部分
        count = 0;
        int intEndPosition = pointPosition - 1;//整数结束的位置
        for ( intEndPosition;intEndPosition>=0;intEndPosition--)
        {
            num = num + (str[intEndPosition] - '0') * pow(10, count);
            ++count;
        }
        return num;
    }
    return -1;
}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.12
//apiname:KFSysOpGetUptime()
//funtion:return uptime
//input:null
//output:uptime
/***********************************/

int KFSysOperation::KFSysOpGetUptime(KFTrafficInfo *result)
{
    rtdebug("start to get Uptime");
    char starttime[256] = {0};
    char luafile[512];
    sprintf(luafile, "%s/%s", this->LuaPosition.c_str(), "kf_sys_op.lua");

    //sprintf(luafile, "%s", "/home/pig/work/code/kfrouter-conn/kfmanager/liblua/kf_sys_op.lua");
    //sprintf(luafile, "%s", "kf_sys_op.lua");

    int status = luaL_loadfile(this->L, luafile);
    if (status)
    {
        rterror("error lua file [%s] [%d] ", luafile, status);
        return -1;
    }
    lua_pcall(this->L, 0, LUA_MULTRET, 0);
    lua_getglobal(this->L, "get_startup_time");

    if (lua_pcall(this->L, 0, 1, 0) != 0)
    {
        rterror("call failed %s", lua_tostring(this->L, -1));
        return -2;
    }
    strncpy(starttime,lua_tostring(this->L, -1),strlen(lua_tostring(this->L, -1)));
    //starttime = lua_tostring(this->L, -1);

    rtinfo("get start time  result: %s", starttime);
    strncpy(result->szStartupTime, starttime, sizeof(result->szStartupTime));
    return 0;

}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.13
//apiname:KFSysOpGetMemInfo()
//funtion:return meminfo
//input:null
//output:total free
/***********************************/
int KFSysOperation::KFGetMemInfo(KFTrafficInfo *result)
{
    rtdebug("start to get meminfo");
    if ( !result) return -1;
    char luafile[512];
    //sprintf(luafile, "%s/%s", this->LuaPosition.c_str(), "/home/pig/work/code/kfrouter-conn/kfmanager/liblua/kf_sys_op.lua");

    //sprintf(luafile, "%s", "/home/pig/work/code/kfrouter-conn/kfmanager/liblua/kf_sys_op.lua");
    sprintf(luafile, "%s/%s", this->LuaPosition.c_str(), "kf_sys_op.lua");

    int status = luaL_loadfile(this->L, luafile);
    if (status) 
    {
        rterror("error lua file [%s] [%d] ", luafile, status);
        return -2;
    }
    lua_pcall(this->L, 0, LUA_MULTRET, 0);
    lua_getglobal(this->L, "kf_get_meminfo");

    if (lua_pcall(this->L, 0, 2, 0) != 0)
    {
        rterror("call failed %s", lua_tostring(this->L, -1));
        return -3;
    }
    long ret = lua_tonumber(this->L, -1);
    if (ret)
    {
        result->iRemainRam = ret;
    }
    /*ret = lua_tonumber(this->L, -2);
    if (ret)
    {
        result->total = ret;
    }*/
    rtdebug("get meminfo result: free: %ld", result->iRemainRam);
    return 0;
}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.13
//apiname:KFSysOpGetWanInfo()
//funtion:return waninfo
//input:null
//output:wanip dns1 dns2 netmask gateway
/***********************************/
int KFSysOperation::KFGetWanInfo(KFTrafficInfo *result)
{
    rtdebug("start to get waninfo");
    if ( !result) return -1;
    char luafile[512];
    sprintf(luafile, "%s/%s", this->LuaPosition.c_str(), "kf_sys_op.lua");

   // sprintf(luafile, "%s", "/home/pig/work/code/kfrouter-conn/kfmanager/liblua/kf_sys_op.lua");

    int status = luaL_loadfile(this->L, luafile);
    if (status) 
    {
        rterror("error lua file [%s] [%d] ", luafile, status);
        return -2;
    }
    lua_pcall(this->L, 0, LUA_MULTRET, 0);
    lua_getglobal(this->L, "kf_get_waninfo");

    if (lua_pcall(this->L, 0, 5, 0) != 0)
    {
        rterror("call failed %s", lua_tostring(this->L, -1));
        return -3;
    }
    const char *ret = lua_tostring(this->L, -5);
    if (ret)
    {
        strncpy(result->szWanIP, ret, sizeof(result->wanip));
        strncpy(result->wanip, ret, sizeof(result->wanip));
        this->szWanIP = string(result->wanip);
        //result->wanip = atof(ret);
        rtdebug("wanip = %s",ret);
    }
    ret = lua_tostring(this->L, -4);
    if (ret)
    {
        strncpy(result->dns, ret, sizeof(result->dns));
        //result->dns1 = ret;
        rtdebug("dns1 = %s",ret);
    }
    ret = lua_tostring(this->L, -3);
    if (ret)
    {
        strncpy(result->dns2, ret, sizeof(result->dns2));
        //result->dns2 = ret;
        rtdebug("dns2 = %s",ret);
    }
    ret = lua_tostring(this->L, -2);
    if (ret)
    {
        strncpy(result->netmask, ret, sizeof(result->netmask));
        //result->netmask = ret;
        rtdebug("netmask = %s",ret);
    }
    ret = lua_tostring(this->L, -1);
    if (ret)
    {
        strncpy(result->gateway, ret, sizeof(result->gateway));
        //result->gateway = ret;
        rtdebug("gateway = %s",ret);
    }
    rtdebug("get waninfo result: \n wanip: %s\n dns1: %s\n  dns2: %s\n netmask: %s\n gateway: %s\n", result->wanip, result->dns, result->dns2, result->netmask, result->gateway);
    return 0;
}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.14
//apiname:KFSysOpGetClientConnNums()
//funtion:return client connetc numbers
//input:null
//output:numbers
/***********************************/
int  KFSysOperation::KFGetClientConnNums(KFTrafficInfo *result)
{
    rtdebug("start to get client connect number");
    FILE *fp;
    char cmd[1024] = "wc -l /tmp/dhcp.leases | awk '{print $1}' > /tmp/kfout;cat /tmp/kfout";
    char num[1024]={0};
    fp=popen(cmd, "r");
    if(fp==NULL)
    {
        rterror("exxcute [%s] error!", cmd);
        pclose(fp);
        return -1;
    }
    if ( fread(num, 1 ,sizeof(num), fp) > 0  )
    {
        rtdebug("The result of execute [%s] is:%s ", cmd,num);
        result->iClientConnNum = atoi(num);
        rtdebug("finish get client connect number");
        pclose(fp);
        return 0;
    }

    rterror("No result of execute [%s] ", cmd);
    pclose(fp);
    return -2;
}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.14
//apiname:KFSysOpGetConnNums()
//funtion:get connetc numbers
//input:null
//output:numbers
/***********************************/
int KFSysOperation::KFGetConnNums(KFTrafficInfo *ti)
{
    rtdebug("start to get connect numbers");
    FILE *fp;
    char cmd[1024] = "cat /proc/net/nf_conntrack | wc -l";
    //char cmd[1024] = "cat ~/work/code/kfrouter-conn/kfmanager/nf_conntrack | wc -l";
    char result[1024]={0};
    fp=popen(cmd, "r");
    if(fp==NULL)
    {
        rterror("exxcute [%s] error!", cmd);
        pclose(fp);
        return -1;
    }
    if ( fgets(result, sizeof(result), fp) != NULL  )
    {
        rtdebug("The result of execute [%s] is:%s ", cmd,result);
        ti->iRequestUrlNum = atoi(result);
        rtdebug("finish to get connect number");
        pclose(fp);
        return 0;
    }

    rterror("No result of execute [%s] ", cmd);
    pclose(fp);
    return -2;
}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.14
//apiname:KFSysOpGetDownloadOutTraffic()
//funtion:get download outtraffic
//input:null
//output:downloadouttraffic
/***********************************/

int KFSysOperation::KFSysOpGetDownloadOutTraffic()
{
    rtdebug("start to get downlload out traffic");
    FILE *fp;
    char cmd[1024] = "iptables -n -v -L -t filter -x | grep delegate_input | head -n 1 | awk '{print $2}'";
    char result[1024]={0};
    fp=popen(cmd, "r");
    if(fp==NULL)
    {
        rterror("exxcute [%s] error!", cmd);
        pclose(fp);
        return -1;
    }
    if ( fgets(result, sizeof(result), fp) != NULL  )
    {
        rtdebug("The result of execute [%s] is:%s ", cmd,result);
        this->DownloadOutTraffic = atol(result);
        rtdebug("finish to get downlload out traffic");
        pclose(fp);
        return 0;
    }

    rterror("No result of execute [%s] ", cmd);
    pclose(fp);
    return -2;
}

/************************************/
//author:zhangtao@roamter.com
//date:2016.04.14
//apiname:KFSysOpGetDownloadInTraffic()
//funtion:get download intraffic
//input:null
//output:downloadintraffic
/***********************************/

int KFSysOperation::KFSysOpGetDownloadInTraffic()
{
    rtdebug("start to get downlload in traffic");
    FILE *fp;
    char cmd[1024] = "extface=$(cat /proc/net/route | awk '$2==00000000 {print $1}');cat /proc/net/dev | grep $extface | tr : \" \" | awk '{print $2}'";
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
        rtdebug("The result of execute [%s] is:%s ", cmd,result);
        this->DownloadInTraffic = atol(result);
        rtdebug("finish to get downlload in traffic");
        pclose(fp);
        return 0;
    }

    rterror("No result of execute [%s] ", cmd);
    pclose(fp);
    return -2;
}

/************************************/
//author:zhangtao@roamter.com
//date:2016.04.14
//apiname:KFSysOpGetUploadInTraffic()
//funtion:get upload intraffic
//input:null
//output:uploadintraffic
/***********************************/

int KFSysOperation::KFSysOpGetUploadInTraffic()
{
    rtdebug("start to get upload in traffic");
    FILE *fp;
    char cmd[1024] = "extface=$(cat /proc/net/route | awk '$2==00000000 {print $1}');cat /proc/net/dev | grep $extface | tr : \" \" | awk '{print $10}'";
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
        rtdebug("The result of execute [%s] is:%s ", cmd,result);
        this->UploadInTraffic = atol(result);
        rtdebug("finish to get upload in traffic");
        pclose(fp);
        return 0;
    }

    rterror("No result of execute [%s] ", cmd);
    pclose(fp);
    return -2;
}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.19
//apiname:KFSysOpGetPrevTraffic()
//funtion:get prevtraffic
//input:null
//output:
/***********************************/

int KFSysOperation::KFSysOpGetPrevTraffic()
{
    rtdebug("start to get prevtraffic");
    FILE *fp;
    char cmd[1024] = "uci -c /etc/kfconfig get kfconfig.traffic.prevTraffic";
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
        rtdebug("The result of execute [%s] is:%s ", cmd,result);
        this->PrevTraffic = atol(result);
        rtdebug("finish to get upload in traffic");
        pclose(fp);
        return 0;
    }

    rterror("No result of execute [%s] ", cmd);
    pclose(fp);
    return -2;
}

/************************************/
//author:zhangtao@roamter.com
//date:2016.04.19
//apiname:KFSysOpGetPrevForeignTraffic()
//funtion:get PrevForeignTraffic
//input:null
//output:
/***********************************/

int KFSysOperation::KFSysOpGetPrevForeignTraffic()
{
    rtdebug("start to get prevtraffic");
    FILE *fp;
    char cmd[1024] = "uci -c /etc/kfconfig get kfconfig.traffic.prevForeignTraffic";
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
        rtdebug("The result of execute [%s] is:%s ", cmd,result);
        this->PrevForeignTraffic = atol(result);
        rtdebug("finish to get upload in traffic");
        pclose(fp);
        return 0;
    }

    rterror("No result of execute [%s] ", cmd);
    pclose(fp);
    return -2;
}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.18
//apiname:KFSysOpExecuteCmd()
//funtion:execute a shell cmd
//input:null
//output:execute result
/***********************************/
char * KFSysOperation::KFSysOpExecuteCmd(char cmd[1024])
{
    rtdebug("start to execute [%s]",cmd);
    FILE *fp;
    static char result[1024]={0};
    fp=popen(cmd, "r");
    if(fp==NULL)
    {
        rterror("execute [%s] error!", cmd);
        pclose(fp);
        return NULL;
    }
    if ( fgets(result, sizeof(result), fp) != NULL  )
    {
        rtdebug("The result of execute [%s] is:%s ", cmd,result);
        pclose(fp);
        return  result;
    }

    rterror("No result of execute [%s] ", cmd);
    pclose(fp);
    return NULL;
}
/************************************/
//author:zhangtao@roamter.com
//date:2016.04.19
//apiname:KFSysOpGetTrafficInfo()
//funtion:get traffic info
//input:
//rx : 下载内外网总流量
//tx : 上传内外网总流量
//out : 外网总流量
//pt : 上次内外网总流量
//pft : 上次外网总流量
//output:
/***********************************/
int KFSysOperation::KFSysOpGetTrafficInfo(KFTrafficInfo *result, long long *pt, long long *pft)
{

    if (!result )
    {
        rterror("Input arg error!!!!");
        return -1;
    }
    rtinfo("last ctraffic: pt = [%lld], pft = [%lld]", *pt, *pft);
    int ret = 0;
    snprintf(result->szMac, sizeof(result->szMac), "%s", this->szMac.c_str());
    ret = this->KFGetWanInfo(result);
    ret = this->KFSysOpGetUptime(result);
    ret = this->KFGetClientConnNums(result);
    ret = this->KFGetConnNums(result);
    ret = this->KFGetMemInfo(result);
    long long rx = 0;
    long long tx = 0;
    long long out = 0;
    //get outdownloadtraffic
    ret = KFSysOpGetDownloadOutTraffic();
    if(ret == 0)
    {
        out = this->DownloadOutTraffic;
    }
    //get indownloadtraffic
    ret = KFSysOpGetDownloadInTraffic();
    if(ret == 0)
    {
        rx = this->DownloadInTraffic;
    }
    //get inuploadtraffic
    ret = KFSysOpGetUploadInTraffic();
    if(ret == 0)
    {
        tx = this->UploadInTraffic;
    }
    rtinfo("======== The traffic real : out = [%lld], rx = [%lld], tx = [%lld]", out, rx, tx);

    //计算内外网总流量
    long long totalTraffic = rx + tx;
    result->lTotalTraffic = totalTraffic;

    //计算外网总流量
    result->lTotalForeignTraffic = out;

    //计算上次内外网总流量
    rtinfo("last trafficinfo: lastin = %lld, lastout = %lld", this->lastin, this->lastout);
    result->lPrevTraffic = *pt;

    //计算上次外网总流量

    result->lPrevTotalForeignTraffic = *pft;

    //计算周期外网流量
    if (out >= this->lastout )
    {
        //rtinfo("out[%lld] >= lastout[%lld]",out, this->lastout);
        //rtinfo("lCircleForeignTotalTraffic=  out[%lld] - lastout[%lld]",out, this->lastout);
        //rtinfo("lastout = out[%lld]",out);
        result->lCircleForeignTotalTraffic = out - this->lastout;
        this->lastout = out;
    }
    else{
        //rtinfo("out[%lld] < lastout[%lld]");
        //rtinfo("lCircleForeignTotalTraffic,lastout = out[%lld]",out);
        result->lCircleForeignTotalTraffic = out;
        this->lastout = out;
    }

    //计算周期内外网流量
    if (totalTraffic >= this->lastin)
    {
        //rtinfo("totalTraffic[%lld] >= lastin[%lld]",totalTraffic, this->lastin);
        //rtinfo("lCircleTotalTraffic=  totalTraffic[%lld] - lastin[%lld]",totalTraffic, this->lastin);
        //rtinfo("lastout = out[%lld]",out);
        result->lCircleTotalTraffic = totalTraffic - this->lastin;
        this->lastin = totalTraffic;
    }
    else{
        //rtinfo("totalTraffic[%lld] < lastin[%lld]");
        //rtinfo("lCircleTotalTraffic,lastin = out[%lld]",totalTraffic);
        result->lCircleTotalTraffic = totalTraffic ;
        this->lastin = totalTraffic;
    }

    result->lPrevTraffic = result->lCircleTotalTraffic + result->lPrevTraffic;
    result->lPrevTotalForeignTraffic = result->lCircleForeignTotalTraffic + result->lPrevTotalForeignTraffic;

    rtinfo("the result of get traffic info is:  at = [%lld] out = [%lld] pt = [%lld] pft = [%lld] ct = [%lld] cft = [%lld]",
           result->lTotalTraffic, result->lTotalForeignTraffic, result->lPrevTraffic, result->lPrevTotalForeignTraffic,
           result->lCircleTotalTraffic, result->lCircleForeignTotalTraffic);

    return 0;
}


/************************************/
//author:zhangtao@roamter.com
//date:2016.04.14
//apiname:KFSysOpGetTrafficInfo()
//funtion:get traffic info
//input:null
//output:traffic info
/***********************************/
/*
int KFSysOperation::KFGetTrafficInfo(long *pt, long *pft)
{
    rtdebug("start to get traffic info");
    KFTrafficInfo *ti = new KFTrafficInfo();
    //get uptime
    int ret = KFSysOpGetUptime(ti);
    if (ret == 0)
    {
        rtdebug("get uptime success!");
    }
    else
    {

    }
    //get clientnumbers
    ret = KFGetClientConnNums(ti);
    if (ret == 0)
    {
        rtdebug("get clientnumbers success!");
    }
    else
    {

    }
    //get connectnumbers
    ret = KFGetConnNums(ti);
    if (ret == 0)
    {
        rtdebug("get connectnumbers success!");
    }
    else
    {

    }

    ret = KFSysOpGetTrafficInfo(ti, pt, pft);//仅获取与流量有关的信息

    if (ret == 0)
    {
        rtdebug("get TrafficInfo success!");
    }

    ret = KFGetWanInfo(ti);
    if (ret == 0)
    {
        rtdebug("get wanInfo success!");
    }

    ret = KFGetMemInfo(ti);
    if (ret == 0)
    {
        rtdebug("get memInfo success!");
    }

    rtdebug("get trafficinfo result finish");
    return 0;
}
*/

int KFSysOperation::KFSysOpClearTraffic(KFTrafficInfo *ti) {
    if (!ti) return -1;

    snprintf(ti->szMac, sizeof(ti->szMac), "%s", this->szMac.c_str()); 
    int ret = this->KFGetWanInfo(ti);
    if (ret < 0) return -2;
    ret = this->KFSysOpGetUptime(ti);
    ret = this->KFGetClientConnNums(ti);
    ret = this->KFGetConnNums(ti);
    ret = this->KFGetMemInfo(ti);

    return ret;
}
