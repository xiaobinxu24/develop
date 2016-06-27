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
#include <kf_process_pingcheck.h>
#include <main.h>
#include "gtest/gtest.h"
#include "kf_process_queryip.h"
#include "main.h"
#include "ngx_md5.h"




// result
// 0
// 1



KFProcessPingCheck  *ppc  = new KFProcessPingCheck();

KFDataParseQueryIP *dp = new KFDataParseQueryIP();
KFSysOperation *sys = new KFSysOperation();

KFPingCheckResult result = {5,12,1,22.12,33.13};

KFDataParseIPProperties *tips = new KFDataParseIPProperties[5];


TEST(MultiThreadDoPingCheckTest, Negative) {


    EXPECT_TRUE(NULL != dp);
    EXPECT_TRUE(NULL != sys);
    EXPECT_TRUE(NULL != tips);
    EXPECT_TRUE(NULL != ppc);

    strcpy(tips->szIP , "192.168.133.1");
    tips->iPort= 1001;
    strcpy(tips->szDistance , "1");

    tips->iBelongGroup = 11;
    tips->fLatitude = 11.01;
    tips->fLongitude = 11.01;

    strcpy((tips+1)->szIP , "101.251.254.2");
    (tips+1)->iPort= 1002;
    strcpy((tips+1)->szDistance , "2");

    (tips+1)->iBelongGroup = 22;
    (tips+1)->fLatitude = 22.01;
    (tips+1)->fLongitude = 22.01;


    strcpy((tips+2)->szIP, "101.251.254.3");
    (tips+2)->iPort= 1003;
    strcpy((tips+2)->szDistance , "3");

    (tips+2)->iBelongGroup = 33;
    (tips+2)->fLatitude = 33.01;
    (tips+2)->fLongitude = 33.01;

    strcpy((tips+3)->szIP, "23.109.36.4");
    (tips+3)->iPort= 1003;
    strcpy((tips+3)->szDistance , "3");

    (tips+3)->iBelongGroup = 33;
    (tips+3)->fLatitude = 33.01;
    (tips+3)->fLongitude = 33.01;

    strcpy((tips+4)->szIP, "23.109.36.235");
    (tips+4)->iPort= 1003;
    strcpy((tips+4)->szDistance , "3");

    (tips+4)->iBelongGroup = 33;
    (tips+4)->fLatitude = 33.01;
    (tips+4)->fLongitude = 33.01;


int ret = ppc->MultiThreadDoPingCheck(dp,sys,&result,&tips);
    EXPECT_TRUE(ret);
}

TEST(MultiThreadDoPingCheckNoIPSTest, Negative) {

    int ret = ppc->MultiThreadDoPingCheck(dp,sys,&result,&tips);
    //EXPECT_TRUE(ret);
    EXPECT_EQ(ret, -2);
}

TEST(DoPingCheckHasConfigTest, Negative) {

    EXPECT_TRUE(NULL != dp);
    EXPECT_TRUE(NULL != sys);
    EXPECT_TRUE(NULL != tips);
    EXPECT_TRUE(NULL != ppc);


    strcpy(tips->szIP , "192.168.1.111");
    tips->iPort= 1001;
    strcpy(tips->szDistance , "1");

    tips->iBelongGroup = 11;
    tips->fLatitude = 11.01;
    tips->fLongitude = 11.01;

    strcpy((tips+1)->szIP , "192.168.1.122");
    (tips+1)->iPort= 1002;
    strcpy((tips+1)->szDistance , "2");

    (tips+1)->iBelongGroup = 22;
    (tips+1)->fLatitude = 22.01;
    (tips+1)->fLongitude = 22.01;


    strcpy((tips+2)->szIP, "192.168.1.133");
    (tips+2)->iPort= 1003;
    strcpy((tips+2)->szDistance , "3");

    (tips+2)->iBelongGroup = 33;
    (tips+2)->fLatitude = 33.01;
    (tips+2)->fLongitude = 33.01;


    int ret = ppc->DoPingCheck(dp,sys,&result,&tips);

    EXPECT_EQ(1,ret);


    if(  NULL != ppc ){

        delete ppc;
        ppc  = NULL;
    }

    if(  NULL != dp ){

        delete dp;
        dp = NULL;
    }

    if(  NULL != sys ){

        delete sys;
        sys  = NULL;
    }
    if(tips != NULL) {
        delete  tips;
        tips = NULL;
    }
}


int main(int argc,char *argv[])
{

	testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}
