// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// A sample program demonstrating using Google C++ testing framework.
//
// Author: wan@google.com (Zhanyong Wan)


// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include "rt_print.h"
#include "kf_config.h"
#include <fstream>
#include <stdio.h>
#include <main.h>
#include <fcntl.h>
#include "kf_config.h"
#include "gtest/gtest.h"
#include "main.h"
#include "ngx_md5.h"
using namespace std;


int clear_string(char *save, const char *value)
{
    const char *p = value;
    char *n = save;
    for(;;) {
        if (*p == '\0') break;
        if (*p == '\n' || *p == '\t' || *p == ' ') {
            p++;
            continue;
        }
        (*n) = *p;
        n++;
        p++;
    }
    return 0;
}


//如果没有配置文件存在
TEST(KFConfigInit_NoconfigFileTest, Negative) {

    KFConfig *c = new KFConfig();


    c->KFConfigInit();
//RemoteServerInfo
    EXPECT_STREQ("api.melinkr.com", c->KFConfigRemoteServerInfo()->szHost);
    EXPECT_EQ(8085,c->KFConfigRemoteServerInfo()->iPort);
    EXPECT_STREQ("unite/service", c->KFConfigRemoteServerInfo()->szUrl);
//IPProperties
    EXPECT_STREQ("", c->KFConfigIPProperties()->szIP);
    EXPECT_STREQ("", c->KFConfigIPProperties()->szDistance);
    EXPECT_EQ(0,c->KFConfigIPProperties()->fLatitude);
    EXPECT_EQ(0,c->KFConfigIPProperties()->fLongitude);
    EXPECT_EQ(0,c->KFConfigIPProperties()->iBelongGroup);
//TrafficInfo
    EXPECT_EQ(0,c->KFConfigTrafficInfo()->lCircleForeignTotalTraffic);
    EXPECT_EQ(0,c->KFConfigTrafficInfo()->lCircleTotalTraffic);
    EXPECT_EQ(0,c->KFConfigTrafficInfo()->lPrevTotalForeignTraffic);
    EXPECT_EQ(0,c->KFConfigTrafficInfo()->lPrevTraffic);
    EXPECT_EQ(0,c->KFConfigTrafficInfo()->lTotalForeignTraffic);
    EXPECT_EQ(0,c->KFConfigTrafficInfo()->lTotalTraffic);


    EXPECT_EQ(5,c->KFConfigTimeout());
    EXPECT_STREQ("/etc/shadowsocks/config.json",c->GetSSFilename());
    EXPECT_EQ(3,c->PingUploadCountsMaxGet());
    if(NULL !=c ) {
        delete c;
        c = NULL;
    }
}

//如果配置文件存在
TEST(KFConfigInit_HasconfigFileTest, Negative) {

    KFConfig *c = new KFConfig();

    FILE *serverconfigfile;
    serverconfigfile = fopen("/etc/kf/kfServerConfig.json", "w");
    fprintf(serverconfigfile,"%s","{\"iPort\":\"8888888\",\"szHost\":\"test.com\",\"szUrl\":\"test/test\"}");
    fclose(serverconfigfile);

    FILE *Ipconfigfile;
    Ipconfigfile = fopen("/etc/kfconfig/kfIPConfig.json", "w");
    fprintf(Ipconfigfile,"%s","{\"belongGroup\":\"2323\",\"config\":\"1\",\"distance\":\"121\",\"latitude\":\"212.09\",\"longitude\":\"23.10\",\"serverIp\":\"192.168.1.133\"}");
    fclose(Ipconfigfile);


    FILE *Trafficfile;
    Trafficfile = fopen("/etc/kfconfig/kfTraffic.json", "w");
    fprintf(Trafficfile,"%s","{\"totalTraffic\":\"34\",\"totalForeignTraffic\":\"123\",\"prevTraffic\":\"3323\",\"prevTotalForeignTraffic\":\"113\",\"circleTotalTraffic\":\"1000\",\"circleForeignTotalTraffic\":\"88888\"}");
    fclose(Trafficfile);

    c->KFConfigInit();

    //RemoteServerInfo
    EXPECT_STREQ("test.com", c->KFConfigRemoteServerInfo()->szHost);
    EXPECT_EQ(8888888,c->KFConfigRemoteServerInfo()->iPort);
    EXPECT_STREQ("test/test", c->KFConfigRemoteServerInfo()->szUrl);

//IPProperties
    EXPECT_STREQ("192.168.1.133", c->KFConfigIPProperties()->szIP);
    EXPECT_STREQ("121", c->KFConfigIPProperties()->szDistance);
    EXPECT_EQ(212.09,c->KFConfigIPProperties()->fLatitude);
    EXPECT_EQ(23.10,c->KFConfigIPProperties()->fLongitude);
    EXPECT_EQ(2323,c->KFConfigIPProperties()->iBelongGroup);

//TrafficInfo

    EXPECT_EQ(88888,c->KFConfigTrafficInfo()->lCircleForeignTotalTraffic);
    EXPECT_EQ(1000,c->KFConfigTrafficInfo()->lCircleTotalTraffic);
    EXPECT_EQ(113,c->KFConfigTrafficInfo()->lPrevTotalForeignTraffic);
    EXPECT_EQ(3323,c->KFConfigTrafficInfo()->lPrevTraffic);
    EXPECT_EQ(123,c->KFConfigTrafficInfo()->lTotalForeignTraffic);
    EXPECT_EQ(34,c->KFConfigTrafficInfo()->lTotalTraffic);

    EXPECT_EQ(5,c->KFConfigTimeout());
    EXPECT_STREQ("/etc/shadowsocks/config.json",c->GetSSFilename());
    EXPECT_EQ(3,c->PingUploadCountsMaxGet());




    remove("/etc/kfconfig/kfIPConfig.json");
    remove("/etc/kf/kfServerConfig.json");
    remove("/etc/kfconfig/kfTraffic.json");


    if(NULL != c ) {
        delete c;
        c = NULL;
    }
}


