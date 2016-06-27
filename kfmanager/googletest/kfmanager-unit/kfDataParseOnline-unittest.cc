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
#include "gtest/gtest.h"
#include "kf_data_parse_onlinenotify.h"
#include "main.h"
#include "ngx_md5.h"


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


//0        success
//-1       NULL input
//-2       Generral error
//-3       Head error
//-4       Result error

TEST(KFDataParseOnlineNotifyTest, Negative) {

	KFOnlineNotify on = {"20F41BF69BB0"};
	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	EXPECT_TRUE(NULL != dpon);

	EXPECT_EQ(0,dpon->OnlineNotifyGenerateData(&on));



	char str_for_md5[128] = "";
	char time_str[32] = {0};
	sprintf(time_str, "%ld", time(NULL));
	sprintf(str_for_md5, "routerOnlineNotify-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
	unsigned char output[16] = {0};
	char md5_result[32] = {0};
	kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
	kfmd5_ngx_tostring(output, md5_result);
	char gen_str_expect[512] = {0};
	sprintf(gen_str_expect, "{\"MsgBody\":[{\"PARAMS\":{\"mac\":\"20F41BF69BB0\"}}],"
			"\"MsgHead\":{\"ServiceCode\":\"routerOnlineNotify\",\"SrcSysID\":\"0001\","
			"\"SrcSysSign\":\"%s\","
			"\"transactionID\":\"%s\"}}", md5_result, time_str);

	const char *generate_result =  dpon->OnlineNotifyGenerateData();
	char generate_result_format[512] = {0};
	clear_string(generate_result_format, generate_result);

	// expect the string  get string      compare them
	EXPECT_STREQ(gen_str_expect, generate_result_format);

	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}
	
}



TEST(KFDataParseOnlineNotifyErrorTest, Negative) {

	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	EXPECT_TRUE(NULL != dpon);

	char  dataerror[256]="{\"errorCode\":\"004\",\"errorMsg\":\"请求异常\"}";
	EXPECT_EQ(-2,dpon->KFDataParseResult(dataerror,sizeof(dataerror)));
	std::string error_code = dpon->KFDataParseCmdErrorCode();
	std::string error_msg = dpon->KFDataParseCmdErrorMsg();
	EXPECT_STREQ("004",error_code.c_str());
	EXPECT_STREQ("请求异常",error_msg.c_str());


	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}

}


TEST(KFDataParseOnlineNotifymd5Test, Negative) {

	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	EXPECT_TRUE(NULL != dpon);

	//  head   md5  is  error  then  return  -3  and  resultcode  is  null  and  resultmsg  is  null
	char  datamd[512]="{\"MsgHead\":{ \"ServiceCode\":\"routerOnlineNotify\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"21269ed615b6f80e1536598883f657b3\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":\"success\"} } ";
	EXPECT_EQ(-3,dpon->KFDataParseResult(datamd,sizeof(datamd)));

	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}

}


TEST(KFDataParseOnlineNotifyCodeTest, Negative) {

	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	EXPECT_TRUE(NULL != dpon);

	//  code  is null   then return -4
	char datacodemsgn[512]="{\"MsgHead\":{ \"ServiceCode\":\"routerOnlineNotify\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"b1269ed615b6f80e1536598883f657b3\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":} } ";
	EXPECT_EQ(-4,dpon->KFDataParseResult(datacodemsgn,sizeof(datacodemsgn)));


	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}

}



TEST(KFDataParseOnlineNotifyFail801Test, Negative) {

	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	EXPECT_TRUE(NULL != dpon);

	//  code  is FAIL-801  then return -4
	char dataresult[512]="{\"MsgHead\":{ \"ServiceCode\":\"routerOnlineNotify\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"b1269ed615b6f80e1536598883f657b3\", \"transactionID\":\"1448006795202\" },\"MsgBody\": {\"result\": {\"resultCode\":\"FAIL-801\",\"resultMsg\":\"【20F41BF69BB0】设备对应端口【0】已经释放，请重新获取\" } ";
	EXPECT_EQ(-4,dpon->KFDataParseResult(dataresult,sizeof(dataresult)));
	EXPECT_STREQ("FAIL-801",dpon->KFDataParseResultCode().c_str());
	EXPECT_STREQ("【20F41BF69BB0】设备对应端口【0】已经释放，请重新获取",dpon->KFDataParseResultMsg().c_str());

	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}

}



