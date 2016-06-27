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
#include <kf_process_queryip.h>
#include "kf_data_parse.h"
#include <rt_print.h>
#include "gtest/gtest.h"
#include "main.h"
#include "ngx_md5.h"

//This program must run in router or it can't run success

//host error return -3
TEST(KFProcessQueryIPInvalidHeadTest, Negative) {
    KFProcessQueryIP *pqip = new KFProcessQueryIP;
    KFDataParseQueryIP *dpqip = new KFDataParseQueryIP();
    KFNetwork *net = new KFNetwork("cloud.melin",8085, "unite/service", 5);
    KFSysOperation *sysop = new KFSysOperation();
    KFConfig *conf = new KFConfig();
    //char mac[36] = "34BDF966A8";
    //strncpy(sysop->MacGet()->,mac, strlen(mac));

    int ret = pqip->DoQueryIP(dpqip,net,conf,sysop);

    //head is error  or didn't give msg head
    EXPECT_EQ(-3,ret);


    if(NULL !=pqip ) {
    delete pqip;
    pqip = NULL;
    }
    if(NULL !=dpqip ) {
    delete dpqip;
    dpqip = NULL;
    }
    if(NULL != net){
    delete net;
    net = NULL;
    }
    if(NULL != conf ) {
    delete conf;
    conf = NULL;
    }


}

//success return 0
TEST(KFProcessQueryIPSuccessTest, Negative) {
    KFProcessQueryIP *pqip = new KFProcessQueryIP;
    KFDataParseQueryIP *dpqip = new KFDataParseQueryIP();
    KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);
    KFSysOperation *sysop = new KFSysOperation();
    KFConfig *conf = new KFConfig();
    //char mac[36] = "34BDF966A8";
    //strncpy(sysop->MacGet()->,mac, strlen(mac));

    int ret = pqip->DoQueryIP(dpqip,net,conf,sysop);


    EXPECT_EQ(0,ret);


    if(NULL !=pqip ) {
    delete pqip;
    pqip = NULL;
    }
    if(NULL !=dpqip ) {
    delete dpqip;
    dpqip = NULL;
    }
    if(NULL != net){
    delete net;
    net = NULL;
    }
    if(NULL != conf ) {
    delete conf;
    conf = NULL;
    }


}

/*
//error MAC return -4
TEST(KFProcessQueryIPErrorMacTest, Negative) {
    KFProcessQueryIP *pqip = new KFProcessQueryIP;
    KFDataParseQueryIP *dpqip = new KFDataParseQueryIP();
    KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);
    KFConfig *conf = new KFConfig();

    char mac[36] = "34BDF966A8";
    strncpy(conf->KFConfigMac(),mac, strlen(mac));

    int ret = pqip->DoQueryIP(dpqip,net,conf);



    EXPECT_EQ(-4,ret);
    if(NULL !=dpqip ) {
    delete dpqip;
    dpqip = NULL;
    }
    if(NULL != net){
    delete net;
    net = NULL;
    }
    if(NULL != conf ) {
    delete conf;
    conf = NULL;
    }


}
*/

int main(int argc,char *argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
