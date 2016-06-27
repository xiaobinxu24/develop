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
#include <rt_print.h>
#include <main.h>
#include <fcntl.h>
#include "gtest/gtest.h"
#include "main.h"
#include "ngx_md5.h"
#include "kf_sys_operation.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __cplusplus
}
#endif




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



// result
// 0      success
// -1     Input error

KFSysOperation *sysop  =  new  KFSysOperation();



TEST(KFSysOpPingCheckInVailIPTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    //  InVail   IP
    KFDataParseIPProperties prop= {"192.168.133.222",1024,22,"222",22.00,23.00};
    KFPingCheckResult result = {5,1,2,2,3};


   char * data = "--- baidu.com ping statistics ---\n"
           "3 packets transmitted, 3 received, 0% packet loss, time 30091ms\n"
           "rtt min/avg/max/mdev = 32.882/33.595/34.236/0.555 ms";

    EXPECT_EQ(0,sysop->KFSysOpPingCheck(&prop,&result,data));

}


TEST(KFSysOpPingCheckIPTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    //  InVail   IP
    KFDataParseIPProperties prop= {"192.168.133.1",1024,22,"222",22.00,23.00};
    KFPingCheckResult result = {5,1,2,2,3};


char * data = "--- baidu.com ping statistics ---\n"
        "3 packets transmitted, 3 received, 0% packet loss, time 30091ms\n"
        "rtt min/avg/max/mdev = 32.882/33.595/34.236/0.555 ms";

    EXPECT_EQ(0,sysop->KFSysOpPingCheck(&prop,&result,data));



}


TEST(KFSysOpPingAvailTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    KFDataParseIPProperties prop= {"192.168.2.111",1024,22,"222",22.00,23.00};
    KFPingCheckResult result = {15,11,12,12,13};

    EXPECT_GE(sysop->KFSysOpPingAvail(&prop,&result),0);

}



TEST(KFSysOpPingDataResetTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    EXPECT_EQ(0,sysop->KFSysOpPingDataReset());

}


TEST(KFSysOpSetSSTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    char readData[256]={0};
    KFSSResultData ssData = {"aes-256-cfb","03952577-b3cf-4956-a7e5-210b3dcfa09c","133.130.89.11",10034,1080};
    const char *filename= "/tmp/etc/shadowsocks.json";

    EXPECT_EQ(0,sysop->KFSysOpSetSS(&ssData,filename));

    char  test[256]= {0};
    sprintf(test,"%s","{\"local_port\":1080,\"method\":\"aes-256-cfb\",\"password\":\"03952577-b3cf-4956-a7e5-210b3dcfa09c\",\"server\":\"133.130.89.11\",\"server_port\":10034}");

    if (access(filename, F_OK) == 0) {

        int f = open(filename, O_RDONLY);
        ASSERT_GE(f,0);

        int n = read(f, readData, sizeof(readData));
        ASSERT_GE(f,0);
        if (n < 0) {
            close(f);
            ASSERT_GE(n,0);

        }

        if (f) close(f);
    }

    char generate_result_format[512] = {0};
    clear_string(generate_result_format,readData);

    EXPECT_STREQ(test,generate_result_format);

}

TEST(KFSysOpGetPrevTrafficTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    EXPECT_EQ(0,sysop->KFSysOpGetPrevTraffic());

}


TEST(KFSysOpGetPrevForeignTrafficTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    EXPECT_EQ(0,sysop->KFSysOpGetPrevForeignTraffic());

}



TEST(KFSysOpExecuteCmdNULLTest, Negative) {

    EXPECT_TRUE(NULL != sysop);
    char cmd[1024] = "";

    EXPECT_TRUE( NULL == sysop->KFSysOpExecuteCmd(cmd));

}

TEST(KFSysOpExecuteCmdTest, Negative) {

    EXPECT_TRUE(NULL != sysop);
    char cmd[1024] = "uci -c /etc/kfconfig get kfconfig.traffic.prevForeignTraffic";

    EXPECT_TRUE( NULL != sysop->KFSysOpExecuteCmd(cmd));

}


TEST(KFSysOpGetTrafficInfoTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    KFTrafficInfo result = {};


/*
    sysop->KFSysOpGetPrevTraffic();
    sysop->KFSysOpGetPrevForeignTraffic();

    double pt = sysop->getPT();
    double pft = sysop->getPFT();
*/
    long pt  =0;
    long pft =0;
    EXPECT_EQ(0, sysop->KFSysOpGetTrafficInfo(&result, pt, pft));

}


TEST(KFGetTrafficInfoTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    KFTrafficInfo result = {};



    long pt  =0;
    long pft =0;

    EXPECT_EQ(0,sysop->KFGetTrafficInfo(pt,pft));

/*
    EXPECT_STREQ("20f41bf68bba",result.szMac);
    EXPECT_STREQ("192.168.99.111",result.wanip);
    EXPECT_STREQ("192.168.99.1",result.dns);
    EXPECT_STREQ("255.255.255.0",result.netmask);
    EXPECT_STREQ("192.168.99.1",result.gateway);
*/




}

TEST(KFSysOpClearTrafficTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    KFTrafficInfo ti ={};
    EXPECT_EQ(0,sysop->KFSysOpClearTraffic(&ti));
    EXPECT_STREQ("20f41bf68bba",ti.szMac);

}


TEST(KFGetWanInfoTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    KFTrafficInfo ti ={};

    EXPECT_EQ(0,sysop->KFGetWanInfo(&ti));

}


TEST(MacInitTest, Negative) {

    EXPECT_TRUE(NULL != sysop);

    EXPECT_TRUE(sysop->MacInit());


    if( NULL != sysop){
        delete  sysop;
        sysop = NULL;
    }

}

int main(int argc,char *argv[])
{

	testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}
