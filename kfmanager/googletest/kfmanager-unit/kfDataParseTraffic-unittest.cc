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
#include <kf_process_bind.h>
#include <rt_print.h>
#include <main.h>
#include "kf_data_parse_traffic.h"
#include "kf_data_parse_bind.h"
#include "gtest/gtest.h"
#include "main.h"
#include "ngx_md5.h"
#include <sys/time.h>


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



TEST(KFDataParseTrafficDataParseTest, Negative) {

    //traffic
    //rtinfo("start to test traffic");
    KFDataParseTraffic *dpt = new KFDataParseTraffic();
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
    EXPECT_TRUE(NULL != dpt);

    //GenerateTrafficData
    EXPECT_EQ(0,dpt->KFGenerateTrafficData(&ti));


    char str_for_md5[128] = "";
    char time_str[32] = {0};
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t time_ms = (tv.tv_usec / 1000) + (tv.tv_sec * 1000);
    sprintf(time_str, "%ld", time_ms);
    sprintf(str_for_md5, "uploadRouteMsg-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
    char md5_result[32] = {0};
    kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
    kfmd5_ngx_tostring(output, md5_result);
    char gen_str_expect[1024] = {0};

    sprintf(gen_str_expect, "{\"MsgBody\":[{\"PARAMS\":[{\"circleForeignTotalTraffic\":\"100000\",\"circleTotalTraffic\":\"100000\",\"clientConnNum\":\"24\",\"mac\":\"240A64FE432B\",\"prevTotalForeignTraffic\":\"134533\",\"prevTraffic\":\"3323432\",\"remainedRam\":\"86343\",\"requestUrlNum\":\"123\",\"resourceIp\":\"2.2.2.2\",\"resourcePort\":\"10001\",\"startupTimestamp\":\"2015-07-0815:00:00\",\"totalForeignTraffic\":\"1234533\",\"totalTraffic\":\"3423432\",\"wanIp\":\"192.168.1.1\"}]}],"
            "\"MsgHead\":{\"ServiceCode\":\"uploadRouteMsg\",\"SrcSysID\":\"0001\","
            "\"SrcSysSign\":\"%s\","
            "\"transactionID\":\"%s\"}}", md5_result, time_str);


    const char *generate_result = dpt->GeneratedData();
    char generate_result_format[1024] = {0};


    clear_string(generate_result_format, generate_result);
    // expect the string  get string      compare them
    EXPECT_STREQ(gen_str_expect, generate_result_format);

    if(NULL !=dpt ) {
       delete dpt;
       dpt = NULL;
   }
}

TEST(KFDataParseClearTrafficDataParseTest, Negative) {
    //clearTraffic
    //rtinfo("start to test cleartraffic");
    KFDataParseTraffic *dpt = new KFDataParseTraffic();
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
    EXPECT_EQ(0,dpt->KFClearTraffic(&ti));
    char str_for_md5[128] = "";
    char time_str[32] = {0};



    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t time_ms = (tv.tv_usec / 1000) + (tv.tv_sec * 1000);
    sprintf(time_str, "%ld", time_ms);


    sprintf(str_for_md5, "uploadRouteMsg-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
    char md5_result[32] = {0};
    kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
    kfmd5_ngx_tostring(output, md5_result);


    char gen_clear_str_expect[1024]={0};
    sprintf(gen_clear_str_expect, "{\"MsgBody\":[{\"PARAMS\":[{\"circleForeignTotalTraffic\":\"0\",\"circleTotalTraffic\":\"0\",\"clientConnNum\":\"24\",\"mac\":\"240A64FE432B\",\"prevTotalForeignTraffic\":\"0\",\"prevTraffic\":\"0\",\"remainedRam\":\"86343\",\"requestUrlNum\":\"123\",\"resourceIp\":\"2.2.2.2\",\"resourcePort\":\"10001\",\"startupTimestamp\":\"2015-07-0815:00:00\",\"totalForeignTraffic\":\"0\",\"totalTraffic\":\"0\",\"wanIp\":\"192.168.1.1\"}]}],"
            "\"MsgHead\":{\"ServiceCode\":\"uploadRouteMsg\",\"SrcSysID\":\"0001\","
            "\"SrcSysSign\":\"%s\","
            "\"transactionID\":\"%s\"}}", md5_result, time_str);

    const char *generate_clear_result = dpt->GeneratedData();


    char generate_clear_result_format[256] = {0};

    memset(generate_clear_result_format,0,512);

    clear_string(generate_clear_result_format, generate_clear_result);
    // expect the string  get string      compare them
    EXPECT_STREQ(gen_clear_str_expect, generate_clear_result_format);


    if(NULL !=dpt ) {
        delete dpt;
        dpt = NULL;
    }
}




//data error
TEST(KFDataParseTrafficDataParseErrorTest, Negative) {

    KFDataParseTraffic *dpt = new KFDataParseTraffic();

    char str_for_md5[128] = "";
    char time_str[32] = {0};
    sprintf(time_str, "%ld", time(NULL));
    sprintf(str_for_md5, "uploadRouteMsg-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
    char md5_result[32] = {0};
    kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
    kfmd5_ngx_tostring(output, md5_result);
    char  dataerror[512]="{\"errorCode\":\"004\",\"errorMsg\":\"请求异常\"}";
    EXPECT_EQ(-2,dpt->KFDataParseTrafficResult(dataerror,sizeof(dataerror)));
    std::string error_code = dpt->KFDataParseCmdErrorCode();
    std::string error_msg = dpt->KFDataParseCmdErrorMsg();
    EXPECT_STREQ("004",error_code.c_str());
    EXPECT_STREQ("请求异常",error_msg.c_str());

    if(NULL !=dpt ) {
        delete dpt;
        dpt = NULL;
    }
}

//md5 error
TEST(KFDataParseTrafficDataParseMd5ErrorTest, Negative) {

    KFDataParseTraffic *dpt = new KFDataParseTraffic();
    char str_for_md5[128] = "";
    char time_str[32] = {0};
    sprintf(time_str, "%ld", time(NULL));
    sprintf(str_for_md5, "uploadRouteMsg-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
    char md5_result[32] = {0};
    kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
    kfmd5_ngx_tostring(output, md5_result);


    //  head   md5  is  error  then  return  -3  and  resultcode  is  null  and  resultmsg  is  null
    char  datamd[1024]="{\"MsgHead\":{ \"ServiceCode\":\"uploadRouteMsg\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"2bad7c36f77f552c010e22d6af7f5586\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":\"success\", \"resultMsg\":\"数据上报成功\" } } ";
    EXPECT_EQ(-3,dpt->KFDataParseTrafficResult(datamd,sizeof(datamd)));

    if(NULL !=dpt ) {
        delete dpt;
        dpt = NULL;
    }
}

//code is null
TEST(KFDataParseTrafficDataParseCodeNUllTest, Negative) {
    KFDataParseTraffic *dpt = new KFDataParseTraffic();
    char str_for_md5[128] = "";
    char time_str[32] = {0};
    sprintf(time_str, "%ld", time(NULL));
    sprintf(str_for_md5, "uploadRouteMsg-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
    char md5_result[32] = {0};
    kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
    kfmd5_ngx_tostring(output, md5_result);


    //  code  is null   then return -4  and resultcode  is  null  and  resultmsg  is  null
    char datacodemsgn[1024]="{\"MsgHead\":{ \"ServiceCode\":\"uploadRouteMsg\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"f8bdde08376088b4344ee116d9831129\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\":{ \"resultCode\":\"error\", \"resultMsg\":\"数据上报成功\" } } ";
    EXPECT_EQ(-4,dpt->KFDataParseTrafficResult(datacodemsgn,sizeof(datacodemsgn)));

    if(NULL !=dpt ) {
        delete dpt;
        dpt = NULL;
    }
}

int main(int argc,char *argv[])
{

    testing::InitGoogleTest(&argc,argv);

    return RUN_ALL_TESTS();
}