TEST(KFDataParseOnlineNotifyFail017Test, Negative) {

	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	EXPECT_TRUE(NULL != dpon);

	//  code  is FAIL-017  then return -4
	char datacodemsg[512]="{\"MsgHead\":{ \"ServiceCode\":\"routerOnlineNotify\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"b1269ed615b6f80e1536598883f657b3\", \"transactionID\":\"1448006795202\" },\"MsgBody\": {\"result\": {\"resultCode\":\"FAIL-017\",\"resultMsg\":\"路由器使用信息查询失败【sql】\" } ";
	EXPECT_EQ(-4,dpon->KFDataParseResult(datacodemsg,sizeof(datacodemsg)));
	EXPECT_STREQ("FAIL-017",dpon->KFDataParseResultCode().c_str());
	EXPECT_STREQ("路由器使用信息查询失败【sql】",dpon->KFDataParseResultMsg().c_str());


	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}

}



TEST(KFDataParseOnlineNotifyFail018Test, Negative) {

	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	EXPECT_TRUE(NULL != dpon);



	//  code  is FAIL-018  then return -4
	char datatest[512]="{\"MsgHead\":{ \"ServiceCode\":\"routerOnlineNotify\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"b1269ed615b6f80e1536598883f657b3\", \"transactionID\":\"1448006795202\" },\"MsgBody\": {\"result\": {\"resultCode\":\"FAIL-018\",\"resultMsg\":\"查询的路由器mac没有激活\" } ";
	EXPECT_EQ(-4,dpon->KFDataParseResult(datatest,sizeof(datatest)));
	EXPECT_STREQ("FAIL-018",dpon->KFDataParseResultCode().c_str());
	EXPECT_STREQ("查询的路由器mac没有激活",dpon->KFDataParseResultMsg().c_str());


	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}

}


TEST(KFDataParseOnlineNotifyFail019Test, Negative) {

	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	EXPECT_TRUE(NULL != dpon);



	//  code  is FAIL-019  then return -4
	char datatestmsg[512]="{\"MsgHead\":{ \"ServiceCode\":\"routerOnlineNotify\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"b1269ed615b6f80e1536598883f657b3\", \"transactionID\":\"1448006795202\" },\"MsgBody\": {\"result\": {\"resultCode\":\"FAIL-019\",\"resultMsg\":\"路由器端口信息更新失败【sql】\" } ";
	EXPECT_EQ(-4,dpon->KFDataParseResult(datatestmsg,sizeof(datatestmsg)));
	EXPECT_STREQ("FAIL-019",dpon->KFDataParseResultCode().c_str());
	EXPECT_STREQ("路由器端口信息更新失败【sql】",dpon->KFDataParseResultMsg().c_str());


	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}

}

TEST(KFDataParseOnlineNotifySuccessTest, Negative) {

    KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
    EXPECT_TRUE(NULL != dpon);

	// code  is  success
	char datamsg[512]="{\"MsgHead\":{ \"ServiceCode\":\"routerOnlineNotify\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"b1269ed615b6f80e1536598883f657b3\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":\"success\"} } ";
	EXPECT_EQ(0,dpon->KFDataParseResult(datamsg,sizeof(datamsg)));
	std::string ret_code = dpon->KFDataParseResultCode();
	EXPECT_STREQ("success",ret_code.c_str());

    if(NULL != dpon ) {
        delete dpon;
        dpon = NULL;
    }
}






int main(int argc,char *argv[])
{

	testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}

