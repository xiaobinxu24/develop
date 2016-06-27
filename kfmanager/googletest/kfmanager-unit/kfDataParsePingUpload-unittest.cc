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
#include "main.h"
#include "kf_data_parse_pingupload.h"
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

const char * id = "20F41BF69BB0";
const char *wanip = "192.168.133.1";
const char *resourceip= "123.133.23.201";

KFDataParsePingUpload * ppu = new KFDataParsePingUpload(id,wanip,resourceip);
KFDataParseIPProperties ips = {"212.168.333.212"};
KFPingCheckResult result = {3,2,1,22,10};
KFSSResultData ssData = {"aes-256-cfb","8fd742c5-3689-4d47-885b-ea5dc550d5e6","133.130.102.247",10083,1080};

KFSSResultData resultData;

//0        success
//-1       NULL input
//-2       Generral error
//-3       Head error
//-4       Result error

TEST(GenerateDataTest, Negative) {


	EXPECT_TRUE(NULL != ppu);

	EXPECT_EQ(0,ppu->KFDataParseGenerateData(&ips,&result));


	char str_for_md5[128] = "";
	char time_str[32] = {0};
	sprintf(time_str, "%ld", time(NULL));
	sprintf(str_for_md5, "uploadTestMsgAndGetServerPort-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
	unsigned char output[16] = {0};
	char md5_result[32] = {0};
	kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
	kfmd5_ngx_tostring(output, md5_result);
	char gen_str_expect[512] = {0};
	sprintf(gen_str_expect, "{\"MsgBody\":[{\"PARAMS\":{\"mac\":\"20F41BF69BB0\",\"pingVal\":2,\"pkgLossRate\":22,\"resourceIp\":\"212.168.333.212\",\"wanIp\":\"192.168.133.1\"}}],"
			"\"MsgHead\":{\"ServiceCode\":\"uploadTestMsgAndGetServerPort\",\"SrcSysID\":\"0001\","
			"\"SrcSysSign\":\"%s\","
			"\"transactionID\":\"%s\"}}", md5_result, time_str);

	const char *generate_result =  ppu->KFDataParseGetData();
	char generate_result_format[512] = {0};
	clear_string(generate_result_format, generate_result);

	// expect the string  get string      compare them
	EXPECT_STREQ(gen_str_expect, generate_result_format);

}



TEST(KFDataParseResultErrorTest, Negative) {

	char dataerror[256] = "{\"errorCode\":\"004\",\"errorMsg\":\"请求异常\"}";
	EXPECT_EQ(-2,ppu->KFDataParseResult(&resultData,dataerror,sizeof(dataerror)));
	std::string error_code = ppu->KFDataParseCmdErrorCode();
	std::string error_msg = ppu->KFDataParseCmdErrorMsg();
	EXPECT_STREQ("004",error_code.c_str());
	EXPECT_STREQ("请求异常",error_msg.c_str());

}

//md5
TEST(KFDataParseResultMd5Test, Negative) {

	EXPECT_TRUE(NULL != ppu);

	char strdata[512] = "{\"MsgHead\":{\"ServiceCode\":\"uploadTestMsgAndGetServerPort\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"85b4f9a386bd2f9210d25fe8df03d6f6\",\"transactionID\":\"1461207901844\"},\"MsgBody\":{\"result\": {\"resultCode\":\"success\",\"resultData\":{\"local_port\":1080,\"method\":\"aes-256-cfb\",\"password\":\"8fd742c5-3689-4d47-885b-ea5dc550d5e6\",\"server\":\"133.130.102.247\",\"server_port\":10083}}}}";

	EXPECT_EQ(-3,ppu->KFDataParseResult(&resultData,strdata,sizeof(strdata)));

}

//resultCode FAIL-011
TEST(KFDataParseResultCodeFail011Test, Negative) {

	EXPECT_TRUE(NULL != ppu);

	char resultStrData[512] ="{\"MsgHead\":{\"ServiceCode\":\"uploadTestMsgAndGetServerPort\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"85f7e0c6b46c3586764e3980bc6395fd\",\"transactionID\":\"1461209252655\"},\"MsgBody\":{\"result\":{\"resultCode\":\"FAIL-011\",\"resultMsg\":\"路由器使用信息查询失败【sql】\"}}}";

	EXPECT_EQ(-4,ppu->KFDataParseResult(&resultData,resultStrData,sizeof(resultStrData)));

	EXPECT_STREQ("FAIL-011",ppu->KFDataParseResultCode().c_str());
	EXPECT_STREQ("路由器使用信息查询失败【sql】",ppu->KFDataParseResultMsg().c_str());


}
//resultCode FAIL-012
TEST(KFDataParseResultCodeFail012Test, Negative) {

	EXPECT_TRUE(NULL != ppu);

	char resultStrTest[512] ="{\"MsgHead\":{\"ServiceCode\":\"uploadTestMsgAndGetServerPort\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"85f7e0c6b46c3586764e3980bc6395fd\",\"transactionID\":\"1461209252655\"},\"MsgBody\":{\"result\":{\"resultCode\":\"FAIL-012\",\"resultMsg\":\"服务器可用端口信息查询失败【sql】\"}}}";

	EXPECT_EQ(-4,ppu->KFDataParseResult(&resultData,resultStrTest,sizeof(resultStrTest)));

	EXPECT_STREQ("FAIL-012",ppu->KFDataParseResultCode().c_str());
	EXPECT_STREQ("服务器可用端口信息查询失败【sql】",ppu->KFDataParseResultMsg().c_str());


}


//resultCode FAIL-013
TEST(KFDataParseResultCodeFail013Test, Negative) {

	EXPECT_TRUE(NULL != ppu);

	char resultStr[512] ="{\"MsgHead\":{\"ServiceCode\":\"uploadTestMsgAndGetServerPort\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"85f7e0c6b46c3586764e3980bc6395fd\",\"transactionID\":\"1461209252655\"},\"MsgBody\":{\"result\":{\"resultCode\":\"FAIL-013\",\"resultMsg\":\"查询的服务器ip【212.168.333.212】不存在或者没有可用端口\"}}}";

	EXPECT_EQ(-4,ppu->KFDataParseResult(&resultData,resultStr,sizeof(resultStr)));

	EXPECT_STREQ("FAIL-013",ppu->KFDataParseResultCode().c_str());
	EXPECT_STREQ("查询的服务器ip【212.168.333.212】不存在或者没有可用端口",ppu->KFDataParseResultMsg().c_str());


}



//resultCode FAIL-014
TEST(KFDataParseResultCodeFail014Test, Negative) {

	EXPECT_TRUE(NULL != ppu);

	char resultStrData[512] ="{\"MsgHead\":{\"ServiceCode\":\"uploadTestMsgAndGetServerPort\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"85f7e0c6b46c3586764e3980bc6395fd\",\"transactionID\":\"1461209252655\"},\"MsgBody\":{\"result\":{\"resultCode\":\"FAIL-014\",\"resultMsg\":\"路由器使用信息查询失败【sql】\"}}}";

	EXPECT_EQ(-4,ppu->KFDataParseResult(&resultData,resultStrData,sizeof(resultStrData)));

	EXPECT_STREQ("FAIL-014",ppu->KFDataParseResultCode().c_str());
	EXPECT_STREQ("路由器使用信息查询失败【sql】",ppu->KFDataParseResultMsg().c_str());


}
//resultCode FAIL-015
TEST(KFDataParseResultCodeFail015Test, Negative) {

	EXPECT_TRUE(NULL != ppu);

	char resultStrTest[512] ="{\"MsgHead\":{\"ServiceCode\":\"uploadTestMsgAndGetServerPort\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"85f7e0c6b46c3586764e3980bc6395fd\",\"transactionID\":\"1461209252655\"},\"MsgBody\":{\"result\":{\"resultCode\":\"FAIL-015\",\"resultMsg\":\"该设备对应的mac信息不存在\"}}}";

	EXPECT_EQ(-4,ppu->KFDataParseResult(&resultData,resultStrTest,sizeof(resultStrTest)));

	EXPECT_STREQ("FAIL-015",ppu->KFDataParseResultCode().c_str());
	EXPECT_STREQ("该设备对应的mac信息不存在",ppu->KFDataParseResultMsg().c_str());


}


//resultCode FAIL-016
TEST(KFDataParseResultCodeFail016Test, Negative) {

	EXPECT_TRUE(NULL != ppu);

	char resultStr[512] ="{\"MsgHead\":{\"ServiceCode\":\"uploadTestMsgAndGetServerPort\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"85f7e0c6b46c3586764e3980bc6395fd\",\"transactionID\":\"1461209252655\"},\"MsgBody\":{\"result\":{\"resultCode\":\"FAIL-016\",\"resultMsg\":\"路由器测试数据入库失败【sql】\"}}}";

	EXPECT_EQ(-4,ppu->KFDataParseResult(&resultData,resultStr,sizeof(resultStr)));

	EXPECT_STREQ("FAIL-016",ppu->KFDataParseResultCode().c_str());
	EXPECT_STREQ("路由器测试数据入库失败【sql】",ppu->KFDataParseResultMsg().c_str());


}




//success
TEST(KFDataParseResultTest, Negative) {

	EXPECT_TRUE(NULL != ppu);

	char strdata[512] = "{\"MsgHead\":{\"ServiceCode\":\"uploadTestMsgAndGetServerPort\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"55b4f9a386bd2f9210d25fe8df03d6f6\",\"transactionID\":\"1461207901844\"},\"MsgBody\":{\"result\": {\"resultCode\":\"success\",\"resultData\":{\"local_port\":1080,\"method\":\"aes-256-cfb\",\"password\":\"8fd742c5-3689-4d47-885b-ea5dc550d5e6\",\"server\":\"133.130.102.247\",\"server_port\":10083}}}}";

	EXPECT_EQ(0,ppu->KFDataParseResult(&resultData,strdata,sizeof(strdata)));

	EXPECT_STREQ("success",ppu->KFDataParseResultCode().c_str());

	EXPECT_EQ(10083,resultData.iServerPort);
	EXPECT_EQ(1080,resultData.iLocalPort);
	EXPECT_STREQ("aes-256-cfb",resultData.szMethod);
	EXPECT_STREQ("8fd742c5-3689-4d47-885b-ea5dc550d5e6",resultData.szPassword);
	EXPECT_STREQ("133.130.102.247",resultData.szServerIp);


	if(NULL !=ppu){

		delete ppu;
		ppu = NULL;
	}

}






int main(int argc,char *argv[])
{

	testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}

