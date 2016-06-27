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
#include "kf_data_parse_error.h"
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

TEST(KFGenerateDataTest, Negative) {

	KFErrorReport er = {"20F41BF69BB0","FAIL-017","路由器使用信息查询失败【sql】"};
	KFDataParseError * dpe = new KFDataParseError();
	EXPECT_TRUE(NULL != dpe);


	EXPECT_EQ(0,dpe->GenerateData(&er));


	char str_for_md5[128] = "";
	char time_str[32] = {0};
	sprintf(time_str, "%ld", time(NULL));
    sprintf(str_for_md5, "uploadWarningMsg-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
	char md5_result[32] = {0};
	kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
	kfmd5_ngx_tostring(output, md5_result);
	char gen_str_expect[512] = {0};
	sprintf(gen_str_expect, "{\"MsgBody\":[{\"PARAMS\":{\"warningCode\":\"FAIL-017\",\"warningEntity\":\"20F41BF69BB0\",\"warningLevel\":\"\",\"warningMsg\":\"路由器使用信息查询失败【sql】\"}}],"
			"\"MsgHead\":{\"ServiceCode\":\"uploadWarningMsg\",\"SrcSysID\":\"0001\","
			"\"SrcSysSign\":\"%s\","
			"\"transactionID\":\"%s\"}}", md5_result, time_str);

	const char *generate_result =  dpe->GeneratedData();
	char generate_result_format[512] = {0};
	clear_string(generate_result_format, generate_result);

	// expect the string  get string      compare them
	EXPECT_STREQ(gen_str_expect, generate_result_format);


    if(  NULL != dpe ){

		delete dpe;
		dpe  = NULL;
	}


}

TEST(KFDataParseErrorTest, Negative) {

    KFDataParseError * dpe = new KFDataParseError();
	EXPECT_TRUE(NULL != dpe);

    char  dataerror[512]="{\"errorCode\":\"004\",\"errorMsg\":\"请求异常\"}";
    EXPECT_EQ(-2,dpe->KFDataParseResult(dataerror,sizeof(dataerror)));
    std::string error_code = dpe->KFDataParseCmdErrorCode();
    std::string error_msg = dpe->KFDataParseCmdErrorMsg();
    EXPECT_STREQ("004",error_code.c_str());
    EXPECT_STREQ("请求异常",error_msg.c_str());

	if(  NULL != dpe ){

		delete dpe;
		dpe  = NULL;
	}

}

TEST(KFDataParseErrorMd5Test, Negative) {

    KFDataParseError * dpe = new KFDataParseError();
    EXPECT_TRUE(NULL != dpe);

    //head  md5
    char datamd[256]="{\"MsgHead\":{\"ServiceCode\":\"uploadWarningMsg\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"338dbdb7c577b3af37cb7c873d73fada\",\"transactionID\":\"1460969083031\"},\"MsgBody\": {\"result\": {\"resultCode\":\"success\"}}}";
    EXPECT_EQ(-3,dpe->KFDataParseResult(datamd,sizeof(datamd)));

    if(  NULL != dpe ){

        delete dpe;
        dpe  = NULL;
    }

}


TEST(KFDataParseErrorResultTest, Negative) {

    KFDataParseError * dpe = new KFDataParseError();
	EXPECT_TRUE(NULL != dpe);

	// result  is null
	char dataResultcode[256]="{\"MsgHead\":{\"ServiceCode\":\"uploadWarningMsg\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"638dbdb7c577b3af37cb7c873d73fada\",\"transactionID\":\"1460969083031\"},\"MsgBody\": {\"result\":}}";
	EXPECT_EQ(-4,dpe->KFDataParseResult(dataResultcode,sizeof(dataResultcode)));

	if(  NULL != dpe ){

		delete dpe;
		dpe  = NULL;
	}

}


TEST(KFDataParseErrorResultCodeTest, Negative) {

    KFDataParseError * dpe = new KFDataParseError();
	EXPECT_TRUE(NULL != dpe);

	// resultCode  is null
	char dataRescode[256]="{\"MsgHead\":{\"ServiceCode\":\"uploadWarningMsg\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"638dbdb7c577b3af37cb7c873d73fada\",\"transactionID\":\"1460969083031\"},\"MsgBody\": {\"result\": {\"resultCode\":}}}";
	EXPECT_EQ(-4,dpe->KFDataParseResult(dataRescode,sizeof(dataRescode)));

	if(  NULL != dpe ){

		delete dpe;
		dpe  = NULL;
	}

}


TEST(KFDataParseErrorResultSuccessTest, Negative) {

    KFDataParseError * dpe = new KFDataParseError();
	EXPECT_TRUE(NULL != dpe);

	// resultcode is  not null
	char dataRes[256]="{\"MsgHead\":{\"ServiceCode\":\"uploadWarningMsg\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"638dbdb7c577b3af37cb7c873d73fada\",\"transactionID\":\"1460969083031\"},\"MsgBody\": {\"result\": {\"resultCode\":\"success\"}}}";
	EXPECT_EQ(0,dpe->KFDataParseResult(dataRes,sizeof(dataRes)));
	std::string code = dpe->KFDataParseResultCode();
	EXPECT_STREQ("success",code.c_str());

	if(  NULL != dpe ){

		delete dpe;
		dpe  = NULL;
	}

}

int main(int argc,char *argv[])
{

	testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}