//QueryIPProperties保存配置文件测试
TEST(KFConfigSaveConfigQueryIPPropertiesTest, Negative) {

    KFConfig *c = new KFConfig();
    KFDataParseIPProperties dpip[2];
    //第一组数据
    dpip[0].iBelongGroup = 1;
    dpip[0].fLatitude = 22.222222;
    dpip[0].fLongitude = 33.333333;
    strcpy(dpip[0].szDistance, "5555");
    strcpy(dpip[0].szIP, "6.6.6.6");
    //第二组数据
    dpip[1].iBelongGroup = 9;
    dpip[1].fLatitude = 88.888888;
    dpip[1].fLongitude = 77.777777;
    strcpy(dpip[1].szDistance, "6666");
    strcpy(dpip[1].szIP, "5.5.5.5");

    c->KFSaveConfigQueryIPProperties(dpip, 2);
    //读取文件
    char ch[1024] = "";
    char *data = ch;
    int len = 1024;
    if (access("/etc/kfconfig/kfQueryIPConfig.json", F_OK) == 0) {

        int f = open("/etc/kfconfig/kfQueryIPConfig.json", O_RDONLY);

        int n = read(f, data, len);
        if (n < 0) {
            rterror("no [/etc/kfconfig/kfQueryIPConfig.json] file, or open faild!!");
            close(f);
        }

        rtinfo("read from  file [%s][%d]", data, len);
        if (f) close(f);
    }
    char newdate[2048] = {0};
    //格式化字符串
    clear_string(newdate,data);
    rtinfo("newdata is  [%s][%lu]", newdate, strlen(newdate));


    EXPECT_STREQ("{\"PARAMS\":[{\"belongGroup\":\"1\",\"distance\":\"5555\",\"latitude\":\"22.222222\",\"longitude\":\"33.333333\",\"serverIp\":\"6.6.6.6\"},{\"belongGroup\":\"9\",\"distance\":\"6666\",\"latitude\":\"88.888888\",\"longitude\":\"77.777777\",\"serverIp\":\"5.5.5.5\"}]}",newdate);
    if(NULL != c ) {
        delete c;
        c = NULL;
    }
    remove("/etc/kfconfig/kfQueryIPConfig.json");
}

//IPProperties保存配置文件测试
TEST(KFConfigSaveConfigIPPropertiesTest, Negative) {

    KFConfig *c = new KFConfig();
    KFDataParseIPProperties dpip;
    //第一组数据
    dpip.iBelongGroup = 1;
    dpip.fLatitude = 22.222222;
    dpip.fLongitude = 33.333333;
    strcpy(dpip.szDistance, "5555");
    strcpy(dpip.szIP, "6.6.6.6");

    c->KFSaveConfigIPProperties(&dpip);
    //读取文件
    char ch[2048] = "";
    char *data = ch;
    int len = 1024;
    if (access("/etc/kfconfig/kfIPConfig.json", F_OK) == 0) {

        int f = open("/etc/kfconfig/kfIPConfig.json", O_RDONLY);

        int n = read(f, data, len);
        if (n < 0) {
            rterror("no [/etc/kfconfig/kfIPConfig.json] file, or open faild!!");
            close(f);
        }

        rtinfo("read from  file [%s][%d]", data, len);
        if (f) close(f);
    }
    char newdate[4056] = {0};
    //格式化字符串
    clear_string(newdate,data);
    rtinfo("newdata is  [%s][%lu]", newdate, strlen(newdate));

    EXPECT_STREQ("{\"belongGroup\":\"1\",\"config\":\"1\",\"distance\":\"5555\",\"latitude\":\"22.222222\",\"longitude\":\"33.333333\",\"serverIp\":\"6.6.6.6\"}",newdate);
    if(NULL != c ) {
        delete c;
        c = NULL;
    }
    remove("/etc/kfconfig/kfIPConfig.json");
}

