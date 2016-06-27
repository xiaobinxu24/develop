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
#include <kf_process_traffic.h>
#include <rt_print.h>
#include "gtest/gtest.h"
#include "main.h"
#include "ngx_md5.h"


TEST(KFProcessTrafficTest, Negative) {

    KFTrafficInfo ti ;
    strcpy(ti.szMac, "240A64FE432B");
    strcpy(ti.szWanIP, "192.168.1.1");
    strcpy(ti.szStartupTime, "2015-07-08 15:00:00");
    strcpy(ti.szResourceIP, "2.2.2.2");
    ti.iRemainRam = 86343;
    ti.iResourcePort = 10001;
    ti.iClientConnNum = 24;
    ti.iRequestUrlNum = 123;
    ti.lTotalTraffic = 3423432;
    ti.lTotalForeignTraffic = 1234533;
    ti.lPrevTraffic = 3323432;
    ti.lPrevTotalForeignTraffic = 134533;
    ti.lCircleForeignTotalTraffic = 100000;
    ti.lCircleTotalTraffic = 100000;

    KFProcessTraffic *pt = new KFProcessTraffic();
    KFDataParseTraffic *dpt = new KFDataParseTraffic();

    KFSysOperation *sysOp = new KFSysOperation();
    KFConfig *conf = new KFConfig();

    KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

    EXPECT_TRUE(NULL != pt);
    EXPECT_TRUE(NULL != dpt);
    EXPECT_TRUE(NULL != net);


    EXPECT_EQ(200,pt->UploadTraffic(net,sysOp,dpt, conf));

    //EXPECT_EQ(0,pt->ClearTraffic(net,sysOp,dpt));

    // invalid host
    KFNetwork *nn = new KFNetwork("api",8085, "unite/service", 5);

    EXPECT_TRUE(NULL != nn);

    //EXPECT_EQ(0,pt->UploadTraffic(nn,sysOp,dpt, conf));

    EXPECT_EQ(-2,pt->ClearTraffic(nn,sysOp,dpt,conf));

    if(NULL !=pt ) {
        delete pt;
        pt = NULL;
    }
    if(NULL != net){
        delete net;
        net = NULL;
    }
    if(NULL != dpt ) {
        delete dpt;
        dpt = NULL;
    }

    if(NULL != nn){
         delete nn;
         nn = NULL;
    }
}


TEST(KFProcessClearTrafficTest, Negative) {



 KFTrafficInfo ti ;
    strcpy(ti.szMac, "240A64FE432B");
    strcpy(ti.szWanIP, "192.168.1.1");
    strcpy(ti.szStartupTime, "2015-07-08 15:00:00");
    strcpy(ti.szResourceIP, "2.2.2.2");
    ti.iRemainRam = 86343;
    ti.iResourcePort = 10001;
    ti.iClientConnNum = 24;
    ti.iRequestUrlNum = 123;
    ti.lTotalTraffic = 3423432;
    ti.lTotalForeignTraffic = 1234533;
    ti.lPrevTraffic = 3323432;
    ti.lPrevTotalForeignTraffic = 134533;
    ti.lCircleForeignTotalTraffic = 100000;
    ti.lCircleTotalTraffic = 100000;    KFProcessTraffic *pt = new KFProcessTraffic();
    KFDataParseTraffic *dpt = new KFDataParseTraffic();
    KFSysOperation *sysOp = new KFSysOperation();
    KFConfig *conf = new KFConfig();

    KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

    EXPECT_TRUE(NULL != pt);
    EXPECT_TRUE(NULL != dpt);
    EXPECT_TRUE(NULL != net);


    EXPECT_EQ(200,pt->UploadTraffic(net,sysOp,dpt,conf));

   // EXPECT_EQ(0,pt->ClearTraffic(net,sysOp,dpt, conf));
                    //ClearTraffic(KFNetwork *net, KFSysOperation *sysOp, KFDataParseTraffic *traffic, KFConfig *conf);

    // invalid host
    KFNetwork *nn = new KFNetwork("api",8085, "unite/service", 5);

    EXPECT_TRUE(NULL != nn);

    EXPECT_EQ(-2,pt->UploadTraffic(nn,sysOp,dpt,conf));

   // EXPECT_EQ(0,pt->ClearTraffic(nn,sysOp,dpt, conf));

    if(NULL !=pt ) {
        delete pt;
        pt = NULL;
    }
    if(NULL != net){
        delete net;
        net = NULL;
    }
    if(NULL != dpt ) {
        delete dpt;
        dpt = NULL;
    }

    if(NULL != nn){
        delete nn;
        nn = NULL;
    }
}

int main(int argc,char *argv[])
{

    testing::InitGoogleTest(&argc,argv);

    return RUN_ALL_TESTS();
}

