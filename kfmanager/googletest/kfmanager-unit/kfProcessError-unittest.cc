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
#include "kf_process_error.h"
#include <rt_print.h>
#include "gtest/gtest.h"
#include "main.h"
#include "ngx_md5.h"


// result
// 0      success
// -1     Input error
// -2     GenerateData error
// -3     NetworkRequest != 200
//-4      DateParseResult error
TEST(KFProcessErrorTest, Negative) {

    KFProcessError  *pe  = new KFProcessError();
    KFDataParseError *dpe = new KFDataParseError();
    KFErrorReport er = {"20F41BF69BB0","FAIL-017","路由器使用信息查询失败【sql】"};

    KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);
    EXPECT_TRUE(NULL != pe);
    EXPECT_TRUE(NULL != dpe);
    EXPECT_TRUE(NULL != net);

    EXPECT_EQ(0,pe->DoErrorReport(dpe,&er,net));

    if(  NULL != pe ){

        delete pe;
        pe  = NULL;
    }

    if(  NULL != dpe ){

        delete dpe;
        dpe  = NULL;
    }

    if(  NULL != net ){

        delete net;
        net  = NULL;
    }


}


TEST(KFProcessErrorInvalidHostTest, Negative) {

    KFProcessError  *pe  = new KFProcessError();
    KFDataParseError *dpe = new KFDataParseError();
    KFErrorReport er = {"20F41BF69BB0","FAIL-017","路由器使用信息查询失败【sql】"};



    //invalid  host
    KFNetwork *nw = new KFNetwork("api",8085, "unite/service", 5);
    EXPECT_TRUE(NULL != nw);
    EXPECT_TRUE(NULL != pe);
    EXPECT_TRUE(NULL != dpe);
    EXPECT_EQ(-3,pe->DoErrorReport(dpe,&er,nw));

    if(  NULL != pe ){

        delete pe;
        pe  = NULL;
    }

    if(  NULL != dpe ){

        delete dpe;
        dpe  = NULL;
    }


    if(  NULL != nw ){

        delete nw;
        nw  = NULL;
    }
}

int main(int argc,char *argv[])
{

    testing::InitGoogleTest(&argc,argv);

    return RUN_ALL_TESTS();
}

