//
// Created by pig on 5/6/16.
//

#ifndef KFROUTER_CHECKNETWORKAVAILABLE_H
#define KFROUTER_CHECKNETWORKAVAILABLE_H

#include "main.h"
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <cstdio>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef __cplusplus
}
#endif

#define URLNUM 3
#define URLS struct Urls
#define PINGARGS struct Pingargs

class checkNetworkAvailable {
public:
    checkNetworkAvailable();
    ~checkNetworkAvailable();
    int parsesPingresult(char *pingResult);
    int getNetworkAvailabel();
    int count ;
    int counts ;
    int threadCounts;
    pthread_t  pthread_n[URLNUM];
    int pingFailflag[URLNUM];// 1 fail
    int allpingFailflag;//1 all fail

    URLS{
        char url[64];
    } urls[URLNUM];

    PINGARGS{
        char pingresult[512];
        float pkgloss;
        float max;
        float avg;
        float min;
    } pingargs[URLNUM];
private:

    lua_State *L;
    std::string   LuaPosition;
};


#endif //KFROUTER_CHECKNETWORKAVAILABLE_H
