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
#include <ngx_md5.h>
#include <rt_print.h>
#include "kf_process.h"
#include "kf_network.h"
#include "gtest/gtest.h"


// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>


TEST(KFNetworkSucsessTest, Negative) {
    KFNetwork *net = new KFNetwork("192.168.99.110", 7779, "", 5);

   int ret= net->KFNetworkRequest("data", 5);

    EXPECT_EQ(200,ret);

    delete net;
    net = NULL;
}



TEST(KFNetworkUrlTest, Negative) {
    KFNetwork *net = new KFNetwork("api", 7779, "", 5);

    int ret= net->KFNetworkRequest("data", 5);

    EXPECT_EQ(-3,ret);

    delete net;
    net = NULL;
}


TEST(KFNetworkTest, Negative) {
    KFNetwork *net = new KFNetwork("api", 12, "", 5);

    int ret= net->KFNetworkRequest("data", 5);

    EXPECT_EQ(-3,ret);

    delete net;
    net = NULL;
}


TEST(KFNetworkCodeTest, Negative) {
    KFNetwork *net = new KFNetwork("192.168.99.110", 12, "", 5);

    int ret= net->KFNetworkRequest("data", 5);

    EXPECT_EQ(-3,ret);

    delete net;
    net = NULL;
}




TEST(KFNetworkCheckOutsidecodeTest, Negative) {
    KFNetwork *net = new KFNetwork("192.168.99.110", 7779, "", 5);

    int ret= net->KFNetworkCheckOutside("api",8085, "unite/service","data");

    EXPECT_EQ(-2,ret);

    delete net;
    net = NULL;
}



TEST(KFNetworkSucsessSSTest, Negative) {



    char str_for_md5[64] = "";
    char time_str[32] = {0};
    sprintf(time_str, "%ld", time(NULL));
    sprintf(str_for_md5, "bindJudge-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
    char md5_result[32] = {0};
    kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
    kfmd5_ngx_tostring(output, md5_result);
    char gen_str_expect[512] = {0};
    sprintf(gen_str_expect, "{\"MsgBody\":[{\"PARAMS\":{\"mac\":\"20F41BF69BB0\"}}],"
            "\"MsgHead\":{\"ServiceCode\":\"bindJudge\",\"SrcSysID\":\"0001\","
            "\"SrcSysSign\":\"%s\","
            "\"transactionID\":\"%s\"}}", md5_result, time_str);


   // KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

     KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

    int ret= 0;
    rtdebug("----ret----0--------------  %d",ret);

    ret  = net->KFNetworkRequest(gen_str_expect, 5);
    rtdebug("----ret----1---------------  %d",ret);

    ret= net->KFNetworkRequest(gen_str_expect, 5);
    rtdebug("----ret----2-------------  %d",ret);

    ret= net->KFNetworkRequest(gen_str_expect, 5);

    rtdebug("----ret----3---------------  %d",ret);

   // const char *  errortest =  net->KFHttpClientError();
   // if(errortest != NULL)
    {
    //    rtdebug("----error-------------------  %s", errortest);
    }

    EXPECT_EQ(200,ret);

    delete net;
    net = NULL;


    KFNetwork *nn= new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

    ret= nn->KFNetworkRequest(gen_str_expect, 5);
    rtdebug("----ret----4--------------  %d",ret);
    ret= nn->KFNetworkRequest(gen_str_expect, 5);
    rtdebug("----ret----5--------------  %d",ret);

    EXPECT_EQ(200,ret);

    delete nn;
    nn = NULL;

}




TEST(KFNetworkCheckOutsideSucsessTest, Negative) {
    KFNetwork *net = new KFNetwork("192.168.99.110", 7779, "", 5);

    int ret= net->KFNetworkCheckOutside("cloud.melinkr.cn",8085, "unite/service","data");
    EXPECT_EQ(0,ret);

    ret= net->KFNetworkCheckOutside("cloud.melinkr.cn",8085, "unite/service","datatest");
    EXPECT_EQ(0,ret);

    delete net;
    net = NULL;
}




int main(int argc,char *argv[])
{

    testing::InitGoogleTest(&argc,argv);

    return RUN_ALL_TESTS();
}
