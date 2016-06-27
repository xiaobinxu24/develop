//
// Created by pig on 5/6/16.
//

#include <rt_print.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "checkNetworkAvailable.h"
#define LUA_POSITION_DEFAULT  "/tmp/kf/liblua"

checkNetworkAvailable::checkNetworkAvailable() {
    this->L = luaL_newstate();
    luaL_openlibs(this->L);
    count = 0;
    threadCounts = 0;
    allpingFailflag = -1;
    strcpy(urls[0].url,"114.114.114.114");
    strcpy(urls[1].url,"baidu.com");
    strcpy(urls[2].url,"api.melinkr.com");
    this->L = luaL_newstate();
    luaL_openlibs(this->L);
    this->LuaPosition = LUA_POSITION_DEFAULT;
}
checkNetworkAvailable::~checkNetworkAvailable() {

    lua_close(this->L);
}
void *MultiThreadPingUrl(void *arg)
{
    checkNetworkAvailable *cna =  (checkNetworkAvailable *)arg;
    int c = cna->counts;
    if(c >= URLNUM){
        c = 0;
        cna->counts = 0;
    }
    cna->counts ++;
    rtinfo("start to ping url[%d]:[%s]",c,cna->urls[c].url);
    FILE *fp = NULL;
    std::string cmdpart1 = "ping -c 1 ";
    std::string cmd = cmdpart1+cna->urls[c].url;
    rtinfo("cmd[%s]", cmd.c_str());
    char result[2048]={0};
    fp = popen(cmd.c_str(), "r");
    if(fp==NULL)
    {
        rterror("execute [%s] error!", cmd.c_str());
        pclose(fp);
        return 0;
    }
    int iresult = 0;
    while (true) {
        if (feof(fp) != 0) break;
        char iret[128] = {0};
        char *ret = fgets(iret, sizeof(iret), fp);
        if (ret != NULL) {
            int iretlen = strlen(iret);
            strncpy(result+iresult, iret, iretlen);
            iresult += iretlen;
        }
        usleep(500);
    }
    rtinfo("result of execute [%s] is [%s]", cmd.c_str(), result);
    strncpy(cna->pingargs[c].pingresult,result,sizeof(cna->pingargs[c].pingresult));
    cna->threadCounts += 1;
    pclose(fp);
    return 0 ;
}
int checkNetworkAvailable::parsesPingresult(char *pingResult)
{
    rtinfo("start to run parsesPingresult with result = %s ", pingResult);
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

    lua_pushstring(this->L, pingResult);
    if (lua_pcall(this->L, 1, 4, 0) != 0) {
        rterror("call failed %s", lua_tostring(this->L, -1));
        return -2;
    }
    const char *ret = lua_tostring(this->L, -1);
    if (ret) {
        pingargs[count].max = atof(ret);
    }
    ret = lua_tostring(this->L, -2);
    if (ret) {
        pingargs[count].avg = atof(ret);
    }
    ret = lua_tostring(this->L, -3);
    if (ret) {
        pingargs[count].min = atof(ret);
    }
    ret = lua_tostring(this->L, -4);
    if (ret) {
        pingargs[count].pkgloss = atof(ret);
        rtinfo("pkgloss = %f", atof(ret));
    }
    rtinfo("ping result: url: %s, pkgloss: %f, avg: %f, min: %f, max: %f", this->urls[this->count].url,
           this->pingargs[this->count].pkgloss, this->pingargs[this->count].avg,
           this->pingargs[this->count].min, this->pingargs[this->count].max);

    return 0;
}

int checkNetworkAvailable::getNetworkAvailabel() {
    for (int i=0;i < URLNUM; i++){
        rtinfo("start to creat num[%d] thread", this->counts);
        pthread_create(&pthread_n[this->counts], NULL, MultiThreadPingUrl, this);

    }
    rtinfo("all thead creaqt success,start to wait all thread finish......");
    while(threadCounts < URLNUM)
    {
        usleep(500);
    }
    this->threadCounts = 0;
    rtinfo("all thead finish,start to parse ping result");
    count = 0;
    for(int i = 0;i < URLNUM;i++){
        float  a;
        rtinfo("start to parse result [%s]",this->pingargs[count].pingresult);
        this->parsesPingresult(this->pingargs[count].pingresult);
        //对ping结果进行分析如果有成功的提前退出
        if((this->pingargs[count].pkgloss > 0) && (this->pingargs[count].pkgloss < 100.0)){
            rtinfo("url [%s] is available, return!!!",this->urls[count].url);
            return 0;
        }
        if(this->pingargs[count].pkgloss == 0){
            if(!((this->pingargs[count].max + this->pingargs[count].min + this->pingargs[count].avg + this->pingargs[count].pkgloss) > 0.000001)){
                rtinfo("url [%s] is not accessable, go to next url.",this->urls[count].url);
                this->pingFailflag[count] = 1;
            }else{
                rtinfo("url [%s] is available \nmax = [%f]\navg = [%f]\nmin = [%f]\npkgloss = [%f]\nreturn!!!\n",this->urls[count].url,this->pingargs[count].max,this->pingargs[count].avg,this->pingargs[count].min,this->pingargs[count].pkgloss);
                return 0;
            }
        }
        if(this->pingargs[count].pkgloss == 100.00){
            rtinfo("url [%s] is not accessable, go to next url.",this->urls[count].url);
            this->pingFailflag[count] = 1;
        }
        this->count++;
        if(this->count >= URLNUM){
            this->count = 0;
        }
    }
    for (int i = 0;i < URLNUM;i++)
    {
        pthread_detach(pthread_n[i]);
        if(i == 0){
            this->allpingFailflag = this->pingFailflag[i];
        }else{
            this->allpingFailflag = this->allpingFailflag && this->pingFailflag[i];
        }
    }
    if(this->allpingFailflag == 1)
    {
        rtinfo("network is unavalible");
        return -1;
    }else{
        rtinfo("network is available");
        return 0;
    }
}