//KFSaveConfigTraffic测试
TEST(KFConfigSaveConfigTrafficTest, Negative) {

    KFConfig *c = new KFConfig();
    KFTrafficInfo ti;
    ti.lCircleForeignTotalTraffic = 5555;
    ti.lCircleTotalTraffic = 6666;
    ti.lPrevTotalForeignTraffic = 7777;
    ti.lPrevTraffic = 8888;
    ti.lTotalTraffic = 9999;
    ti.lTotalForeignTraffic = 10000;

    c->KFSaveConfigTraffic(&ti);

    //读取文件
    char ch[1024] = "";
    char *data = ch;
    int len = 1024;
    if (access("/etc/kfconfig/kfTraffic.json", F_OK) == 0) {

        int f = open("/etc/kfconfig/kfTraffic.json", O_RDONLY);

        int n = read(f, data, len);
        if (n < 0) {
            rterror("no [/etc/kfconfig/kfTraffic.json] file, or open faild!!");
            close(f);
        }

        rtinfo("read from  file [%s][%d]", data, len);
        if (f) close(f);
    }
    char newdate[2048] = {0};
    //格式化字符串
    clear_string(newdate,data);
    rtinfo("newdata is  [%s][%lu]", newdate, strlen(newdate));


    EXPECT_STREQ("{\"circleForeignTotalTraffic\":\"5555\",\"circleTotalTraffic\":\"6666\",\"prevTotalForeignTraffic\":\"7777\",\"prevTraffic\":\"8888\",\"totalForeignTraffic\":\"10000\",\"totalTraffic\":\"9999\"}",newdate);
    if(NULL != c ) {
        delete c;
        c = NULL;
    }
    remove("/etc/kfconfig/kfTraffic.json");
}

//KFSaveConfigSSResultData测试
TEST(KFConfigSaveConfigSSResultDataTest, Negative) {

    KFConfig *c = new KFConfig();
    KFSSResultData ssrd;
    ssrd.iLocalPort = 1234;
    ssrd.iServerPort = 88;
    strcpy(ssrd.szMethod, "aes-256-cfb");
    strcpy(ssrd.szPassword, "7381a3e0-5122-4ced-bfb0-53e540259364");
    strcpy(ssrd.szServerIp, "192.166.11.11");

    c->KFSaveConfigSSResultData(&ssrd);

    //读取文件
    char ch[2048] = {0};
    char *data = ch;
    int len = 2048;
    if (access("/etc/shadowsocks/config.json", F_OK) == 0) {

        int f = open("/etc/shadowsocks/config.json", O_RDONLY);

        int n = read(f, data, len);
        if (n < 0) {
            rterror("no [/etc/shadowsocks/config.json] file, or open faild!!");
            close(f);
        }
        rtinfo("read from  file [%s][%d]", data, n);
        if (f) close(f);
    }
    char newdate[2048] = {0};
    //格式化字符串
    clear_string(newdate,data);
    rtinfo("newdata is  [%s][%lu]", newdate, strlen(newdate));

    EXPECT_STREQ("{\"local_address\":\"0.0.0.0\",\"local_port\":\"1234\",\"method\":\"aes-256-cfb\",\"password\":\"7381a3e0-5122-4ced-bfb0-53e540259364\",\"server\":\"192.166.11.11\",\"server_port\":\"88\"}",newdate);
    if(NULL != c ) {
        delete c;
        c = NULL;
    }
    remove("/etc/shadowsocks/config.json");
}

//KFConfigSetSSResultData测试
TEST(KFConfigSetSSResultDataTest, Negative) {

    KFConfig *c = new KFConfig();
    KFSSResultData ssrd;
    ssrd.iLocalPort = 1234;
    ssrd.iServerPort = 88;
    strcpy(ssrd.szMethod, "aes-256-cfb");
    strcpy(ssrd.szPassword, "7381a3e0-5122-4ced-bfb0-53e540259364");
    strcpy(ssrd.szServerIp, "192.166.11.11");

    c->KFConfigSetSSResultData(&ssrd);
    EXPECT_EQ(1234, c->KFConfigGetSSResultData()->iLocalPort);
    EXPECT_EQ(88, c->KFConfigGetSSResultData()->iServerPort);
    EXPECT_STREQ("7381a3e0-5122-4ced-bfb0-53e540259364",c->KFConfigGetSSResultData()->szPassword );
    EXPECT_STREQ("aes-256-cfb",c->KFConfigGetSSResultData()->szMethod );
    EXPECT_STREQ("192.166.11.11",c->KFConfigGetSSResultData()->szServerIp );

    if(NULL != c ) {
        delete c;
        c = NULL;
    }
}

int main(int argc,char *argv[]) {

    testing::InitGoogleTest(&argc, argv);

    remove("/etc/shadowsocks/config.json");
    remove("/etc/kf/kfServerConfig.json");
    remove("/etc/kfconfig/kfQueryIPConfig.json");
    remove("/etc/kfconfig/kfIPConfig.json");
    remove("/etc/kfconfig/kfTraffic.json");

    return RUN_ALL_TESTS();
}